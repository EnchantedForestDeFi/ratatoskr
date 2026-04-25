# Ratatoskr (RATR) — A Miner-First Masternode Coin

**Whitepaper v1.0 — April 2026**
**Mainnet launch: June 1, 2026**

---

## Abstract

Ratatoskr (RATR) is a small, opinionated yespower proof-of-work cryptocurrency
with deterministic masternodes, a consensus-enforced per-block treasury drip,
bounded governance, and a native bridge to Alephium.

It is a technical fork of Smartiecoin v0.2.0 — itself a Dash derivative —
with tokenomics deliberately rebalanced around two lessons from prior
masternode-coin failures: **miners must remain profitable for the network
to stay secure**, and **governance must not be able to extract value from
the people securing the network**. Neither is aspirational here; both are
enforced in consensus through hard-coded reward floors, capped governance
velocity, and a permanently-funded treasury that drips block by block
rather than accumulating into votable superblock payouts.

This document describes what Ratatoskr is, why its economic design diverges
from upstream Dash-family chains, and how it aims to sustain a stable
equilibrium between miners, masternode operators, and long-term holders.

---

## 1. Background: Why Another Masternode Coin?

Masternode proof-of-work coins have produced some of the longest-lived projects
in crypto (Dash launched 2014, PIVX 2016, Horizen 2017) alongside a long
graveyard of failures (ESBC, Crown, Polis, and dozens of smaller forks).
Reviewing both camps reveals a consistent failure pattern:

> **Extractive governance decisions that shift block rewards away from miners
> typically kill the network within 6–18 months.**

The mechanics are predictable. When a masternode network votes to cut miner
reward below the threshold of "profitable to mine at current price," hashrate
leaves for more profitable yespower or equivalent-PoW coins. The cost of a
51% attack drops proportionally with the hashrate loss. Attacks become
economically rational. The network becomes untrustworthy. Users exit. The
chain dies — usually within a year, rarely longer.

This pattern has played out too often to treat as coincidence. It is the
predictable consequence of a governance structure that gives complete
authority over reward allocation to the group — masternode operators — that
benefits directly from shifting reward toward themselves. Left unchecked, it
is a one-way ratchet: every vote incrementally favours MNs, the miner share
compresses, and when miner participation finally breaks, so does the chain.

Ratatoskr's design explicitly prevents this failure mode through
**consensus-enforced floors** on miner and masternode rewards, combined with
gradualism caps on governance-driven changes. The result is a chain where
the miner-MN-treasury equilibrium is stable **by construction** rather than
by the ongoing goodwill of any single voter coalition.

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

Ratatoskr launches with a native bridge to Alephium, giving the coin immediate
utility beyond speculative mining. Through a single bridge hop, RATR holders
reach an established DeFi ecosystem (Elexium gauge-voted DEX, NUTTY community
token, bridged-in majors and stables) — and through Alephium's own bridge
infrastructure, Ethereum and BSC presence without needing Ratatoskr-operated
EVM bridges. See §6 for the full architecture.

### 2.6 Accessible Mining

Most proof-of-work networks start out CPU- or GPU-mineable and gradually
specialise toward ASICs as the economics reward raw efficiency. It's a
natural trajectory — the fastest hash wins — but it raises the
participation floor along the way. The hobbyist with a laptop becomes a
spectator; mining consolidates into a handful of industrial-scale
operators.

Ratatoskr deliberately uses **yespower** (§3.1), a memory-hard
CPU-optimised algorithm designed to resist ASIC specialisation. The goal
is not to match any larger chain's absolute hashrate. It is to keep
mining accessible to anyone with a modern CPU — the desktop on a desk,
the laptop in a bag, the Mac mini on a shelf, hardware most people
already own. At Ratatoskr's scale, a broad base of casual participants
matters more than raw absolute throughput.

This is a deliberate "back to roots" stance for a small masternode coin.
Proof-of-work's original promise was that anyone with a computer could
participate in securing the network. Yespower keeps that promise real.

---

## 3. Consensus

### 3.1 Proof-of-Work Algorithm

