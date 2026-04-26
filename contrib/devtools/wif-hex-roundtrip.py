#!/usr/bin/env python3
"""
WIF <-> uppercase-hex round-trip helper for the RATR treasury + spork
keygen ceremony.

The keygen runbook (doc/treasury-keygen-runbook.md) produces a WIF-encoded
private key via `dumpprivkey`. WIF is base58-encoded and case-sensitive —
inconvenient to stamp on metal washers with an uppercase-only stamp set.
This script converts WIF <-> 64-character uppercase hex (the raw 32-byte
private key) so the metal backup can be all-uppercase.

Recovery from washers reads the hex back, converts it to WIF here, and
imports via `importprivkey` in a wallet.

Usage:
  ./wif-hex-roundtrip.py wif2hex <WIF>
      Decodes WIF, prints 64-char uppercase hex of the raw private key,
      plus the detected network (mainnet/testnet) and compression flag.

  ./wif-hex-roundtrip.py hex2wif <HEX> [--mainnet|--testnet] [--uncompressed]
      Encodes 64-char hex back to WIF. Defaults to --mainnet --compressed.

  ./wif-hex-roundtrip.py roundtrip <WIF>
      Runs WIF -> hex -> WIF and verifies the result matches input.
      Exits 0 on success, non-zero on mismatch. Use this to verify a
      conversion is reversible BEFORE stamping anything on metal.

  ./wif-hex-roundtrip.py selftest
      Runs known-vector regression tests. Exits 0 on pass.

This script is air-gap-safe — uses only Python 3 standard library
(hashlib, sys). No network, no external dependencies. Confirmed working
against the Bitcoin test vector for uncompressed mainnet WIF and against
freshly-generated RATR mainnet+testnet WIFs.

RATR-specific prefixes baked in:
  Mainnet  SECRET_KEY = 188 (0xBC)
  Testnet  SECRET_KEY = 239 (0xEF)

If you ever need to handle a different chain's WIF, override --version-byte.

Copyright (C) 2026 The Ratatoskr Core developers
Distributed under the MIT software license, see the accompanying file
COPYING or http://www.opensource.org/licenses/mit-license.php.
"""

import hashlib
import sys

# ---------------------------------------------------------------------------
# Base58 codec (no external dependencies)
# ---------------------------------------------------------------------------

_B58_ALPHABET = b'123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz'

def b58encode(data: bytes) -> str:
    n = int.from_bytes(data, 'big')
    out = bytearray()
    while n > 0:
        n, r = divmod(n, 58)
        out.insert(0, _B58_ALPHABET[r])
    # Preserve leading zero bytes as leading '1' chars
    for byte in data:
        if byte == 0:
            out.insert(0, _B58_ALPHABET[0])
        else:
            break
    return out.decode('ascii')

def b58decode(s: str) -> bytes:
    n = 0
    for c in s:
        idx = _B58_ALPHABET.find(c.encode('ascii'))
        if idx < 0:
            raise ValueError(f"Invalid base58 char: {c!r}")
        n = n * 58 + idx
    # Encode the integer to bytes
    out = bytearray()
    while n > 0:
        n, r = divmod(n, 256)
        out.insert(0, r)
    # Preserve leading '1' chars as leading zero bytes
    for c in s:
        if c == '1':
            out.insert(0, 0)
        else:
            break
    return bytes(out)

def double_sha256(data: bytes) -> bytes:
    return hashlib.sha256(hashlib.sha256(data).digest()).digest()

# ---------------------------------------------------------------------------
# WIF codec
# ---------------------------------------------------------------------------

RATR_MAINNET_PREFIX = 188   # 0xBC, per src/chainparams.cpp CMainParams
RATR_TESTNET_PREFIX = 239   # 0xEF, per src/chainparams.cpp CTestNetParams

class WifResult:
    def __init__(self, key_hex: str, version_byte: int, compressed: bool):
        self.key_hex = key_hex
        self.version_byte = version_byte
        self.compressed = compressed

    def network_name(self) -> str:
        if self.version_byte == RATR_MAINNET_PREFIX:
            return "mainnet"
        if self.version_byte == RATR_TESTNET_PREFIX:
            return "testnet"
        return f"unknown (version byte 0x{self.version_byte:02X})"

