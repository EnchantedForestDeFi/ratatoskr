# Ratatoskr Core 1.0.0-alpha Release Notes

**Target release: June 1, 2026**
**Network: Mainnet launch**

Ratatoskr Core 1.0.0-alpha is the initial mainnet release of the Ratatoskr
(RATR) cryptocurrency. This is an alpha release — use only amounts you can
afford to lose during the first 2-3 months while the network stabilizes.

## About Ratatoskr

Ratatoskr is a yespower Proof-of-Work cryptocurrency with deterministic
masternodes, a treasury fund for ecosystem development, and a native bridge
to Alephium. The name comes from Norse mythology — the squirrel that runs
up and down Yggdrasil, carrying messages between the eagle at the top and
the serpent at the roots. A fitting metaphor for a cross-chain bridge coin.

Ratatoskr is forked from Smartiecoin v0.2.0 (which is itself forked from
Dash), with tokenomics rebalanced around two lessons learned from prior
MN-coin failures:

1. **Miners must remain profitable for the network to stay secure.**
   Extractive reward splits that crush miner rewards drive away hashrate
   and make 51% attacks financially rational.

2. **Governance must be bounded so it cannot extract value from the people
   securing the network.** Unrestricted governance votes have killed PoW+MN
   coins by shifting rewards away from miners in a single proposal.

## Key parameters

| Parameter          | Value                                     |
|--------------------|-------------------------------------------|
| Ticker             | RATR                                      |
| Algorithm          | yespower (CPU mineable)                   |
| Block time         | 60 seconds                                |
| Block reward       | 50 RATR (halving every 1,030,596 blocks)  |
| Max supply         | 100,000,000 RATR                          |
| Reward split       | 60% miner / 30% masternode / 10% treasury |
| Regular MN         | 7,500 RATR, 1× voting weight              |
| EvoNode            | 30,000 RATR, 4× voting weight             |
| Privacy            | CoinJoin, Tor v3                          |
| Network magic      | `ac c0 f0 52`                             |
| Network port       | 9393                                      |
| RPC default port   | unchanged from upstream                   |
| Address prefix     | Addresses start with `R`                  |
| BIP44 coin type    | 530                                       |
| Config file        | `ratatoskr.conf`                          |
| Data directory     | `~/.ratatoskrcore` (Linux)                |

## Activation schedule

| Block       | Event                                        |
|-------------|----------------------------------------------|
| 0           | Chain genesis, miners earn post-treasury subsidy fully (10% drips to treasury) |
| 2           | DIP0001/0003/0008 basic protocol features    |
| 20,000      | V19/V20/DIP0020/DIP0024 consensus upgrades + Evo features |
| 25,000      | MN payments begin (60/30/10 split activates) |
| 30,000      | First superblock, governance voting activates |

The 25,000-block delay on MN payments (~17 days at 60-second blocks) gives
hashrate time to stabilize and the early mining community to establish
before MN economics go live.

## Changes from Smartiecoin v0.2.0 baseline

### Identity & installation
- New network magic bytes (`ac c0 f0 52` mainnet, `ad c1 f1 53` testnet)
  so Ratatoskr nodes and SMT nodes cannot connect to each other
- New P2P port 9393 (testnet 19393)
- Address prefix 60 (addresses start with `R`)
- BIP44 coin type 530 (SLIP-0044 registration target)
- Config file: `ratatoskr.conf`
- Data directory: `RatatoskrCore` (Windows/macOS) / `.ratatoskrcore` (Linux)
- Binary names: `ratatoskrd`, `ratatoskr-qt`, `ratatoskr-cli`, etc.

### Consensus changes
- **Reward split**: 60% miner / 30% MN / 10% treasury (was SMT's 45/45/10
  schedule). The legacy Dash Block Reward Reallocation (BRR) code and
  SMT v0.1.4 reward schedule have been removed.
- **MN collateral**: 7,500 RATR for Regular, 30,000 RATR for EvoNode
  (was SMT's 15,000 / 75,000). Chosen to sit between Dash (0.0056% of
  supply per MN) and Firo (0.0047%) — the two longest-surviving MN coins.
