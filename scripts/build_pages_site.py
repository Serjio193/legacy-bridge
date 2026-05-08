import json
import os
import hashlib
import shutil
import urllib.error
import urllib.request

REPO = "Serjio193/legacy-bridge"
API = f"https://api.github.com/repos/{REPO}/releases?per_page=100"
PINNED_FACTORY_TAG = "v110"
REQUIRED = [
    "update.lbpack",
    "factory.bin",
]
OPTIONAL = [
    "bootloader.bin",
    "partitions.bin",
    "recovery.bin",
]
GENERATED = {}
SITE = "_site"
DEMO_DIR = os.path.join(SITE, "demo")


def copy_tree(src_dir: str, dst_dir: str) -> None:
    os.makedirs(dst_dir, exist_ok=True)
    for name in os.listdir(src_dir):
        src = os.path.join(src_dir, name)
        dst = os.path.join(dst_dir, name)
        if os.path.isdir(src):
            shutil.copytree(src, dst)
        else:
            shutil.copy2(src, dst)


def inject_demo_overlay(index_path: str) -> None:
    if not os.path.isfile(index_path):
        return
    with open(index_path, "r", encoding="utf-8") as f:
        html = f.read()
    overlay_css = (
        "<style>\n"
        "  #lbDemoOverlay{position:fixed;inset:0;pointer-events:none;z-index:2147483647;"
        "display:flex;align-items:center;justify-content:center;font-family:Arial,sans-serif;"
        "font-weight:900;font-size:clamp(72px,18vw,320px);letter-spacing:.12em;color:rgba(255,0,0,.18);"
        "text-shadow:0 0 2px rgba(255,0,0,.42),0 0 18px rgba(255,0,0,.34),0 0 42px rgba(255,0,0,.22);"
        "mix-blend-mode:lighten;user-select:none;}\n"
        "</style>"
    )
    overlay_html = '<div id="lbDemoOverlay" aria-hidden="true">DEMO</div>'
    if "</head>" in html:
        html = html.replace("</head>", overlay_css + "\n</head>", 1)
    if "</body>" in html:
        html = html.replace("</body>", overlay_html + "\n</body>", 1)
    with open(index_path, "w", encoding="utf-8") as f:
        f.write(html)


def sha256_file(path: str) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(1024 * 1024), b""):
            h.update(chunk)
    return h.hexdigest()


def main() -> None:
    if os.path.isdir(SITE):
        shutil.rmtree(SITE)
    os.makedirs(SITE, exist_ok=True)

    copy_tree("flasher", SITE)
    copy_tree("ui", DEMO_DIR)
    inject_demo_overlay(os.path.join(DEMO_DIR, "index.html"))

    headers = {
        "Accept": "application/vnd.github+json",
        "X-GitHub-Api-Version": "2022-11-28",
    }
    tok = os.getenv("GITHUB_TOKEN", "").strip()
    if tok:
        headers["Authorization"] = f"Bearer {tok}"

    req = urllib.request.Request(API, headers=headers, method="GET")
    with urllib.request.urlopen(req, timeout=60) as resp:
        rels = json.loads(resp.read().decode("utf-8"))

    tags = {str(r.get("tag_name") or "").strip() for r in rels}
    if PINNED_FACTORY_TAG and PINNED_FACTORY_TAG not in tags:
        pinned_api = f"https://api.github.com/repos/{REPO}/releases/tags/{PINNED_FACTORY_TAG}"
        req = urllib.request.Request(pinned_api, headers=headers, method="GET")
        with urllib.request.urlopen(req, timeout=60) as resp:
            rels.append(json.loads(resp.read().decode("utf-8")))

    out_index = []
    rel_root = os.path.join(SITE, "releases")
    os.makedirs(rel_root, exist_ok=True)

    for r in rels:
        if r.get("draft"):
            continue
        tag = str(r.get("tag_name") or "").strip()
        if not tag:
            continue
        assets = {
            str(a.get("name") or ""): str(a.get("browser_download_url") or "")
            for a in (r.get("assets") or [])
        }
        if not all(assets.get(n) for n in REQUIRED):
            continue

        tag_dir = os.path.join(rel_root, tag)
        os.makedirs(tag_dir, exist_ok=True)
        local_assets = {}
        for fname in REQUIRED + OPTIONAL:
            if fname in OPTIONAL and not assets.get(fname):
                continue
            url = assets[fname]
            dst = os.path.join(tag_dir, fname)
            try:
                urllib.request.urlretrieve(url, dst)
            except (urllib.error.URLError, TimeoutError) as e:
                raise RuntimeError(f"Failed to mirror {tag}/{fname}: {e}") from e
            out_name = fname
            if fname == "factory.bin":
                size = os.path.getsize(dst)
                digest = sha256_file(dst)[:12]
                # GitHub Pages gzip-compresses .bin for browser clients, which makes
                # esp-web-tools flash the compressed stream. Use an image extension so
                # Pages serves the raw bytes without Content-Encoding.
                out_name = f"factory-{size}-{digest}.bin.jpg"
                out_path = os.path.join(tag_dir, out_name)
                os.replace(dst, out_path)
            local_assets[fname] = f"./releases/{tag}/{out_name}"
        for fname, size in GENERATED.items():
            dst = os.path.join(tag_dir, fname)
            with open(dst, "wb") as f:
                f.write(b"\xff" * size)
            local_assets[fname] = f"./releases/{tag}/{fname}"

        out_index.append(
            {
                "tag_name": tag,
                "prerelease": bool(r.get("prerelease")),
                "pinned_factory": tag == PINNED_FACTORY_TAG,
                "published_at": r.get("published_at"),
                "assets": local_assets,
            }
        )

    def ver_key(item):
        t = str(item.get("tag_name", ""))
        if t.lower().startswith("v") and t[1:].isdigit():
            return int(t[1:])
        return -1

    out_index.sort(key=ver_key, reverse=True)
    with open(os.path.join(rel_root, "index.json"), "w", encoding="utf-8") as f:
        json.dump(out_index, f, ensure_ascii=False, indent=2)


if __name__ == "__main__":
    main()
