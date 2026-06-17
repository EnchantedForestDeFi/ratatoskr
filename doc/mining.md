# Mining Ratatoskr (RATR)

Ratatoskr uses the **yespower** proof-of-work algorithm, which is designed for
CPUs — no ASICs, no GPUs required (and neither would provide a speedup worth
the complexity). Any modern CPU can mine.

**Algorithm parameters (for miner software):**
- `yespower` (version 1.0)
- `N = 2048`
- `R = 32`
- No personalization string

**Block time:** 60 seconds (target)
**Block subsidy at launch:** 50 RATR per block (halving every 1,030,596 blocks)
**Max supply:** 100,000,000 RATR
**Miner share:** 90% of the subsidy at launch (~45 RATR) — 10% funds the treasury from block 1. From block 25,000, masternodes take 30%, leaving miners 60% (~30 RATR). See the [whitepaper](whitepaper.md) for the full emission schedule.

**Status:** Mainnet live since 2026-06-01. All endpoints below are operational.

---

## Two ways to mine

| Path | Best for | Setup difficulty |
|---|---|---|
| [Pool mining (PPLNS)](#1-pool-mining-pplns) | Steady payouts, small CPUs | Easy |
| [Pool solo mode](#2-pool-solo-mode) | Solo rewards without running your own pool | Easy |

> **Note:** there is no built-in daemon miner. Modern Dash-derived clients
> (including Ratatoskr) don't ship the legacy `setgenerate` RPC — it was
> removed years ago. For all mining, point an external CPU miner
> (`cpuminer-opt` is the standard) at a pool's stratum endpoint.

---

## 1. Pool mining (PPLNS)

Use a pool operator's stratum server. You contribute hash rate; pool splits
block rewards proportionally among miners.

### Requirements
- `cpuminer-opt` (prebuilt Windows/Linux binary from
  <https://github.com/JayDDee/cpuminer-opt/releases>)
- A Ratatoskr wallet address (create one with `ratatoskr-qt` or
  `ratatoskr-cli getnewaddress`)

### EnchantedForestDeFi pool endpoints

| Region | Stratum host | PPLNS port | SOLO port | Pool fee |
|---|---|---|---|---|
| 🇪🇺 EU | `pool-eu.ratatoskr.enchantedforestdefi.com` | 3035 | 3036 | 1.5% PPLNS / 2.0% SOLO |
| 🇺🇸 US | `pool-us.ratatoskr.enchantedforestdefi.com` | 3035 | 3036 | same |
| 🌏 Asia | `pool-asia.ratatoskr.enchantedforestdefi.com` | 3035 | 3036 | same |

Pick the region closest to you. **Community-run pools** also mine RATR (same algorithm + miner, different stratum) — more pools = a healthier network:
- **AriaPool** — `pool.ariabrain.com` PPLNS port `3335`, 1% fee — <https://pool.ariabrain.com/ratr.html>
- **rplant** (multi-coin) — pick Ratatoskr from the list — <https://pool.rplant.xyz/>

### Windows

Download cpuminer-opt Windows release, extract, create `start-mining.bat`:

```bat
cpuminer.exe -a yespower -o stratum+tcp://pool-eu.ratatoskr.enchantedforestdefi.com:3035 -u YOUR_RATR_ADDRESS -p x -t 4
```

Replace:
- `YOUR_RATR_ADDRESS` with your R-prefixed address (e.g. `RKexdLvbo5W9...`)
- `-t 4` with your thread count (usually = CPU core count)

Double-click the .bat file to start.

### Linux

```bash
./cpuminer -a yespower \
  -o stratum+tcp://pool-eu.ratatoskr.enchantedforestdefi.com:3035 \
  -u YOUR_RATR_ADDRESS \
  -p x \
  -t $(nproc)
```

---

## 2. Pool solo mode

Same `cpuminer-opt` command, different port. SOLO mode: you keep the miner's
share of the block (~45 RATR at launch, ~30 RATR after block 25,000 when
masternodes take their 30%) when you find a block. Variance is high — small
CPUs may wait days between finds. For predictable income, use PPLNS.

```bash
cpuminer -a yespower \
  -o stratum+tcp://pool-eu.ratatoskr.enchantedforestdefi.com:3036 \
  -u YOUR_RATR_ADDRESS \
  -p x \
  -t $(nproc)
```

The only change from PPLNS: port `3036` instead of `3035`. The `pool-us.`
and `pool-asia.` endpoints also have port 3036 for SOLO.

---

## Choosing threads (`-t`)

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
is the dominant variable for profitability — see the EnchantedForestDeFi
Discord for community calculators.

---

## What addresses look like

Ratatoskr addresses start with `R` (version byte 60). Examples:

- `RKexdLvbo5W9NPfDKphhkTjXUbedx5U8yk`
- `RaJz4qXyTVmeNGh9tqFFh1MUp1s6YG7Cm2`

Don't send to an `S...` address (that's Smartiecoin) or an `X...` address
(that's Dash). Different chains, different networks.

---

## FAQ

**Q: Can I use a GPU?**
A: Not effectively. Yespower's memory access pattern doesn't parallelize well on GPU memory. Any GPU yespower miner you find will be slower than a modern CPU. ASIC mining is structurally impossible by design (yespower's memory-hardness defeats efficient ASIC implementation — that's the whole point).

**Q: Can I use the daemon's built-in miner (`setgenerate`)?**
A: No — that RPC was removed in modern Dash-derived clients, including Ratatoskr. If you find a guide or video that says to use `setgenerate true`, the guide is outdated. Use `cpuminer-opt` against any of the pool endpoints above (SOLO port for solo-style mining).

**Q: Will mining damage my CPU?**
A: Mining is intensive but not unusual. Modern CPUs are designed to run at full load indefinitely as long as cooling is adequate. Keep an eye on temperatures (under ~80°C is comfortable for most chips); if you're hitting thermal limits, reduce thread count or add airflow. Laptops are the worst case — they're not built for sustained 100% CPU load.

**Q: What's the difference between pool mining and solo mining?**
A: Pool mining (PPLNS) = smaller, predictable, frequent payouts (you get a share of every block the pool finds, proportional to your contribution). Solo mining (pool SOLO port) = lottery-style — you keep the block's miner share (~45 RATR at launch, ~30 RATR after block 25,000) of every block you find, but blocks may be days or weeks apart on a small CPU.

**Q: How much RATR will I earn per day?**
A: Depends on network hashrate, your CPU hashrate, and the block reward. At launch with low hashrate, hobbyist miners can earn meaningfully — as more miners join, share-per-CPU drops. Pool dashboards publish per-worker hashrate and estimated payouts.

**Q: Can I mine on a VPS?**
A: Technically yes, but most VPS providers prohibit cryptocurrency mining in their TOS and will terminate your account. Read the fine print. Hetzner, OVH, and most major providers explicitly ban it. If you do mine on a VPS, it's at your own risk.

**Q: Where can I check if I'm mining successfully?**
A: For pool mining: the pool's stats page will show your worker hashrate and shares submitted. SOLO uses the same dashboard. You can also run `ratatoskr-cli getmininginfo` against your local daemon to see current network difficulty and total network hashrate, and `ratatoskr-cli listtransactions` to see any block rewards as they land.

**Q: Is there a minimum to start mining?**
A: No minimum. Any modern computer with adequate cooling can mine. For pool mining, pools typically have a minimum payout threshold (commonly 0.5–1.0 RATR) before they release accumulated balance — pool operator publishes this.

---

## Support

- Discord: EnchantedForestDeFi server, channel `#ratr-mining`
- GitHub issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
- Pool dashboard (EU): <https://pool-eu.ratatoskr.enchantedforestdefi.com/>

## See also

- [`doc/pool-operator-spec.md`](pool-operator-spec.md) — for operators standing up their own pool
- [`README.md`](../README.md) — project overview and wallet downloads
- [`doc/whitepaper.md`](whitepaper.md) §2.6 (Accessible Mining) — design rationale for yespower

**Want to run a masternode instead of mining?** Two paths:

- **Hosted (easiest):** Nodes24.online — $1.75/month, non-custodial, your 7,500 RATR collateral stays in your wallet. <https://nodes24.online/supported-coins/RATR/>
- **Self-host:** see `doc/masternode-setup.md` (VPS + ratatoskrd + ProRegTx).
