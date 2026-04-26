# Bitcointalk ANN draft — Ratatoskr Core (RATR) [yespower]

**Status: HOLD.** Not yet published. Targeting **T-21 days (May 11, 2026)** as
the publish window — early enough that pool operators have time to onboard
before mainnet, late enough that the daemon binaries on GitHub are stable
release candidates rather than alpha builds.

**Where to post:**
- Primary: [Bitcointalk → Alternate cryptocurrencies → Announcements (Altcoins)](https://bitcointalk.org/index.php?board=159.0)
- Account: post from operator's existing bitcointalk handle (the one used
  for the Smartiecoin ANN — gives instant trust signal among yespower pool
  operators who already know that handle)

**Pre-post checklist:**
- [ ] Block explorer is publicly resolvable at
      `explorer.ratatoskr.enchantedforestdefi.com`
- [ ] Linux + Windows binaries published as a GitHub release tag
      (`v1.0.0-rc1` minimum)
- [ ] At least one yespower pool operator has confirmed they will add RATR
      at mainnet launch
- [ ] Whitepaper PDF available at a stable URL
- [ ] Discord invite is permanent (not expiring) and verified working
- [ ] Treasury keygen ceremony complete + final v1.0 build cut with the
      real `treasuryPaymentScript`
- [ ] Genesis block hash confirmed and added to spec table below

**Edits to make before posting:**
- Replace `[GENESIS_HASH_TBD]` with the real mainnet genesis block hash
- Replace `[POOL_LIST_TBD]` with confirmed pool URLs once 1–3 are committed
- Replace `[WHITEPAPER_URL]` with the public whitepaper link
- Update the "current status" line based on actual binaries ready

---

## The post (BBCode, ready to paste)

