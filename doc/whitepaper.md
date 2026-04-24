# Ratatoskr (RATR) — A Miner-First Masternode Coin

**Whitepaper v1.0 — April 2026**
**Mainnet launch: June 1, 2026**

---

## Abstract

Ratatoskr (RATR) is a yespower proof-of-work cryptocurrency with deterministic
masternodes, a continuous treasury, and a native bridge to the Alephium smart
contract platform. It is a technical fork of Smartiecoin v0.2.0 — itself a
Dash derivative — with tokenomics deliberately rebalanced around two lessons
learned from prior masternode-coin failures: **miners must remain profitable
for the network to stay secure**, and **governance must be bounded so it
cannot extract value from the people securing the network**.

This document describes what Ratatoskr is, why its economic design differs
from upstream Dash-family chains, and how it intends to sustain a balanced
equilibrium between miners, masternode operators, and long-term holders.

---

## 1. Background: Why Another Masternode Coin?

Masternode-based proof-of-work coins have produced some of the longest-lived
projects in crypto (Dash launched 2014, PIVX 2016, Horizen 2017), alongside a
long graveyard of failures (ESBC, Crown, Polis, and dozens of smaller forks).
A review of both camps reveals a consistent failure mode:

> **Extractive governance decisions that shift block rewards away from miners
> typically kill the network within 6–18 months.**

When a masternode network votes to reduce miner reward below the threshold of
"profitable to mine at current price," three things happen in sequence:

1. Hashrate leaves for more profitable yespower or equivalent-PoW coins
2. The cost of a 51% attack drops by the same percentage hashrate dropped
3. Attacks become economically rational, the network becomes untrustworthy,
   and the remaining user base exits

This pattern has played out too many times to dismiss as coincidence. It is
the predictable consequence of a governance structure that gives 100% voting
power over reward allocation to the group (masternode operators) that
benefits directly from shifting reward toward themselves.

Ratatoskr's design explicitly prevents this failure mode through
**consensus-enforced floors** on miner and masternode rewards, combined with
gradualism caps on governance-driven changes.

---

## 2. Design Principles

### 2.1 Miners First

Miner reward cannot fall below 50% of subsidy. This is a consensus rule —
any governance proposal violating it is rejected at block validation,
regardless of vote outcome.

### 2.2 Masternodes Earn, But Do Not Extract

Masternode reward cannot fall below 20% of subsidy. MNs provide essential
network services (InstantSend, ChainLocks, governance deliberation,
lightweight client servicing) and must be compensated — but not at the cost
of miner economics.

### 2.3 Treasury is Transparent and Bounded

10% of every block subsidy flows continuously to a publicly-documented
treasury address. This is fixed at the consensus layer — it cannot be
redirected by governance. Treasury spending remains discretionary within a
public scope, with anything outside that scope requiring masternode approval.

### 2.4 Governance Gradualism

When v1.1 introduces governance-editable reward splits, changes are capped
at 5 percentage points per proposal, with a 90-day cooldown between
successful proposals, a 60% supermajority requirement, and phased quorum
floors. No single vote can radically reshape the economy.

### 2.5 Bridge Proximity

Ratatoskr launches with a native bridge to Alephium, providing access to an
established DeFi ecosystem (Elexium DEX, NUTTY, etc.) from day one. This is
designed to give the coin immediate utility beyond pure speculative mining.

---

## 3. Consensus

### 3.1 Proof-of-Work Algorithm

Ratatoskr uses **yespower v1.0** with parameters `N = 2048, R = 32`, no
personalization string. Yespower is a memory-hard, ASIC-resistant algorithm
designed by Alexander Peslyak for long-term CPU mineability.

### 3.2 Block Timing

Target block interval: **60 seconds**. Difficulty retargets every block
using DigiShield-derived logic.

### 3.3 Masternode Consensus

Ratatoskr inherits Dash's DIP3 (deterministic masternodes) and related
infrastructure:

- **ChainLocks** (DIP8): LLMQ-signed block finality
- **InstantSend** (DIP10): Sub-second probabilistic transaction finality
- **CbTx Special Transactions** (DIP4): Coinbase output metadata
- **PrivateSend / CoinJoin**: Optional transaction mixing

Masternode collateral:
- **Regular MN**: 7,500 RATR (1× voting weight)
- **EvoNode**: 30,000 RATR (4× voting weight)

