# Security Policy

## Reporting a Vulnerability

If you believe you have found a security vulnerability in Ratatoskr Core, the bridge contract, or any operator-run infrastructure, please report it privately rather than opening a public issue.

**Contact:** [releases@enchantedforestdefi.com](mailto:releases@enchantedforestdefi.com)

Please include:

- A clear description of the issue and its impact
- Steps to reproduce, ideally with proof-of-concept code or transactions
- Any relevant version / commit / network (mainnet, testnet, local) the issue affects
- Your preferred contact method for follow-up

You will receive an acknowledgement within **48 hours**. If you do not, please follow up — emails sometimes get caught in filters.

## What Counts as a Security Issue

- Consensus bugs (chain split, invalid block accepted, valid block rejected)
- Cryptographic weaknesses or key-handling issues
- Privilege escalation in masternode / DKG / quorum signing
- Bridge contract vulnerabilities (custody drain, mint/burn manipulation, replay attacks)
- Remote DoS that requires only modest attacker resources
- Wallet key extraction or transaction signing flaws
- RPC / P2P remote code execution
- Memory-corruption bugs in the daemon or Qt wallet

## What Does NOT Count

- Theoretical concerns without concrete attack paths
- Issues already documented in the whitepaper risk-disclosures section (e.g., single-sig treasury at v1.0, no third-party audit at launch)
- Stale or trivially-fixable bugs that don't affect users (filed as normal issues instead)
- Bugs in dependencies that have already been disclosed upstream
- Findings that depend on physical access to the operator's hardware

## Coordinated Disclosure

We follow a standard coordinated-disclosure timeline:

1. **0 hours** — your report received
2. **0–48 hours** — acknowledgement sent
3. **48 hours – 14 days** — issue triaged, severity assessed, fix scoped
4. **14–60 days** — fix developed, tested, and prepared for release
5. **At release** — patch shipped + public disclosure on the GitHub release notes + Discord announcement
6. **You credited** in the release notes (unless you prefer anonymity)

For consensus-affecting or actively-exploited issues, the timeline compresses. The operator may push a SPORK-based mitigation before the full patch ships.

## Bounties

There is no formal bug bounty program at v1.0.0. We do reward verified reports at operator discretion based on severity:

| Severity | Approximate range |
|---|---|
| Critical (consensus break, custody drain, key extraction) | $1,000 – $5,000 USD equivalent in RATR |
| High (DoS, wallet flaw, significant chain-level impact) | $250 – $1,000 USD equivalent |
| Medium (RPC misbehavior, P2P abuse, recoverable issues) | $100 – $250 USD equivalent |
| Low (informational, hardening suggestions) | discretionary thanks; credit on release notes |

Bounty amounts are paid from the on-chain treasury once it has accumulated sufficient balance (estimated T+30 to T+90 post-launch depending on RATR price). Reports made before the treasury can fund bounties will still be honored — payment timing may simply be delayed.

A formal bug bounty program with fixed rates is a Phase 2 roadmap item, target T+30 to T+90 post-mainnet.

## Scope

In scope:

- The Ratatoskr Core daemon and Qt wallet (this repository)
- The wRATR bridge Ralph contracts on Alephium (separate repo at [EnchantedForestDeFi/ratatoskralephiumbridge](https://github.com/EnchantedForestDeFi/ratatoskralephiumbridge), once public)
- Operator-run public infrastructure: seed nodes, block explorer, bridge UI, web wallet
- The Ratatoskr testnet (issues affecting testnet are valued lower than mainnet, but still in scope)

Out of scope:

- Third-party pools, wallets, or services that aren't operator-run
- Issues in unmodified upstream Bitcoin / Dash code that have already been disclosed
- Social engineering of community members or operator
- Physical attacks on operator hardware

## Source-Code Inheritance

Ratatoskr Core is forked from Dash Core (which is forked from Bitcoin Core). If you find an issue that originates upstream, please ALSO report it to:

- [Dash Security](https://www.dash.org/security) for Dash-derived code
- [Bitcoin Core Security](https://bitcoincore.org/en/contact/) for Bitcoin-derived code

We will coordinate on upstream-shared issues.

## GPG / Encrypted Communication

A GPG public key for security correspondence will be published at `https://ratatoskr.enchantedforestdefi.com/.well-known/pgp` alongside the v1.0.0 launch. Until then, please use plain email at the address above for initial contact; we will exchange a key for any subsequent sensitive details.

---

**Thank you** for taking the time to report responsibly. Security review is genuinely valued and is the single most-leveraged contribution to a young chain.
