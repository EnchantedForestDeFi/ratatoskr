# Ratatoskr Quickstart

**Download, unzip, mine. 5 minutes.**

Mainnet has been live since **2026-06-01 00:00 UTC**. Pool endpoints below are operational.

This is the one-page version. The full details are in [`mining.md`](mining.md)
(miners), [`masternode-setup.md`](masternode-setup.md) (MN operators), and
[`whitepaper.md`](whitepaper.md) (everyone else).

---

## 1. Grab the binary

- **Windows:** <https://github.com/EnchantedForestDeFi/ratatoskr/releases/latest>
  — download `ratatoskr-*-win64.tar.gz`, extract with 7-Zip or Windows built-in.
- **Linux:** same release page, grab `ratatoskr-*-linux-x86_64.tar.gz`.
  ```bash
  tar xzf ratatoskr-*-linux-x86_64.tar.gz
  cd ratatoskr-*-linux-x86_64
  ```

Verify the SHA256 before extracting:
```bash
sha256sum -c ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz.sha256
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
cpuminer -a yespower -o stratum+tcp://pool.ratatoskr.enchantedforestdefi.com:3035 -u YOUR_RATR_ADDRESS -p x
```

**Pool — SOLO** (lottery — full block reward when you find one, port 3036):
```bash
cpuminer -a yespower -o stratum+tcp://pool.ratatoskr.enchantedforestdefi.com:3036 -u YOUR_RATR_ADDRESS -p x
```

US endpoint: `pool-us.ratatoskr.enchantedforestdefi.com:3035` (PPLNS) / `:3036` (SOLO).

Modern Dash-derived clients don't have a built-in daemon miner — the legacy `setgenerate` RPC was removed years ago. All mining goes through `cpuminer-opt` against a pool stratum. See [`mining.md`](mining.md) for the full guide.

---

## 5. That's it

Questions: Discord `#ratatoskr` or <https://github.com/EnchantedForestDeFi/ratatoskr/issues>.

Mainnet has been live since **2026-06-01 00:00 UTC**. `getblockcount` should
report the current tip height; if it stays at 0, your daemon hasn't connected
to peers yet (see [`mining.md`](mining.md) FAQ for sync troubleshooting).