Ratatoskr uses **yespower v1.0** with parameters `N = 2048, R = 32` and no
personalization string. Yespower is a memory-hard, ASIC-resistant algorithm
designed by Alexander Peslyak for long-term CPU mineability — see §2.6 for
the design rationale behind choosing a CPU-only algorithm at Ratatoskr's
scale.

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

The 60/30/10 ratio is informed by three data points. First, Dash's long
operational history under varied reward splits. Second, Firo's steady-state
configuration, which has survived several market cycles. Third, the recent
trajectory of SMT — Ratatoskr's immediate fork source — where an April 2026
governance vote shifted the split to 18/72/10, cutting miner rewards
approximately 60% overnight. That outcome is the specific case study behind
§1's "extractive governance" argument and is referenced here without hostility:
it simply illustrates the pattern the design prevents.

Comparative analysis of active and failed masternode coins suggests
sustainable equilibria cluster in the 50/35/15 to 65/25/10 band. 60/30/10
sits in the middle. It preserves meaningful masternode income at typical
market-cycle valuations while keeping miners solidly above the
"unprofitable to mine" threshold that has killed prior chains.

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

### 5.3 MN Collateral Adjustments (v1.1+)

v1.1+ will permit governance to raise — but never lower — MN collateral
requirements, with grandfathering of previously-registered nodes at their
original collateral level. Raises are rate-limited to +50% per proposal with
a 90-day cooldown between successful raises.

This allows the network to adapt to RATR price appreciation over time
without stranding existing MN operators who registered at earlier, lower
collateral amounts. New MN registrations after a raise use the new amount;
old MNs keep their original terms until they voluntarily re-register.

---

## 6. Bridge and Liquidity Strategy

Ratatoskr's cross-chain architecture is deliberately concentrated: **one
operator-run bridge** connects the Ratatoskr chain to Alephium, and
**Alephium Bridge** (`bridge.alephium.org`) is adopted as the official
pathway for users who want wRATR exposure on Ethereum or BNB Smart Chain.
This lets the project focus operational attention on one bridge while
still offering multi-chain accessibility.

### 6.1 RATR ↔ Alephium bridge (operator-run)

The native bridge locks RATR on Ratatoskr and mints wRATR on Alephium
(and burns wRATR to unlock RATR on the return trip).

- **Mechanism:** lock-and-mint, with a hardware-wallet-secured custodian
  holding locked RATR on the Ratatoskr side and a Ralph smart contract
  handling mint/burn on the Alephium side
- **Custodial model at v1.0:** single operator, Ledger-backed.
  Multi-operator federation is on the Phase 2 roadmap
- **Published limits:** per-tx and daily caps at launch, raised as
  operational confidence grows
- **Transparency:** bridge custodian address and wRATR contract are
  published at launch; supply parity (locked RATR vs minted wRATR) is
  independently verifiable

The bridge is adapted from the operator's existing mainnet-proven
Alephium bridge infrastructure (operational since Q1 2026, bidirectional
verified April 2026), with asset identifiers and contract state
re-parameterised for RATR. Activation may lag chain liveness by up to 72
hours if final testing requires; announced in Discord rather than gating
the chain launch.

### 6.2 Why Alephium

Alephium launched its mainnet on November 8, 2021, following several years
of R&D by a Switzerland-based team. The chain has operated continuously
since then and is on its 4.x release series as of 2026. It is PoW-secured
with a mined-dominant supply curve — 86% of the 1-billion ALPH maximum is
emitted through proof-of-work over approximately 82 years, released block
by block. The remaining 14% is a genesis allocation covering R&D, ecosystem
development, and team, subject to multi-year quarterly vesting. The result
is a chain where long-term issuance tracks active network participation
rather than insider holdings (see references in Appendix A for official
tokenomics detail).

For Ratatoskr's purposes, Alephium offers several things no alternative
does as cleanly:

- **PoW-secured** — same consensus family as RATR. Alignment rather than
  a PoS platform that would sit uncomfortably with a PoW chain's ethos.
- **Sharded UTXO architecture** (BlockFlow) — closer to Bitcoin's UTXO
  model than Ethereum's account model; state locality keeps contracts
  simple and makes asset flow rules enforceable at the VM level