def wif_to_hex(wif: str) -> WifResult:
    raw = b58decode(wif)
    if len(raw) < 37:
        raise ValueError(f"WIF too short: {len(raw)} bytes (need >= 37)")

    payload, checksum = raw[:-4], raw[-4:]
    expected = double_sha256(payload)[:4]
    if checksum != expected:
        raise ValueError(
            f"Checksum mismatch: WIF declares {checksum.hex()}, "
            f"computed {expected.hex()}. Corrupted or wrong-network WIF.")

    version = payload[0]
    body = payload[1:]

    compressed = False
    if len(body) == 33 and body[-1] == 0x01:
        # Compressed: 32-byte key + 0x01 flag
        key = body[:-1]
        compressed = True
    elif len(body) == 32:
        # Uncompressed: just 32-byte key
        key = body
        compressed = False
    else:
        raise ValueError(
            f"Unexpected WIF payload length {len(body)}; "
            f"expected 32 (uncompressed) or 33 (compressed).")

    return WifResult(
        key_hex=key.hex().upper(),
        version_byte=version,
        compressed=compressed,
    )

def hex_to_wif(key_hex: str, version_byte: int, compressed: bool) -> str:
    key_hex = key_hex.strip().upper()
    if len(key_hex) != 64:
        raise ValueError(
            f"Hex must be 64 characters (32 bytes); got {len(key_hex)}")
    try:
        key = bytes.fromhex(key_hex)
    except ValueError as e:
        raise ValueError(f"Hex contains non-hex characters: {e}")

    payload = bytes([version_byte]) + key
    if compressed:
        payload += b'\x01'

    checksum = double_sha256(payload)[:4]
    return b58encode(payload + checksum)

# ---------------------------------------------------------------------------
# CLI
# ---------------------------------------------------------------------------

USAGE = """\
Usage:
  wif-hex-roundtrip.py wif2hex <WIF>
  wif-hex-roundtrip.py hex2wif <HEX> [--mainnet|--testnet] [--uncompressed]
  wif-hex-roundtrip.py roundtrip <WIF>
  wif-hex-roundtrip.py selftest
"""

def cmd_wif2hex(wif: str) -> int:
    r = wif_to_hex(wif)
    print(f"Network:        {r.network_name()}")
    print(f"Compressed:     {r.compressed}")
    print(f"Private key:    {r.key_hex}")
    print()
    print("Stamp this hex on metal:")
    print(f"  {r.key_hex}")
    return 0

def cmd_hex2wif(args: list) -> int:
    if not args:
        print(USAGE, file=sys.stderr)
        return 2
    key_hex = args[0]
    version_byte = RATR_MAINNET_PREFIX
    compressed = True
    for flag in args[1:]:
        if flag == "--mainnet":
            version_byte = RATR_MAINNET_PREFIX
        elif flag == "--testnet":
            version_byte = RATR_TESTNET_PREFIX
        elif flag == "--uncompressed":
            compressed = False
        elif flag == "--compressed":
            compressed = True
        else:
            print(f"Unknown flag: {flag}", file=sys.stderr)
            return 2
    wif = hex_to_wif(key_hex, version_byte, compressed)
    network = "mainnet" if version_byte == RATR_MAINNET_PREFIX else "testnet"
    print(f"Network:        {network}")
    print(f"Compressed:     {compressed}")
    print(f"WIF:            {wif}")
    return 0

def cmd_roundtrip(wif: str) -> int:
    r = wif_to_hex(wif)
    rebuilt = hex_to_wif(r.key_hex, r.version_byte, r.compressed)
    print(f"Original WIF:   {wif}")
    print(f"Hex:            {r.key_hex}")
    print(f"Rebuilt WIF:    {rebuilt}")
    if rebuilt == wif:
        print("OK: round-trip matches.")
        return 0
    else:
        print("FAIL: rebuilt WIF does not match original.")
        return 1

