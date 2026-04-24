# Launch Outreach Contact List

For a smooth mainnet launch you need more than code — you need people
ready to move with you. This tracks who to contact, what to ask for,
and when.

---

## A. Pool operators (CRITICAL)

### A1. Primary pool (already engaged)
- **Who:** [TBD — name in your Discord DMs]
- **Status:** ✅ confirmed listing at launch
- **What's next:**
  - [ ] Share final v1.0.0 binary when it's built (post-treasury bake)
  - [ ] Get their stratum URL + port 1 week pre-launch
  - [ ] Add their URL to `doc/mining.md` and GitHub release notes
  - [ ] Coordinate T-0 activation (they flip stratum live once block 1 mined)

### A2. Backup pool (target: 1 more)
- **Candidates to approach:** any yespower pool op (look at SMT's pool list, Memeium, Yenten, other yespower coins' pool operators)
- **Ask:** "yespower CPU coin launching June 1, same tokenomics vibe as Dash but miner-first, looking for 2nd pool for redundancy. Spec: github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/pool-operator-spec.md"
- **Why:** if primary pool has a day-1 outage, backup keeps the network alive

### A3. Solo miner ops (nice to have)
- DMs in yespower-coin Discord servers — "launching June 1, anyone CPU mining want in early?"

---

## B. Exchange listings (DEFERRED until chain is alive)

Don't contact exchanges pre-launch — they want to see:
- Live chain with blocks
- Some volume / users
- A few weeks of history

**Plan:** contact after T+14 days.

### Tier-3 exchange targets (realistic for day-30 listing)
- **NestEx** — same exchange SMT listed on; they know the team
- **KlingEx** — same
- **Gatevia** — same
- **Notes:** they charge listing fees typically $500-5000. Budget from treasury.

### Tier-2 (nice to have, day-60+)
- **Mercatox, CoinEx, TradeOgre** — higher standards, need volume proof first

### DO NOT pursue at launch
- Binance, Kraken, Coinbase — not realistic for a small launch; waste of effort

---

## C. Aggregators + data sources (DAY-3 to T+30)

Once chain is running:

### C1. CoinGecko (priority 1)
- **URL:** https://www.coingecko.com/en/request_form
- **Needs:** explorer link, website, social links, token contract (N/A — native), supply data
- **Timeline:** submit T+7, typical approval T+14 to T+30
- **What it unlocks:** price feeds for our bridge site, legitimacy signal for listers

### C2. CoinMarketCap (priority 2)
- **URL:** https://coinmarketcap.com/request/
- **Stricter than CoinGecko.** Requires at least 2 exchange listings with volume
- **Realistic timeline:** T+30 after we have NestEx + one other listed

### C3. Alephium DeFi dashboards
- Reach out once wRATR bridge is live with liquidity
- Elexium dashboards — may auto-list the pool
- PowFi — track as they mainnet

### C4. miningpoolstats.stream
- yespower-coins auto-aggregate if pools submit. Ask primary pool op to add RATR

---

## D. Content + community (ONGOING)

### D1. Discord build-out
- [ ] Move #ratatoskr from a channel inside EnchantedForestDeFi to its own small server if growth warrants
- [ ] Pin: whitepaper, quickstart, mining, launch date
- [ ] Set up bots: price ticker (post-listing), block-height tracker (post-launch)

### D2. Reddit (LOW priority, modest effort)
- r/CryptoCurrencyClassic (allows low-cap projects)
- r/yespower (if exists, or the general PoW subs)
- r/Alephium (bridge angle)
- **What to post:** launch announcement T-7 days + launch confirmation T+0
- **What NOT to do:** spam, shill, cross-post aggressively. One honest post per sub.

### D3. X / Twitter
- Post from your handle or project handle
- Pre-launch teaser T-30, T-7, T-1 day
- Launch-day post with block 1 hash + screenshot
- Engage with Alephium community replies

### D4. Bitcointalk (OPTIONAL)
- Forum still has a small but active altcoin audience
- Announcement thread is free, gets archived
- Skip if you're time-constrained — modest ROI

---

## E. Masternode hosting partners (NICE TO HAVE)

People who host MNs for a fee on behalf of owners. Important for:
- Owners who don't want to run their own VPS
- Broader MN participation

