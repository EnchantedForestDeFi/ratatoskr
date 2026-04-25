# Ratatoskr — Icon Production Brief

This is the spec to hand to a designer (human or AI image gen) when it
is time to replace the placeholder icon files in the repo. The
placeholders currently in `share/pixmaps/ratatoskr*.png|.ico` and
`src/qt/res/icons/ratatoskr.{icns,ico,png}` were inherited from the
upstream Smartiecoin fork — they show that project's artwork, not
Ratatoskr's. They need to be replaced before public launch.

For the *why* behind the design choices, see `brand-direction.md`.
This file is the *what*.

---

## The mark

A stylized **Ratatoskr the squirrel** in motion on a section of
**Yggdrasil's trunk or branch**.

- The squirrel is the subject. He should occupy roughly 60–70% of
  the canvas.
- He is **moving** — running up or across the branch, body in a
  diagonal that suggests motion. Not sitting, not portrait-style.
- He is **carrying something** — a small message scroll, an acorn,
  or a coin. This is the "messenger" detail; without it the mark is
  just a squirrel.
- A short section of **branch or trunk** anchors him. The branch
  hints at the larger tree without trying to draw the whole thing.
- Optional: a single **rune** or **leaf** in negative space, far
  enough off-center that it doesn't compete with the squirrel.

Pose reference: think of how heraldic animals are drawn — three-quarter
view, dynamic but balanced, readable from a distance.

---

## Sizes required

| Size | Purpose | Where it ships |
|---|---|---|
| 16×16 | Favicon, smallest tray icon | `share/pixmaps/ratatoskr16.png` |
| 32×32 | Window icon, list-view icon | `share/pixmaps/ratatoskr32.png` |
| 64×64 | Application launcher | `share/pixmaps/ratatoskr64.png` |
| 128×128 | macOS Dock, app drawer | `share/pixmaps/ratatoskr128.png` |
| 256×256 | High-DPI displays, splash | `share/pixmaps/ratatoskr256.png` |
| `.ico` (multi-res) | Windows installer + .exe | `share/pixmaps/ratatoskr.ico` (contains 16/32/48/64/128/256) |
| `.icns` (multi-res) | macOS `.app` bundle | `src/qt/res/icons/ratatoskr.icns` (contains 16/32/64/128/256/512/1024) |
| 512×512 PNG | macOS .icns input + Discord server icon | source file |
| 1024×1024 PNG | macOS .icns input (retina), high-res hero | source file |

**Ship the source file too.** Whoever produces this needs to also commit
the SVG (or layered Photoshop/Illustrator/Affinity file) to
`doc/brand/source/` so future-you can re-export at new sizes without
chasing down the original artist.

---

## The 16×16 test

This is the single hardest constraint and where most icons fail.

At 16×16 you cannot show:
- Whiskers, claws, fur texture
- The carried object's detail (no readable scroll, no readable acorn)
- More than one or two colors of branch

You **can** show:
- The squirrel's silhouette — pointed ears, bushy tail, head + body
- A single high-contrast color block for "branch beneath him"
- One accent color (the carried object collapses to a dot)

If the 16×16 doesn't read as "small mammal on a stick" at a glance,
the design needs to simplify before scaling up. **Design at 16 first,
then add detail at 32, 64, 128 — not the other way around.**

---

## Color usage

Pulling from `brand-direction.md`:

- **Squirrel body**: warm bark brown (`#5B3A29`) with a slightly
  darker outline for separation against light backgrounds. A lighter
  tan belly/underbelly highlight is fine but optional at small sizes.
- **Carried object** (scroll/coin/acorn): ember orange (`#D7882F`)
  or rune gold (`#C9A227`). This is the only "pop" color in the mark.
- **Branch**: darker brown than the squirrel (`#3C2415`) so the
  squirrel reads as foreground.
- **Optional rune/leaf accent**: forest green (`#2E5339`) at very
  low opacity, or omitted entirely.

The icon should look correct on **both** a parchment-cream background
(`#EDE2C5`) and a dark navy/charcoal background (matching the
EnchantedForestDeFi night theme — operator to confirm exact dark hex).
Avoid colors that disappear on either.

---

## What to avoid

Concrete failure modes that have killed past crypto icons:

- **The "RATR" letterform stamped on a coin.** Generic, forgettable,
  identical to a hundred other Bitcoin forks.