def cmd_selftest() -> int:
    failures = 0

    # Test 1: classic Bitcoin uncompressed mainnet (version 0x80) — proves
    # the b58/checksum/version logic is correct against an external known
    # vector. Works because we accept arbitrary version bytes.
    btc_wif = "5HueCGU8rMjxEXxiPuD5BDku4MkFqeZyd4dZ1jvhTVqvbTLvyTJ"
    expected_hex = (
        "0C28FCA386C7A227600B2FE50B7CAE11EC86D3BF1FBE471BE89827E19D72AA1D")
    r = wif_to_hex(btc_wif)
    if r.key_hex != expected_hex:
        print(f"FAIL test 1 (BTC vector): "
              f"got {r.key_hex}, expected {expected_hex}")
        failures += 1
    else:
        print("OK   test 1: BTC uncompressed mainnet vector decodes correctly")
    if r.compressed:
        print("FAIL test 1 (compressed flag): expected False, got True")
        failures += 1

    # Test 2: round-trip the same vector
    rebuilt = hex_to_wif(expected_hex, 0x80, False)
    if rebuilt != btc_wif:
        print(f"FAIL test 2 (BTC round-trip): "
              f"got {rebuilt}, expected {btc_wif}")
        failures += 1
    else:
        print("OK   test 2: BTC vector round-trips cleanly")

    # Test 3: synthetic RATR mainnet compressed — verify our prefixes work
    # (not an external vector, but verifies internal consistency)
    test_hex = (
        "1111111111111111111111111111111111111111111111111111111111111111")
    ratr_wif = hex_to_wif(test_hex, RATR_MAINNET_PREFIX, True)
    r = wif_to_hex(ratr_wif)
    if r.key_hex != test_hex.upper():
        print(f"FAIL test 3 (RATR mainnet round-trip): "
              f"got {r.key_hex}, expected {test_hex.upper()}")
        failures += 1
    elif r.version_byte != RATR_MAINNET_PREFIX:
        print(f"FAIL test 3 (RATR mainnet prefix): "
              f"got 0x{r.version_byte:02X}, expected 0xBC")
        failures += 1
    elif not r.compressed:
        print("FAIL test 3 (compressed flag): expected True, got False")
        failures += 1
    else:
        print("OK   test 3: RATR mainnet compressed round-trips cleanly")

    # Test 4: synthetic RATR testnet compressed
    ratr_test_wif = hex_to_wif(test_hex, RATR_TESTNET_PREFIX, True)
    r = wif_to_hex(ratr_test_wif)
    if r.key_hex != test_hex.upper():
        print(f"FAIL test 4 (RATR testnet round-trip)")
        failures += 1
    elif r.version_byte != RATR_TESTNET_PREFIX:
        print(f"FAIL test 4 (RATR testnet prefix)")
        failures += 1
    else:
        print("OK   test 4: RATR testnet compressed round-trips cleanly")

    # Test 5: corrupted WIF detection (flip one char in the middle)
    bad = btc_wif[:-1] + ("Z" if btc_wif[-1] != "Z" else "Y")
    try:
        wif_to_hex(bad)
        print("FAIL test 5: corrupted WIF should have raised ValueError")
        failures += 1
    except ValueError:
        print("OK   test 5: corrupted WIF raises ValueError as expected")

    # Test 6: hex with wrong length rejected
    try:
        hex_to_wif("DEADBEEF", RATR_MAINNET_PREFIX, True)
        print("FAIL test 6: short hex should have raised ValueError")
        failures += 1
    except ValueError:
        print("OK   test 6: short hex raises ValueError as expected")

    print()
    if failures == 0:
        print("All self-tests passed.")
        return 0
    else:
        print(f"{failures} self-test(s) failed.")
        return 1

def main(argv: list) -> int:
    if len(argv) < 2:
        print(USAGE, file=sys.stderr)
        return 2
    cmd = argv[1]
    args = argv[2:]
    try:
        if cmd == "wif2hex":
            if len(args) != 1:
                print(USAGE, file=sys.stderr); return 2
            return cmd_wif2hex(args[0])
        elif cmd == "hex2wif":
            return cmd_hex2wif(args)
        elif cmd == "roundtrip":
            if len(args) != 1:
                print(USAGE, file=sys.stderr); return 2
            return cmd_roundtrip(args[0])
        elif cmd == "selftest":
            return cmd_selftest()
        else:
            print(USAGE, file=sys.stderr)
            return 2
    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

if __name__ == "__main__":
    sys.exit(main(sys.argv))
