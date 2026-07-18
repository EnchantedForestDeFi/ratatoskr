#!/usr/bin/env bash
# mn-self-revive.sh v3 (2026-07-16) — self-heal a PoSe-banned Ratatoskr masternode + node watchdog.
# BLS stays on the box; the tiny "revive" wallet pays the update_service fee. Conf: /etc/mn-self-revive.conf (v1/v2-compatible).
#
# v3 change (Evo auto-detect — fixes the "is not a Regular" footgun a community Evo hit 2026-07-16):
#  - MN_TYPE now defaults to `auto`: the script reads the MN's type from `protx info` at revive time
#    and fires update_service (Regular) or update_service_evo (Evo) accordingly — no manual MN_TYPE needed.
#  - Platform fields (nodeID/p2p/http) auto-pulled from protx info when they aren't set in the conf.
#  - Self-correcting fallback: if the Regular revive returns "is not a Regular", it retries as Evo
#    automatically. So a mis-set Evo heals itself even if protx info type-detection was skipped.
#  - MN_TYPE=regular|evo in the conf still forces that path (backward compatible).
#
# v2 hardening (kept): ban detection PRIMARY = `masternode status` (needs no txindex; can't trigger the
# protx-info segfault). protx info is optional/timeout-guarded and its failure is tolerated. Wedge watch,
# crash-loop watch, stale-txindex tell, daemon/RPC-down alerts, per-condition alert cooldown.
. /etc/mn-self-revive.conf 2>/dev/null
: "${CLI:?}"; : "${CONF:?}"; : "${WALLET:=revive}"
: "${MIN_INTERVAL:=1800}"; : "${MAX_PER_DAY:=8}"; : "${STATE:=/var/lib/mn-self-revive}"; : "${WEBHOOK:=}"; : "${PROTX_CONF:=}"
: "${ALERT_COOLDOWN:=21600}"; : "${STAGNANT_SECS:=1800}"; : "${SERVICE_NAME:=ratatoskrd}"
: "${DEBUG_LOG:=}"; : "${MN_TYPE:=auto}"; : "${LOW_FEE_WARN:=1}"
: "${PLATFORM_NODEID:=}"; : "${PLATFORM_P2P_PORT:=}"; : "${PLATFORM_HTTP_PORT:=}"   # evo-only; auto-pulled if empty
mkdir -p "$STATE"
NOWS="$(date -u +%s)"
log(){ printf '%s %s\n' "$(date -u +%FT%TZ)" "$*" >>"$STATE/revive.log"; }
notify(){ [ -n "$WEBHOOK" ] && curl -sf -m 8 -H 'Content-Type: application/json' -d "{\"content\":\"[RATR MN] $1\"}" "$WEBHOOK" >/dev/null 2>&1; return 0; }
alert(){ # alert <key> <msg> — always logged; webhook at most once per ALERT_COOLDOWN per key
  local K="$STATE/alert_$1" LASTA; LASTA="$(cat "$STATE/alert_$1" 2>/dev/null || echo 0)"
  log "ALERT($1): $2"
  [ $((NOWS-LASTA)) -lt "$ALERT_COOLDOWN" ] && return 0
  printf '%s' "$NOWS" >"$K"; notify "$2"
}
clear_alert(){ rm -f "$STATE/alert_$1" 2>/dev/null; }
field(){ printf '%s' "$1" | sed -n "s/.*\"$2\": *\"\{0,1\}\([-0-9a-zA-Z._:]*\)\"\{0,1\}.*/\1/p" | head -1; }
TMO=""; command -v timeout >/dev/null 2>&1 && TMO="timeout 20"

# ---- 1) daemon process alive? -------------------------------------------------
if ! pgrep -x ratatoskrd >/dev/null 2>&1; then
  alert daemon_down "daemon NOT RUNNING on $(hostname) — MN offline. Check: systemctl status $SERVICE_NAME"
  exit 0
fi
clear_alert daemon_down

# ---- 2) crash-loop watch (systemd restart counter) -----------------------------
NR="$(systemctl show "$SERVICE_NAME" -p NRestarts --value 2>/dev/null)"
if printf '%s' "$NR" | grep -qE '^[0-9]+$'; then
  LASTNR="$(cat "$STATE/nrestarts" 2>/dev/null || echo "$NR")"
  printf '%s' "$NR" >"$STATE/nrestarts"
  if [ "$NR" -gt "$LASTNR" ]; then
    alert crash_loop "daemon restarted $((NR-LASTNR))x since last check (NRestarts=$NR) on $(hostname) — check: journalctl -u $SERVICE_NAME | grep -iE 'signal|segmentation'"
  fi
fi

# ---- 3) stale/corrupt txindex tell (segfault precursor) -------------------------
if [ -n "$DEBUG_LOG" ] && tail -300 "$DEBUG_LOG" 2>/dev/null | grep -q 'FindTx: txid mismatch'; then
  alert stale_index "txindex STALE/CORRUPT on $(hostname) (FindTx mismatch in debug.log; protx RPCs can crash the daemon). Fix: stop daemon, mv indexes indexes.old, start — it rebuilds itself."