```bbcode
[center][size=18pt][color=#5B3A29][b]Ratatoskr Core [RATR] — yespower CPU PoW, masternodes, native bridge to Alephium[/b][/color][/size]

[i]The squirrel that runs the tree.[/i][/center]

[hr]

[size=14pt][b]🌳 What is Ratatoskr?[/b][/size]

In Norse mythology, [b]Ratatoskr[/b] is the squirrel who runs up and down [b]Yggdrasil[/b], the world tree, carrying messages between the eagle at the crown and the serpent at the roots. Ratatoskr (RATR) is a yespower CPU-mineable proof-of-work cryptocurrency with deterministic masternodes, a treasury fund for ecosystem development, and a [b]native cross-chain bridge to Alephium[/b]. The metaphor is the project: RATR is the messenger between chains.

The partner chain RATR bridges to is [b]Alephium[/b] — phonetically cognate with [b]Alfheim[/b], one of the upper realms of Yggdrasil. The naming wasn't random. Each future bridge endpoint becomes another realm hanging from the tree's branches.

[hr]

[size=14pt][b]⚙️ Core specifications[/b][/size]

[code]
Ticker             RATR
Algorithm          yespower (CPU-mineable, ASIC-resistant)
Block time         60 seconds
Block reward       50 RATR (initial)
Halving interval   1,030,596 blocks (~2 years)
Max supply         100,000,000 RATR
Reward split       60% miner / 30% masternode / 10% treasury
P2P port           9393  (mainnet)
RPC port           8282  (mainnet)
Premine            None
ICO                None
Genesis            [GENESIS_HASH_TBD]
Mainnet launch     2026-06-01 00:00 UTC
[/code]

[hr]

[size=14pt][b]💰 Reward distribution (per block)[/b][/size]

[table]
[tr][td][b]Slice[/b][/td][td][b]%[/b][/td][td][b]Per block[/b][/td][td][b]Notes[/b][/td][/tr]
[tr][td]Miner[/td][td]60%[/td][td]30 RATR[/td][td]Direct to coinbase[/td][/tr]
[tr][td]Masternode[/td][td]30%[/td][td]15 RATR[/td][td]Activates at block 25,000 (T+~17 days)[/td][/tr]
[tr][td]Treasury[/td][td]10%[/td][td]5 RATR[/td][td]Per-block drip to air-gapped multisig (not superblock-gated)[/td][/tr]
[/table]

The 60/30/10 split is consensus-fixed. No on-chain superblock pool — treasury funding is continuous and operator-managed, with all allocations published transparently. Governance proposals (`gobject submit` / `gobject vote`) remain functional as community signal-of-intent.

[hr]

[size=14pt][b]🐿️ Masternode tiers (DIP3)[/b][/size]

[table]
[tr][td][b]Tier[/b][/td][td][b]Collateral[/b][/td][td][b]Voting weight[/b][/td][td][b]Use[/b][/td][/tr]
[tr][td]Regular MN[/td][td]7,500 RATR[/td][td]1×[/td][td]Quorum signing, payments, governance[/td][/tr]
[tr][td]EvoNode[/td][td]30,000 RATR[/td][td]4×[/td][td]Same as Regular plus L2 platform reservation (post-launch)[/td][/tr]
[/table]

Standard DIP3 split-key model: collateral never leaves your wallet. Operator key handles signing only. You retain owner + voting + payout keys.

[hr]

[size=14pt][b]⛏️ Mining info — yespower CPU[/b][/size]

[b]Algorithm:[/b] yespower — memory-hard, designed to remain economical on general-purpose CPUs. Same algorithm family used by Smartiecoin, Sugarchain, Cranepay, Yenten, and others.

[b]Solo mining:[/b] Run a yespower-aware miner against your local node:
[code]
cpuminer -a yespower \
  -o http://127.0.0.1:8282 \
  -u <rpcuser> -p <rpcpassword> \
  --coinbase-addr=<your_RATR_receiving_address>
[/code]

[b]Pool mining:[/b] [POOL_LIST_TBD]
- Pool 1 (URL pending operator confirmation)
- Pool 2 (URL pending operator confirmation)

[b]For pool operators considering adding RATR:[/b] yespower setup is functionally identical to existing yespower coins. RPC port 8282, P2P port 9393, getblocktemplate works, BIP22/BIP23 standard. Direct your "supported coins" config at the local ratatoskrd RPC. Reach out via Discord (link below) for the latest binaries and any integration questions.

[hr]

[size=14pt][b]🔗 Cross-chain bridge to Alephium[/b][/size]

RATR ships with a bidirectional bridge to [url=https://alephium.org]Alephium[/url] launching alongside mainnet:

[list]
[li]Wrap RATR → wRATR on Alephium (1:1 backed)[/li]
[li]Unwrap wRATR → RATR on Ratatoskr (burn-on-Alephium → mint-on-RATR)[/li]
[li]Operator-run relayer (transparency report monthly)[/li]
[li]Hard caps: 100M absolute / 500K soft / 1K min wrap / 50K max per tx / 2K daily limit per address[/li]
[li]wRATR token already verified on Alephium testnet (group 0)[/li]
[/list]

Bridge UI: [url=https://ratatoskrbridge.enchantedforestdefi.com]ratatoskrbridge.enchantedforestdefi.com[/url]

[hr]

[size=14pt][b]📅 Activation timeline (block heights)[/b][/size]

[code]
T+0       block 0      Mainnet launch, miners earn 60% of subsidy
T+~2min   block 2      DIP3 active — anyone can register a masternode
T+~5 days block 7,500  Small-network LLMQ quorums activate (ChainLocks + InstantSend signable)
T+~17 day block 25,000 Masternode reward payments begin
T+~?      future       L2 Platform / EvoNode service layer (post-launch milestone)
[/code]

The first ~5 days run on pure-PoW security while the masternode network bootstraps. Treasury + operator MNs spin up at block 2. Community MNs welcome from block 2 onward — payment queue order matches registration time, so early registration = earlier first payout (no benefit to waiting).

[hr]

[size=14pt][b]🔒 Honest pre-launch disclosures[/b][/size]

[list]
[li][b]Operator concentration at launch is real.[/b] Day 1, ~17 masternodes will exist (treasury EvoNodes + operator personal MNs). Community MN onboarding starts immediately and payment queue order favors early registrants. Network decentralizes as community uptake grows.[/li]
[li][b]Treasury MNs abstain from governance[/b] per the chain's treasury governance policy. They participate in quorum signing (ChainLocks + InstantSend) but not in proposal voting.[/li]
[li][b]No premine, no ICO, no founder reward.[/b] All RATR enters circulation via mining + the per-block treasury drip starting block 1.[/li]
[li][b]Treasury keys held in air-gapped 5-of-7 multisig.[/b] Custodian list + recovery procedures published in the whitepaper.[/li]
[li][b]Pre-launch builds are alpha.[/b] Treat any binary released before mainnet as "infrastructure pre-staging" — chain isn't live, daemon will idle at height 0 with no peers. The final v1.0 release with real treasury script is published before T-0.[/li]
[/list]

[hr]

[size=14pt][b]📦 Downloads (when published)[/b][/size]

GitHub releases: [url=https://github.com/EnchantedForestDeFi/ratatoskr/releases]github.com/EnchantedForestDeFi/ratatoskr/releases[/url]

[list]
[li]Linux x86_64 — daemon + Qt GUI wallet (.tar.gz)[/li]
[li]Windows x86_64 — Qt GUI wallet (.exe / .zip)[/li]
[li]macOS arm64 — Qt GUI wallet .app bundle[/li]
[li]Source code — MIT license[/li]
[/list]

Build instructions: see `doc/build-unix.md`, `doc/build-osx.md`, `doc/build-windows.md` in the source tree.

[hr]

[size=14pt][b]🔗 Resources[/b][/size]

[table]
[tr][td][b]Website[/b][/td][td][url=https://ratatoskr.enchantedforestdefi.com]ratatoskr.enchantedforestdefi.com[/url][/td][/tr]
[tr][td][b]Source code[/b][/td][td][url=https://github.com/EnchantedForestDeFi/ratatoskr]github.com/EnchantedForestDeFi/ratatoskr[/url][/td][/tr]
[tr][td][b]Block explorer[/b][/td][td][url=https://explorer.ratatoskr.enchantedforestdefi.com]explorer.ratatoskr.enchantedforestdefi.com[/url][/td][/tr]
[tr][td][b]Bridge[/b][/td][td][url=https://ratatoskrbridge.enchantedforestdefi.com]ratatoskrbridge.enchantedforestdefi.com[/url][/td][/tr]
[tr][td][b]Discord[/b][/td][td][url=https://discord.gg/SrffQVYqee]discord.gg/SrffQVYqee[/url][/td][/tr]
[tr][td][b]Whitepaper[/b][/td][td][[WHITEPAPER_URL]][/td][/tr]
[tr][td][b]Twitter/X[/b][/td][td](TBD)[/td][/tr]
[/table]

[hr]

[size=14pt][b]🌳 Why Ratatoskr?[/b][/size]

There are a lot of small Dash-fork coins. Most don't last. The ones that do tend to share three traits:

[b]1. They keep miners profitable.[/b] Extractive splits (40-60% to founders/dev/treasury) drive miners away. RATR's 60/30/10 keeps the people securing the network earning the most. No founder reward, no permanent dev tax — just a 10% drip to a transparent multisig treasury.

[b]2. They have a genuine purpose.[/b] RATR is the messenger between chains. The Alephium bridge isn't a v2-roadmap promise — it's launching alongside mainnet. wRATR is already on Alephium testnet (verified group 0). The bridge is the product; the chain is the delivery layer.

[b]3. They're honest about scale.[/b] RATR is a small project. Realistic year-1 masternode count is 50–150, not 4,000. The chain's quorum tier is sized accordingly (LLMQ_50_60 for ChainLocks, with small-network LLMQ_10_60 fallback for the first month). The whitepaper makes the centralization-during-launch window explicit and time-bounds it.

If those three traits matter to you, RATR may be worth your hashrate.

[hr]

[size=11pt][i]This is experimental software. Not financial advice. Read the whitepaper before mining or staking. The chain is not yet live — running pre-launch builds now will idle at height 0. Mainnet activates 2026-06-01 00:00 UTC.[/i][/size]

[size=10pt]Part of the [url=https://enchantedforestdefi.com]EnchantedForestDeFi[/url] ecosystem.[/size]
```