- **Stateful contracts** in Ralph, with asset-passing annotations that
  make bridge-style custody patterns natural
- **Native token issuance** — tokens are first-class primitives, not
  ERC-20 simulacra
- **Low transaction fees** (sub-cent typical) — bridging economics work
  for small transactions, not just whales
- **Established DeFi ecosystem** — Elexium (ve(3,3) DEX with gauge
  voting, bribes, and EX emissions); PowFi (Alephium-native DEX from
  the core team, currently on testnet); multi-chain bridged assets
  (wBTC, wETH, wUSDC, wUSDT, wBNB) accessible on Alephium via
  `bridge.alephium.org` — the same bridge we adopt for cross-chain
  wRATR access (§6.3)
- **Four+ years of production operation** with active development, real
  users, and an audited bridge — not a prototype we're betting RATR on

The combination of (a) a native wRATR bridge into Alephium and (b)
Alephium's existing bridged-in majors/stables means RATR holders can
access BTC, ETH, USDC, or BNB exposure **without leaving Alephium**.
Trading happens in one venue; liquidity concentrates; operational
complexity stays low.

### 6.3 Official cross-chain path: Alephium Bridge

For users who need wRATR presence on Ethereum or BSC specifically (for
integration with chain-native protocols, preferred wallet support, etc.),
the official path is:

> RATR  →  wRATR-Alephium  *(operator-run bridge)*  →  wRATR-ETH / wRATR-BSC
> *(via `bridge.alephium.org`)*

Alephium Bridge is a lock-and-mint Wormhole-fork operated by the Alephium
team and ecosystem. It handles the Alephium → EVM leg via its shared
`BridgeToken` proxy infrastructure, and tokens bridged this way appear
on Etherscan / BscScan with the suffix "(AlphBridge)" — a clear,
self-labeled identifier distinguishing bridge-wrapped representations
from any native token contracts.

Ratatoskr does **not** operate independent Ethereum or BSC bridges. This
is deliberate:

- **Single canonical supply on Alephium** — no fragmentation between
  competing wRATR representations
- **Lower operational surface** — one bridge to secure, one gas tank
  to maintain, one set of consensus rules to track
- **Shared trust layer** — Alephium Bridge is audited infrastructure used
  by the broader Alephium ecosystem; users don't depend on a small team's
  EVM-side engineering

Users who want wRATR on Ethereum or BSC are free to use Alephium Bridge
at any time after wRATR-Alephium exists. This post-launch workflow is
documented separately.

### 6.4 Primary liquidity: Elexium

The primary liquidity pair at launch is **wRATR / NUTTY** on Elexium,
with a **wRATR / wUSDC** stable pair added for price discovery.

NUTTY is a community token operated by the same team and already trades
across multiple Elexium pools — NUTTY/ALPH (with an active gauge vote),
NUTTY/EX, NUTTY/USDC.eth, and NUTTY/wSMT — meaning any holder of ALPH,
EX, USDC, or wSMT can route into wRATR through NUTTY from day one,
without waiting for additional direct wRATR legs to be seeded.

NUTTY is also used to bootstrap voting incentives on the wRATR pool
through self-minted bribes — a low-cost mechanism to direct Elexium EX
emissions toward the new pool while organic liquidity accumulates. As
usage grows, additional direct pairs (wRATR/ALPH, wRATR/wETH, wRATR/wBTC)
are expected to emerge and concentrate rather than fragment liquidity,
because all of them live on the same venue.

### 6.5 What's explicitly not built

To be clear on scope:

- **No independent RATR ↔ Ethereum bridge.** Alephium Bridge handles it.
- **No independent RATR ↔ BSC bridge.** Same.
- **No Uniswap/PancakeSwap pools as primary markets.** Elexium is the
  primary venue. Secondary-chain pools may emerge organically post-launch
  but are not a project-operated priority.

This is a concentration-over-expansion bet. If real demand for native
cross-chain bridges materialises post-launch, the decision is
revisitable; until then, depth on one venue beats dispersion across many.

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

### 7.3 Treasury Masternode Operation and Governance Abstention

