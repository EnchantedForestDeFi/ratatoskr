# Mining Ratatoskr (RATR)

Ratatoskr uses the **yespower** proof-of-work algorithm, which is designed for
CPUs — no ASICs, no GPUs required (and neither would provide a speedup worth
the complexity). Any modern CPU can mine.

**Algorithm parameters (for miner software):**
- `yespower` (version 1.0)
- `N = 2048`
- `R = 32`
- No personalization string

**Block time:** 60 seconds
**Block reward at launch:** 50 RATR (halving every 1,030,596 blocks)
**Max supply:** 100,000,000 RATR

---

## Three ways to mine

| Path | Best for | Setup difficulty |
|---|---|---|
| [Pool mining](#1-pool-mining) | Steady payouts, small CPUs | Easy |
| [Pool solo mode](#2-pool-solo-mode) | Solo rewards without running a node | Easy |
| [Daemon internal miner](#3-daemon-internal-miner-solo) | Zero external dependencies | Easiest |

---

## 1. Pool mining

Use a pool operator's stratum server. You contribute hash rate; pool splits
block rewards proportionally among miners.

### Requirements
- `cpuminer-opt` (prebuilt Windows/Linux binary from
  <https://github.com/JayDDee/cpuminer-opt/releases>)
- A Ratatoskr wallet address (create one with `ratatoskr-qt` or
  `ratatoskr-cli getnewaddress`)
- Pool URL + port (TBD — pool operator will publish these at launch)

### Windows

Download cpuminer-opt Windows release, extract, create `start-mining.bat`:

```bat
cpuminer.exe -a yespower -o stratum+tcp://POOL_URL:PORT -u YOUR_RATR_ADDRESS -p x -t 4
```

Replace:
- `POOL_URL:PORT` with your pool's stratum endpoint
- `YOUR_RATR_ADDRESS` with your R-prefixed address (e.g. `RKexdLvbo5W9...`)
- `-t 4` with your thread count (usually = CPU core count)

Double-click the .bat file to start.

### Linux

```bash
./cpuminer -a yespower \
  -o stratum+tcp://POOL_URL:PORT \
  -u YOUR_RATR_ADDRESS \
  -p x \
  -t $(nproc)
```

---

## 2. Pool solo mode

Some pools offer a "solo" port where you keep the full block reward if you
find it. Pool handles block construction, you contribute hashrate. Same
cpuminer-opt command, different port:

```
stratum+tcp://POOL_URL:SOLO_PORT
```

Pool operator will publish the solo endpoint alongside the pool endpoint.

---

## 3. Daemon internal miner (solo)

No external miner, no pool — the Ratatoskr daemon mines against itself.

### Via Qt wallet (easiest)

1. Download `ratatoskr-qt` from the latest release
2. Launch it, let it sync (on day 1 this is instant — just the genesis block)
3. Menu: **Tools → Debug Console**
4. Type and run:
   ```
   setgenerate true 4
   ```
   (`4` = number of CPU threads; use whatever you want to dedicate)
5. Mining starts immediately. Check status:
   ```
   getmininginfo
   ```
6. To stop:
   ```
   setgenerate false
   ```

Rewards land in the wallet automatically and appear in the overview balance.

### Via command line (headless)

Start the daemon with mining already on:

```bash
ratatoskrd -server -gen=1 -genproclimit=4
```

Or while running, use the CLI:

```bash
ratatoskr-cli setgenerate true 4
ratatoskr-cli getmininginfo
ratatoskr-cli setgenerate false
```

### Why the internal miner?

- **Simplest**: one command, no separate binary
- **Self-contained**: no pool URL, no account, no stratum dependency
- **Reliable**: daemon constructs its own blocks (correct Dash-style coinbase
  with masternode / treasury splits baked in)
- **Full reward**: every block you find pays the full 50 RATR to your wallet

### Drawbacks

- **~20-30% slower** than optimized external miners (daemon isn't tuned for
  raw hash rate like cpuminer-opt)
- **Variance**: you might wait days between finds if you have a small CPU.
  For predictable income, use the pool instead.

---

## Choosing threads (`-t` or `-genproclimit`)

- **Rule of thumb:** set to number of physical cores (not hyperthreads)
- **If you use the machine for other things:** leave 1-2 cores free
- **Too many threads:** you'll hit memory bandwidth limits and gain nothing
  (yespower is memory-hard)

---

## What addresses look like

Ratatoskr addresses start with `R` (version byte 60). Examples:

- `RKexdLvbo5W9NPfDKphhkTjXUbedx5U8yk`
- `RaJz4qXyTVmeNGh9tqFFh1MUp1s6YG7Cm2`

Don't send to an `S...` address (that's Smartiecoin) or an `X...` address
(that's Dash). Different chains, different networks.

---

## Launch timing

Mainnet launches **June 1, 2026**. Before launch:

- Network isn't live yet
- Block 1 won't exist until the first miner finds it after launch
- Don't expect to mine "early blocks" — the chain starts at 0 on June 1

---

## Support

- Discord: (EnchantedForestDeFi server, see project site)
- GitHub issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
- Pool operator contact: TBD (published at launch)
