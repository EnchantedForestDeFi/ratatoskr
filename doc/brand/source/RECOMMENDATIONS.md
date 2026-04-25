# Brand Mark Selection — Working Notes

Status: **iterating in Google AI**, not yet finalized. This file
captures the four candidate marks generated in the second prompt
cycle (the woodcut / manuscript-illumination round) and the role
each one is best suited for. When the operator picks finals, this
file gets updated to reflect the chosen set.

For the metaphor + production spec see:
- `../brand-direction.md` — the brand compass
- `../icon-brief.md` — the icon production spec
- `../yggdrasil-reference.md` — the Norse lore reference

---

## The five candidates

Files are committed into this folder under canonical names so the
docs reference them stably. The original Google AI session outputs
are preserved in the operator's `Pictures/Screenshots/` folder under
their auto-generated `image_*.png` filenames.

### `candidate-1-rope-cartoon.png`
- **Style**: simple cartoon woodcut, rope-frame border
- **Composition**: brown squirrel on a thin branch, orange scroll in
  mouth, white background
- **Strengths**: cleanest silhouette of any candidate; the carried
  scroll is the most visible carried-object across all renders;
  scales well; instantly readable
- **Weaknesses**: tonally lighter than the rest of the brand; reads
  as "playful sticker," not "ancient manuscript"
- **Best role**: Discord custom emojis, sticker packs, casual
  social posts, T-shirt prints

### `candidate-2-color-manuscript.png`
- **Style**: medieval manuscript illumination, ash leaves, runic
  border, parchment background
- **Composition**: red squirrel mid-climb on an ash branch with
  hanging seed clusters, scroll in mouth, full runic border
- **Strengths**: lore-accurate (Yggdrasil is canonically an ash
  tree); rich color palette in the project's territory (red ochre,
  forest green, gold); the carried-scroll metaphor is unmistakable
- **Weaknesses**: too detailed for icon use below 128×128; the body
  patterns muddle when shrunk
- **Best role**: color hero — bridge UI splash, block explorer
  header, anywhere a parchment-cream block fits, About dialog
  large-format header, marketing one-pagers

### `candidate-3-bw-stripped.png`
- **Style**: pure ink line work woodcut, no runic border, no label
- **Composition**: stylized squirrel with knotwork patterns,
  acorn in paws, ash branch with leaves, aged paper background
- **Strengths**: clean "naked" version with no frame to crop around;
  perfect starting point for tighter icon crops; works at almost any
  size
- **Weaknesses**: missing the wider lore context (no serpent, no
  label) — feels like "a squirrel" rather than "Ratatoskr"
- **Best role**: **icon source.** Crop tighter to the squirrel +
  immediate branch, then resize to 16/32/64/128/256 and bundle
  into `.ico` and `.icns` per `../icon-brief.md`

### `candidate-4-bw-full-composition.png`
- **Style**: ink line work woodcut, full runic border, faded serpent
  in background, "Ratatoskr" label
- **Composition**: same squirrel-on-ash-branch as candidate 3, but
  with Niðhöggr the serpent ghosting in the background behind the
  trunk, ornamental runic border, aged parchment with ink splotches,
  Ratatoskr labeled in the lower portion
- **Strengths**: the only image that tells the *full* lore in one
  frame — squirrel mid-climb (messenger), acorn (the message),
  ash tree (Yggdrasil), serpent at the roots (Niðhöggr); the runic
  border firmly anchors it in Norse cosmology rather than generic
  fantasy; the aging effects make it look like a recovered document
- **Weaknesses**: too dense for tiny sizes; the serpent and the
  label disappear at <512px
- **Best role**: **canonical brand mark** — README banner, website
  hero, About dialog small-format header, whitepaper cover, social
  banners, anywhere the project introduces itself

### `candidate-5-narrative-map-1024.png`
- **Style**: cartographic / explainer illustration on aged parchment;
  shows the message-path between realms in a single composition
- **Composition**: Yggdrasil tree on the left with Asgard at the
  crown and roots descending into Helheim; on the right, a separate
  panel showing the same trunk + cave continuing down; squirrels
  with scrolls move between the labeled realms; runic borders top
  and bottom; compass rose; title "Yggdrasil: The Nine Worlds —
  Ratatoskr's Message Path"
- **Strengths**: tells the *story* of what the project does in one
  image; no other candidate does this. Useful for explainer
  contexts where a single mark wouldn't suffice.
