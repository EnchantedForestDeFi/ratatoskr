# Ratatoskr (RATR) — Pool Operator Spec

Everything you need to add RATR to a Dash-style mining pool (yiimp, NOMP,
Coin-Pool, dashpool-pp, MPOS, or similar).

**Launch:** June 1, 2026 (block 0 — first mineable block is block 1)

---

## Chain identity

| Field | Value |
|---|---|
| Ticker | **RATR** |
| Name | Ratatoskr Core |
| Network | mainnet |
| Genesis hash | `00001059e1c1b34440ea46fcc1a3deb84e827f061ecf06f5013056219a66a4df` |
| Genesis nTime | `1777000000` (2026-04-23 UTC) |
| Genesis nBits | `0x1e3fffff` |
| BIP44 coin type | 530 |

## Network magic + ports

| Field | Value |
|---|---|
| Message start (magic) | `ac c0 f0 52` |
| P2P port | **9393** |
| RPC port (default) | **8282** |
| Address prefix | 60 (addresses start with `R`) |
| Script prefix | 122 |
| Secret key prefix (WIF) | 188 |

## Consensus

| Field | Value |
|---|---|
| Block time | 60 seconds |
| Max supply | 100,000,000 RATR |
| Subsidy at launch | **50 RATR/block** |
| Halving interval | every 1,030,596 blocks (~2 years) |
| Reward split | 60% miner / 30% masternode / 10% treasury |
| MN payments activate | **block 25,000** (~17 days post-launch) |
| First superblock / governance active | **block 30,000** |

Before block 25,000: **miners get 90%** of subsidy (60% miner + 30% MN share
temporarily flows to miner — actually, correction below), treasury gets 10%.

### Reward split pre/post-MN

**Before block 25,000** (MN payments inactive):
- Treasury: 10% (5 RATR of every 50-RATR block, to treasury address)
- Miner: 90% (45 RATR/block)

**From block 25,000 onwards:**
- Treasury: 10% (5 RATR)
- Miner: 60% (30 RATR)
- Masternode: 30% (15 RATR, paid to the selected MN collateral address)

### Coinbase TX format

Dash-lineage chain. Coinbase is a **CbTx special transaction** (DIP0003):

- Standard coinbase output to miner (after treasury subtracted)
- Treasury output: goes to a fixed treasury address (see below) for 10% of subsidy
- MN payment output (from block 25,000): goes to the selected MN's collateral
  address; selection is deterministic from the chain's MN registry
- BIP34 height is encoded as the first push in `scriptSig`

**Treasury address (mainnet, locked in chainparams):** TBD — will be posted
publicly and included in release notes before June 1. The treasury address is
hardware-wallet-secured; pool just pays to it as dictated by
`ratatoskrd`'s `getblocktemplate`.

## Algorithm

**yespower** (version 1.0, stock upstream Alexander Peslyak implementation)

| Parameter | Value |
|---|---|
| Variant | `yespower` (not yespowerr16 or yespower-b2b) |
| N | **2048** |
| R | **32** |
| Personalization string | **none** (NULL pers, perslen=0) |
| Hash output | 32 bytes |
| Input size | 80 bytes (block header) |

Stock `cpuminer-opt` with `-a yespower -N 2048 -R 32` works for miners via
stratum. No patches needed on the miner side because the pool server
constructs blocks and miners just submit nonces.

---

## Running a pool-side RATR node

Download the latest `ratatoskrd` release binary:

<https://github.com/EnchantedForestDeFi/ratatoskr/releases/tag/v1.0.0-alpha>

Or build from source (autotools + Dash build system).

Minimum `ratatoskr.conf`:

```ini
server=1
listen=1
txindex=1

# RPC binding
rpcuser=CHOOSE_A_USERNAME
rpcpassword=CHOOSE_A_STRONG_PASSWORD
rpcallowip=127.0.0.1
rpcport=8282

# If your pool software needs ZMQ notifications:
zmqpubrawblock=tcp://127.0.0.1:28332
zmqpubrawtx=tcp://127.0.0.1:28333

# Node connectivity
port=9393
```

Required txindex=1 is standard for pool operations (lookups by txid).

### Useful RPCs for pool integration

- `getblocktemplate {"rules": ["segwit", "mweb"]}` — block template incl. MN
  and treasury payments. Pool uses this to construct the coinbase.
- `submitblock <hex>` — submit a completed block
- `getmininginfo` — network difficulty, hashrate estimate
- `getblockchaininfo` — verify chain state
- `getpeerinfo` — check P2P health
- `listmasternodes` — registered MN set (for validating template MN payments)

---

## Stratum integration notes

- Mining protocol: **standard stratum/v1 + getblocktemplate**
- Miners will connect with `-a yespower` (no extra params needed)
- Coinbase is constructed pool-side. Use `getblocktemplate` output directly
  for `masternode` and `treasury` payment fields.
- Extra-nonce: standard 8-byte extranonce1 + 4-byte extranonce2 layout works.
- Coinbase scriptSig: BIP34 height push is mandatory. Without it, blocks
  rejected with `bad-cb-height`.

Pool templates that already work for Dash should work for RATR with
chainparams updated.

---

## Seed nodes (for pool's RATR node to peer with)

As of launch:

- `seed1.ratatoskr.enchantedforestdefi.com` — VPS1 (IP: `74.208.37.172`)
- `seed2.ratatoskr.enchantedforestdefi.com` — VPS2 (TBD)

Add to `ratatoskr.conf`:

```
addnode=seed1.ratatoskr.enchantedforestdefi.com
addnode=seed2.ratatoskr.enchantedforestdefi.com
```

Or via command line: `-addnode=seed1.ratatoskr.enchantedforestdefi.com`

---

## Block timing + difficulty

- 60-second target, initial difficulty is near-zero (0x1e3fffff)
- Difficulty retargets every block (DigiShield/Dash-style)
- Expect difficulty to spike 100x+ in the first hour as miners ramp up
- Don't pre-tune stratum vardiff to Bitcoin-style 1-minute target; start loose
  (diff ~1e-6) and let vardiff adjust

---

## Contact / support for pool setup

- GitHub: <https://github.com/EnchantedForestDeFi/ratatoskr>
- Issues: <https://github.com/EnchantedForestDeFi/ratatoskr/issues>
- Discord: EnchantedForestDeFi server (channel `#ratatoskr` will be live
  before launch)
- Project operator direct: NexusAether@protonmail.com

Please let the project know your pool URL(s) so miners can find you on
launch day.

---

## Alpha caveats

- **Chain is not yet live.** Launch is June 1, 2026.
- **No audit yet.** Small delta on top of well-audited Dash codebase, but
  RATR-specific changes are not externally reviewed.
- **Treasury is single-sig at v1.0-alpha.** Transition to multisig planned
  for v1.1+ (6-12 months post-launch).
- **Reward split is hardcoded at v1.0-alpha.** Governance-editable splits
  (with floors and gradualism caps) are planned for the v1.1 hard fork.
