# RATR Masternode Setup — Qt-Side Walkthrough

*The official RATR masternode setup guide — the Qt-side steps every operator does, whether you self-host or use a managed host like Nodes24. Battle-tested on real setups. Questions? `#ratr-masternode` in the EnchantedForestDeFi Discord.*

**Network values:** Regular collateral `7,500 RATR` (1× vote) · EvoNode `30,000 RATR` (4× vote, live) · P2P port `9393` · 15 collateral confirmations · MN payments **live** (activated at block 25,000).

---

## SECTION 1 — Qt Wallet Walkthrough (everything you do before handing off the collateral packet)

This is the **owner side** — everything you do in the RATR Qt wallet up to the point you hand off the collateral packet. A managed host (e.g. Nodes24) then takes it from there (BLS operator key, daemon config, ProRegTx draft); if you self-host, see **Section 5** for the daemon side.

### Step 1 — Download & install the RATR Qt wallet
- Official build from the Ratatoskr GitHub releases.
- **Windows:** installer / portable `.exe`.
- **macOS:** the `.dmg` — ⚠️ **separate Intel and Apple Silicon (M-series) builds exist; grab the right one** or it won't launch.
- **Linux:** the tarball — extract and run the Qt binary (or build from source).
- ⚠️ **Verify your download** — match the SHA256 against `SHA256SUMS` and the GPG signature against `SHA256SUMS.asc` (signing key `50B8E0E4EADA2FF29B1DE68F7A33CFF563BB2E0A`, `releases@enchantedforestdefi.com`).
- Let it **fully sync** to the chain tip before doing anything (block height should match the explorer).

### Step 2 — Encrypt the wallet
- Settings → Encrypt Wallet → strong passphrase.
- ⚠️ **The passphrase cannot be recovered.** Write it offline. Lost passphrase = lost collateral.

### Step 3 — Back up wallet.dat
- File → Backup Wallet → save `wallet.dat` somewhere safe (offline / second location).
- Back up **again after Step 5** (the new keys only live in the wallet once generated).
- The MN collateral wallet is the most important one you'll ever own.

### Step 4 — Enable Coin Control
- Settings → Options → Wallet tab → check **"Enable coin control features"** → OK (restart if prompted).
- Unlocks the "Inputs…" button on Send + right-click UTXO locking — both needed later.

### Step 5 — Generate the 4 masternode addresses
Tools → **Debug Console**, run (replace `NAME` with your handle):
```
getnewaddress "NAME-mn1-owner"
getnewaddress "NAME-mn1-voting"
getnewaddress "NAME-mn1-payout"
getnewaddress "NAME-mn1-collateral"
```
**The 4-role split (Dash-family architecture):**
- **Owner** — controls the MN; signs any future registration update. *The* key — back it up.
- **Voting** — casts governance/treasury votes; can be delegated separately from owner.
- **Payout** — where the MN's block-reward share lands (live since block 25,000).
- **Collateral** — holds the 7,500 RATR stake (funded next).

Labels are local-only. ✅ Check: 4 distinct addresses starting with **`R`**. A **`y`** prefix means a **testnet** wallet — wrong build.

### Step 6 — Fund the collateral: EXACTLY 7,500 RATR in a SINGLE UTXO
🚨 **The #1 rule, and the #1 thing newcomers get wrong:** exactly **7,500 RATR in ONE single UTXO.** Not 7,499.99, not 7,500.01, and **NOT 7,500 split across two transactions — even if both land at the same address.** The chain rejects registration without one UTXO of exactly 7,500.

⚠️ **No "test send" first.** Sending a little to "check the address" then topping up makes TWO UTXOs → registration fails. **One transaction, one UTXO. No test-sends, no top-ups.**

How (Qt Send tab): paste collateral address, enter `7500`, send. ⚠️ **Do NOT tick "subtract fee from amount"** — fee comes from change, or the destination lands at 7,499.99x and fails.

Then wait ~1 block to confirm, then **15+ confirmations** (~15 min) before registration.