---

## After posting — follow-up actions

| When | Action |
|---|---|
| Within 24h of OP | Subscribe to your own thread, watch for first questions. Pool operators often ask within day 1. |
| Day 1–7 | Reply to questions promptly. Slow replies kill ANN momentum. |
| Day 1–14 | Update the OP via edit when pool operators confirm support — list them under the "Pool mining" section. |
| Day 7 | DM MiningPoolStats with link to the bitcointalk thread + first 1–3 confirmed pool URLs. They'll add RATR to their tracker. |
| Mainnet T-3 | Update OP with final v1.0.0 release tag, real genesis hash, real binary hashes. |
| Mainnet T+0 | Update OP first line with "🟢 LIVE" and the launch block. |
| Mainnet T+5 | Once first quorum forms and ChainLocks active, post a follow-up reply confirming. |
| Mainnet T+17 | Post follow-up confirming MN payments started at block 25,000. |
| Ongoing | Reply with monthly treasury transparency report links. |

## Avoidable mistakes (lessons from the SMT thread)

The Smartiecoin ANN got criticized in-thread for using vague marketing
language ("revolutionizing," "cutting-edge") without substantive
differentiation claims, and for one user offering a 1 ETH bounty for
legitimate innovation answers that went unanswered.

The RATR draft above intentionally:

- **Avoids "revolutionary" / "cutting-edge" / "next-generation" entirely.** Claims are specific (60/30/10 split, real bridge launching alongside mainnet, lore-cognate naming) and verifiable.
- **Discloses centralization honestly** in a dedicated section.
- **Names the failure modes** of small Dash-forks and contrasts RATR's design choices.
- **Keeps "what's not done yet" visible** (Twitter TBD, whitepaper URL TBD, pool list TBD) — better to admit gaps than pretend they're filled.

If a 1-ETH-style bounty challenge lands in this thread, the answer is
already pre-written: **"Cross-chain bridge launching alongside mainnet,
operator-funded with no premine, lore-honest naming where the partner
chain's name is phonetically cognate to the realm it represents in the
metaphor. Verifiable on testnet now."**
