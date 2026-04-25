# `doc/brand/source/` — Editable / source brand files

This folder holds the master files that the shipping brand assets
are derived from. Specifically:

- **AI-generated originals** (PNGs straight out of Google AI, with
  watermarks intact) — committed for provenance
- **Cropped / edited derivatives** — watermark removed, tighter
  framing for icon use
- **Vector versions** (`.svg`) when produced — best-effort
  re-traces of the rasters
- **Layered editor files** (`.psd`, `.ai`, `.kra`, `.afdesign`)
  if a designer ever delivers them

Files here are **not** referenced by the wallet, website, or any
other shipping surface. The shipping assets live in
`share/pixmaps/` and `src/qt/res/icons/`. This folder is the
upstream from which those are produced.

## Files in this folder

- `README.md` — this file
- `RECOMMENDATIONS.md` — selection logic for the four current
  candidates and the role each one plays
- `PROVENANCE.md` — what generated each asset (for IP defense + 
  reproducibility)
- `candidate-*.png` — the AI-generated originals (operator drops
  these in when ready)
- `icon-source-*.png` — derivative crop used as the input to the
  icon family
- _(future)_ `*.svg` vector versions

## Workflow

The full production workflow — from "I have a Google AI image"
through to "the wallet ships with the right icon" — is documented
in `RECOMMENDATIONS.md` under "Production phase." Read that.

## Why commit AI originals to git?

Two reasons:

1. **Provenance.** If anyone ever asks "where did this image come
   from?", the answer is in the repo with a timestamp and a prompt.
2. **Future regeneration.** Higher-resolution variants, alternate
   crops, or "the eagle to pair with the squirrel" are all easier
   to produce when the original is sitting in a stable location.

Tradeoff is repo size — large PNGs aren't great in git. Acceptable
because:
- These are infrequently changed (write once, read often)
- The repo isn't huge to begin with
- The alternative (separate asset host) introduces fragility
