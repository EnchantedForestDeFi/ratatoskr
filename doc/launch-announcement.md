# Ratatoskr (RATR) — Launch Announcement

**Mainnet launch: June 1, 2026 · 00:00 UTC**

---

## What

**Ratatoskr** is a small, opinionated masternode cryptocurrency. Yespower
CPU-mined, 60-second blocks, 100M max supply, bridged to Alephium from
day one.

Think "Dash, but re-weighted so miners come first, governance can't dilute
them overnight, and the treasury funds itself transparently every block."

- **Ticker:** RATR
- **Algorithm:** yespower v1.0 (N=2048, R=32) — CPU-only, no ASICs
- **Block time:** 60 seconds
- **Max supply:** 100,000,000 RATR
- **Initial reward:** 50 RATR/block · halves every 1,030,596 blocks (~2 years)
- **Reward split (post block 25,000):** 60% miner / 30% masternode / 10% treasury
- **Masternode tiers:** Regular 7,500 RATR (1× voting) / EvoNode 30,000 RATR (4× voting). Per-block MN payments from block 25,000
- **Bridge:** native Alephium bridge (wRATR token) live at launch

---

## Why another coin

Most masternode coins have drifted toward *payment extraction*: governance
votes keep reallocating reward away from miners until the network is a
thin hash-rate shell around a rich MN set. Eventually the miner economics
break, security collapses, and the chain goes dark.

Ratatoskr bakes a different social contract into consensus:

1. **Miners get paid first.** The 60% miner share is hard-coded in v1.0 and
   protected by a floor-and-cap governance rule in v1.1 (no single vote
   can move the split more than 5% in either direction).
2. **Masternodes earn, they don't extract.** 30% is a durable, predictable
   share — enough to run infrastructure and earn yield, not enough to
   starve PoW.
3. **Treasury drips, not dumps.** 10% of every block goes to the treasury
   address, visible on-chain in real time. No biweekly superblock batches,
   no "governance proposal" gating. The treasury is always funded, always
   accountable.
4. **Bridge-first.** wRATR on Alephium is live from launch. RATR isn't a
   walled garden; liquidity can flow immediately.

---

## Launch-day paths

### Miners

- **Pool** (recommended) — standard stratum+yespower. Pool URL TBD, will
  be posted at <https://github.com/EnchantedForestDeFi/ratatoskr>.
- **Daemon solo** — `ratatoskrd -gen=1 -genproclimit=N` or
  `setgenerate true N` in Qt debug console. Zero dependencies.
- Full guide: [`doc/mining.md`](mining.md).

### Masternode operators

- Two MN tiers: **Regular 7,500 RATR** (1× vote) and **EvoNode 30,000 RATR**
  (4× vote). MN payments activate at block **25,000** (~17 days post-launch).
  Standard DIP0003 deterministic MN registration.
- Setup guide published before launch.

### Bridge users

- Alephium bridge live at launch. Wrap RATR → wRATR and back.
- Bridge architecture has been deployed and verified in production
  since April 2026 (bidirectional operation, no custodial incident).

### Pool operators

- Full integration spec: [`doc/pool-operator-spec.md`](pool-operator-spec.md).
- Please email `releases@enchantedforestdefi.com` with your pool URL so we can
  list you on day one.

---

## Governance

**v1.0 (launch):** reward split hard-coded at 60/30/10. Superblock
infrastructure is present but reserved for non-treasury governance
payouts (community grants, etc.).

**v1.1 (6-12 months post-launch, hard fork):** governance-editable reward
split, with guardrails:

- **Floors:** miner ≥ 50%, treasury ≥ 5%
- **Cap:** no single vote can move any share more than 5 percentage points
- **Cooldown:** one split change per 30-day window

Miners who show up on day one are protected from being diluted by day 100.

---

## Treasury

- **v1.0 launch:** single-sig, air-gapped cold key. Address published in
  release notes before June 1.
- **v1.1 roadmap:** multisig (3-of-5 or 2-of-3), hardware-wallet-secured.

Every drip is visible on-chain in real time; any wallet can verify
`sum(treasury_outputs) == blocks_mined × 10% × subsidy`.

---

## Team & transparency

Ratatoskr is built by an operator with production cross-chain bridge
infrastructure already running on Alephium since April 2026 (bidirectional
operation, no custodial incident). Code is a small delta on top of the
well-audited Dash codebase — RATR-specific consensus changes are documented
in `doc/pool-operator-spec.md` and the whitepaper.

**Alpha caveats (honest list):**

- Chain is not yet live. Launch is 2026-06-01.
- No external audit of RATR-specific changes. Small delta on Dash, but
  the delta itself is unreviewed.
- Treasury is single-sig at v1.0. Multisig is v1.1+.
- Reward split is hardcoded at v1.0. Governance editability is v1.1+.

---

## Links

- **Repo:** <https://github.com/EnchantedForestDeFi/ratatoskr>
- **Releases:** <https://github.com/EnchantedForestDeFi/ratatoskr/releases>
- **Whitepaper:** [`doc/whitepaper.md`](whitepaper.md)
- **Miner guide:** [`doc/mining.md`](mining.md)
- **Pool spec:** [`doc/pool-operator-spec.md`](pool-operator-spec.md)
- **Discord:** EnchantedForestDeFi server, channel `#ratatoskr` (live before launch)
- **Contact:** `releases@enchantedforestdefi.com`

---

## Countdown

See you on June 1, 2026 at block 1.
