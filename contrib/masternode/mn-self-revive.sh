#!/usr/bin/env bash
#
# mn-self-revive.sh - Ratatoskr masternode PoSe-ban auto-revive
#
# Keeps YOUR masternode automatically ENABLED if it ever gets PoSe-banned.
# It signs the revive (ProUpServTx) with this box's OWN BLS operator key,
# which NEVER leaves the server.
#
# Works for BOTH Regular and Evo masternodes: the node type is detected
# automatically from protx info, so Evo nodes are revived with
# update_service_evo (Regular nodes use update_service). No extra config
# needed - set MN_TYPE only if you want to force one path.
#
# Your masternode COLLATERAL stays wherever you keep it. This box only needs
# the operator BLS key (already in your masternode config) plus a few RATR for
# the tiny transaction fees.
#
# SETUP (one time) - see README.md in this directory:
#   1. Put your settings in /etc/mn-self-revive.conf
#   2. Fund the "revive" wallet on this box with a few RATR
#   3. chmod +x /usr/local/bin/mn-self-revive.sh
#   4. Run every 10 min from cron:
#        */10 * * * * /usr/local/bin/mn-self-revive.sh >/dev/null 2>&1
#
# Logs to /var/lib/mn-self-revive/revive.log
#
set -u
. /etc/mn-self-revive.conf 2>/dev/null
: "${CLI:?set CLI in /etc/mn-self-revive.conf (e.g. ratatoskr-cli, plus any -datadir/-conf flags)}"
: "${CONF:?set CONF in /etc/mn-self-revive.conf (the file containing your masternodeblsprivkey)}"
: "${PROTX_CONF:?set PROTX_CONF in /etc/mn-self-revive.conf to your masternode proTxHash}"
: "${WALLET:=revive}"
: "${MIN_INTERVAL:=1800}"     # don't re-fire within this many seconds (30 min)
: "${MAX_PER_DAY:=8}"         # stop + alert past this many revives/day - a box that keeps
                              # re-banning has a real problem the script should not hide
: "${STATE:=/var/lib/mn-self-revive}"
: "${WEBHOOK:=}"              # optional Discord webhook URL for alerts
: "${MN_TYPE:=auto}"          # auto | regular | evo  (auto = detect from protx info)
: "${PLATFORM_NODEID:=}"      # Evo only; auto-filled from protx info when left blank
: "${PLATFORM_P2P_PORT:=}"    # Evo only
: "${PLATFORM_HTTP_PORT:=}"   # Evo only

mkdir -p "$STATE"
log()   { printf '%s %s\n' "$(date -u +%FT%TZ)" "$*" >>"$STATE/revive.log"; }
alert() { log "ALERT: $1"; [ -n "$WEBHOOK" ] && curl -sf -m 8 -H 'Content-Type: application/json' \
            -d "{\"content\":\"[RATR MN] $1\"}" "$WEBHOOK" >/dev/null 2>&1; return 0; }
field() { printf '%s' "$1" | sed -n "s/.*\"$2\": *\"\{0,1\}\([-0-9a-fA-F.:]*\)\"\{0,1\}.*/\1/p" | head -1; }

# Use the saved proTxHash: a banned daemon stops reporting it, so it cannot be auto-detected.
INFO="$($CLI protx info "$PROTX_CONF" 2>/dev/null)"
BAN="$(field "$INFO" PoSeBanHeight)"; BAN="${BAN:--1}"
[ "$BAN" = "-1" ] && exit 0                       # not banned - nothing to do

SVC="$(field "$INFO" service)"
PEERS="$($CLI getconnectioncount 2>/dev/null || echo 0)"
[ "${PEERS:-0}" -lt 1 ] && { alert "MN $SVC is banned but has 0 peers - check the node, not reviving."; exit 0; }

NOW="$(date -u +%s)"; LAST="$(cat "$STATE/last_fire" 2>/dev/null || echo 0)"
[ $((NOW-LAST)) -lt "$MIN_INTERVAL" ] && { log "rate-limited - skip"; exit 0; }

DAYF="$STATE/count_$(date -u +%F)"; CNT="$(cat "$DAYF" 2>/dev/null || echo 0)"
[ "$CNT" -ge "$MAX_PER_DAY" ] && { alert "MN $SVC hit $MAX_PER_DAY revives today - STOPPING. Check your node/firewall."; exit 0; }