fi

# ---- 4) RPC answering? ----------------------------------------------------------
H="$($TMO $CLI getblockcount 2>/dev/null | tr -d '[:space:]')"
if ! printf '%s' "$H" | grep -qE '^[0-9]+$'; then
  alert rpc_down "daemon up but RPC not answering on $(hostname) (getblockcount failed) — possible crash/restart in progress"
  exit 0
fi
clear_alert rpc_down

# ---- 5) wedge / stagnation watch (the false-synced tell) ------------------------
LASTH="$(cat "$STATE/last_height" 2>/dev/null || echo 0)"
LASTHT="$(cat "$STATE/last_height_ts" 2>/dev/null || echo "$NOWS")"
if [ "$H" != "$LASTH" ]; then
  printf '%s' "$H"    >"$STATE/last_height"
  printf '%s' "$NOWS" >"$STATE/last_height_ts"
  clear_alert wedged
elif [ $((NOWS-LASTHT)) -ge "$STAGNANT_SECS" ]; then
  CLH="$($TMO $CLI getbestchainlock 2>/dev/null | sed -n 's/.*"height": *\([0-9]*\).*/\1/p' | head -1)"
  alert wedged "possible CHAIN WEDGE on $(hostname): height STUCK at $H for $(( (NOWS-LASTHT)/60 ))min (chainlock=${CLH:-?}). Node may be false-synced — bootstrap chaindata (blocks/chainstate/evodb/indexes) per runbook."
fi

# ---- 6) blockchain synced enough to act? ---------------------------------------
if ! $TMO $CLI mnsync status 2>/dev/null | grep -q '"IsBlockchainSynced": true'; then
  log "not blockchain-synced yet - skip revive attempt"
  exit 0
fi

# ---- 7) ban detection — PRIMARY: masternode status (txindex-free, crash-safe) ---
MNS="$($TMO $CLI masternode status 2>&1)"
STATEV="$(field "$MNS" state)"
BANNED=0; PI=""
case "$STATEV" in
  POSE_BANNED) BANNED=1 ;;
  *) # wedged nodes can lie READY — cross-check protx info, GUARDED + failure tolerated
     if [ -n "$PROTX_CONF" ]; then
       PI="$($TMO $CLI protx info "$PROTX_CONF" 2>/dev/null)"
       if [ -n "$PI" ]; then
         BH="$(field "$PI" PoSeBanHeight)"
         [ -n "$BH" ] && [ "$BH" != "-1" ] && BANNED=1
       else
         log "protx info unavailable (tolerated) - trusting masternode status=$STATEV"
       fi
     fi ;;
esac
[ "$BANNED" -eq 1 ] || exit 0

# ---- 8) resolve identity -------------------------------------------------------
PROTX="$PROTX_CONF"
[ -z "$PROTX" ] && PROTX="$(printf '%s' "$MNS" | sed -n 's/.*"proTxHash"[^"]*"\([0-9a-f]\{64\}\)".*/\1/p' | head -1)"
if [ -z "$PROTX" ]; then
  alert no_protx "MN on $(hostname) is POSE_BANNED but proTxHash is unknown (banned status strips it) — set PROTX_CONF in /etc/mn-self-revive.conf"
  exit 0
fi
SVC="$(field "$MNS" service)"
if [ -z "$SVC" ] && [ -n "$PI" ]; then SVC="$(field "$PI" service)"; fi
[ -z "$SVC" ] && { alert no_svc "cannot determine service ip:port for revive on $(hostname) — check masternode status"; exit 0; }

# ---- 9) guards: peers / rate-limit / day-cap ------------------------------------
PEERS="$($TMO $CLI getconnectioncount 2>/dev/null || echo 0)"
[ "${PEERS:-0}" -lt 1 ] && { alert no_peers "MN $SVC banned but 0 peers - NOT reviving, check networking. ${PROTX:0:12}"; exit 0; }
LAST="$(cat "$STATE/last_fire" 2>/dev/null || echo 0)"
[ $((NOWS-LAST)) -lt "$MIN_INTERVAL" ] && { log "rate-limited - skip"; exit 0; }
DAYF="$STATE/count_$(date -u +%F)"; CNT="$(cat "$DAYF" 2>/dev/null || echo 0)"
[ "$CNT" -ge "$MAX_PER_DAY" ] && { alert day_cap "MN $SVC hit $MAX_PER_DAY revives today - STOPPING, needs a human. ${PROTX:0:12}"; exit 0; }

