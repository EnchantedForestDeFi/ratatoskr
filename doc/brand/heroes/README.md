# `doc/brand/heroes/` — Hero illustrations

These are full-quality brand illustrations that **do not** ship in
the wallet binary, but are pulled in by other project surfaces:
the README, the website, the whitepaper, social posts, the bridge
UI splash. The wallet's actual application icon ships from
`share/pixmaps/` and `src/qt/res/icons/`, which are derived from
`candidate-1` (the rope-cartoon).

## Files

| File | Source | Use for |
|---|---|---|
| `hero-canonical-mark.png` | `candidate-4` (B&W full composition with serpent + label) | README banner, social cards, whitepaper cover, anywhere the project introduces itself |
| `hero-color-manuscript.png` | `candidate-2` (color manuscript with ash leaves + runic border) | Bridge UI splash, About-dialog header art, color contexts on the website |
| `hero-narrative-map.png` | `candidate-6` (narrative map, Svartalfheim variant) | "How the bridge works" page, whitepaper inside cover, onboarding splash, Twitter / Medium explainer threads. Has two known minor issues — duplicate "Urd's Well" label and a garbled label near the central squirrel — fix via crop or one more re-roll before deploying publicly. |

## Why a separate folder

`source/` holds the AI-generated originals + the icon-preview
derivatives. `heroes/` holds the final-quality assets ready for
non-icon use. Keeping them separate means:

- Source files (with provenance, alternative crops, working
  files) stay together in `source/`
- Hero assets ready to drop into the website / whitepaper /
  social have stable filenames that won't churn

Future hero illustrations (e.g. an eagle-only mark to pair with
the squirrel for the L1, an aged-parchment background tile)
should land here under the same `hero-*.png` naming convention.

## Source provenance

All three files derive from the AI-generated candidates documented
in `../source/PROVENANCE.md`. The actual prompt + Google AI product
metadata lives there, not duplicated here.