- **Weaknesses (production-blocking — fix before deploying)**:
  1. **"YGDRASIL" is missing a G in the title** (should be
     YGGDRASIL — double G). Re-roll or post-edit before any
     public use.
  2. **"HELHEIM — REALM OF THE DEAD" appears twice** (bottom-left
     panel and bottom-right panel). The right-side panel should be
     a different realm — most likely NIFLHEIM (icy realm; matches
     the cave/skeleton aesthetic) or ASGARD if showing the
     messenger origin.
  3. Runic borders are mixed actual runes + decorative noise.
     Acceptable for a stylized brand asset but not literally
     translatable.
- **Best role**: **narrative explainer / lore illustration** —
  "How the bridge works" website page, whitepaper inside cover,
  blog/Medium explainer posts, Twitter explainer threads, bridge
  UI first-time-user splash. **Not** an icon, hero, or merch piece.

### `candidate-6-narrative-map-svartalfheim-1024.png`
- **Style**: same cartographic / aged-parchment direction as
  candidate-5, but a substantial refinement
- **Composition**: wider 16:9 aspect, all nine realms uniquely
  labeled with canonical Norse names (Asgard / Vanaheim / Alfheim /
  **Svartalfheim** / Midgard / Jotunheim / Muspelheim / Niflheim /
  Hel), Bifröst rainbow bridge from Asgard to Midgard, Jörmungandr
  ringing Midgard, Urd's Well noted, two squirrels visible on the
  trunk
- **Strengths over candidate-5**:
  - **"YGGDRASIL" spelled correctly** in the title (candidate-5's
    "YGDRASIL" typo is fixed)
  - **All 9 realms uniquely labeled** (candidate-5's double-Helheim
    error is fixed)
  - Rich cosmological detail — Bifröst, Jörmungandr, Urd's Well —
    that wasn't in candidate-5
  - Subtitle "The Nine Realms of Norse Cosmology" reads cleanly
