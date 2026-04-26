# Ratatoskr Testnet Plan

**Purpose:** validate consensus, masternode formation, sporks, treasury drip,
reorg recovery, wallet flows, RPC surface, and the bridge round-trip on a
real-world multi-node network before mainnet launches 2026-06-01.

**Duration:** 27 days, **2026-05-02 through 2026-05-29** (T-30 → T-3 relative
to mainnet launch).

**Mode:** **Solo operator testnet.** No public testnet, no external alpha
tester program. The operator runs all testnet nodes, mines testnet blocks,
registers testnet MNs, and exercises every consensus path personally.

---

## What solo testnet covers — and what it doesn't

A solo testnet captures roughly 70% of the consensus surface area. The
remaining ~30% — specifically `LLMQ_50_60` quorum formation, ChainLocks
under realistic node distribution, and InstantSend at scale — depend on
having ≥50 geographically distributed MNs from independent operators.
Those paths are deferred to mainnet, with mitigations.

### Validated by solo testnet (criteria 1, 2, 4, 7, 8, 9, 10, 11)

| # | Criterion | Solo-validatable? |
|---|---|---|
| 1 | Continuous block production for ≥14 days | ✅ |
| 2 | ≥3 difficulty retargets across hashrate regimes | ✅ |
| 4 | All 7 sporks toggle via spork key | ✅ |
| 7 | Treasury drip pays test treasury at expected heights | ✅ |
| 8 | One forced reorg recovers cleanly | ✅ |
| 9 | Wallet flows: send / receive / MN register / encrypt / restore | ✅ |
| 10 | RPC API: all RATR-specific calls return correctly | ✅ |
| 11 | wRATR bridge mint+burn round-trip on testnet | ✅ |

### Partially validated by solo testnet (criteria 3, 5, 6)

| # | Criterion | What gets validated solo |
|---|---|---|
| 3 | LLMQ quorum formation | `LLMQ_10_60` and `LLMQ_10_75` (smaller tiers). Operator runs 10-15 testnet MNs across 3-5 VPS, sufficient for small quorums to form. **`LLMQ_50_60` is not validated** — will form on mainnet only after ≥50 real MNs register. |
| 5 | ChainLocks active and firing | ChainLocks are produced by `LLMQ_50_60` per RATR's tightened quorum tier. **Cannot be solo-validated.** Risk accepted; spork-based emergency disable available if bug surfaces post-launch. |
| 6 | InstantSend lock + double-spend reject | Same dependency on `LLMQ_50_60`. **Cannot be solo-validated.** Same risk acceptance. |

### Not validated by solo testnet (criterion 12)

| # | Criterion | Note |
|---|---|---|
| 12 | Mining pool stratum payout cycle | Requires a pool operator. Operator can run pool software locally for self-test, but doesn't simulate real-world stratum traffic. Marked as deferred to T+0 onward; first mainnet pool that integrates is the real test. |

### Risk acceptance note

`LLMQ_50_60` formation, ChainLocks behavior, and InstantSend at scale are
the three paths that go from "untested" to "production" on T-0. Mitigations:

- The `LLMQ_50_60` algorithm is shared upstream code (Dash's `LLMQ_400_60`
  with adjusted size and threshold parameters). The algorithm has years of
  production history at the `LLMQ_400_*` tier; the parameter change is the
  delta under test.
- Sporks `SPORK_19_CHAINLOCKS_ENABLED` and `SPORK_2_INSTANTSEND_ENABLED`
  remain disabled at mainnet T-0 and only flip on after the first quorum
  forms cleanly (per `doc/spork-launch-timing.md`). If a quorum-formation
  bug appears, the operator can keep these sporks off indefinitely while
  patching.
- The chain remains fully functional under PoW alone if `LLMQ_50_60` never
  forms — security degrades from "PoW + ChainLocks" to "PoW only," which is
  Bitcoin-tier security and acceptable for an alpha-stage chain.

---

## Testnet chainparams

Defined in `src/chainparams.cpp` under `CTestNetParams`:

| Field | Mainnet | Testnet |
|---|---|---|
| Network magic | `ac c0 f0 52` | `ad c1 f1 53` |
| P2P port | 9393 | 19393 |
| Address prefix | 60 (R…) | 140 (y…) |
| Genesis `nTime` | 2026-06-01 00:00 UTC | 2026-05-02 00:00 UTC |
| Treasury drip | block 1 | block 1 |
| MN payments start | block 25,000 | **block 1,000** (compressed) |
| Superblock start | disabled (INT_MAX) | block 2,000 |
| `nSMTSmallQuorumsHeight` | 7,500 | **750** (compressed 10×) |
| Block time | 60s | 60s |
| Spork key | mainnet key | separate testnet spork key |

Compressed activation heights mean testnet exercises every height-gated
consensus rule within the 27-day window without waiting weeks for the
mainnet schedule.

---

## Timeline (solo)

### T-30 (2026-05-02): testnet genesis

- Regenerate testnet genesis block with new `nTime`
- Bake testnet spork key into chainparams
- Build + tag `v1.0.0-alpha.2-testnet` (Linux + Windows)
- Spin up testnet seed1 + seed2 + 3-5 worker nodes for MN-hosting
- Mine block 1; verify treasury output

### T-30 → T-23 (May 2 → May 9): block production + difficulty retarget #1

- Verify difficulty retargets cleanly on the first cycle
- Treasury drip auditing (criterion #7)
- Wallet flow exercise — send, receive, encrypt, restore from seed (criterion #9)

### T-23 → T-14 (May 9 → May 18): MN registration + small-quorum formation

- Operator registers 10-15 testnet MNs across 3-5 VPS (criterion #3 partial)
- Verify `LLMQ_10_60` and `LLMQ_10_75` quorums form at testnet height ~750
- Treasury drip continues (criterion #7 ongoing)
- Difficulty retarget #2

### T-14 → T-7 (May 18 → May 25): full feature exercise

- Cycle all 7 sporks through expected states (criterion #4)
- Run forced reorg test, both 1-block and 6-block (criterion #8)
- Wallet GUI exercise on Windows + Linux builds (criterion #9 GUI)
- RPC API smoke test (criterion #10)
- wRATR bridge mint+burn round-trip on testnet binaries (criterion #11)
- Difficulty retarget #3

### T-7 → T-3 (May 25 → May 29): wind-down + go/no-go

- Each criterion 1-12 marked GREEN, YELLOW, or RED
- YELLOW (partial validation, like LLMQ_50_60) is acceptable for alpha launch
- Any RED → patch, rebuild, retest within window OR delay mainnet
- All GREEN-or-YELLOW → declare testnet validated; freeze mainnet RC

### T-0 (2026-06-01 00:00 UTC): mainnet launch

Mainnet genesis. Testnet remains live for post-launch reference.

---

## How others can join testnet (informal)

Solo testnet is the operator's responsibility, but the testnet is open. Anyone
who wants to follow along can run a testnet node:

```bash
./bin/ratatoskrd -testnet -daemon
./bin/ratatoskr-cli -testnet getblockchaininfo
./bin/ratatoskr-cli -testnet getnewaddress    # returns y... testnet address

# Connect via the testnet seeds:
addnode=seed1.ratatoskr-testnet.enchantedforestdefi.com
addnode=seed2.ratatoskr-testnet.enchantedforestdefi.com
```

Testnet RATR (tRATR) has no monetary value. Anyone running a node helps
testnet diversity (more peers = more realistic propagation). MN registration
is open to anyone with tRATR collateral; reach out via Discord if you want
the operator to fund a testnet MN for you.

---

## Risk if testnet is skipped entirely

A consensus bug at mainnet block N forces one of:

- Accept the bug and hard-fork later
- Halt + restart mainnet
- Publish a fix + ask everyone to re-sync

All three are bad outcomes for an alpha-stage chain. Solo testnet catches
~70% of the surface cheaply; the remaining ~30% is genuinely deferred to
mainnet with explicit mitigations (see "Risk acceptance note" above).

---

## Recommendation

Go testnet **2026-05-02**. The 27-day window fits within the pre-launch
schedule, captures the consensus paths that solo can exercise, and gives
real go/no-go data for the 2026-06-01 mainnet flip on the validated
criteria. The deferred-to-mainnet criteria (LLMQ_50_60, ChainLocks,
InstantSend at scale) ship with explicit risk acceptance and spork-based
fallbacks rather than untested optimism.
