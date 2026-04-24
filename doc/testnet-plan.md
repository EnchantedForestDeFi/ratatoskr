# Ratatoskr Testnet Plan

**Purpose:** surface consensus bugs + give pool ops a target before mainnet
launch 2026-06-01.

**Duration:** 2026-05-15 through 2026-05-28 (13 days, ending 3 days before
mainnet so we have time to react to anything found).

**Goal output:**
1. Confirm treasury-drip consensus works across a multi-node network
2. Confirm MN payment activation at testnet block 1,000 works
3. Give at least one pool operator time to spin up stratum against testnet
4. Catch anything we missed

---

## Testnet chainparams

Defined in `src/chainparams.cpp` under `CTestNetParams`:

- Network magic: different from mainnet (inherited from Dash testnet tradition)
- P2P port: **19393** (1 off from mainnet 9393)
- Address prefix: **140** (`y...`-style)
- Genesis nTime: updated to `1779609600` (2026-05-15 UTC)
- Treasury drip: **active from block 1** (same placeholder burn script)
- MN payments start: **block 1,000** (compressed from mainnet's 25,000)
- Superblock start: **block 2,000**
- Block time: 60 seconds (same as mainnet)

---

## Timeline

### T-14 days (2026-05-11): testnet genesis

- [ ] Regenerate testnet genesis block with new nTime
- [ ] Bake into `CTestNetParams`
- [ ] Commit + push, rebuild tarballs, upload to v1.0.0-alpha.2 release
- [ ] Publish a testnet bootstrap file (empty is fine — fresh chain)

### T-7 days (2026-05-15 00:00 UTC): testnet launch

- [ ] Spin up **testnet seed1** on VPS1 (new data dir, different ports)
- [ ] Spin up **testnet seed2** on VPS2 (same)
- [ ] Internal miner on one of them to find block 1
- [ ] Post in Discord: "testnet live, anyone can point their binary at
      `-testnet` and start mining"
- [ ] Coordinate with pool op: stratum-to-testnet smoke test

### T-0 days through T+7 days (2026-05-15 → 2026-05-22)

Watch for:
- Treasury output at every block (5 tRATR per block)
- Block-time stability (60s target)
- Seed-to-seed peering
- MN payment activation at block 1,000 (~17 hr after genesis)
- Any consensus rule rejections (`bad-cb-*`, `bad-treasury-*`)

### T+13 days (2026-05-28): testnet wind-down

- [ ] Compile a "what we found" list
- [ ] If blockers: patch, rebuild, retest before mainnet
- [ ] If clean: post Discord "testnet validated, mainnet launch still 2026-06-01"

### T+17 days (2026-06-01 00:00 UTC): mainnet launch

Per `doc/launch-day-runbook.md`.

---

## How users + pools participate

```bash
# Run daemon on testnet
./bin/ratatoskrd -testnet -daemon
./bin/ratatoskr-cli -testnet getblockchaininfo
./bin/ratatoskr-cli -testnet getnewaddress    # yields y... testnet address

# Pool operators: same pool software, different chainparams, new seed nodes
addnode=seed1.ratatoskr.enchantedforestdefi.com
addnode=seed2.ratatoskr.enchantedforestdefi.com
# (seeds serve both mainnet on 9393 and testnet on 19393 via -testnet flag)
```

---

## Is testnet required?

**No, but strongly recommended.** A week of testnet catches most consensus
bugs that would otherwise show up at block 2 of mainnet. The alternative —
launching mainnet cold — is how Smartiecoin v1 died (no public testnet,
bugs found at the worst time).

**Cost:** VPS1 + VPS2 already have capacity; 2 weeks of testnet is
essentially free, just operator attention.

**Risk if skipped:** a consensus bug at mainnet block N forces us to either
(a) accept the bug and hard-fork later, (b) halt + restart mainnet, or
(c) publish a fix + ask everyone to re-sync — all bad outcomes that
testnet would have caught cheaply.

---

## Open decision

Do we launch testnet? **Yes** unless there's a reason not to. The only
reasons to skip:
1. We don't have time to prep (implausible — we have 5 weeks)
2. We think the bug surface is essentially zero (risky given no external audit)
3. We want to conserve attention for mainnet prep (real but small)

Recommendation: **go testnet 2026-05-15**, spend 10 min/day watching it,
call it validated or catch something.
