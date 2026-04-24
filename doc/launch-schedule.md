# Ratatoskr Launch Schedule (T-38 to T+7)

**Today: 2026-04-24 · Launch: 2026-06-01 00:00 UTC · 38 days out**

Week-by-week sequencing of everything that needs to happen before mainnet
launch and the first week post. Each week has a **must-do**, a **nice-to-do**,
and a **contingency time budget**.

---

## Week 1 — T-38 to T-32 · 2026-04-27 → 2026-05-03

**Theme: close out launch-critical code, publish all docs.**

### Must-do
- [ ] Finish Windows cross-compile (pre-treasury-bake build) · ~2h
- [ ] Republish v1.0.0-alpha release with Linux + Windows tarballs · ~30m
- [ ] Verify both tarballs smoke-test (`--version` + RPC basic call) · ~30m

### Nice-to-do
- [ ] Post Discord teaser (from `doc/discord-teaser.md`) · ~10m
- [ ] Contact a second pool operator as backup · ~30m

### Contingency: **4h**

---

## Week 2 — T-31 to T-25 · 2026-05-04 → 2026-05-10

**Theme: treasury keygen + bridge contract deploy. The security-critical week.**

### Must-do
- [ ] **Treasury address keygen** (air-gapped) per `doc/treasury-keygen-runbook.md` · ~1h
- [ ] Bake real treasury script into `chainparams.cpp` · ~15m
- [ ] Rebuild Linux + Windows tarballs from post-treasury commit · ~2h
- [ ] Tag `v1.0.0` proper, upload to GitHub Release · ~30m
- [ ] Publish signed attestation from treasury key · ~15m
- [ ] **Bridge Ralph contract deploy on Alephium mainnet** (WrappedRATR + RatatoskrBridge) · ~2h
- [ ] Bridge relayer keypair (can share air-gapped session with treasury) · ~1h
- [ ] Configure bridge `.env` on VPS · ~30m

### Nice-to-do
- [ ] Publish bridge contract IDs in memory + whitepaper · ~15m
- [ ] Run bridge relayer on testnet end-to-end · ~2h

### Contingency: **6h**

---

## Week 3 — T-24 to T-18 · 2026-05-11 → 2026-05-17

**Theme: testnet launch.**

### Must-do
- [ ] Regenerate testnet genesis block with new nTime (~2026-05-15) · ~1h
- [ ] Commit + publish testnet-ready v1.0.0-alpha.2 · ~1h
- [ ] **Testnet mainnet-style launch** on both VPS seeds · ~2h
- [ ] Mine first testnet block · ~30m
- [ ] Verify treasury drip on testnet block 1 · ~15m
- [ ] Verify MN payment activation at testnet block 1,000 (~17h post-start) · ~30m to check

### Nice-to-do
- [ ] Pool operator tests stratum against testnet · ~2h coordination
- [ ] Discord post "testnet live, everyone welcome" · ~15m

### Contingency: **8h** (most likely week to catch bugs)

---

## Week 4 — T-17 to T-11 · 2026-05-18 → 2026-05-24

**Theme: testnet monitoring + bridge activation on testnet.**

### Must-do
- [ ] Monitor testnet daily (block production, peer health, treasury accuracy) · 15m/day
- [ ] **Bridge testnet activation** — wRATR mint/burn E2E tested · ~2-4h
- [ ] nginx + DNS + certbot for `ratatoskrbridge.enchantedforestdefi.com` · ~30m
- [ ] Wire bridge frontend to testnet contracts for final preview · ~1h

### Nice-to-do
- [ ] Bridge dashboard showing supply parity (wRATR minted ⇔ RATR locked) · ~2h
- [ ] Liquidity seed on Elexium testnet if possible · ~1h

### Contingency: **6h**

---

## Week 5 — T-10 to T-4 · 2026-05-25 → 2026-05-31

**Theme: final freeze, pre-flight, paperwork.**

### Must-do (T-10 to T-7)
- [ ] All intended v1.0.0 code merged to `main`, branch frozen · ~30m
- [ ] Final tarball rebuild (if any fixes landed during testnet) · ~2h
- [ ] Republish v1.0.0 GitHub Release · ~30m
- [ ] Testnet wind-down post in Discord · ~15m

