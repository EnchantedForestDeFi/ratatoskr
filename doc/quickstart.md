# Ratatoskr Quickstart

**Download, unzip, mine. 5 minutes.**

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

**Pool** (steady payouts):
```bash
cpuminer -a yespower -o stratum+tcp://POOL_URL:PORT -u YOUR_RATR_ADDRESS -p x
```
Pool URLs will be posted in Discord `#ratatoskr` when live.

**Solo** (own your blocks, daemon mines against itself):

- GUI: Tools → Debug Console → `setgenerate true 4`
- Headless: `./bin/ratatoskr-cli setgenerate true 4`

`4` = CPU threads. Stop with `setgenerate false`.

---

## 5. That's it

Questions: Discord `#ratatoskr` or <https://github.com/EnchantedForestDeFi/ratatoskr/issues>.

Mainnet launches **2026-06-01 00:00 UTC**. Before that, the chain isn't live —
you can install now but `getblockcount` will stay at 0 until launch.