### Step 7 — Capture txid + vout (verify, don't assume)
After 15+ confirmations, in Debug Console:
```
masternode outputs
```
- Returns a `txid-vout` line → a **valid, exactly-7,500** collateral. Green light. (Lists *only* valid MN-collateral UTXOs — the chain-canonical check.)
- Returns nothing → amount wrong, multi-UTXO, or wrong address (see Q&A → consolidation).

⚠️ **The vout can be `0` OR `1`** depending on whether the tx had change. **Verify it — don't assume 0.** (Qt's "Output index" field on Send refers to the *change* output, not the collateral — trust `masternode outputs`.)

### Step 8 — Lock the collateral UTXO
- Send tab → "Inputs…" (Coin Control) → right-click the 7,500 UTXO → **Lock Unspent.**
- ⚠️ **If the collateral UTXO ever moves, the MN auto-deregisters** on the next block.

### Step 9 — Hand off the collateral packet
Provide (to your managed host, or to yourself for the self-host registration in Section 5):
```
Owner address:      [step 5]
Voting address:     [step 5]
Payout address:     [step 5]
Collateral address: [step 5]
Collateral txid:    [step 7, verified]
Collateral vout:    [step 7, verified — 0 or 1]
```
**Custody note (managed hosting):** A managed host generates the BLS operator key on its own infrastructure and runs the daemon — but **cannot spend the collateral or change the registration.** The owner key signs from *your* Qt; it never leaves your wallet. You don't trust the host with your money, only with uptime.

---

## SECTION 2 — Pre-Handoff Checklist
- [ ] RATR Qt wallet **latest release**, fully synced
- [ ] Wallet **encrypted** + `wallet.dat` **backed up**
- [ ] **Coin Control enabled**
- [ ] **4 addresses** (owner / voting / payout / collateral) — all start with `R`
- [ ] Collateral funded: **exactly 7,500 RATR, single UTXO** (verified via `masternode outputs`)
- [ ] Collateral UTXO **locked**
- [ ] **txid + vout** captured and verified (vout 0 or 1 — not assumed)

---

## SECTION 3 — Common Q&A

**Q: When do MN payments start?** They're **already live** — payments activated at block **25,000** (passed in June 2026) and are flowing now. MNs go ENABLED earlier (from ~block 7,500 they join ChainLocks/DKG quorums); the reward share began at 25,000, after the bootstrap gap that let the MN count build up first.

**Q: My addresses start with `y`, not `R`.** That's a **testnet** wallet. Download the mainnet RATR Qt build and regenerate.

**Q: I already sent the collateral in two pieces.** Consolidate into one fresh UTXO: coin control → select all the pieces + a tiny fee input → generate a fresh `…-collateral-v2` address → send exactly 7,500 (no "subtract fee") → wait 15+ confs → `masternode outputs` shows it → lock it.

**Q: `masternode outputs` shows nothing.** Not enough confirmations, amount isn't exactly 7,500, multi-UTXO, or wrong address.

**Q: I'm coming from a web/exchange wallet.** Just **send** native RATR to a fresh Qt receive address (don't "import"). Then proceed from Step 5.

**Q: My MN shows POSE_BANNED.** PoSe ban = node stopped answering network probes for 1+ hour (usually host downtime). Recoverable with `protx update_service`. On managed hosting, the host fixes it.

**Q: Can I move my collateral after the MN is live?** No — moving the 7,500 UTXO auto-deregisters the MN. Leave it locked.

---

## SECTION 4 — Cross-Platform Gotchas Catalog
- **Exactly 7,500, single UTXO** — the universal #1 failure. No test-sends, no top-ups, no "subtract fee from amount."
- **vout is 0 OR 1** — verify with `masternode outputs`; don't trust the Send-tab "Output index."
- **`R` not `y`** — `y` = testnet wallet.
- **Port must be `9393`** — chainparams hard-enforce the default port; the daemon's `externalip` must end in `:9393` or registration is rejected.
- **Windows has no `-daemon` flag** — run the daemon in a console/service, or use Qt. *(Linux/macOS support `-daemon`.)*
- **Linux:** run the daemon under a **systemd service** for auto-restart + boot persistence.
- **macOS:** **separate Intel vs Apple Silicon binaries** — wrong arch won't launch.
- **IPv4 only** — IPv6 MN externalip isn't supported yet; use a public IPv4.
- **Lock the collateral UTXO** — unlocked collateral can get spent → instant deregister.
- **15 confirmations** before registration (PRE_ENABLED → ENABLED ≈ 15 min).

