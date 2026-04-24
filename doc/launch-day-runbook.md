# Launch-Day Runbook — Ratatoskr Mainnet

**Target:** 2026-06-01 · 00:00 UTC · Block 1

This is the minute-by-minute for launch day. Every step has a verification
check — if a check fails, **stop and fix before proceeding** rather than
power through. We only get one clean launch.

Times are in UTC. Convert for your local zone: 00:00 UTC = 20:00 previous
day US Eastern, 01:00 UK, 02:00 Central Europe, 08:00 Singapore.

---

## T-7 days: freeze

- [ ] All intended code for v1.0.0 final merged to `main`
- [ ] Treasury address generated (per `doc/treasury-keygen-runbook.md`)
      and baked into `chainparams.cpp`
- [ ] Treasury address published (signed message) in release notes + README
- [ ] Linux + Windows tarballs rebuilt from the post-merge commit, hashes
      published at the GitHub release page
- [ ] Both seed nodes have the final binary installed (VPS1 `74.208.37.172`,
      VPS2 `178.238.224.23`)
- [ ] Systemd units on both boxes are installed but `disabled` (will enable
      on T-0)
- [ ] DNS for `seed1.ratatoskr.enchantedforestdefi.com` and
      `seed2.ratatoskr.enchantedforestdefi.com` has been resolving for ≥24 hr
- [ ] Pool operator(s) have the spec and the Linux tarball — give them at
      least a week so they can pre-stage
- [ ] Discord channel `#ratatoskr` is live; pinned: launch announcement
- [ ] Alephium bridge side has wRATR contract pre-deployed + verified on
      explorer (bridge contract ID captured in `alph_bridge_status.md`)
- [ ] Backup of treasury paper WIF verified (read it back one more time)

## T-24 hr: freeze verification

- [ ] On both VPS1 and VPS2 as root:
  ```bash
  systemctl status ratatoskrd  # loaded, inactive, disabled
  su - ratr -c '~/ratatoskr-*/bin/ratatoskrd --version | head -3'
  # confirm version string contains the post-treasury-bake commit hash
  ```
- [ ] From anywhere:
  ```bash
  dig seed1.ratatoskr.enchantedforestdefi.com +short
  dig seed2.ratatoskr.enchantedforestdefi.com +short
  # expect 74.208.37.172 and 178.238.224.23
  curl -I https://seed2.ratatoskr.enchantedforestdefi.com  # 200
  ```
- [ ] GitHub release page shows Linux + Windows tarballs with matching
      sha256 files
- [ ] Whitepaper, mining guide, pool spec, launch announcement all pushed
      to `main` on the repo
- [ ] **Sleep.** Set 3 alarms for 23:45 UTC. Caffeine staged.

## T-30 min (23:30 UTC): pre-flight

Open three terminals:
- **Terminal A**: ssh to VPS1 as root
- **Terminal B**: ssh to VPS2 as root
- **Terminal C**: Windows box with the mining binary unpacked

In A + B:
```bash
# sanity check the local time against NTP
timedatectl | grep "System clock synchronized"  # yes
date -u  # should show current UTC within 1s of actual

# verify config still points to correct data dir and RPC creds
cat ~ratr/.ratatoskrcore/ratatoskr.conf | grep -v '^#' | grep .

# confirm ports open on firewall
ufw status | grep 9393  # ALLOW

# dry-run: systemd unit can find binary + config
systemd-analyze verify ratatoskrd.service && echo "unit OK"
```

## T-5 min (23:55 UTC): ready to start

Don't start yet. The genesis nTime is `1777000000` (2026-04-23 UTC — already
past; chain is ready to start whenever the first miner finds a block).
**Mainnet is "live" the instant one node comes up AND a miner finds block 1.**

Convention: start the seed nodes first so any early-arriving external nodes
have something to sync against.

## T-0 (00:00 UTC): seed nodes live

