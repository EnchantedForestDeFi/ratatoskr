# Changelog

All notable changes to Ratatoskr Core are documented here. The format roughly follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project uses [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

Pre-release candidates (`-rc1`, `-rc2`, etc.) precede each major release.

---

## [Unreleased]

Tracking work intended for v1.0.0 final or later. The active queue is tracked in EFD's internal project calendar.

---

## [v1.0.0-rc3] — 2026-05-15

**Pre-release.** Release candidate 3 for the v1.0.0 mainnet launch.

### Changed
- **build:** Windows Qt cross-build compatibility (mechanical MinGW C++20 ranges compat fix) — port of upstream commit [`63ea965`](https://github.com/SmartiesCoin/Smartiecoin/commit/63ea965) from SMT v0.3.1. 12 files, no logic changes.

### Binaries
- `ratatoskr-v1.0.0-rc3-win64.tar.gz` — Windows x86_64 daemon + Qt wallet (42 MB)
- `ratatoskr-v1.0.0-rc3-linux-x86_64.tar.gz` — Linux x86_64 daemon + Qt wallet (30 MB)
- Verification via `SHA256SUMS`

### Verified
- Qt splash screen renders cleanly on Windows
- All six binaries present (daemon, qt, cli, tx, util, wallet)
- Daemon CLI self-reports as `v1.0.0-rc3-dirty` (the `-dirty` suffix reflects the working tree state at build; final v1.0.0 will be a clean build)

---

## [v1.0.0-rc2] — 2026-05-14

**Pre-release.** Release candidate 2.

### Changed
- **build:** include `<cstdint>` for mingw chain params ([`65b3007`](../../commit/65b3007))
- **build:** include `<cstdint>` for chain helper ([`020cad4`](../../commit/020cad4))
- **build:** include `<cstdint>` for UI interface ([`f788e5a`](../../commit/f788e5a))
- **build:** make fixed-width integer headers self-contained across 116 headers ([`c3f0d9a`](../../commit/c3f0d9a))

These are mechanical hygiene cherry-picks from upstream SMT v0.3.1, part of EFD's hybrid pragmatic upstream-tracking policy.

---

## [v1.0.0-rc1] — 2026-05-08

**Pre-release.** First release candidate for the v1.0.0 mainnet launch.

### Added
- **consensus:** mainnet activation heights locked for T-23 ([`a9133af`](../../commit/a9133af)). Block 25,000 = masternode payments activate, block 30,000 = first superblock window.
- **consensus, spork:** governance-adjustable miner/MN reward split via SPORK_25 ([`0e0d513`](../../commit/0e0d513)). Floors enforced in consensus (cannot be voted below 60/30/10).
- **website:** public landing page at <https://ratatoskr.enchantedforestdefi.com> ([`7e41967`](../../commit/7e41967)).
- **contrib/devtools:** WIF↔hex round-trip helper for the keygen ceremony ([`f24bb44`](../../commit/f24bb44)).

### Changed
- **docs(testnet):** rewrote for 27-day solo-testnet schedule with explicit risk acceptance ([`406fa58`](../../commit/406fa58)).
- **docs:** SMT-neutral B-minus prose pass across whitepaper, release notes, BitcoinTalk ANN, launch schedule ([`1de0d3b`](../../commit/1de0d3b), [`93f688a`](../../commit/93f688a), [`3a9b780`](../../commit/3a9b780)).
- **build, ci:** unified GitHub URLs to `EnchantedForestDeFi/ratatoskr` ([`c70f525`](../../commit/c70f525)).
- **consensus:** tightened quorum tier defaults and disabled on-chain superblock budget for mainnet ([`0d87ea7`](../../commit/0d87ea7)).
- **llmq, evo, masternode, coinjoin:** rebranded quorum + special-tx user-visible strings ([`be2cec9`](../../commit/be2cec9)).
- **rpc, wallet/rpc:** rebranded 90+ user-visible Smartiecoin strings to Ratatoskr ([`e7c2e94`](../../commit/e7c2e94)).
- **qt:** Ratatoskr brand strip on the Overview page ([`3a48abb`](../../commit/3a48abb)).
- **build, packaging:** OS-integration metadata aligned with Ratatoskr identity (icon, .desktop, package descriptions) ([`db5d365`](../../commit/db5d365)).

---

## [v1.0.0-alpha] — 2026-04-24

**Pre-release.** Pool ops + infrastructure verification only — not for end users.

Initial alpha build used to verify the pool stack (MiningCore + dedicated SMT daemon model, validated end-to-end 2026-04-29; pattern documented for RATR mainnet pool reuse at T-0). The chain itself was not yet ready for general use.

---

## Pre-fork history

Ratatoskr Core forked from Smartiecoin Core after the v0.3.0 release in early 2026. Pre-fork history is preserved in the git log; pre-fork tags (`v0.3.0`, `v0.3.1`, `v0.3.1-rcN`) refer to the Smartiecoin lineage.

For pre-fork changelog entries, see the upstream Smartiecoin Core repository.

Smartiecoin itself forked from Dash Core 22.x; Dash forked from Bitcoin Core. Acknowledgement and credits to both upstream projects are in the README and the whitepaper.

---

[Unreleased]: https://github.com/EnchantedForestDeFi/ratatoskr/compare/v1.0.0-rc3...HEAD
[v1.0.0-rc3]: https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-rc3
[v1.0.0-rc2]: https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-rc2
[v1.0.0-rc1]: https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-rc1
[v1.0.0-alpha]: https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-alpha
