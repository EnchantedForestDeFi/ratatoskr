# Ratatoskr — Brand Direction

This document is the visual-identity north star for Ratatoskr (RATR).
It exists so that when icons, logos, hero art, or marketing visuals get
made, they all pull from the same metaphor instead of drifting toward
generic crypto art.

It is intentionally text-only. No raster assets are committed here —
those are produced from this brief and live in `share/pixmaps/` and
`src/qt/res/icons/` once they exist.

---

## The core metaphor

In Norse mythology, **Ratatoskr** is a squirrel who runs up and down
the trunk of **Yggdrasil**, the world tree, carrying messages between
the **eagle perched at the crown** and **Niðhöggr the serpent gnawing
at the roots**. The nine worlds of Norse cosmology hang from
Yggdrasil's branches.

Ratatoskr is a *messenger*. He moves information across the tree.
That maps almost perfectly onto what the project does: **carry value
and messages between chains.**

| Lore element | What it represents in the project |
|---|---|
| Yggdrasil (the tree) | The multi-chain ecosystem (RATR L1, Alephium, future bridges) |
| **Alfheim (home of the light elves)** | **Alephium — the partner chain. The names are phonetic cognates: ALF-heim → AL-EPH-ium. This is the strongest single piece of brand kismet the project has, and it should be foregrounded everywhere the cosmology gets explained.** |
| The eagle at the crown | The bright upper realms (Asgard / Alfheim) — high-throughput counterparty chains |
| Niðhöggr at the roots | The PoW foundation — RATR's own chain at the base of the tree |
| **Ratatoskr running the trunk** | **The bridge — messages and value moving between chains** |
| The nine worlds | Future bridge endpoints / DeFi venues we plug into |
| EnchantedForestDeFi | The project umbrella; the forest where Yggdrasil grows |

The single most important thing this metaphor gives us: the project's
mascot is a **bridge by definition**. Ratatoskr exists to move things
between layers. The token, the bridge, and the brand all reinforce
each other.

### The Alephium ↔ Alfheim cognate (lock this in)

The phonetic alignment between **Alephium** (the partner chain RATR
bridges to) and **Alfheim** (one of the upper realms of Yggdrasil,
home of the light elves) is too clean to be coincidence — and even
if it *was* coincidence, the moment the project notices it, it
becomes part of the brand.

What this gives us:

- **A concrete realm-to-chain mapping.** When future readers ask
  "why a Norse mascot for an Alephium bridge?", the answer is no
  longer "we liked the lore" — it's "Alephium *is* Alfheim in the
  metaphor; the names are cognate; Ratatoskr running between
  Alfheim (high realm) and the RATR root layer (lower realm) is
  literally what the bridge does."
- **A natural extension path.** When the project bridges to a
  second chain, that chain becomes a different realm. Bitcoin →
  Asgard (the gold-domed citadel of the Aesir, home of Odin). A
  privacy-focused chain → Niflheim (mist and ice, hidden things).
  An EVM L2 → Vanaheim (a parallel realm, related but distinct).
  Each new chain gets a realm; each realm has Norse character to
  draw from.
- **A piece of language for the docs.** Project copy can say
  things like "messages between Yggdrasil's branches" or "the
  Alfheim leg of the bridge" without sounding pretentious — once
  the cognate is established, the lore-language is grounded.

Use this. In website hero copy, in whitepaper introductions, in
the "How the bridge works" page — establish the
Alephium-as-Alfheim mapping early and let everything else hang
off it.

---

## What the visuals should communicate

In rough order of priority:

1. **Movement / messaging.** Ratatoskr is in motion — running, climbing,
   carrying something. He is not a static portrait squirrel.
2. **Connection between layers.** A branch, a trunk, two anchor points.
   The viewer should sense that Ratatoskr is between two places.
3. **Norse / mythological tone.** Slightly rune-coded, slightly
   parchment-textured, but not LARP-themed. Closer to "thoughtful
   medieval bestiary" than "Skyrim merchandise."
4. **Solid silhouette.** The mark needs to be recognizable at 16×16
   pixels — at favicon size, you only get the shape, not the detail.
5. **Calm confidence.** This is not a meme coin. Avoid Comic Sans,
   avoid stacked emojis, avoid laser eyes. Tone is closer to a
   well-made indie game studio than to a 2021 NFT project.

---

## What the visuals should NOT communicate

- Not a Bitcoin/Dash clone. No "RATR" stamped on a gold coin in front
  of generic blockchain hex. That's every fork's first attempt and
  it's instantly forgettable.
- Not aggressive masculinity. No flame backdrops, no Viking warrior
  poses, no "to the moon" iconography. The lore is about wisdom and
  message-bearing, not conquest.
- Not over-cute. Ratatoskr is mischievous in the lore — he provokes
  and stirs up trouble between the eagle and the serpent. The
  visual should hint at sharpness and intelligence, not Pixar
  cuteness.
- Not photoreal. A photoreal squirrel scales badly and reads as
  stock-footage rather than identity. Prefer stylized illustration.

---

## Color palette (proposed, not locked)

Pulling from EnchantedForestDeFi's existing palette + the tree-bark
metaphor:

| Role | Color | Notes |
|---|---|---|
| Primary mark | Deep umber `#5B3A29` or warm bark brown | Tree-trunk feel, readable on light + dark |
| Accent / energy | Ember orange `#D7882F` or rune gold `#C9A227` | The "messenger" pop — coins, runes, motion lines |
| Secondary | Forest green `#2E5339` | Pulls in the EnchantedForestDeFi name |
| Neutral | Parchment cream `#EDE2C5` | Backgrounds for non-icon assets (web hero, decks) |
| Accent — danger / serpent | Smoky red `#7A2A1F` | For Niðhöggr-coded references (warnings, root-level events) |

The palette is here for **icon design**, not strict UI tokens. The
website + Qt theme already have their own palettes; this is the
identity layer that sits on top.

---

## Tone of copy

When written marketing copy accompanies the visuals:

- **Short, lore-aware, unfussy.** "Carry messages between worlds." not
  "Revolutionizing decentralized cross-chain interoperability."
- **First-person sparing.** The project is the squirrel; let the
  squirrel act, don't narrate.
- **Honest about scope.** Ratatoskr is a small chain. Lean into
  craftwork, not hyper-growth language.

---

## Where this brand surfaces

Ordered by priority for pre-launch:

1. **Wallet icon** (Qt `ratatoskr-qt`, Windows `.ico`, macOS `.icns`).
   See `icon-brief.md` for spec.
2. **Website hero** (`ratatoskr.enchantedforestdefi.com`). Currently a
   countdown page — gets a proper hero at T-7.
3. **Bridge UI splash** (`ratatoskrbridge.enchantedforestdefi.com`).
   Empty-state and connect-wallet-prompt could feature Ratatoskr at
   the trunk.
4. **Block explorer header** (`explorer.ratatoskr…`). Currently
   eIquidus default theme — could get a small Ratatoskr mark in
   the header logo slot.
5. **Discord server icon + splash.** Already live with placeholder.
6. **Whitepaper cover / launch announcement / Twitter banner.**

Each of these should pull from the same set of approved marks. Don't
commission five different squirrels for five different surfaces.

---

## References

For lore accuracy and free-to-use historical art, see
`yggdrasil-reference.md`. For the concrete icon production spec, see
`icon-brief.md`.

This document is intentionally short. It is a compass, not a manual.
When in doubt, return to the metaphor: **Ratatoskr is the messenger
between the eagle and the serpent.** That's the brand.
