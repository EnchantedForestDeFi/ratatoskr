# Ratatoskr Launch Schedule

**Mainnet target:** 2026-06-01 00:00 UTC (block 1)

## Phase overview

| Phase | When | Public-facing milestones |
|---|---|---|
| Pre-launch | T-30 → T-3 | Testnet active (closed alpha). Release candidate builds published. |
| Launch | T-0 (2026-06-01) | Mainnet genesis block. v1.0.0 binaries on GitHub Releases. Seed nodes online. Block explorer public. |
| Stabilization | T+0 → T+5d | PoW-only window. Sporks `SPORK_2_INSTANTSEND_ENABLED`, `SPORK_3_INSTANTSEND_BLOCK_FILTERING` ready to enable as soon as enough masternodes register. |
| Masternode activation | T+5d → T+17d | DIP3 enforcement, ChainLocks (LLMQ_50_60), InstantSend become active per `doc/spork-launch-timing.md`. |
| Bridge live | T+0 (with mainnet) | wRATR ↔ ALPH bridge transitions from pre-launch banner to fully live at `ratatoskrbridge.enchantedforestdefi.com`. |
| Treasury active | T+~25,000 blocks | Per-block treasury drip begins. |

## What users can do, when

| Window | What you can do |
|---|---|
| Pre-launch | Read the [whitepaper](whitepaper.md), follow the [Discord](https://discord.gg/SrffQVYqee), build from source. |
| T-0 → T+5d | Download the wallet, mine RATR (yespower CPU PoW), send/receive transactions. |
| T+5d onward | Register a masternode (see [masternode-setup.md](masternode-setup.md)), use InstantSend, observe ChainLocks. |
| Bridge live | Wrap RATR → wRATR on Alephium via [the bridge](https://ratatoskrbridge.enchantedforestdefi.com). |

## Resources

- [Quickstart](quickstart.md)
- [Mining guide](mining.md)
- [Masternode setup](masternode-setup.md)
- [Spork timing reference](spork-launch-timing.md)
- [Whitepaper](whitepaper.md)
- [Releases](https://github.com/EnchantedForestDeFi/ratatoskr/releases)

---

*This page describes user-visible milestones. Operator runbooks and internal coordination are kept separately.*