In Terminal A (VPS1):
```bash
systemctl enable --now ratatoskrd
sleep 15
su - ratr -c 'ratatoskr-cli getblockchaininfo | head -15'
# expect: blocks 0, headers 0, chain "main", verificationprogress 1.0
```

In Terminal B (VPS2):
```bash
systemctl enable --now ratatoskrd
sleep 15
su - ratr -c 'ratatoskr-cli getblockchaininfo | head -15'
```

Verify peering:
```bash
# on A
su - ratr -c 'ratatoskr-cli getpeerinfo | grep "addr\":" | head'
# should show 178.238.224.23 within ~30s

# on B
su - ratr -c 'ratatoskr-cli getpeerinfo | grep "addr\":" | head'
# should show 74.208.37.172
```

If peering fails:
- Check UFW on both sides (9393 open)
- Check `addnode` entries in config
- Try `addnode=74.208.37.172 onetry` manually

**Do not proceed to mining until seed-to-seed peering is confirmed.**

## T+0..5 min: mine block 1

Two viable paths — pick one:

### Path A: daemon internal miner on VPS1 (simplest)
```bash
# on VPS1 as root
MINER_ADDR=$(su - ratr -c 'ratatoskr-cli getnewaddress "launch-miner-1"')
echo "Miner address: $MINER_ADDR"
su - ratr -c "ratatoskr-cli setgenerate true 2"
# wait ~30-60s (difficulty is near-zero at genesis)
su - ratr -c 'ratatoskr-cli getmininginfo'
# watch for "blocks": to increment to 1
```

### Path B: external miner from your Windows box
```powershell
# Terminal C
cd C:\Users\mikey\Downloads\cpuminer-opt
.\cpuminer.exe -a yespower `
    -o stratum+tcp://POOL_URL:PORT `
    -u YOUR_RATR_ADDRESS `
    -p x -t 4
```
Requires a pool to be live at 00:00 UTC — coordinate timing with the pool op.

### Once block 1 is mined
```bash
# on any daemon
ratatoskr-cli getblockhash 1
ratatoskr-cli getblock $(ratatoskr-cli getblockhash 1) 2 | head -40
# verify: 3 outputs in coinbase (miner, treasury; MN comes later at block 25000)
# treasury output value should be exactly 5 RATR (10% of 50 RATR subsidy)
ratatoskr-cli getblock $(ratatoskr-cli getblockhash 1) 2 | jq '.tx[0].vout[] | {value, scriptPubKey: .scriptPubKey.address // .scriptPubKey.asm}'
```
**If the treasury output is missing or wrong, stop mining immediately.**
The consensus code didn't deploy correctly. Don't allow the chain to build
on a broken foundation.

## T+5..30 min: open the gates

Once block 1 + block 2 have been mined and verified on both seed nodes:

- [ ] Stop the internal miner if you used Path A:
  ```bash
  su - ratr -c 'ratatoskr-cli setgenerate false'
  ```
- [ ] Post in Discord `#ratatoskr`:
  ```
  🚀 Ratatoskr mainnet is LIVE.
  Block 1: <hash>
  Seed nodes: seed1.ratatoskr.enchantedforestdefi.com (74.208.37.172),
              seed2.ratatoskr.enchantedforestdefi.com (178.238.224.23)
  Daemon: https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0
  Mining guide: https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/mining.md
  Pool: <pool URL once the op confirms they're up>
  ```
- [ ] Ping the pool operator: "Block 1 at <hash>, daemon is healthy, feel
      free to flip your stratum live"
- [ ] DM the bridge relayer: "mainnet live, can activate wRATR mint now"

## T+1 hr: first-hour health

- [ ] Blocks being produced roughly every 60s: `ratatoskr-cli getblockcount`
      should be around 60 one hour in
- [ ] Difficulty starting to adjust upward (DigiShield is per-block):
      `ratatoskr-cli getmininginfo | grep difficulty`