- **A squirrel holding a literal Bitcoin "B" coin.** Wrong identity,
  invites confusion.
- **Photo-real fur and shading.** Doesn't survive the 16×16 test.
- **More than 4 colors.** Any more and the .ico file balloons + the
  shape stops reading.
- **A full Yggdrasil tree as the mark.** Too dense to ever scale
  down. The full tree is a *web hero* asset, not an icon.
- **Comic eyes / cartoon expression.** Overcommits to "cute," locks
  out the more serious uses (whitepaper cover, etc.). Keep the eye
  small and neutral.

---

## Where the icon files plug in

Once produced, the asset filenames are already wired into the build
system from the rebrand pass. Nothing to update in code — just
overwrite the placeholder bytes:

```
share/pixmaps/ratatoskr.ico         # Windows installer + .exe
share/pixmaps/ratatoskr16.png       # autostart hint
share/pixmaps/ratatoskr32.png
share/pixmaps/ratatoskr64.png
share/pixmaps/ratatoskr128.png
share/pixmaps/ratatoskr256.png
src/qt/res/icons/ratatoskr.icns     # macOS .app bundle
src/qt/res/icons/ratatoskr.ico      # Qt resource bundle (Windows)
src/qt/res/icons/ratatoskr.png      # Qt resource bundle (generic)
```

After replacement, rebuild:
- **Linux/macOS**: `make -j` from the source root
- **Windows**: full reinstall via NSIS (icon is baked in at install time)
- **Qt resources**: a clean rebuild — `make clean && make` — to force
  Qt's `rcc` to re-bundle the new images into the binary

No code changes. No version bump required.

---

## Commissioning vs. AI-generating

Both are viable. Tradeoffs:

**Commissioning a designer (~$150–$600 one-time)**
- ✅ Original work, fully licensed, source files included
- ✅ Designer can iterate on the 16×16 problem with you
- ✅ Cleaner story for a copyright/IP audit later
- ❌ 1–3 weeks turnaround
- ❌ Higher cash outlay pre-launch

Recommended platforms: 99designs (logo contest), Fiverr Pro tier,
Reddit r/designjobs, or any local illustrator with a portfolio of
small mammals + medieval/heraldic work.

**AI image gen (~$0–$30 one-time)**
- ✅ Fast — usually under an hour to land on a usable mark
- ✅ Cheap
- ❌ Output is raster, not vector — scaling to .icns/.ico requires
  manual cleanup or vectorization (e.g. via Adobe Illustrator's
  Image Trace, or `vtracer`)
- ❌ Licensing is murky for some platforms; check the platform's
  commercial-use terms before shipping
- ❌ Style consistency across the icon family (16/32/…/1024) is
  hard — each generation drifts slightly

If going the AI route: generate **one** large source image (1024×1024
or higher), then manually downscale + simplify for smaller sizes
rather than re-prompting at each size. Use a vector-tracing pass to
get a clean .svg before exporting the family.

A reasonable AI prompt to start from:

> A stylized illustration of a small bushy-tailed squirrel running
> diagonally up a section of dark tree branch, carrying a small
> rolled message scroll in its mouth. Heraldic / medieval bestiary
> art style. Solid colors, minimal detail, readable as a silhouette.
> Warm bark-brown squirrel, ember-orange scroll, dark brown branch,
> off-white background. Centered composition, square format,
> high contrast.

Iterate on that until the silhouette reads at 16×16, then commit to
the result.

---

## Acceptance checklist

Before merging the new icon files, verify all of these:

- [ ] All eight filenames above exist and are non-zero size
- [ ] `.ico` opens in Windows Explorer and shows multiple resolutions
      embedded
- [ ] `.icns` opens in macOS Preview and shows the icon family
- [ ] At 16×16 in your browser tab, the silhouette is recognizable
      as "small animal on a branch" — not a smudge
- [ ] At 256×256 the carried object (scroll/coin) is clearly readable
- [ ] Icon is visually distinct from Bitcoin's "B," Dash's wave,
      Smartiecoin's mark, and Litecoin's "Ł"
- [ ] Source file (`.svg` or layered editable file) is committed to
      `doc/brand/source/`
- [ ] License + attribution noted in `doc/assets-attribution.md` if
      the artist is external
- [ ] Operator has rebuilt the wallet locally and confirmed the new
      icon shows up in: window title bar, taskbar/dock, About
      dialog, and Windows installer