- **EvoNode voting weight**: 4× (was 5× in SMT; matches Dash's proven ratio).
- **MN payments activation**: block 25,000 (was block 50).
- **First superblock**: block 30,000 (was 27,700).

### Treasury
- 10% of every block subsidy flows continuously to a treasury wallet
  (hardware-wallet-secured at v1.0; multisig migration is on the
  post-launch roadmap — see limitations below).
- Treasury purpose: Alephium DeFi ecosystem participation (gauge
  incentives where available, liquidity provisioning for wRATR trading
  pairs), bridge infrastructure costs, and operational needs.
- Initial wRATR liquidity will be seeded on an Alephium DEX venue at
  launch. Pair selection and venue will reflect ecosystem conditions
  at the time. Additional pairs are expected to emerge as usage grows.
- Monthly on-chain treasury reports posted publicly. All treasury
  addresses public from launch.
- Liquidity provisioning and ecosystem participation are routine
  treasury operations within documented scope; anything outside this
  scope (audits, listings, NFT program, etc.) requires masternode
  vote.

### Roadmap: NFTs as a revenue stream for miners (Phase 3)

A revenue distribution program is **planned but NOT live at launch**.
Block rewards (60% miner, 30% MN) are live from day 1. The additional
revenue stream below is a planned Phase 3 (~6 months post-launch).

**The miner angle:** NFTs are the entry point for miners to receive
on-going LP/treasury revenue without having to first accumulate the
full 7,500 RATR collateral required for a masternode. A miner who has
mined any amount of RATR can purchase an NFT and start receiving their
share of weekly/monthly distributions.

Target design:
- NFT collection minted on Alephium, capped supply (specifics TBD)
- NFT sale proceeds expand protocol-owned liquidity in wRATR pools
- Weekly or monthly distribution of LP earnings (trading fees + EX
  emissions captured by treasury LP positions) split between:
  - MN holders (pro-rata by collateral weight)
  - NFT holders (pro-rata by NFT count)
- Three participation paths for miners (use any or all):
  1. Block rewards only (mine, sell, repeat) — no extra setup needed
  2. Mine → buy NFT → earn distributions (low entry cost)
  3. Mine → save up 7,500 RATR → run MN → earn block rewards +
     distributions (highest yield, highest commitment)

Why not at launch:
- Bridge + LP must be operational and earning before there's anything
  to distribute (Phase 2 territory)
- Distribution contract requires audit before handling real revenue
- NFT mint mechanics need design + community input
- Better to launch chain cleanly and add rewards layer on top once
  base infrastructure is proven

This roadmap is intent, not commitment. Subject to refinement based on
launch experience and community feedback. Will be developed transparently
in Discord with masternode operator input.

## Known limitations (v1.0.0-alpha)

1. **Reward split is hardcoded, not governance-editable.** A planned v1.1
   hard fork (~2-3 months post-launch) will add governance-editable
   reward splits with consensus-enforced guardrails:
   - Miner floor: 50% (proposals below auto-rejected)
   - MN floor: 20% (proposals below auto-rejected)
   - Treasury: locked at 10% (hard fork only)
   - 5 percentage point max shift per proposal (gradualism)
   - 90-day cooldown between successful reward-split changes
   - 60% supermajority required
   - Phased quorum: 20% launch → 25% growth → 30% mature

2. **MN collateral is hardcoded.** v1.1 will add raise-only governance
   (never lowerable), grandfathered at registration, +50% max per raise,
   90-day cooldown.

3. **No third-party security audit yet.** The code is a small delta on
   top of a well-tested Dash/Smartiecoin base, but the Ratatoskr-specific
   changes have not been externally audited. Audit is on the Phase 2
   roadmap.

4. **Bridge to Alephium is separate infrastructure.** The bridge is an
   independent service (operator-run) and not part of the Ratatoskr
   consensus. Bridge security is documented in the bridge repository.

5. **Treasury is single-signature at v1.0.** The treasury wallet is held
   on a hardware wallet by the operator, not a multisig. This is an
   intentional choice: multisig security only works with engaged cosigners
   capable of evaluating transactions, and the Ratatoskr community at
   launch is too small to credibly fill cosigner roles. Planned upgrade
   path: transition to 2-of-3 multisig at v1.1+ once trusted community
   contributors emerge (6-12 months post-launch estimated).
   Mitigations in the interim:
   - All treasury addresses publicly posted from launch day
   - Monthly on-chain treasury report published to the public Discord
   - Treasury spending scope documented; anything outside scope requires
     a masternode vote regardless of single-sig control
   - Hardware wallet + offline seed backup (multiple physical locations)
   - Clear disclosure: this release note, the README, and the whitepaper
     all name this limitation so users can evaluate it

## Supported platforms

- Linux (x86_64, aarch64)
- Windows (x86_64)
- macOS (x86_64, aarch64)

## Upgrading from previous versions

This is the initial release; no upgrade path applies.

## Credits

Ratatoskr stands on the work of:
- **Smartiecoin Core developers** (v0.0.1 → v0.2.0)
- **Dash Core developers** (the upstream codebase)
- **Bitcoin Core developers** (the ancestral codebase)

Thank you to everyone who built the foundations we are building on.

## Getting started

- [README](/README.md) — project overview and parameters
- [Build instructions](/doc/) — compile from source for your platform
- [Bridge](https://smartiecoinalephiumbridge.enchantedforestdefi.com) —
  will be adapted for Ratatoskr at/near launch

For support during the alpha period, please use the Ratatoskr Discord.
