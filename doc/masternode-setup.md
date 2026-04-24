# Ratatoskr Masternode Setup Guide

Short, direct, old-school: send collateral to yourself, register, done.

**Network values:**
- **Regular MN collateral:** `7,500 RATR` (1× voting weight)
- **EvoNode collateral:** `30,000 RATR` (4× voting weight)
- **P2P port:** `9393`
- **RPC port:** `8282` (default; localhost-bound recommended)
- **Collateral confirmations:** `15`
- **MN payments activate:** block **25,000** (~17 days post-launch)

---

## 1. Reality check

- You **can** run the MN and wallet in the same `ratatoskrd` process.
- You **do not need** a VPS if your home machine is reachable from the
  internet (`9393/tcp` open + stable public IP). Most operators use a VPS
  anyway for uptime.
- Ratatoskr uses Dash-style deterministic masternodes (ProTx), so one
  registration transaction is required.

---

## 2. Minimal `ratatoskr.conf`

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

Restart `ratatoskrd` after editing. The `masternodeblsprivkey` line is added
after you generate keys below (Step A).

---

## 3. Fast flow (Regular MN, 7,500 RATR)

```bash
CLI=ratatoskr-cli
WALLET=main

# create the wallet if needed
$CLI createwallet "$WALLET" 2>/dev/null || true
```

### Step A — generate keys + addresses

```bash
BLS_JSON=$($CLI bls generate)
OPERATOR_SECRET=$(echo "$BLS_JSON" | jq -r '.secret')
OPERATOR_PUB=$(echo "$BLS_JSON" | jq -r '.public')

COLLATERAL_ADDR=$($CLI -rpcwallet=$WALLET getnewaddress "mn_collateral")
OWNER_ADDR=$($CLI -rpcwallet=$WALLET getnewaddress "mn_owner")
VOTING_ADDR=$($CLI -rpcwallet=$WALLET getnewaddress "mn_voting")
PAYOUT_ADDR=$($CLI -rpcwallet=$WALLET getnewaddress "mn_payout")
FEE_ADDR=$($CLI -rpcwallet=$WALLET getnewaddress "mn_fee")
```

Put `OPERATOR_SECRET` into `ratatoskr.conf` as `masternodeblsprivkey=...`
and restart `ratatoskrd` once before continuing.

### Step B — send collateral to yourself

```bash
TXID=$($CLI -rpcwallet=$WALLET sendtoaddress "$COLLATERAL_ADDR" 7500)
echo "$TXID"
```

Wait for **15 confirmations** (~15 minutes at 60s blocks).

### Step C — get collateral outpoint

```bash
OUTPOINT=$($CLI -rpcwallet=$WALLET masternode outputs | jq -r 'to_entries[0] | "\(.key)-\(.value)"')
COLL_TXID="${OUTPOINT%-*}"
COLL_VOUT="${OUTPOINT##*-}"
echo "$COLL_TXID $COLL_VOUT"
```

### Step D — register the masternode

```bash
MN_IP="YOUR_PUBLIC_IP"

PROTX_HASH=$($CLI -rpcwallet=$WALLET protx register \
    "$COLL_TXID" "$COLL_VOUT" "[\"$MN_IP:9393\"]" \
    "$OWNER_ADDR" "$OPERATOR_PUB" "$VOTING_ADDR" \
    0 "$PAYOUT_ADDR" "$FEE_ADDR" true)

echo "$PROTX_HASH"
```

### Step E — verify

```bash
$CLI protx info "$PROTX_HASH"
$CLI masternode list status
$CLI masternode status
```

Status should land on `READY` / `ENABLED` within a few blocks. If it takes
longer, check the log (`debug.log`) for errors and confirm port 9393 is
actually reachable from the outside (`nc -zv YOUR_PUBLIC_IP 9393` from a
different network).

---

## 4. One-call alternative

If you want fewer steps, use `protx register_fund` — it creates the
collateral output and registers the MN in a single transaction:

```bash
$CLI -rpcwallet=$WALLET protx register_fund \
    "$COLLATERAL_ADDR" "[\"$MN_IP:9393\"]" \
    "$OWNER_ADDR" "$OPERATOR_PUB" "$VOTING_ADDR" \
    0 "$PAYOUT_ADDR" "$FEE_ADDR" true
```

---

## 5. EvoNode (30,000 RATR, 4× voting)

Same flow but use:
- `protx register_evo` (replaces `protx register`)
- `protx register_fund_evo` (replaces `protx register_fund`)
- Send **30,000 RATR** collateral (not 7,500)

EvoNodes have 4× voting weight in governance but also host additional
services (InstantSend signing, ChainLocks, LLMQ quorums). Only run one if
you can commit to high uptime.

---

## 6. Payments timing

- **Before block 25,000** (~17 days post-launch 2026-06-01): MN payments
  are **not yet active**. Miners get 90% of subsidy, treasury gets 10%.
  Your MN won't earn during this phase — it's intended. Register anyway
  to claim a spot early.
- **From block 25,000 onwards**: 30% of subsidy goes to MNs, selected
  deterministically from the registered MN set.
- Average pay frequency depends on total registered MNs. With 20 MNs
  registered, each MN pays approximately every 20 × 60 seconds = 20 min
  on average.

---

## 7. Top 5 mistakes

1. Collateral amount is not exactly `7,500` (Regular) or `30,000` (Evo).
2. Fewer than 15 confirmations before registering.
3. `masternodeblsprivkey` in config doesn't match the operator public key
   used in the ProTx.
4. Port `9393/tcp` closed on firewall or cloud provider security group.
5. Node not reachable at `externalip` (test from a different network).

---

## 8. Backing up

- **Wallet:** `backupwallet /path/to/wallet-backup.dat` via RPC/CLI, or
  File → Backup wallet in the Qt GUI. Store offline.
- **BLS operator key:** the `OPERATOR_SECRET` from Step A. Back it up
  alongside the wallet — if you lose it, you can't operate the MN even
  if you still control the collateral.
- **Collateral privkey:** `dumpprivkey "$COLLATERAL_ADDR"` — guards the
  7,500 or 30,000 RATR. Store offline (paper or metal).

---

## 9. Operating costs

At VPS pricing (~$1.75/month for minimal MN hosting via Nodes24 or similar):
- Regular MN opex: ~$21/year
- Break-even at any RATR/USD > $0 given the 30% MN subsidy share post-block-25k
- EvoNode: same opex, 4× voting weight, same payment frequency but only
  at EvoNode tier — diminishing returns unless governance voting is
  important to you

---

## Links

- Pool operator spec: [`doc/pool-operator-spec.md`](pool-operator-spec.md)
- Mining guide: [`doc/mining.md`](mining.md)
- Whitepaper: [`doc/whitepaper.md`](whitepaper.md)
- Discord: EnchantedForestDeFi server, `#ratatoskr`
- Issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
