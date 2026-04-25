"""
Render the testnet / devnet / regtest network-style HSL rotations on
top of the live wallet icon (share/pixmaps/ratatoskr256.png) so the
operator can eyeball that the rotated colors are still visually
distinct from mainnet AND from each other.

Mirrors what NetworkStyle::rotateColors does in
src/qt/networkstyle.cpp:

  test    : hue +190, sat -20
  devnet  : hue  +35, sat -15
  regtest : hue +160, sat -30

PIL doesn't have an HSL primitive matching Qt's QColor::setHsl
exactly, but does it close enough for an eyeball check.

Run:
  python doc/brand/source/icon-preview/check-network-styles.py
"""

from pathlib import Path
from PIL import Image
import colorsys

REPO = Path(__file__).resolve().parents[4]
SRC  = REPO / "share/pixmaps/ratatoskr256.png"
OUT  = REPO / "doc/brand/source/icon-preview/network-style-check"

NETWORKS = {
    "mainnet" : (   0,   0),  # baseline
    "testnet" : ( 190,  20),
    "devnet"  : (  35,  15),
    "regtest" : ( 160,  30),
}

def rotate(img, hue_shift_deg, sat_reduction):
    """Apply hue shift + saturation reduction matching networkstyle.cpp."""
    out = Image.new("RGBA", img.size)
    px_in = img.load()
    px_out = out.load()
    for y in range(img.height):
        for x in range(img.width):
            r, g, b, a = px_in[x, y]
            if a == 0:
                px_out[x, y] = (0, 0, 0, 0)
                continue
            h, l, s = colorsys.rgb_to_hls(r/255, g/255, b/255)
            h = (h + hue_shift_deg/360.0) % 1.0
            s = max(0.0, s - sat_reduction/255.0)
            r2, g2, b2 = colorsys.hls_to_rgb(h, l, s)
            px_out[x, y] = (int(r2*255), int(g2*255), int(b2*255), a)
    return out

OUT.mkdir(parents=True, exist_ok=True)

print(f"Source: {SRC}")
src = Image.open(SRC).convert("RGBA")
print(f"Size: {src.size}")
print()

for name, (hue, sat) in NETWORKS.items():
    rotated = rotate(src, hue, sat)
    out_path = OUT / f"ratatoskr256-{name}.png"
    rotated.save(out_path, "PNG")
    print(f"  {name:8s} hue+{hue:>3d} sat-{sat:>3d}  ->  {out_path.name}")

print()
print(f"Compare in: {OUT}")
print("If any two networks look indistinguishable, adjust the HSL")
print("shifts in src/qt/networkstyle.cpp before launch.")
