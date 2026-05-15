# Mining Ratatoskr (RATR)

> ⚠️ **Pre-launch — mainnet is not yet live.**
>
> Ratatoskr mainnet launches **2026-06-01 00:00 UTC**. Until then,
> downloading a binary and running it will not earn you any RATR —
> there is no chain to mine against and no pool URLs are live yet.
> Pool operators should read [`doc/pool-operator-spec.md`](pool-operator-spec.md)
> to pre-stage; individual miners should wait for launch.

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

## Hardware expectations

Yespower is memory-hard and CPU-bound. There's no ASIC for it (by design)
and GPU mining isn't worthwhile (the memory access pattern doesn't suit GPU
parallelism). Hashrate scales roughly linearly with CPU core count for
well-cooled chips with adequate memory bandwidth.

| CPU class | Approximate hashrate | Notes |
|---|---|---|
| Modern desktop (Ryzen 5/7, Intel i5/i7 12th gen+) | 5–15 KH/s | Per machine, all cores. Best $/hash for hobbyists. |
| Older desktop (Ryzen 1xxx, Intel i5 8th gen) | 2–6 KH/s | Still viable, particularly with cheap power |
| Laptop CPU | 0.5–3 KH/s | Heat is the bottleneck — sustained mining throttles fast |
| Server CPU (EPYC, Xeon W) | 20–80 KH/s | Best total hashrate if you have one idle |
| ARM (Raspberry Pi, mobile) | 0.05–0.3 KH/s | More for hobby than profit |

**Power draw:** typical yespower mining consumes ~80–90% of TDP on the CPU.
A 65W desktop CPU running flat-out is ~55W under load. Power-bill math
is the dominant variable for profitability after launch — see the EFD
discord for community calculators.

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

## FAQ

**Q: Can I use a GPU?**
A: Not effectively. Yespower's memory access pattern doesn't parallelize well on GPU memory. Any GPU yespower miner you find will be slower than a modern CPU. ASIC mining is structurally impossible by design (yespower's memory-hardness defeats efficient ASIC implementation — that's the whole point).

**Q: Will mining damage my CPU?**
A: Mining is intensive but not unusual. Modern CPUs are designed to run at full load indefinitely as long as cooling is adequate. Keep an eye on temperatures (under ~80°C is comfortable for most chips); if you're hitting thermal limits, reduce thread count or add airflow. Laptops are the worst case — they're not built for sustained 100% CPU load.

**Q: What's the difference between pool mining and solo mining?**
A: Pool mining = smaller, predictable, frequent payouts (you get a share of every block the pool finds, proportional to your contribution). Solo mining = lottery-style — you keep the full ~50 RATR reward of every block you find, but blocks may be days or weeks apart on a small CPU.

**Q: How much RATR will I earn per day?**
A: Depends on network hashrate, your CPU hashrate, and the block reward. At launch with low hashrate, hobbyist miners can earn meaningfully — as more miners join, share-per-CPU drops. Use a pool's calculator (each pool typically publishes one) once mainnet is live.

**Q: Why is the daemon's internal miner slower than cpuminer-opt?**
A: The daemon's `setgenerate` uses a generic mining loop optimized for simplicity, not raw hash throughput. JayDDee's cpuminer-opt has hand-tuned yespower assembly that exploits CPU instruction-level parallelism. For solo mining where speed matters more than convenience, point cpuminer-opt at the daemon's RPC port instead of using `setgenerate`.

**Q: Can I mine on a VPS?**
A: Technically yes, but most VPS providers prohibit cryptocurrency mining in their TOS and will terminate your account. Read the fine print. Hetzner, OVH, and most major providers explicitly ban it. If you do mine on a VPS, it's at your own risk.

**Q: Where can I check if I'm mining successfully?**
A: For pool mining: the pool's stats page will show your worker hashrate and shares submitted. For solo (daemon): run `ratatoskr-cli getmininginfo` to see current difficulty + your hashrate, and `ratatoskr-cli listtransactions` to see any block rewards as they land.

**Q: Is there a minimum to start mining?**
A: No minimum. Any modern computer with adequate cooling can mine. For pool mining, pools typically have a minimum payout threshold (commonly 0.5–1.0 RATR) before they release accumulated balance — pool operator publishes this.

---

## Support

- Discord: (EnchantedForestDeFi server, see project site)
- GitHub issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
- Pool operator contact: TBD (published at launch)

## See also

- [`doc/pool-operator-spec.md`](pool-operator-spec.md) — for operators standing up their own pool
- [`README.md`](../README.md) — project overview and wallet downloads
- [`doc/whitepaper.md`](whitepaper.md) §2.6 (Accessible Mining) — design rationale for yespower