---

## SECTION 5 — Self-host: daemon + registration (CLI)

For operators running their own VPS/daemon instead of a managed host.

### Minimal `ratatoskr.conf`
```ini
server=1
daemon=1
listen=1
port=9393
externalip=YOUR_PUBLIC_IP:9393
txindex=1
prune=0
masternodeblsprivkey=PASTE_OPERATOR_SECRET_HERE
disablewallet=0
rpcuser=CHOOSE_A_USERNAME
rpcpassword=CHOOSE_A_STRONG_PASSWORD
rpcallowip=127.0.0.1
```
The `masternodeblsprivkey` line is added after you generate keys below; restart `ratatoskrd` after editing.

### Generate the operator BLS key
```bash
CLI=ratatoskr-cli
$CLI bls generate    # -> {"secret": "...", "public": "..."}
```
Put `secret` into `ratatoskr.conf` as `masternodeblsprivkey=...`, restart once. Keep `public` for the ProTx.

### Registration

**Normal path — collateral already funded in Section 1.** Register against that existing collateral using its verified `txid` + `vout` (from Section 1, Step 7):
```bash
$CLI -rpcwallet=main protx register \
    "$COLLATERAL_TXID" $COLLATERAL_VOUT "[\"$MN_IP:9393\"]" \
    "$OWNER_ADDR" "$OPERATOR_PUB" "$VOTING_ADDR" \
    0 "$PAYOUT_ADDR" "$FEE_ADDR"
```
⚠️ **Do NOT use `register_fund` if you already funded collateral in Section 1** — it *creates and funds a second 7,500 UTXO.*

**Alternative — fund + register in one call (only if you skipped Section 1).** `register_fund` takes the collateral *address* and creates the 7,500 UTXO for you:
```bash
$CLI -rpcwallet=main protx register_fund \
    "$COLLATERAL_ADDR" "[\"$MN_IP:9393\"]" \
    "$OWNER_ADDR" "$OPERATOR_PUB" "$VOTING_ADDR" \
    0 "$PAYOUT_ADDR" "$FEE_ADDR" true
```

**External / managed-host signing.** If the collateral key lives in a separate wallet (e.g. your Qt while a host runs the daemon): `protx register_prepare` returns an unsigned tx + a message → sign that message with the collateral key (`signmessage`) → `protx register_submit "tx" "sig"` broadcasts it. The collateral key never leaves your wallet.

### Verify
```bash
$CLI protx info "$PROTX_HASH"
$CLI masternode status
```
Status should reach `READY` / `ENABLED` within a few blocks. If not, check `debug.log` and confirm `9393/tcp` is reachable from another network (`nc -zv YOUR_PUBLIC_IP 9393`).

### EvoNode (30,000 RATR)
Same flow with `protx register_evo` (existing collateral) / `protx register_fund_evo` (creates the collateral) and **30,000 RATR**. EvoNodes carry **4× voting weight and are live now** — register exactly like a Regular MN, just with the 30,000 collateral and the evo variant of the command.

### Top mistakes
1. Collateral not exactly `7,500` (Regular) / `30,000` (Evo). 2. Fewer than 15 confirmations before registering. 3. `masternodeblsprivkey` doesn't match the operator public key in the ProTx. 4. Port `9393/tcp` closed on firewall / cloud security group. 5. Node unreachable at `externalip` (test from a different network).

### Back up
- **Wallet:** `backupwallet /path/to/backup.dat` (or File → Backup Wallet in Qt). Offline.
- **BLS operator key:** the `secret` from `bls generate` — lose it and you can't operate the MN even with the collateral.
- **Collateral privkey:** `dumpprivkey "$COLLATERAL_ADDR"` — guards the stake. Offline (paper/metal).

---

## Links
- Mining guide: [`doc/mining.md`](mining.md)
- Whitepaper: [`doc/whitepaper.md`](whitepaper.md)
- Discord: EnchantedForestDeFi server, `#ratr-masternode`
- Issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