- **Realm-naming choice**: uses **Svartalfheim** ("Realm of the
  Dark Elves") for the middle-left slot. Both Svartalfheim and
  Nidavellir are canon-defensible there; Snorri's *Prose Edda*
  treats them as related. The operator also has a v2 of this map
  with **Nidavellir** ("Realm of the Dwarves") in the same slot —
  not committed, available in `Downloads/image_e7f64e55.png` if
  the operator wants it added later as candidate-7.
- **Remaining issues (minor, fixable post-hoc)**:
  1. **"Urd's Well" appears twice** (once near the upper trunk,
     once near the bottom). Urd's Well is a single feature in
     canon. AI artifact; one label is enough.
  2. **Garbled text near the central squirrel** (looks like the
     model attempted "YGGDRASIL" or "RATATOSKR" and produced
     non-word characters). Acceptable for design history; should
     be cropped or re-rolled before public deployment.
- **Best role**: **canonical narrative explainer**, replacing
  candidate-5 in production positions. Same role mapping as
  candidate-5 — "How the bridge works" page, whitepaper inside
  cover, Twitter / Medium explainer threads, bridge UI onboarding.
  candidate-5 stays in the repo as design history but is not the
  one to deploy.

---

## Recommended role mapping (proposed)

| Surface | Candidate | Why |
|---|---|---|
| Wallet icon family (16→256) + `.ico` + `.icns` | **3** (cropped tighter) | Cleanest source, scales to small sizes |
| GitHub README banner / hero | **4** | Tells the full story; first impression |
| Website hero (`ratatoskr.enchantedforestdefi.com`) | **4** for the night/dark theme; **2** for the light/parchment theme | Two heroes, one for each background mode |
| About dialog header in the wallet | **4** as a small horizontal crop | Already-aged feel matches the dialog tone |
| Bridge UI splash | **2** | Color contrast against the bridge's UI |
| Block explorer header logo | tight crop of **3** | Tiny rendering area; needs the silhouette |
| Discord server icon (round) | tight crop of **3** | Discord icons are circular; the squirrel-only crop survives |
| Discord custom emojis | **1** + zoomed details from **3** | Casual tone fits the rope-cartoon aesthetic |
| Whitepaper cover page | **4** full | Full lore on the cover sets the document tone |
| Twitter / social banner | **4** stretched horizontal | Same as whitepaper logic |
| Merch (T-shirts, mugs, hats) | **1** for casual; **4** for premium | Two SKU tiers |
| Whitepaper inside cover / "How the bridge works" page | **5** (after typo + duplicate-realm fixes) | Tells the full message-path story |
| Bridge UI first-time-user splash / onboarding | **5** | Explains the metaphor for new users |
| Twitter / Medium explainer threads | **5** | Cartographic style draws engagement |

This is intentionally a system, not a single mark. Once committed,
each surface uses its assigned variant — no improvising. Consistency
matters more than which variant wins.

The brand system now spans **five roles**: icon source, color hero,
B&W canonical mark, casual/merch, and narrative explainer.

---

## Stop-iterating checklist

Common trap: keep generating until "the perfect one" appears. Don't.
The brand is the system, not any single image. When the operator
agrees these four (or close variants) are usable, the iteration ends
and the cropping / format-conversion work begins.

Markers that you've stopped converging:
- ☐ The new image isn't better than #4 — just different
- ☐ Each new image fixes one thing and breaks another
- ☐ The cycle has run more than 6 times
- ☐ You've started thinking about font choices

When two of those four are true, lock the current best four and
move to the production phase below.

---

## Production phase — what happens once finals are picked

1. **Save high-resolution originals to this folder** at canonical
   filenames:
   ```
   doc/brand/source/
     candidate-1-rope-cartoon-1024.png
     candidate-2-color-manuscript-1024.png
     candidate-3-bw-stripped-1024.png
     candidate-4-bw-full-composition-1024.png
   ```
   (1024 = whatever the largest size Google AI returned. Higher is
   always better.)

2. **Save the prompts and provenance** in
   `doc/brand/source/PROVENANCE.md` — exact prompt text per image,
   which Google AI product (Gemini app / ImageFX / Whisk / etc.),
   timestamp, and the operator's email/account that generated it.
   This is your IP defense if anyone ever questions ownership.

3. **Crop the watermark.** All four likely have a small Google AI
   diamond in the bottom-right corner. Open in any image editor,
   crop ~30px off the bottom-right corner, save as
   `*-cropped.png` next to the originals.

4. **Generate the icon source crop.** From candidate 3's cropped
   version, tighten to just the squirrel + the immediate piece of
   branch under him. Save as
   `doc/brand/source/icon-source-1024.png`. This is the master
   from which the whole icon family derives.

5. **Generate the icon family.** From `icon-source-1024.png`:
   - `share/pixmaps/ratatoskr16.png` (16×16)
   - `share/pixmaps/ratatoskr32.png` (32×32)
   - `share/pixmaps/ratatoskr64.png` (64×64)
   - `share/pixmaps/ratatoskr128.png` (128×128)
   - `share/pixmaps/ratatoskr256.png` (256×256)
   - `share/pixmaps/ratatoskr.ico` — multi-res Windows icon
     (16/32/48/64/128/256 bundled)
   - `src/qt/res/icons/ratatoskr.icns` — multi-res macOS icon
     (16/32/64/128/256/512/1024 bundled)
   - `src/qt/res/icons/ratatoskr.ico` — same as the share/pixmaps
     one, duplicated into the Qt resource path
   - `src/qt/res/icons/ratatoskr.png` — single 256×256 master

   Tools:
   - GIMP / Krita / Photoshop / Affinity for resizing
   - ImageMagick `convert` for batch resize + multi-res .ico
     bundling
   - `iconutil` (macOS only) for .icns
   - Online fallback: cloudconvert.com or icoconvert.com (both
     handle multi-res .ico and .icns)

6. **Verify each render.** Open the wallet locally; check that the
   icon shows up correctly in:
   - Window title bar
   - Taskbar / Dock
   - About dialog (top-left of the dialog header)
   - Windows installer (after rebuilding NSIS)
   - macOS .app bundle Finder display

7. **Mark the placeholder-assets TODO complete.** From the session
   memory: "operator to swap ratatoskr.ico/PNGs pixel content."

---

## Open questions (operator to resolve)

- ☐ Which Google AI product produced these? (Determines the
  commercial-use license. Likely Gemini consumer or ImageFX —
  either is fine for commercial use, but document it.)
- ☐ Is image 4's exact composition keepable, or do you want one
  more iteration to refine (e.g. larger acorn, clearer serpent,
  different label position)?
- ☐ Locked palette? `brand-direction.md` proposed values — image 2
  closely matches them but it'd be useful to formally adopt the
  palette as the project's color tokens.
- ☐ Do you want a vector (.svg) version of the chosen icon? Worth
  it for long-term scalability. Easiest path: take the
  `icon-source-1024.png`, run it through Adobe Illustrator's Image
  Trace or [vtracer](https://www.visioncortex.org/vtracer-docs)
  for a reasonable SVG.

---

## Update log

- 2026-04-25: Initial four candidates generated in second prompt
  cycle. Recommendations drafted; operator continuing to iterate.
- (next): operator picks finals, saves originals + provenance,
  production phase begins.