These values put each Regular MN at ~0.0075% of total supply and each
EvoNode at ~0.030%. For comparison, Dash's standard MN collateral is
~0.0056% of supply and Firo's is ~0.0048%. Ratatoskr's Regular tier is
therefore *looser* (higher per-MN share of supply) than both — a
deliberate trade-off that lowers the entry cost in RATR-denominated
terms and keeps the MN set accessible to miners who accumulate rewards
from mining. Supply percentage alone is a crude metric; actual
accessibility depends on market price and mining yield, both of which
the 60/30/10 split is tuned to preserve.

---

## 4. Economics

### 4.1 Supply

| Parameter | Value |
|---|---|
| Max supply | 100,000,000 RATR |
| Initial block reward | 50 RATR |
| Halving interval | 1,030,596 blocks (~2 years) |
| Block time | 60 seconds |

### 4.2 Reward Split

Every block's 50 RATR is allocated as:

| Recipient | Percentage | Per-block |
|---|---|---|
| Miner (PoW) | 60% | 30 RATR |
| Masternode | 30% | 15 RATR |
| Treasury | 10% | 5 RATR |

This split is **hardcoded in consensus** for v1.0.0. v1.1 will introduce
governance-editable adjustments with the floors and gradualism caps
described in §2.4.

### 4.3 Activation Schedule

| Block | Event |
|---|---|
| 0 | Chain genesis (hardcoded, not mined) |
| 1 | First mineable block; treasury drip begins |
| 2 | DIP0001/0003/0008 base protocol features |
| 20,000 | DIP0020/0024, V19/V20 upgrades |
| 25,000 | Masternode payments activate (~17 days post-launch) |
| 30,000 | First superblock, governance voting enabled |

The 25,000-block delay on MN payments allows hashrate to stabilize and the
early mining community to establish before MN economics go live.

### 4.4 Why 60/30/10

The choice of 60/30/10 is informed by Dash's long history under various reward
splits, Firo's steady state, and the specific failure of SMT's April 2026
governance vote (which shifted to 18/72/10, cutting miner rewards 60%). The
60/30/10 ratio preserves meaningful MN income while keeping miners solidly
profitable at typical market-cycle valuations.

Comparative analysis of active and failed masternode coins confirms that
sustainable equilibria lie in the 50/35/15 to 65/25/10 band. 60/30/10 sits
squarely in the middle of that range.

---

## 5. Governance (v1.0 and v1.1 Roadmap)

### 5.1 v1.0 (Launch)

Masternode governance is **active** for treasury spending proposals beyond
the operator's documented scope, but **reward splits are hardcoded and
cannot be changed**. The superblock cadence is preserved from upstream Dash.

### 5.2 v1.1 (Hard Fork, ~2-3 months post-launch)

v1.1 introduces governance-editable reward splits with consensus guardrails:

- **Miner floor**: 50% (proposals below auto-rejected at validation)
- **MN floor**: 20% (proposals below auto-rejected at validation)
- **Treasury**: locked at 10% (hard fork required to change)
- **5 percentage point max shift per proposal** (gradualism)
- **90-day cooldown** between successful reward-split changes
- **60% supermajority** of voting weight required
- **Phased quorum requirement**: 20% at launch → 25% at growth → 30% at maturity

This structure is designed to prevent the "governance death spiral" observed
in SMT, ESBC, and other failed projects, where a simple-majority MN vote
can instantly reshape miner economics.

### 5.3 v1.2+ MN Collateral Adjustments

v1.1+ will also permit governance to raise (but never lower) MN collateral
requirements, with grandfathering of previously-registered nodes. Raising is
rate-limited to +50% per proposal and a 90-day cooldown.

This allows the network to adapt to RATR appreciation over time without
stranding existing MN operators.

---

## 6. Bridge to Alephium

Ratatoskr includes a native bridge to Alephium, activating at or shortly
after mainnet liveness:

- **RATR → wRATR**: deposit native RATR on Ratatoskr, mint wRATR (a token
  contract) on Alephium
- **wRATR → RATR**: burn wRATR on Alephium, receive native RATR
- **Custodial model at v1.0**: single-operator hardware-wallet-secured
  custodian (Ledger-backed)
- **Published limits**: per-tx and daily caps at launch, raised as
  operational confidence grows

