# Ratatoskr Core v1.0.0-alpha

**Status:** Alpha release. Mainnet launch scheduled for **2026-06-01 00:00 UTC**.

This is a pre-launch build for pool operators, miners, and infrastructure
partners to pre-stage. The chain is **not yet live**. Do not treat binaries
built from this tag as the final mainnet release — a rebuild against a
post-treasury-address-bake commit will be tagged `v1.0.0` before launch.

---

## What is Ratatoskr?

A small, opinionated yespower-PoW masternode coin with a consensus-enforced
per-block treasury drip, bounded governance, and a native bridge to
Alephium. Built on the Dash / Smartiecoin lineage, with tokenomics
rebalanced to keep miners profitable and governance bounded.

- **Algorithm:** yespower v1.0 (N=2048, R=32, CPU-only)
- **Block time:** 60 seconds
- **Block reward:** 50 RATR, halving every 1,030,596 blocks (~2 years)
- **Max supply:** 100,000,000 RATR
- **Reward split:** 60% miner / 30% masternode / 10% treasury
- **MN tiers:** Regular 7,500 RATR (1× vote) / EvoNode 30,000 RATR (4× vote)
- **MN payments activate:** block 25,000 (~17 days post-launch)
- **Bridge:** wRATR on Alephium, live at launch

---

## What's in this release

| File | Contents |
|---|---|
| `ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz` | Ubuntu 22.04+ binaries (`ratatoskrd`, `ratatoskr-cli`, `ratatoskr-tx`, `ratatoskr-qt`) + docs |
| `ratatoskr-1.0.0-alpha-win64.tar.gz` | Windows 64-bit binaries (same set) + docs |
| `ratatoskr-1.0.0-alpha-macos-arm64.tar.gz` | macOS Apple Silicon binaries (`ratatoskrd`, `ratatoskr-cli`, `ratatoskr-tx`; no Qt GUI in alpha) + docs |

Both tarballs include `doc/mining.md`, `doc/pool-operator-spec.md`,
`doc/whitepaper.md`, `doc/launch-announcement.md`, and
`doc/launch-day-runbook.md`.

SHA256 checksums are posted alongside each tarball. Verify before extracting:
```bash
sha256sum -c ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz.sha256
```

---

## For miners

Three launch-day paths covered in `doc/mining.md`:
1. **Pool stratum** — standard `cpuminer-opt -a yespower` against any pool
2. **Pool solo mode** — same miner, different port, full reward if you find
3. **Daemon internal miner** — `setgenerate true N` in Qt debug console or `ratatoskrd -gen=1`

No pool URLs are live yet. Pool operators will announce their endpoints
before launch in the project Discord.

---

## For pool operators

See `doc/pool-operator-spec.md`. Everything needed for stratum + node
integration: genesis hash, network magic (`ac c0 f0 52`), P2P port 9393,
RPC port 8282, address prefix 60 (R...), yespower params, Dash-lineage
CbTx coinbase format.

Coinbase is constructed pool-side via `getblocktemplate`. Stock Dash-style
pool software works with updated chainparams.

---

## For masternode operators

Two tiers:
- **Regular MN** — 7,500 RATR collateral, 1× voting weight
- **EvoNode** — 30,000 RATR collateral, 4× voting weight

Standard DIP0003 deterministic masternode registration (`protx register_fund`).
MN payments activate at block 25,000. MN setup guide will be published
before launch.

---

## Known alpha caveats

- **Chain is not live.** Launch is 2026-06-01 00:00 UTC.
- **No external audit.** RATR-specific consensus changes (treasury drip,
  MN collateral tiers, chainparams) are a small delta on the audited Dash
  codebase, but the delta itself is not externally reviewed.
- **Treasury is single-sig at v1.0.** Multisig (2-of-3) migration planned
  for v1.1 hard fork, 6-12 months post-launch.
- **Reward split is hardcoded at v1.0.** Governance-editable splits with
  consensus floors + gradualism caps are planned for v1.1.
