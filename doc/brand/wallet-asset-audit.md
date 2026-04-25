# Wallet visual asset audit

Status snapshot of every visual asset shipping in the Qt wallet, what
it is, and whether it needs brand work before launch.

Last audit: 2026-04-25.

## ✅ Replaced with Ratatoskr-branded artwork

| Asset | Source | Notes |
|---|---|---|
| `share/pixmaps/ratatoskr.ico` (multi-res 16/32/48/64/128/256) | candidate-1 (rope-cartoon) tight crop | Windows installer + .exe icon |
| `share/pixmaps/ratatoskr{16,32,64,128,256}.png` | candidate-1 tight crop | Linux app launcher icon family |
| `src/qt/res/icons/ratatoskr.{ico,icns,png}` | candidate-1 tight crop | Qt resource bundle (compiled into binary) |
| `src/qt/res/images/splash.png` | candidate-1 (1024×1024 master) | Splash screen logo on wallet startup |
| `doc/brand/heroes/hero-canonical-mark.png` | candidate-4 (B&W full composition) | README banner; not bundled into the wallet binary |

## ✅ Verified to work cleanly with the new icon

| Asset / system | Verification |
|---|---|
| `NetworkStyle::rotateColors` HSL shifts | Tested via `doc/brand/source/icon-preview/check-network-styles.py`; mainnet/testnet/devnet/regtest all produce visually distinct colors (warm brown / blue / olive / teal). No tuning needed. |
| OS-level URI scheme registration | Linux `.desktop` MimeType + macOS `Info.plist` CFBundleURLSchemes both register `ratatoskr:` cleanly. Windows installer (`share/setup.nsi.in`) registers `URL:Ratatoskr`. |
| macOS bundle identifier | `Info.plist` and `macnotificationhandler.mm` both agree on `com.enchantedforestdefi.Ratatoskr-Qt`. |

## 🟡 Generic / neutral assets — keeping as-is

These aren't Dash- or Smartiecoin-branded; they're generic UI
indicators that work fine in any chain context. No replacement
needed.

| Asset | Reason it stays |
|---|---|
| `src/qt/res/animation/spinner-{000..089}.png` (90 frames) | Generic monochrome circular-arrow loading spinner. LA mode (greyscale + alpha) so it adapts to light and dark themes. Display size is ~20-30px — any squirrel-themed alternative would lose detail. |
| `src/qt/res/src/spinner.png` (60×60 source for the spinner frames) | Source for the above. Same reasoning. |
| `src/qt/res/icons/transaction*.png` | Generic transaction-state icons (incoming/outgoing/conflict/abandon). Not chain-branded. |

## 🟢 Not yet touched — operator decision points

| Asset | Status / decision needed |
|---|---|
| `src/qt/locale/dash_*.ts` translation files (23 languages) | Source-string translation extracts. Need a `lupdate` re-run after the recent string changes (Welcome dialog, Mining info, Bridge link, RATR ticker, etc.) so future translators see the new strings. Requires Qt's `lupdate` tool installed. |
| `share/pixmaps/dash*.{ico,png,xpm}` (the "dash" alternates next to ratatoskr.*) | Upstream Dash assets retained for fallback / parallel builds. Not referenced by the build system after the rebrand pass. Cleanup-only deletion is safe but cosmetic. |
| `doc/man/dash-*.1` (5 stale Dash man pages) | Replaced by `ratatoskr-*.1` in the `dist_man1_MANS` ship list. The dash-named files are no longer shipped; safe to delete. |
| `src/qt/res/icons/ratatoskr.{ico,icns,png}` actual pixel content | Currently the rope-cartoon (candidate-1). Operator may eventually want to commission a hand-drawn replacement that owns the IP fully. Document the path in `doc/brand/icon-brief.md`. |

## Asset locations summary

```
share/pixmaps/                   # Linux app-launcher icons + Windows .ico
share/qt/                        # macOS Info.plist
src/qt/res/icons/                # Qt resource bundle (compiled into binary)
src/qt/res/images/               # Splash screen image
src/qt/res/animation/            # Status-bar spinner frames
src/qt/res/css/                  # Qt theme stylesheets (dark/light/traditional)
doc/brand/heroes/                # README + website + whitepaper hero illustrations (not bundled)
doc/brand/source/                # AI-generated candidate originals + icon-preview crops
contrib/debian/                  # .desktop file (Linux URI handler registration)
share/setup.nsi.in               # Windows installer script
```