The bridge is adapted from the existing, mainnet-proven Smartiecoin ↔
Alephium bridge (bidirectional verified April 2026), with asset
identifiers and contract state migrated for RATR. Bridge activation may
lag chain liveness by up to 72 hours if final testing requires it; this
will be announced in Discord rather than gating the chain launch.

### 6.1 Why Alephium

Alephium is a scalable sharded UTXO-based smart contract platform with:

- Stateful contracts (Ralph language)
- Native token issuance (tokens are first-class on Alephium)
- Low transaction fees (sub-cent)
- Established DeFi ecosystem (Elexium ve(3,3) DEX, existing liquidity),
  with additional first-party venues such as [PowFi](https://powfi.alephium.org/)
  — an Alephium-native DEX from the core team, currently on testnet —
  expected to broaden the ecosystem as they reach mainnet
- BTC-aligned security model (no pre-mine, PoW-secured)

The bridge specifically enables RATR holders to participate in Elexium gauge
voting, LP yields, and bribes — providing yield pathways beyond masternode
operation.

### 6.2 Launch Pool

The primary liquidity pair at launch is **wRATR/NUTTY** on Elexium. NUTTY is
a community token operated by the same team. At the time of RATR launch,
NUTTY already trades across multiple Elexium pools — **NUTTY/ALPH** (with
an active gauge vote), **NUTTY/EX**, **NUTTY/USDC**, and **NUTTY/wSMT** —
meaning any holder of ALPH, EX, USDC, or wSMT can route into wRATR
through NUTTY on day one, without waiting for direct wRATR legs to be
seeded.

NUTTY is also used to bootstrap voting incentives on the wRATR pool
through self-minted bribes — a low-cost mechanism to direct Elexium EX
emissions toward the new pool while organic liquidity accumulates. As
wRATR usage grows, direct wRATR/ALPH and wRATR/USDC pairs are expected
to emerge, shortening the routing path.

---

## 7. Treasury

### 7.1 Purpose

Treasury funds support the ongoing operational needs of the ecosystem:

- Alephium gauge participation (EX lock ladder, bribes to wRATR pools)
- Protocol-owned liquidity in wRATR pairs
- Bridge infrastructure operational costs
- Community development and grants (post v1.1 via governance)
- Audit funding (Phase 2 roadmap item)
- Exchange listing fees (if and when appropriate)

### 7.2 Security

**v1.0 (launch)**: Single-signature, hardware-wallet-secured by the project
operator. This is an intentional simplification for alpha — multisig only
works with engaged cosigners who can evaluate transactions, and the
Ratatoskr community at launch is too small to credibly fill cosigner roles.

**Mitigations in the single-sig period**:

- All treasury addresses published on day 1
- Monthly on-chain treasury reports published to public Discord
- Hardware wallet + offline seed backup at multiple physical locations
- Operator spending scope publicly documented; anything outside scope
  requires masternode governance approval

**Planned transition**: 2-of-3 multisig at v1.1+ once trusted community
contributors emerge (6-12 months post-launch estimated).

### 7.3 NFT Revenue Program (Phase 3)

A planned Phase 3 feature (targeted ~6 months post-launch) introduces NFTs
minted on Alephium as fractional masternode ownership. Buyers of NFTs
contribute capital that the treasury pools to spin up new masternodes;
the NFT holders receive pro-rata block rewards from that pooled MN, minus
a 5% treasury operator fee.

This creates an entry-level mining/yield product for miners who have not yet
accumulated full MN collateral (7,500 RATR), broadening network
participation without diluting existing MN economics.

Governance firewall: all treasury-operated MNs are designated **non-voting**.
This is critical — without it, treasury MNs would rapidly dominate
governance quora. Enforcement is social (addresses are public; any treasury
MN vote would be a visible trust violation).

---

## 8. Roadmap

### Phase 1: Launch (Q2 2026)

- Mainnet launch June 1, 2026
- Linux, Windows, macOS binaries (Linux + Windows at launch; macOS soon after)
- Bridge to Alephium live day one
- wRATR/NUTTY pool seeded on Elexium
- Mining via partner pool (stratum) and daemon internal miner
- 3+ seed nodes online

### Phase 2: Stabilization (Q3 2026)

- Third-party security audit
- v1.1 hard fork with governance-editable reward splits
- Additional bridge pairs (wRATR/ALPH, wRATR/USDC) based on demand
- Block explorer publicly hosted
- Expanded seed node infrastructure

### Phase 3: Growth (Q4 2026 onwards)

- NFT fractional-MN program launch
- MN collateral governance (raise-only)
- Additional integrations and partnerships
- Transition to multi-sig treasury

### Phase 4: Decentralization (Year 2+)

- Community-operated seed nodes
- Community-run pools
- Treasury transition to federated governance
- Open development contributor program

---

## 9. Risks and Limitations

Ratatoskr is an alpha release. Known limitations as of v1.0.0-alpha:

1. **No third-party audit.** The consensus is a small delta on a well-audited
   Dash codebase, but the Ratatoskr-specific changes have not been externally
   reviewed. Audit is a Phase 2 roadmap item.

2. **Single-sig treasury.** See §7.2 for the rationale and mitigations.

3. **Hardcoded reward splits.** Governance editing is a v1.1 feature. v1.0
   splits are consensus-enforced and cannot be changed without a hard fork.

4. **Bridge is custodial and single-operator.** Users must trust the operator
   with custodial control during the bridge-in period. Multi-operator
   federation is a Phase 2 roadmap item.

5. **Small initial network.** Low hashrate in the first weeks means blocks
   could be mined with relatively inexpensive hardware. This is addressed by
   the 25,000-block delay on MN payments, giving hashrate time to ramp up
   before MN economics create additional attractors.

6. **Regulatory uncertainty.** Masternode-based coins exist in an ambiguous
   regulatory space in some jurisdictions. Users and operators are responsible
   for understanding the laws applicable to them.

Users should treat early-stage participation as experimental and not commit
more capital than they can afford to lose.

---

## 10. Team and Transparency

Ratatoskr is operated by the same team behind the Smartiecoin ↔ Alephium
bridge, which has been operational on mainnet since Q1 2026 (bidirectional
operation verified April 2026) without custodian incident. The operator is a single individual with published
contact (GitHub: `EnchantedForestDeFi` org, email: `NexusAether@protonmail.com`,
Discord: EnchantedForestDeFi server).

Transparency commitments:

- All code in public GitHub repositories under `EnchantedForestDeFi` org
- All treasury addresses published and monitored on-chain
- Monthly treasury spending reports
- Clear disclosure of known limitations (this document, release notes, README)
- Operator direct-contact channels published

---

## 11. Conclusion

Ratatoskr exists because the masternode-coin design space is littered with
failures whose root causes are well-understood but rarely structurally
prevented. By hardcoding miner and masternode reward floors, continuously
draining treasury to a public address, capping governance velocity, and
explicitly naming and preventing known attack vectors (treasury-MN voting
concentration, superblock extraction), Ratatoskr aims to be a chain where
the equilibrium between miners, masternodes, and holders is stable **by
design** rather than by social convention.

The bridge to Alephium provides immediate utility beyond mining. The NFT
program (Phase 3) provides an on-ramp for smaller participants. The audit,
multisig transition, and governance activation (Phase 2) complete the
maturation path.

June 1, 2026.

---

## Appendix A: Relevant References

- Dash Core Documentation: https://dash.org/
- Smartiecoin (fork source): https://github.com/SmartiesCoin/Smartiecoin
- yespower: https://www.openwall.com/yespower/
- Alephium: https://alephium.org/
- Elexium: https://elexium.finance/
- PowFi (Alephium-native DEX, testnet): https://powfi.alephium.org/
- EnchantedForestDeFi: https://enchantedforestdefi.com

## Appendix B: Technical Parameters (Quick Reference)

| Parameter | Value |
|---|---|
| Genesis hash | `00001059e1c1b34440ea46fcc1a3deb84e827f061ecf06f5013056219a66a4df` |
| Genesis nTime | 1777000000 |
| Network magic | `ac c0 f0 52` (mainnet) |
| P2P port | 9393 |
| Address prefix | 60 (R...) |
| BIP44 coin type | 530 |
| Yespower variant | `yespower` v1.0, N=2048, R=32 |
| Block time | 60s |
| Block reward | 50 RATR, halving every 1,030,596 blocks |
| Max supply | 100,000,000 RATR |
| Reward split | 60% miner / 30% MN / 10% treasury |
| MN collateral | 7,500 RATR (Regular), 30,000 RATR (EvoNode) |
| MN voting weight | 1× (Regular), 4× (EvoNode) |

---

*This whitepaper describes design intent as of v1.0.0-alpha. Parameters are
subject to refinement based on pre-launch community input, and post-launch
through the governance mechanisms described in §5.*
