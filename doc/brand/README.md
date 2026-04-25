# `doc/brand/` — Ratatoskr brand & visual-identity docs

This folder is where the brand lives as text. Actual image assets
ship in `share/pixmaps/` and `src/qt/res/icons/` — see
`icon-brief.md` for the production spec.

## Files

| File | Purpose | Read first if you are… |
|---|---|---|
| [`brand-direction.md`](brand-direction.md) | The metaphor, tone, palette, what to avoid | Anyone making a marketing decision |
| [`icon-brief.md`](icon-brief.md) | Concrete spec for the wallet icon family (16/32/.../1024) | A designer / icon producer |
| [`yggdrasil-reference.md`](yggdrasil-reference.md) | Norse lore notes + public-domain reference art pointers | Anyone writing copy or designing visuals |

## Subfolders (created on demand)

- `refs/` — public-domain reference imagery for designers. Not
  shipped in the project; for inspiration only.
- `refs/licensed/` — modern licensed reference images, with their
  license documents as siblings.
- `source/` — editable source files (`.svg`, layered editor files)
  for any committed visual asset, so they can be re-exported in
  the future without chasing the original artist.

## What's *not* here

- Final image assets (`.png`, `.ico`, `.icns`, `.svg` for shipping).
  Those live in `share/pixmaps/` and `src/qt/res/icons/`.
- Web-only graphics. The `enchantedforestdefi.com` site has its own
  asset folder; this brand doc informs them but doesn't store them.
- Discord assets. The Discord server's icon and splash live in
  Discord — operator manages that surface directly.

## Status

- **Brand direction**: drafted ✅
- **Icon brief**: drafted ✅
- **Lore reference**: drafted ✅
- **Reference imagery**: not collected yet
- **Production icon family**: not produced yet — placeholder pixels
  from the Smartiecoin upstream still ship in `share/pixmaps/`

The placeholder situation is tracked as a pre-launch TODO. Once a
real icon family is produced per `icon-brief.md`, the placeholders
get overwritten in place — no code changes needed, the build system
already points at the correct filenames.