- [ ] Seed nodes staying peered: `getpeerinfo | length` ≥ 1 on both
- [ ] No `ERROR` lines in the log:
      `journalctl -u ratatoskrd --since "1 hour ago" | grep -i error`
- [ ] Memory + disk on both VPSes below 80%: `free -h && df -h`

## T+6 hr: mid-day checkpoint

- [ ] External miners connected (check pool dashboard or pool op's report)
- [ ] Hashrate climbing steadily: `getmininginfo | grep networkhashps`
- [ ] No reorg events: `ratatoskr-cli getchaintips | grep invalid` should
      be empty
- [ ] Discord check — field any new user questions

## T+24 hr: day-1 recap

- [ ] Post in Discord a day-1 summary:
  - Blocks mined: expect ~1440
  - Network hashrate
  - Pools listed
  - Any incidents + resolutions
- [ ] Write the day-1 retrospective into `~/.claude/projects/...memory/ratr_launch_day_postmortem.md`

## T+17 days: MN payments activate

Block 25,000 is the masternode-payment activation height. Watch that block
closely: coinbase must now have 3 outputs (miner, MN, treasury). If MN
payment is missing, consensus bug — pause and investigate.

## T+21 days: governance activates

Block 30,000 is the first superblock eligible height. Governance proposals
become possible. At v1.0, superblocks are present but treasury is
per-block (not superblock-gated).

---

## Emergency procedures

### Chain halted / no new blocks for >10 min
1. `ratatoskr-cli getblockchaininfo` — if stuck height, check peers
2. `ratatoskr-cli getpeerinfo | length` — if 0, firewall + addnode
3. If needed, restart: `systemctl restart ratatoskrd`
4. If >1 hr stuck, post Discord pause notice, investigate logs

### Invalid block detected
1. `journalctl -u ratatoskrd -n 200 | grep -i "invalid\|error\|bad-"`
2. Capture the offending block hash + reason
3. Do NOT mine a conflicting block blindly — diagnose first
4. If it's a consensus-rule bug: stop both seeds, do a hotfix build,
   coordinate restart — this is why we tested on testnet

### Treasury output missing/incorrect
1. Stop all mining immediately
2. `getblock <hash> 2` to capture the exact bad output
3. Compare to `chainparams.cpp` treasuryPaymentScript
4. This is a code bug — don't let miners build on it. Hotfix + re-genesis
   is on the table if not caught early

### Bridge relayer drops out
1. Check relayer pm2 status on VPS1: `pm2 status`
2. Check ALPH node sync: `alephium-cli node-info`
3. Relayer can be down briefly without chain impact; mint/burn events
   queue and process when it comes back

---

## Post-launch cleanup (T+7 days)

- [ ] Rotate the temporary RPC creds generated during bring-up to
      production creds if different
- [ ] Raise DNS TTL for seed records from 300 to 3600 now that stable
- [ ] Enable Contabo snapshot schedule on VPS2 if not already
- [ ] Bump the Ratatoskr README with "mainnet live since YYYY-MM-DD"
- [ ] Close/merge any open issues that were "blocks launch"

---

## Known launch-day unknowns

- **Initial difficulty overshoot.** Starting nBits is 0x1e3fffff (near-zero
  difficulty). Once any capable miner shows up, difficulty can spike 100-1000×
  in the first hour. Pool vardiff needs to start *loose* (diff ~1e-6).
- **First miner wins big.** Pre-25k blocks pay 90% to miner. Someone with a
  ready CPU farm can rake 40 RATR × 25,000 blocks = 1M RATR (1% of supply)
  over the first 17 days if no competition. This is tolerated — it rewards
  the day-1 security contribution.
- **wRATR bridge lag.** The bridge operator has to manually activate the
  minting path after chain liveness is confirmed. Expect a 1-6 hour lag
  between "chain live" and "wRATR mintable on Alephium."
