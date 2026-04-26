# Spork launch-timing checklist

Operator-facing reference for **when to flip which sporks** during the
RATR mainnet launch and ongoing operations.

A "spork" is an admin-keyed network message (signed by the operator's
spork private key) that toggles consensus-related features on or off
across the entire network without requiring a code release. Sporks
propagate over the P2P layer in seconds; users do not need to upgrade
their wallets when a spork is flipped.

This file lists exactly what RATR ships with and exactly when to flip
each one.

---

## RATR's spork inventory

Defined in `src/spork.h`. All ship with default value `4070908800`
(Unix epoch year 2099 — convention for "permanently OFF until the
operator decides otherwise").

| Spork | ID | What it gates | Default | Need to flip? |
|---|---|---|---|---|
| `SPORK_2_INSTANTSEND_ENABLED` | 10001 | InstantSend lock signing | OFF | ✅ Once first quorums form |
| `SPORK_3_INSTANTSEND_BLOCK_FILTERING` | 10002 | InstantSend-aware block validation | OFF | ✅ Together with SPORK_2 |
| `SPORK_9_SUPERBLOCKS_ENABLED` | 10008 | Governance superblock payouts | OFF | ❌ **Leave OFF forever** — superblocks are disabled at consensus level (see `chainparams.cpp` mainnet config) |
| `SPORK_17_QUORUM_DKG_ENABLED` | 10016 | Distributed Key Generation sessions | OFF | ✅ **Pre-block-7,500** (DKG is upstream of quorums; without it nothing else works) |
| `SPORK_19_CHAINLOCKS_ENABLED` | 10018 | ChainLocks signing | OFF | ✅ Once first quorums form |
| `SPORK_21_QUORUM_ALL_CONNECTED` | 10020 | Force-all-connected quorum topology | OFF | ✅ Pre-launch — small-network mode |
| `SPORK_23_QUORUM_POSE` | 10022 | Proof-of-Service penalties for offline quorum members | OFF | ✅ A few days after first quorums settle |

## Spork value semantics

The value is interpreted as a Unix timestamp:

- **`value = 0`** → spork is ACTIVE NOW (the feature is on)
- **`value = <future_timestamp>`** → spork will activate at that timestamp
- **`value = 4070908800`** (year 2099) → effectively OFF
- **`value > current_time`** → spork is INACTIVE

So "turn on" = `sporkupdate SPORK_NAME 0`. "Turn off" = `sporkupdate SPORK_NAME 4070908800`.

---

## Pre-launch (T-3 to T-1)

**Verify spork-key infrastructure works before going live.**

1. Generate the spork private key on the air-gapped machine (if not
   already done — should be part of treasury keygen ceremony).
2. Set `-sporkkey=<wif>` and `-sporkaddr=<address>` in the daemon's
   config file on the seed node where you'll be issuing sporks from.
   These should match.
3. Test on testnet first: spin up testnet, broadcast each spork
   you plan to flip, verify it propagates by querying `spork show`
   on a separate testnet node. If anything's wrong, fix it before
   touching mainnet.
4. **Do not put the spork private key on the same machine as the
   public seed node.** Standard practice: generate offline, sign
   spork update transactions offline, broadcast the signed result
   from a relay machine.

---

## Launch day timeline (mainnet)

Assumes 60-second block spacing, so heights ≈ minutes/60 from genesis.

### T+0 (block 0): Genesis

- All sporks default OFF.
- Block reward starts (60% miner, 30% MN withheld until block 25k,
  10% treasury drip starts at block 1).
- **No spork action required at this exact moment.**
- Operator + treasury start registering masternodes via ProRegTx
  immediately (DIP3 active from block 2).

### T+~30 minutes (around block 30): Enable DKG infrastructure

```
ratatoskr-cli sporkupdate SPORK_17_QUORUM_DKG_ENABLED 0
ratatoskr-cli sporkupdate SPORK_21_QUORUM_ALL_CONNECTED 0
```

Why now and not later:
- `SPORK_17` must be ON before DKG sessions can fire. Once the
  small-quorum activation height (block 7,500) is reached, the
  chain wants to start running DKG immediately. If SPORK_17 is
  off at that point, the first quorum forms slower than necessary.
- `SPORK_21` enables the all-connected quorum topology that's
  appropriate for small-network operation. Set early so it's
  in the network's view by the time quorums start forming.
- Both are pure "infrastructure on" sporks. There's no reason to
  wait — flip them in the first hour.

### T+0 to T+~5 days (blocks 0 → 7,500): Pure-PoW window

- Chain runs on PoW security alone. No ChainLocks yet.
- This is the **MRR defensive-rental window** per
  `mrr_launch_day_playbook.md`. Operator should have rented hashrate
  pointed at the chain to defend against 51% attacks during this
  period.
- No additional spork action.
- Monitor via `getmininginfo` (network hashrate), `protx list`
  (active MNs), and `quorum dkgstatus` (DKG progress — should
  start showing activity as height approaches 7,500).

### T+~5 days (block 7,500): Small quorums activate

Consensus-level: `LLMQ_10_60` and `LLMQ_10_75` become signable
automatically at this height. No spork needed for the activation
itself.

What to do:

1. **Wait 1-2 hours past block 7,500** for the first DKG session
   to complete. Check via:
   ```
   ratatoskr-cli quorum list
   ratatoskr-cli quorum dkgstatus
   ```
   You want to see at least one `LLMQ_10_60` quorum in the list.
2. **Once the first quorum is confirmed**, enable ChainLocks +
   InstantSend:
   ```
   ratatoskr-cli sporkupdate SPORK_19_CHAINLOCKS_ENABLED 0
   ratatoskr-cli sporkupdate SPORK_2_INSTANTSEND_ENABLED 0
   ratatoskr-cli sporkupdate SPORK_3_INSTANTSEND_BLOCK_FILTERING 0
   ```
3. **Verify ChainLocks are firing** by watching `getblockchaininfo`
   or `getbestchainlock` for a few blocks after enabling. You should
   see chainlocks appear within minutes if quorums are healthy.
4. **Once stable, wind down the MRR rental.** ChainLocks now provide
   the 51%-attack protection that the rental was substituting for.

### T+~6-7 days (block ~10,000): Enable PoSe

Once a few quorums have been around long enough to identify
underperforming members:

```
ratatoskr-cli sporkupdate SPORK_23_QUORUM_POSE 0
```

This enables Proof-of-Service penalties — masternodes that
consistently fail to participate in quorum DKG / signing get
PoSe-banned and stop earning. Without this on, lazy/broken MNs
clog quorums and reduce signing reliability.

Wait until after first quorums are healthy to flip this on, so
you don't accidentally PoSe-ban legitimate MNs that were just
getting their feet under them.

### T+~17 days (block 25,000): MN payments start

Consensus-level event — automatic, no spork needed.
- DIP3 enforcement also kicks in at this block. Legacy
  (non-deterministic) MN broadcasts stop being accepted.
- **Verify payments by inspecting a few coinbase transactions**
  after this height — they should now include MN payouts in
  addition to the miner output and the treasury drip.

### T+~21 days (block 30,000): Original superblock height

**No action.** Superblocks are disabled at the consensus level
(`nSuperblockStartBlock = INT_MAX` in `chainparams.cpp`). Leave
`SPORK_9_SUPERBLOCKS_ENABLED` permanently OFF.

If a future hard-fork ever re-enables superblocks, that decision
also dictates the spork timing. Until then: ignore.

---

## Quick post-launch summary

After all the launch-period sporks are flipped, the spork dashboard
should look like this:

```
ratatoskr-cli spork show

SPORK_2_INSTANTSEND_ENABLED              0           # ON
SPORK_3_INSTANTSEND_BLOCK_FILTERING      0           # ON
SPORK_9_SUPERBLOCKS_ENABLED              4070908800  # OFF (permanent)
SPORK_17_QUORUM_DKG_ENABLED              0           # ON
SPORK_19_CHAINLOCKS_ENABLED              0           # ON
SPORK_21_QUORUM_ALL_CONNECTED            0           # ON
SPORK_23_QUORUM_POSE                     0           # ON
```

That's the steady state.

---

## When to TURN OFF sporks (emergency / incident response)

Sporks are bidirectional — you can turn them off at any time. Real
scenarios where you'd flip OFF:

| Scenario | Action |
|---|---|
| ChainLocks signing wrong / forking the chain | `sporkupdate SPORK_19_CHAINLOCKS_ENABLED 4070908800` (immediate kill) |
| InstantSend lock spam DoSing the relay | `sporkupdate SPORK_2_INSTANTSEND_ENABLED 4070908800` |
| DKG sessions failing repeatedly (network too small / split) | `sporkupdate SPORK_17_QUORUM_DKG_ENABLED 4070908800` (graceful pause) |
| PoSe-banning legitimate MNs in a network upgrade | `sporkupdate SPORK_23_QUORUM_POSE 4070908800` until upgrade is complete |
| Network partition / dataset rollback / hard-fork prep | Disable all quorum-related sporks until network is healthy |

Sporks are designed for exactly this: rapid incident response without
needing to coordinate a wallet upgrade across hundreds of nodes.

---

## How to actually flip a spork

```
# Make sure -sporkkey is set in the daemon config to the WIF private key.
# Optional: -sporkaddr should match the corresponding address.

# Turn ON:
ratatoskr-cli sporkupdate SPORK_19_CHAINLOCKS_ENABLED 0

# Turn OFF:
ratatoskr-cli sporkupdate SPORK_19_CHAINLOCKS_ENABLED 4070908800

# View current state of all sporks:
ratatoskr-cli spork show

# View only active sporks:
ratatoskr-cli spork active
```

The command writes a signed message to the local node's spork queue,
which then propagates to peers within seconds. There's no
"transaction confirmation" needed — sporks live outside the
blockchain.

---

## Spork key security

The spork private key is the second-most-important piece of
operator-controlled credential after the treasury multisig keys
(arguably *first* during the launch period since it can adjust
network behavior in real-time).

Practical hygiene:

1. **Generate offline.** Air-gapped, single-purpose key.
2. **Don't store on cloud / git / any synced location.** This includes
   GPM, 1Password's cloud sync, etc. Use a hardware wallet or an
   encrypted offline vault.
3. **Have a backup.** If the only copy is lost, sporks become
   un-flippable forever. Two encrypted offline copies in different
   physical locations is the standard.
4. **Rotate eventually.** Bitcoin Core deprecated this whole pattern
   for a reason — long-lived single-key admin authority is a
   security smell. RATR inherits sporks from Dash/Smartiecoin,
   but a future hard-fork can remove or rotate the spork
   infrastructure once the chain is mature enough not to need it.
5. **Never put `-sporkkey` on a node that's exposed to the internet
   long-term.** Use a relay machine: import the key, broadcast the
   spork update, then immediately remove the key from disk.

---

## Spork-key contingency

If the spork private key is lost:

- All currently-active sporks stay in their current state. The chain
  doesn't break.
- You permanently lose the ability to flip them.
- ChainLocks / InstantSend / DKG are stuck in whatever state they
  were in.
- The only way to regain spork control is a hard-fork that either
  rotates the spork key or removes the spork system.

If the spork private key is compromised (stolen):

- The thief can broadcast spork updates that disable ChainLocks /
  InstantSend / DKG, potentially destabilizing the network until
  the operator counter-broadcasts.
- Recovery: same as above (hard-fork to rotate the key).

Either contingency is bad. **Treat the spork key with the same care
as the treasury multisig keys.**

---

## Reference: where each spork is checked in code

For anyone debugging spork-related behavior:

- `SPORK_2_INSTANTSEND_ENABLED` — checked in
  `src/instantsend/instantsend.cpp` before signing/processing
  islocks.
- `SPORK_3_INSTANTSEND_BLOCK_FILTERING` — checked in
  `src/validation.cpp` during block validation.
- `SPORK_9_SUPERBLOCKS_ENABLED` — checked in
  `src/governance/classes.cpp::CSuperblock`. Now moot on RATR
  because `nSuperblockStartBlock = INT_MAX`.
- `SPORK_17_QUORUM_DKG_ENABLED` — checked in `src/llmq/dkgsession.cpp`
  before each DKG phase.
- `SPORK_19_CHAINLOCKS_ENABLED` — checked in
  `src/llmq/chainlocks.cpp` before signing.
- `SPORK_21_QUORUM_ALL_CONNECTED` — checked in
  `src/llmq/utils.cpp` for connection topology.
- `SPORK_23_QUORUM_POSE` — checked in
  `src/llmq/dkgsessionhandler.cpp` for PoSe scoring.