### Must-do (T-7 to T-4)
- [ ] Run the **full launch-day pre-flight** per `doc/launch-day-runbook.md` · ~1h
- [ ] Verify seed1 + seed2 nodes have final binary, systemd unit staged · ~30m
- [ ] Verify DNS + TLS on all sub-domains · ~15m
- [ ] Confirm bridge relayer keys + contracts ready (but service not started) · ~15m
- [ ] Pool operator confirms stratum URL + reveals to community · ~30m

### Contingency: **4h**

---

## Week 6 — T-3 to T-0 · 2026-05-29 → 2026-06-01

**Theme: final approach, do nothing risky.**

### Must-do
- [ ] Post T-72h Discord update · ~10m
- [ ] Post T-24h countdown update · ~10m
- [ ] Sleep before launch · priceless
- [ ] **T-0 launch** per `doc/launch-day-runbook.md` · ~2-4h

### Do NOT
- [ ] No code changes
- [ ] No config tweaks to seeds
- [ ] No DNS changes
- [ ] No new features

### Contingency: **2h**

---

## Post-launch Week 1 — T+1 to T+7 · 2026-06-01 → 2026-06-08

**Theme: bridge activation, liquidity, monitoring.**

### Must-do
- [ ] Monitor chain health every 6h for 48h · as-needed
- [ ] **Activate bridge relayer** (start pm2 process, test E2E with tiny amount) · ~2h
- [ ] Announce bridge live in Discord · ~15m
- [ ] **Seed wRATR/NUTTY Elexium pool** with initial liquidity · ~1h
- [ ] Cast Elexium gauge vote for wRATR/NUTTY · ~15m
- [ ] Daily Discord update for 7 days · 10m/day

### Nice-to-do
- [ ] Post-mortem write-up · ~1h
- [ ] DNS TTL bump from 300 → 3600 on stable records · ~15m
- [ ] Seed3 node (optional third seed) · ~2h
- [ ] Block explorer standup · ~4-8h (or point to a community-run one)

### Contingency: **8h**

---

## Grand total time budget

| Phase | Active work | Contingency | Total |
|---|---|---|---|
| Pre-launch (weeks 1-5) | ~22h | ~28h | ~50h |
| Launch week (week 6) | ~4h | ~2h | ~6h |
| Post-launch week 1 | ~6h | ~8h | ~14h |
| **Total** | **~32h** | **~38h** | **~70h** |

Over 6 weeks that's **~10-12h/week average** — doable for a solo operator with a day job, especially if concentrated on weekends.

---

## Ordered critical path

The one sequence that cannot overlap:

1. Treasury keygen (week 2)
2. Treasury script → chainparams (week 2)
3. Final v1.0.0 build (week 2-3)
4. Testnet (week 3)
5. Testnet validates (week 3-4)
6. Mainnet launch (week 6)
7. Bridge activation (post-launch week 1)

Everything else (docs, pool coordination, DNS, nginx) is parallel or flexible.

---

## Key risk checkpoints

| Checkpoint | Decision if it fails |
|---|---|
| T-31: treasury keygen successful + verified | Stop, retry with fresh machine |
| T-24: testnet produces blocks | Investigate, delay launch 1-2 weeks if needed |
| T-18: treasury drip working on testnet | Consensus bug — halt launch prep, patch |
| T-11: bridge E2E works on testnet | Launch chain, defer bridge 1-2 weeks |
| T-0: seed1 + seed2 both up + peered | Start mining block 1 |
| T+0: block 1 has treasury output | Chain is good, proceed |
| T+6h: bridge relayer works live | Announce bridge; if not, silent-fail until fixed |

---

## What we've done already

- ✅ Consensus code: treasury drip (`8108524`), MN tiers (7500/30000), chainparams rebrand
- ✅ Tarballs: Linux alpha-13 built + deployed to VPS2
- ✅ Docs: whitepaper, mining, pool-op spec, MN setup, quickstart, testnet plan, launch runbook, treasury keygen runbook, release notes
- ✅ VPS2 set up: SSH-hardened, RATR binary ready, systemd unit staged, nginx + TLS, countdown landing page
- ✅ DNS: seed1, seed2 A-records at IONOS
- ✅ GitHub: release v1.0.0-alpha published, `main` ahead of upstream
- ✅ Discord teaser drafted
- ✅ Bridge repo cloned to VPS2 (private, SSH auth)

So we're starting week 1 already ~40% ahead of schedule.