The treasury may use its accumulating RATR balance as collateral for
masternodes operated by the project. These treasury-operated masternodes
contribute to network security in the normal way — block validation,
ChainLocks signing, InstantSend signing — and earn their proportional
share of masternode rewards from the consensus subsidy. That income
flows back to the treasury balance.

**Treasury-operated masternodes will cast `ABSTAIN` on every on-chain
governance proposal. No exceptions.** This includes proposals that
specifically benefit the treasury, security or emergency proposals, and
any future governance mechanism. The operator's individual voice in
governance, when desired, is exercised through personal masternodes
registered separately to operator-owned addresses, not through the
treasury.

The motivation: the 10% treasury drip generates approximately 7,200 RATR
per day. Without abstention, even a modest number of treasury EvoNodes
could control a meaningful share of governance weight in the early
chain — an unacceptable concentration. Abstention separates treasury's
economic role (fund ecosystem development, LP seeding, audits, listings)
from any potential governance role.

To make this externally auditable, the operator commits to publishing a
public list of all treasury masternode collateral addresses, signed by
the treasury private key. Anyone can then verify on-chain that no
`YES`/`NO` votes ever originate from those addresses. Full policy
specification, edge cases, and verification procedure are in
`doc/treasury-governance-policy.md`.

### 7.4 Fractional Masternode NFTs (Phase 3, exploratory)

A planned Phase 3 feature (target ~6 months post-launch) would mint NFTs
on Alephium representing fractional ownership of treasury-operated
masternodes. Buyers contribute capital that the treasury pools to
register new MNs; NFT holders receive pro-rata block rewards from the
pooled MN, minus a small treasury operator fee. This creates an
entry-level MN-yield product for participants who have not yet
accumulated full collateral (7,500 RATR Regular / 30,000 EvoNode).

All masternodes operated by such a pool inherit the abstention policy
from §7.3 — treasury-funded MNs cast `ABSTAIN` on every governance
proposal regardless of whether the underlying capital is direct treasury
RATR or pooled NFT-holder capital. This is not a Phase-3-conditional
rule; it is the chain-wide treasury MN policy already adopted in §7.3.
The Phase 3 product simply scales the operating model without changing
the governance posture.

Phase 3 remains exploratory; this whitepaper should not be read as a
commitment to build it in its current form.

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

5. **Small initial network; rented-hash exposure.** Low launch hashrate,
   combined with a liquid market for yespower rental hash, means block
   production in the first weeks could be concentrated by anyone willing
   to rent. Ratatoskr mitigates this in three layers:

   - **Bounded capture.** Any value an early adversary can extract is
     limited to the miner share of subsidies during the pre-ChainLocks
     window. The 10% treasury is not exposed (paid by consensus to a
     fixed script); the 30% masternode share does not begin until block
     25,000.
   - **ChainLocks finality.** Once the masternode set grows enough to
     form LLMQ signing quorums (shortly after MN payment activation at
     block 25,000), ChainLocks sign every block, making deep reorgs
     impossible regardless of an adversary's hashrate.
   - **Defensive rental posture.** The operator commits to maintaining
     an active defensive hashrate budget during the pre-ChainLocks
     window, directing rented hash to honest mining pools. This raises
     the effective cost of a rental-based attack and keeps block
     production distributed even when organic hashrate is still low.

   Specific attack-cost and defence-budget numbers are kept to internal
   operational planning rather than printed here. The calculation is
   trivial for any interested party from public marketplace rates and
   the subsidy schedule; we simply prefer not to put a target figure
   on our own risk surface.

6. **Regulatory uncertainty.** Masternode-based coins exist in an ambiguous
   regulatory space in some jurisdictions. Users and operators are responsible
   for understanding the laws applicable to them.

Users should treat early-stage participation as experimental and not commit
more capital than they can afford to lose.

---

## 10. Team and Transparency

Ratatoskr is operated by the same team behind the Smartiecoin ↔ Alephium
bridge, which has run on mainnet since Q1 2026 (bidirectional operation
verified April 2026) without custodial incident. The operator is a single
individual with published contact points:

- GitHub organisation: `EnchantedForestDeFi`
- Email: `NexusAether@protonmail.com`
- Discord: EnchantedForestDeFi server, `#ratatoskr` channel

Transparency commitments:

- All code in public GitHub repositories under the `EnchantedForestDeFi` org
- All treasury addresses published; supply parity independently verifiable on-chain
- Monthly treasury spending reports posted to Discord
- Clear disclosure of known limitations (this document, release notes, README)
- Operator-direct contact channels maintained

The single-operator model is a conscious tradeoff at v1.0: faster decisions
and simpler accountability in exchange for key-person risk. The Phase 2
audit, v1.1 governance activation, and eventual treasury multisig (§7.2,
§8) are the explicit path toward decentralisation over time.

---

## 11. Conclusion

Ratatoskr exists because the masternode-coin design space is littered with
failures whose root causes are well-understood but rarely structurally
prevented. Most projects that died did so not because the technology
failed, but because governance — unbounded and unchecked — voted them into
an extractive equilibrium that miners eventually walked away from.

Ratatoskr's response is specific. Hardcode miner and masternode reward
floors so no single vote can starve the hashrate. Drain treasury block by
block to a public address so its funding cannot be front-run by governance
timing. Cap governance velocity so economic redistribution happens
gradually or not at all. Name the known attack vectors — treasury-MN
voting concentration, rental-hash disruption, superblock extraction — and
structurally prevent them rather than hope they stay theoretical.

The bridge to Alephium provides immediate utility beyond mining. Elexium
pools offer multi-asset exposure without cross-chain complexity. The
roadmap (audit, v1.1 governance activation, multisig treasury, Phase 3
community features) defines a credible path from a solo-operated alpha to
a maturing decentralised system.

The chain starts on **June 1, 2026, 00:00 UTC**. See you at block 1. 🐿️

---

## Appendix A: Relevant References

- Dash Core Documentation: https://dash.org/
- Smartiecoin (fork source): https://github.com/SmartiesCoin/Smartiecoin
- yespower: https://www.openwall.com/yespower/
- Alephium: https://alephium.org/
- Alephium whitepaper (BlockFlow + protocol spec): https://github.com/alephium/white-paper
- Alephium tokenomics (official, supply schedule + genesis allocation): https://medium.com/@alephium/tokenomics-of-alephium-61d59b51029c
- Alephium Bridge (official cross-chain path, Wormhole-fork): https://bridge.alephium.org/
- Elexium: https://elexium.finance/
- PowFi (Alephium-native DEX, testnet): https://powfi.alephium.org/
- EnchantedForestDeFi: https://enchantedforestdefi.com

## Appendix B: Technical Parameters (Quick Reference)

| Parameter | Value |
|---|---|
| Mainnet launch | 2026-06-01, 00:00 UTC |
| Genesis hash | `00001059e1c1b34440ea46fcc1a3deb84e827f061ecf06f5013056219a66a4df` |
| Genesis nTime | 1777000000 |
| Network magic | `ac c0 f0 52` (mainnet) |
| P2P port | 9393 |
| RPC port (default) | 8282 |
| Address prefix | 60 (R…) |
| BIP44 coin type | 530 |
| PoW algorithm | `yespower` v1.0, N=2048, R=32, no personalization |
| Block time | 60 s |
| Block reward (initial) | 50 RATR, halving every 1,030,596 blocks (~2 years) |
| Max supply | 100,000,000 RATR |
| Reward split (post-block-25,000) | 60% miner / 30% MN / 10% treasury |
| Reward split (pre-block-25,000) | 90% miner / 0% MN / 10% treasury |
| MN payments activate | block 25,000 (~17 days post-launch) |
| Governance / first superblock | block 30,000 (~21 days post-launch) |
| MN collateral | 7,500 RATR (Regular), 30,000 RATR (EvoNode) |
| MN voting weight | 1× (Regular), 4× (EvoNode) |
| Treasury disbursement | per-block, to hardcoded script |

---

*This whitepaper describes design intent as of v1.0.0-alpha. Parameters are
subject to refinement based on pre-launch community input, and post-launch
through the governance mechanisms described in §5.*