# ---- 10) keys + fee wallet ------------------------------------------------------
BLS="$(grep -m1 '^[[:space:]]*masternodeblsprivkey' "$CONF" | sed 's/.*=[[:space:]]*//' | tr -d ' \t\r\n')"
[ "${#BLS}" -lt 32 ] && { alert no_bls "MN $SVC: BLS key not found in $CONF."; exit 1; }
$TMO $CLI listwallets 2>/dev/null | grep -q "\"$WALLET\"" || $TMO $CLI loadwallet "$WALLET" >/dev/null 2>&1
FEEADDR="$($TMO $CLI -rpcwallet="$WALLET" listunspent 1 9999999 2>/dev/null | sed -n 's/.*"address" *: *"\([^"]*\)".*/\1/p' | head -1)"
[ -z "$FEEADDR" ] && { alert no_fee "MN $SVC: revive wallet '$WALLET' has no confirmed funds - top it up. ${PROTX:0:12}"; exit 1; }
BAL="$($TMO $CLI -rpcwallet="$WALLET" getbalance 2>/dev/null || echo 0)"
awk -v b="$BAL" -v w="$LOW_FEE_WARN" 'BEGIN{exit !(b<w)}' && alert low_fee "revive wallet on $(hostname) is low ($BAL RATR) - top up soon so self-heal keeps working"

# ---- 10b) resolve MN type (auto unless MN_TYPE forces it) -----------------------
# Reuse the protx info from step 7 if we have it; else fetch once (timeout-guarded).
[ -z "$PI" ] && PI="$($TMO $CLI protx info "$PROTX" 2>/dev/null)"
TYPE="$MN_TYPE"
if [ -z "$TYPE" ] || [ "$TYPE" = "auto" ]; then
  if printf '%s' "$PI" | grep -q '"type": *"Evo"'; then TYPE=evo; else TYPE=regular; fi
fi
# Auto-pull Evo platform fields from protx info when the conf didn't set them.
if [ "$TYPE" = "evo" ]; then
  [ -z "$PLATFORM_NODEID" ]    && PLATFORM_NODEID="$(field "$PI" platformNodeID)"
  [ -z "$PLATFORM_P2P_PORT" ]  && PLATFORM_P2P_PORT="$(field "$PI" platformP2PPort)"
  [ -z "$PLATFORM_HTTP_PORT" ] && PLATFORM_HTTP_PORT="$(field "$PI" platformHTTPPort)"
fi

fire_regular(){ $TMO $CLI -rpcwallet="$WALLET" protx update_service "$PROTX" "$SVC" "$BLS" "" "$FEEADDR" 2>&1 | head -c 200; }
fire_evo(){ $TMO $CLI -rpcwallet="$WALLET" protx update_service_evo "$PROTX" "$SVC" "$BLS" "$PLATFORM_NODEID" "$PLATFORM_P2P_PORT" "$PLATFORM_HTTP_PORT" "" "$FEEADDR" 2>&1 | head -c 200; }

# ---- 11) fire the revive (self-correcting if type was guessed wrong) -------------
if [ "$TYPE" = "evo" ]; then
  if [ -z "$PLATFORM_NODEID" ] || [ -z "$PLATFORM_P2P_PORT" ] || [ -z "$PLATFORM_HTTP_PORT" ]; then
    alert evo_manual "Evo MN $SVC banned but platform fields unknown (protx info unavailable) — set PLATFORM_NODEID/PLATFORM_P2P_PORT/PLATFORM_HTTP_PORT in conf, or revive manually. ${PROTX:0:12}"
    exit 0
  fi
  RES="$(fire_evo)"
else
  RES="$(fire_regular)"
  # Self-correct: the daemon tells us this is actually an Evo. Retry as Evo.
  if printf '%s' "$RES" | grep -qi 'is not a Regular'; then
    log "auto-detect: daemon reports Evo for $SVC — retrying as update_service_evo"
    [ -z "$PLATFORM_NODEID" ]    && PLATFORM_NODEID="$(field "$PI" platformNodeID)"
    [ -z "$PLATFORM_P2P_PORT" ]  && PLATFORM_P2P_PORT="$(field "$PI" platformP2PPort)"
    [ -z "$PLATFORM_HTTP_PORT" ] && PLATFORM_HTTP_PORT="$(field "$PI" platformHTTPPort)"
    if [ -n "$PLATFORM_NODEID" ] && [ -n "$PLATFORM_P2P_PORT" ] && [ -n "$PLATFORM_HTTP_PORT" ]; then
      RES="$(fire_evo)"
    else
      alert evo_manual "MN $SVC is Evo but platform fields unknown — set PLATFORM_NODEID/PLATFORM_P2P_PORT/PLATFORM_HTTP_PORT in conf. ${PROTX:0:12}"
    fi
  fi
fi
printf '%s' "$NOWS" >"$STATE/last_fire"; printf '%s' "$((CNT+1))" >"$DAYF"
if printf '%s' "$RES" | grep -qE '^[0-9a-f]{64}$'; then
  clear_alert revive_fail
  log "OK revived MN $SVC ($TYPE) tx $RES"
  notify "OK revived MN $SVC. tx ${RES:0:16} ${PROTX:0:12}"
else
  alert revive_fail "FAIL revive MN $SVC: $RES"
fi
