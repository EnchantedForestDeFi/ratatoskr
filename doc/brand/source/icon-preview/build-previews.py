"""
Generate three icon-family preview sets at three crop tightnesses
from doc/brand/source/candidate-3-bw-stripped-1024.png.

Each set produces:
  - 16/32/48/64/128/256 .png
  - a multi-res .ico
  - a multi-res .icns (Pillow can emit these on Windows too)

Output goes to doc/brand/source/icon-preview/{full,medium,tight}/.
Operator picks one set; the chosen set's files get copied into
share/pixmaps/ and src/qt/res/icons/ to replace placeholders.

Run:
  cd <ratatoskr repo root>
  python doc/brand/source/icon-preview/build-previews.py
"""

from pathlib import Path
from PIL import Image

REPO = Path(__file__).resolve().parents[4]   # .../ratatoskr/

OUTBASE = "icon-preview"
import sys
SRC_NAME = sys.argv[1] if len(sys.argv) > 1 else "candidate-3-bw-stripped-1024.png"
SRC  = REPO / "doc/brand/source" / SRC_NAME
OUT  = REPO / "doc/brand/source/icon-preview" / Path(SRC_NAME).stem.replace("-1024", "")

ICON_SIZES = [16, 32, 48, 64, 128, 256]

# Crop boxes are (left, top, right, bottom) in the 1024x1024 source.
# - "full"   = no crop, scale entire image
# - "medium" = 80% center crop (drops some surrounding leaves)
# - "tight"  = 60% center crop (focuses tightly on the squirrel + branch)
def center_crop(img, fraction):
    w, h = img.size
    side = int(min(w, h) * fraction)
    left = (w - side) // 2
    top  = (h - side) // 2
    return img.crop((left, top, left + side, top + side))

def build_set(name, source_image):
    out_dir = OUT / name
    out_dir.mkdir(parents=True, exist_ok=True)

    # Generate each PNG size
    pngs = []
    for size in ICON_SIZES:
        resized = source_image.resize((size, size), Image.LANCZOS)
        out_path = out_dir / f"ratatoskr{size}.png"
        resized.save(out_path, "PNG")
        pngs.append(resized)
        print(f"  {name}/ratatoskr{size}.png")

    # Multi-res .ico (Pillow writes all listed sizes into a single .ico)
    ico_path = out_dir / "ratatoskr.ico"
    pngs[0].save(
        ico_path,
        format="ICO",
        sizes=[(s, s) for s in ICON_SIZES],
        append_images=pngs[1:],
    )
    print(f"  {name}/ratatoskr.ico (multi-res)")

    # .icns -- Pillow supports it natively
    icns_path = out_dir / "ratatoskr.icns"
    try:
        pngs[-1].save(icns_path, format="ICNS")
        print(f"  {name}/ratatoskr.icns")
    except Exception as e:
        print(f"  ! could not write .icns: {e}")

    # Single-file masters at original 256 + 1024
    source_image.resize((256, 256), Image.LANCZOS).save(
        out_dir / "ratatoskr_master_256.png", "PNG"
    )
    source_image.save(out_dir / "ratatoskr_master_1024.png", "PNG")

print(f"Source: {SRC}")
src = Image.open(SRC).convert("RGBA")
print(f"Source size: {src.size}")
print()

print("Building [full] -- no crop, full image scaled")
build_set("full", src)
print()

print("Building [medium] -- 80% center crop")
build_set("medium", center_crop(src, 0.80))
print()

print("Building [tight] -- 60% center crop (focuses on squirrel)")
build_set("tight", center_crop(src, 0.60))
print()

print("Done.")
print(f"Preview folders: {OUT}")
print("Pick one and tell Claude which to deploy to share/pixmaps/.")