### E1. Nodes24.online
- **Per memory:** already used for SMT MN economics reference ($1.75/mo flat rate)
- **Ask:** "can you list RATR alongside SMT? same architecture, 7,500 RATR regular / 30,000 EvoNode. Spec: [link]"
- **Timeline:** contact T-14

### E2. Other MN hosts (optional)
- DMNservice, MasternodeHosting, ChainHosting, Allnodes (tier-1, high standards — probably not reachable pre-launch)

---

## F. Bridge-adjacent (ELEXIUM + NUTTY side)

### F1. Elexium community
- **Your angle:** "new wRATR/NUTTY pool going live right after RATR mainnet, voting gauge, bribe budget. Heads up for gauge voters."
- **Channels:** Elexium Discord, Telegram
- **Timeline:** T-7 (awareness), T+1 (live announcement)

### F2. Alephium ecosystem feeds
- Alephium Discord, Twitter, newsletter
- Ask core team (politely, once) if they'll mention the bridge as an ecosystem addition
- Don't over-ask — they're busy

---

## G. Security + trust signals (LOW effort, HIGH trust)

### G1. Public audit firms (POST-LAUNCH, PHASE 2)
- Not blocker for launch
- Plan: get informal review or light audit T+60 to T+90
- Good options: Least Authority, OpenZeppelin (expensive), Consensys (expensive). For RATR budget, lean informal: pay a trusted dev $1-3k for a security review sprint.

### G2. Bug bounty
- Small bounty program post-launch ($100-500 for findings)
- Run via GitHub issues, no formal platform needed

---

## H. Personal network (FREE, high conversion)

People who already trust you will check out RATR if you ask them to. Don't underuse this:

- SMT holders you interacted with in bridge
- Anyone who's ever mined SMT
- Crypto-adjacent friends
- Ex-colleagues who dabble

**Ask:** "small coin I'm launching June 1, would appreciate a retweet / mine a block / give feedback on the whitepaper. No pressure."

---

## I. When to contact each group

| Group | First touch | Launch touch | Post-launch |
|---|---|---|---|
| Primary pool (A1) | ✅ done | T-7 confirm | T+7 thank-you |
| Backup pool (A2) | T-28 | T-14 if engaged | T+7 if listed |
| Personal network (H) | T-14 friendly | T-1 ask | T+7 thank-you |
| Discord / forums (D) | T-21 teaser | T-1 countdown | T+0 live post |
| MN hosts (E) | T-14 | T-7 | T+14 |
| Exchanges (B) | DO NOT | DO NOT | T+14 |
| CoinGecko (C1) | DO NOT | DO NOT | T+7 submit |
| CoinMarketCap (C2) | DO NOT | DO NOT | T+30+ |
| Audit firms (G1) | DO NOT | DO NOT | T+60 |

---

## J. What to say (templates)

### Pool op cold DM
```
Hey — quick one. Launching a small yespower CPU coin June 1 (Ratatoskr).
Dash-lineage, 60s blocks, 60/30/10 miner/MN/treasury, MN tiers 7500/30000.
Pool spec: github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/pool-operator-spec.md
Binary: github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-alpha
No pressure — just wanted you on the radar. Would be great to have you
list if it looks interesting.
```

### Personal network
```
Launching a little crypto project June 1 — Ratatoskr (RATR). Yespower CPU
mineable, bridged to Alephium. Fully opensource. Would love a look +
any honest feedback on the whitepaper:
github.com/EnchantedForestDeFi/ratatoskr
```

### CoinGecko listing submission (T+7 to T+30)
Structured form, see their request page. Have ready:
- Block explorer URL
- Official site URL
- Whitepaper URL
- Discord, Twitter, GitHub links
- Logo (512x512 PNG)
- Max supply / current supply / inflation schedule
- Launch date
- Contact email

---

## K. Metrics to track

After launch, watch:
- GitHub stars / forks
- Discord member count
- Pool registered miners + hashrate
- wRATR bridge volume
- Elexium pool TVL + gauge votes
- Unique wallet addresses (via block explorer)
- Any press / mentions

Use for: improving outreach in weeks 2-8, deciding where to invest follow-up effort.
