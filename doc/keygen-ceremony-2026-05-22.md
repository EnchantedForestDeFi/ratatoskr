# RATR Mainnet Keygen Ceremony Results — Session 3 (T-10)

**Date:** 2026-05-22
**Status:** ✅ Phases 6 + 7 complete (treasury + spork keypairs generated, paper-backed, signed)
**Air-gap:** HP EliteBook 8440p, Ubuntu Live USB, no network
**Daemon:** Mainnet chainparams (`chain: main`, genesis `00001059e1c1b34440ea46fcc1a3deb84e827f061ecf06f5013056219a66a4df`)

---

## PUBLIC ARTIFACTS (safe to publish in launch announcement)

### Treasury (chain-funded 10% drip recipient)

```
Address:       RDt67AAyyqAG3u7h2STJ5kvfzbuiTUa9WN
Pubkey:        025c3cd5942a0592d813450cc51a6bbabe8f68758719fdee990fba044b0aea540f
ScriptPubKey:  76a914327a13af66075bbf182c55393106befbb607d0ac88ac
HD path:       m/44'/530'/0'/0/0
HD fingerprint: aba6f586
Sign text:     RATR-Treasury-v1.0 2026-05-22
Signature:     HyUZyTBe/+yPdYaURcBjM9FcbFCO27ehxIXFaNvNc2qJRBMAJYCzq4TRBb80Pl9bebD5Z6jLYwiiCddC9m7MUTg=
```

**The scriptPubKey above is what goes into `chainparams.cpp::CMainParams::treasuryPaymentScript` for rc4.** Decoded: P2PKH with hash160 = `327a13af66075bbf182c55393106befbb607d0ac`.

Verification by anyone post-launch:
```
ratatoskr-cli verifymessage RDt67AAyyqAG3u7h2STJ5kvfzbuiTUa9WN "HyUZyTBe/+yPdYaURcBjM9FcbFCO27ehxIXFaNvNc2qJRBMAJYCzq4TRBb80Pl9bebD5Z6jLYwiiCddC9m7MUTg=" "RATR-Treasury-v1.0 2026-05-22"
```
Returns `true` if the signature is valid for the address + message.

### Spork (signs SPORK_x mainnet messages)

```
Address:       RYJoaj2cuAo4ApV4fiJds2Umnumhzrxvhg
Pubkey:        02f6ace2ee822f8b99591b025796c40e237fc3e48511334b9b2c15f568b7761ab1
ScriptPubKey:  76a914fc98f22723714695bcb7e296e950be2fef869f8788ac
HD path:       m/44'/530'/0'/0/0
HD fingerprint: 7955b444
Sign text:     RATR-Spork-v1.0 2026-05-22
Signature:     IEV8eyWTsimX8gPI40NP8h4SGYGQWockdwQ1Q3ukSWNEDU3tpLUhp71pWaj3rV083ma1OT8cfYwGUD+fMERH7zE=
```

**The pubkey above is what goes into `chainparams.cpp::CMainParams::strSporkPubKey` for rc4.**

Verification:
```
ratatoskr-cli verifymessage RYJoaj2cuAo4ApV4fiJds2Umnumhzrxvhg "IEV8eyWTsimX8gPI40NP8h4SGYGQWockdwQ1Q3ukSWNEDU3tpLUhp71pWaj3rV083ma1OT8cfYwGUD+fMERH7zE=" "RATR-Spork-v1.0 2026-05-22"
```

---

## PRIVATE ARTIFACTS (NEVER touch any networked machine)

For both keypairs, paper backup contains:
- WIF (52-char base58, starts with `U` for RATR mainnet, version byte 0xBC=188)
- HEX (64-char uppercase, the 32-byte raw private key)

Storage plan (per `ratr_keygen_bundle_usb_2026_05_18.md` Phase 10-11):
- Paper backups → notebook → 2 physically separated locations
- HEX → 9 metal washers per key × 2 keys = 18 washers total, stamped post-ceremony, distributed across same 2 locations

---

## Chain integration (post-ceremony, online machine)

The spork PUBLIC key must be baked into `chainparams.cpp::CMainParams::strSporkPubKey` before T-7 founder phase. Get the pubkey via:

```bash
# On air-gap (last command before Phase 9 shutdown):
ratatoskr-cli -datadir=/tmp/keygen-data -rpcwallet=spork getaddressinfo $SPORK_ADDR
# JSON output's "pubkey" field is the 66-char (compressed) hex pubkey — that's what goes in chainparams
```

The treasury ADDRESS (not pubkey) goes into `chainparams.cpp::CMainParams::treasuryPaymentScript` as a P2PKH script (currently OP_RETURN placeholder per `ratr_miningcore_op_return_burn_patch.md`). Specifically:

```cpp
// Replace this placeholder:
// consensus.treasuryPaymentScript = CScript() << OP_RETURN << ParseHex("52415452");
// With this real P2PKH (decoded from treasury address):
// consensus.treasuryPaymentScript = GetScriptForDestination(DecodeDestination("RDt67AAyyqAG3u7h2STJ5kvfzbuiTUa9WN"));
```

Both changes ride into rc4 binary + Linux/Windows release builds before T-7 (founder phase) on 2026-05-25.

---

## Ceremony lessons (for next-time / next-ceremony reference)

1. **The README's Phase 5 conf had testnet-only assumption** (`connect=0` at top level). Mainnet rejects this in strict-mode startup. Correct mainnet conf:
   ```
   rpcuser=k
   rpcpassword=k
   listen=0
   dnsseed=0
   ```
   Sandbox May 18 only tested testnet mode; mainnet path was first-touched on the air-gap during real ceremony.

2. **Bitcoin "L/K" WIF prefix assumption was wrong for RATR.** RATR mainnet `SECRET_KEY = 188 (0xBC)` produces WIFs starting with **`U`**. Should have been documented up-front; instead operator was alarmed mid-ceremony when their WIF didn't start with L/K.

3. **`dumpprivkey` syntax requires space + `$` substitution.** Multiple typo-retries lost time. The README should have used a one-liner that doesn't rely on variable substitution being typed perfectly.

4. **Air-gap typing is significantly slower than expected.** 5-min phase estimates from sandbox-time should be doubled (~10 min real) for actual ceremony.

5. **`wif2hex` script command had an "invalid base58 char '0'" error** that the operator hit on spork (not treasury). Cause unclear — possibly a leading-zero handling issue. The roundtrip command's output already contains the hex, so wif2hex is redundant. Future README should drop the wif2hex step entirely.

6. **`signmessage` requires `$` before variable.** Two retries lost time. Same lesson as #3.

These all go into a revised README for any future keygen ceremony (RATR v1.1, RATR fork-chains, or other coins reusing this playbook).

---

## Cross-references

- `ratr_keygen_bundle_usb_2026_05_18.md` — USB bundle staging
- `ratr_treasury_spork_keygen_ops.md` — ceremony theory predecessor
- `ratr_mainnet_block1_soft_launch_plan.md` — T-7 founder phase, needs chainparams update
- `MEMORY.md` — pinned constants block lists treasury/spork integration points
- `efd_treasury_design_doctrine.md` — treasury-as-policy doctrine