BLS="$(grep -m1 '^[[:space:]]*masternodeblsprivkey' "$CONF" | sed 's/.*=[[:space:]]*//' | tr -d ' \t\r\n')"
[ "${#BLS}" -lt 32 ] && { alert "MN $SVC: masternodeblsprivkey not found in $CONF."; exit 1; }

$CLI listwallets 2>/dev/null | grep -q "\"$WALLET\"" || $CLI loadwallet "$WALLET" >/dev/null 2>&1
FEE="$($CLI -rpcwallet="$WALLET" listunspent 1 9999999 2>/dev/null | sed -n 's/.*"address" *: *"\([^"]*\)".*/\1/p' | head -1)"
[ -z "$FEE" ] && { alert "MN $SVC: revive wallet '$WALLET' has no confirmed funds - send it a few RATR."; exit 1; }

# Decide Regular vs Evo. MN_TYPE forces it; otherwise read the type from protx info.
# Evo nodes are paid 4x and MUST be revived with update_service_evo (extra platform args).
TYPE="$MN_TYPE"
if [ -z "$TYPE" ] || [ "$TYPE" = "auto" ]; then
  if printf '%s' "$INFO" | grep -q '"type": *"Evo"'; then TYPE=evo; else TYPE=regular; fi
fi
if [ "$TYPE" = "evo" ]; then
  [ -z "$PLATFORM_NODEID" ]    && PLATFORM_NODEID="$(field "$INFO" platformNodeID)"
  [ -z "$PLATFORM_P2P_PORT" ]  && PLATFORM_P2P_PORT="$(field "$INFO" platformP2PPort)"
  [ -z "$PLATFORM_HTTP_PORT" ] && PLATFORM_HTTP_PORT="$(field "$INFO" platformHTTPPort)"
fi

fire_regular() { $CLI -rpcwallet="$WALLET" protx update_service     "$PROTX_CONF" "$SVC" "$BLS" "" "$FEE" 2>&1 | head -c 200; }
fire_evo()     { $CLI -rpcwallet="$WALLET" protx update_service_evo "$PROTX_CONF" "$SVC" "$BLS" "$PLATFORM_NODEID" "$PLATFORM_P2P_PORT" "$PLATFORM_HTTP_PORT" "" "$FEE" 2>&1 | head -c 200; }

if [ "$TYPE" = "evo" ]; then
  if [ -z "$PLATFORM_NODEID" ] || [ -z "$PLATFORM_P2P_PORT" ] || [ -z "$PLATFORM_HTTP_PORT" ]; then
    alert "MN $SVC is an Evo node but platform info is unavailable - set PLATFORM_NODEID/PLATFORM_P2P_PORT/PLATFORM_HTTP_PORT in /etc/mn-self-revive.conf"
    exit 0
  fi
  RES="$(fire_evo)"
else
  RES="$(fire_regular)"
  # Safety net: if the daemon says this is an Evo node, retry with the Evo command.
  if printf '%s' "$RES" | grep -qi 'is not a Regular'; then
    log "detected Evo node - retrying with update_service_evo"
    [ -z "$PLATFORM_NODEID" ]    && PLATFORM_NODEID="$(field "$INFO" platformNodeID)"
    [ -z "$PLATFORM_P2P_PORT" ]  && PLATFORM_P2P_PORT="$(field "$INFO" platformP2PPort)"
    [ -z "$PLATFORM_HTTP_PORT" ] && PLATFORM_HTTP_PORT="$(field "$INFO" platformHTTPPort)"
    if [ -n "$PLATFORM_NODEID" ] && [ -n "$PLATFORM_P2P_PORT" ] && [ -n "$PLATFORM_HTTP_PORT" ]; then
      RES="$(fire_evo)"
    else
      alert "MN $SVC is an Evo node - set PLATFORM_NODEID/PLATFORM_P2P_PORT/PLATFORM_HTTP_PORT in /etc/mn-self-revive.conf"
    fi
  fi
fi

printf '%s' "$NOW" >"$STATE/last_fire"; printf '%s' "$((CNT+1))" >"$DAYF"
if printf '%s' "$RES" | grep -qE '^[0-9a-f]{64}$'; then
  alert "OK revived MN $SVC - PoSe ban cleared. tx ${RES:0:16}"
else
  alert "FAILED to revive MN $SVC: $RES"
fi
