# Ratatoskr Masternode Auto-Revive

`mn-self-revive.sh` automatically returns your masternode to **ENABLED** if it ever
gets PoSe-banned. It signs the revive transaction with this box's own BLS operator key,
which **never leaves the server**. Your masternode **collateral** stays wherever you
keep it — this box only needs the operator key (already in your masternode config) plus
a few RATR for transaction fees.

## Setup (one time, ~10 minutes)

**1. Get your proTxHash** (run while your masternode is ENABLED, copy the 64-char value):

```
ratatoskr-cli masternode status
```

**2. Create a small fee wallet** and send the address ~5 RATR:

```
ratatoskr-cli createwallet "revive"
ratatoskr-cli -rpcwallet=revive getnewaddress
```

**3. Create `/etc/mn-self-revive.conf`** and fill in the four values:

```
CLI="ratatoskr-cli"                          # add any -datadir= / -conf= flags you use
CONF="/root/.ratatoskrcore/ratatoskr.conf"   # the file that holds your masternodeblsprivkey
WALLET="revive"
PROTX_CONF="your_proTxHash_from_step_1"
WEBHOOK=""                                   # optional Discord webhook for alerts
```

Not sure where your conf file is? Find it with:

```
sudo grep -rl masternodeblsprivkey /root /home /opt 2>/dev/null
```

**4. Install the script** at `/usr/local/bin/mn-self-revive.sh`, then make it executable:

```
sudo chmod +x /usr/local/bin/mn-self-revive.sh
```

**5. Run it automatically every 10 minutes:**

```
( crontab -l 2>/dev/null; echo '*/10 * * * * /usr/local/bin/mn-self-revive.sh >/dev/null 2>&1' ) | crontab -
```

(No `cron`? A systemd timer works too — see the bottom of this file.)

## How it works

Every 10 minutes it checks your masternode. If it is PoSe-banned **and** your node is
healthy, it submits a revive and your masternode returns to ENABLED — with no manual
action from you.

**Guardrails:**

- Only fires when the masternode is actually banned.
- Skips if your node has 0 peers (a connectivity problem a revive won't fix).
- Rate-limited to one revive per 30 minutes.
- Stops and alerts after 8 revives in one day — a box that keeps re-banning has a real
  problem (firewall, sync, downtime) that should not be silently masked.

Logs to `/var/lib/mn-self-revive/revive.log`.

## systemd timer (alternative to cron)

`/etc/systemd/system/mn-self-revive.service`:

```
[Unit]
Description=Ratatoskr masternode PoSe self-revive
[Service]
Type=oneshot
ExecStart=/usr/local/bin/mn-self-revive.sh
```

`/etc/systemd/system/mn-self-revive.timer`:

```
[Unit]
Description=Run Ratatoskr MN self-revive every 10 minutes
[Timer]
OnBootSec=5min
OnUnitActiveSec=10min
[Install]
WantedBy=timers.target
```

Then enable it:

```
sudo systemctl daemon-reload && sudo systemctl enable --now mn-self-revive.timer
```