- **Placeholder treasury script in this build.** The final v1.0.0 build
  will have the real treasury address (air-gapped keygen) baked in.

---

## Unsigned binaries — expected OS warnings

These alpha binaries are **not code-signed**. Code signing certificates
(Apple Developer ID, Windows EV cert) are deferred until post-launch when
the project has sustained revenue. This means your operating system will
show security warnings on first run. **These warnings are expected and
do not indicate malware** — but you should always verify the SHA256
against the value published on the GitHub Release page before running.

### Windows

- **SmartScreen:** On first launch, Windows shows "Windows protected your
  PC." Click **"More info"** → **"Run anyway"**.
- **Microsoft Defender:** May briefly flag `ratatoskrd.exe` or
  `ratatoskr-qt.exe` as "unknown publisher." Allow through Defender if
  needed. A reputation-less binary is not the same as a flagged malicious
  one.
- **Browser download warning:** Edge / Chrome may warn "this file isn't
  commonly downloaded." Click **Keep** after you've verified the SHA256.

### macOS

- **Gatekeeper:** On first launch, macOS shows "cannot be opened because
  the developer cannot be verified." Go to **System Settings → Privacy &
  Security**, scroll to the bottom, click **"Open Anyway"** next to the
  Ratatoskr binary. Alternatively from Terminal:
  ```bash
  xattr -d com.apple.quarantine ./ratatoskrd
  ```
- **No notarization:** Apple notarization is deferred until v1.0 release
  with real signing infra.

### Linux

- No OS-level signing warnings. Distribution package managers (apt, dnf)
  are not yet targeted for alpha — raw tarball extraction only. Future
  `.deb` and `.rpm` packages will be signed.

### Verifying SHA256

Always verify the tarball checksum before extracting. Checksums are
published alongside each tarball on the GitHub Release page, in the
project Discord `#ratatoskr` pinned message, and in the whitepaper
appendix at release time.

```bash
# Linux / macOS
sha256sum -c ratatoskr-1.0.0-alpha-linux-x86_64.tar.gz.sha256

# Windows PowerShell
Get-FileHash ratatoskr-1.0.0-alpha-win64.tar.gz -Algorithm SHA256
```

If the hash doesn't match the one published on the release page, **do
not run the binaries** — report in Discord.

---

## Verification

Reproducible build instructions are in `doc/build-unix.md` (Linux) and
`doc/build-windows.md` (Windows cross-compile via Debian Docker). Pool
operators and security-conscious users are encouraged to build from source
and compare binary hashes.

Chain parameters are in `src/chainparams.cpp`. Treasury payment logic is
in `src/masternode/payments.cpp` (`GetTreasuryPayment`, `FillBlockPayments`,
`IsBlockPayeeValid`). MN collateral tiers are in `src/evo/dmn_types.h`.

---

## Links

- **Whitepaper:** [`doc/whitepaper.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/whitepaper.md)
- **Launch announcement:** [`doc/launch-announcement.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/launch-announcement.md)
- **Mining guide:** [`doc/mining.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/mining.md)
- **Pool operator spec:** [`doc/pool-operator-spec.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/pool-operator-spec.md)
- **Launch-day runbook:** [`doc/launch-day-runbook.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/launch-day-runbook.md)
- **Treasury keygen runbook:** [`doc/treasury-keygen-runbook.md`](https://github.com/EnchantedForestDeFi/ratatoskr/blob/main/doc/treasury-keygen-runbook.md)
- **Discord:** EnchantedForestDeFi server, channel `#ratatoskr`
- **Contact:** `releases@enchantedforestdefi.com`

---

## Attribution

Ratatoskr Core is a fork of [Smartiecoin Core](https://github.com/SmartiesCoin/Smartiecoin)
v0.2.0, which itself derives from Dash Core. MIT license throughout;
attribution preserved in all source files.

🐿️
