# Ratatoskr Quickstart

**Download, unzip, mine. 5 minutes.**

Mainnet has been live since **2026-06-01 00:00 UTC**. Pool endpoints below are operational.

This is the one-page version. The full details are in [`mining.md`](mining.md)
(miners), [`masternode-setup.md`](masternode-setup.md) (MN operators), and
[`whitepaper.md`](whitepaper.md) (everyone else).

---

## 1. Grab the binary

Latest release: <https://github.com/EnchantedForestDeFi/ratatoskr/releases/latest> (**v1.0.2**)

- **Windows:** `ratatoskr-1.0.2-win64-setup-unsigned.exe` (installer) or `ratatoskr-1.0.2-win64.zip` (portable — extract with 7-Zip or Windows built-in).
- **Linux:** `ratatoskr-1.0.2-x86_64-linux-gnu.tar.gz`.
  ```bash
  tar xzf ratatoskr-1.0.2-x86_64-linux-gnu.tar.gz
  cd ratatoskr-1.0.2-x86_64-linux-gnu
  ```
- **macOS:** `ratatoskr-1.0.2-arm64-apple-darwin.tar.gz` (Apple Silicon) or `ratatoskr-1.0.2-x86_64-apple-darwin.tar.gz` (Intel) — grab the matching one.

**Verify before you run it.** The release ships one `SHA256SUMS` plus a GPG signature `SHA256SUMS.asc`:
```bash
# 1. checksum your download against the manifest
sha256sum --ignore-missing -c SHA256SUMS
# 2. confirm the manifest is genuinely ours (key 50B8E0E4EADA2FF29B1DE68F7A33CFF563BB2E0A · releases@enchantedforestdefi.com)
gpg --verify SHA256SUMS.asc SHA256SUMS
```

---

## 2. Run the wallet

**GUI (easiest):** double-click `ratatoskr-qt` (Linux) or `ratatoskr-qt.exe` (Windows).

**Headless:**
```bash
./bin/ratatoskrd -daemon
./bin/ratatoskr-cli getblockchaininfo
```

Data dir is `~/.ratatoskrcore` (Linux) or `%APPDATA%\RatatoskrCore` (Windows).

---

## 3. Get an address

**GUI:** menu **File → Receiving addresses → New**. Addresses start with `R`.

**CLI:**
```bash
./bin/ratatoskr-cli getnewaddress
```

---

## 4. Mine

**Pool — PPLNS** (steady payouts, port 3035):
```bash
cpuminer -a yespower -o stratum+tcp://pool-eu.ratatoskr.enchantedforestdefi.com:3035 -u YOUR_RATR_ADDRESS -p x
```

**Pool — SOLO** (lottery — the block's miner share when you find one, port 3036):
```bash
cpuminer -a yespower -o stratum+tcp://pool-eu.ratatoskr.enchantedforestdefi.com:3036 -u YOUR_RATR_ADDRESS -p x
```

US: `pool-us.ratatoskr.enchantedforestdefi.com` · Asia: `pool-asia.ratatoskr.enchantedforestdefi.com` — same ports (3035 PPLNS / 3036 SOLO). Full list + community pools in [`mining.md`](mining.md).

Modern Dash-derived clients don't have a built-in daemon miner — the legacy `setgenerate` RPC was removed years ago. All mining goes through `cpuminer-opt` against a pool stratum. See [`mining.md`](mining.md) for the full guide.

---

## 5. That's it

Questions: Discord `#ratatoskr` or <https://github.com/EnchantedForestDeFi/ratatoskr/issues>.

Mainnet has been live since **2026-06-01 00:00 UTC**. `getblockcount` should
report the current tip height; if it stays at 0, your daemon hasn't connected
to peers yet (see [`mining.md`](mining.md) FAQ for sync troubleshooting).
