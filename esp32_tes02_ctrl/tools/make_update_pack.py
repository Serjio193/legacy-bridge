#!/usr/bin/env python3
"""
Build one-file recovery package for LB bridge.

Format (little-endian):
  magic[8]      = b"LBPACK1\\0"
  version u32   = 1
  fw_size u32
  fs_size u32
  fw_sig_len u16
  fs_sig_len u16
  reserved u32  = 0
  fw_sig bytes
  fs_sig bytes
  firmware bytes
  littlefs bytes
"""

from __future__ import annotations

import argparse
import struct
from pathlib import Path


MAGIC = b"LBPACK1\x00"
VERSION = 1
HEADER_FMT = "<8sIIIHHI"


def readb(path: Path) -> bytes:
    return path.read_bytes()


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--firmware", required=True, type=Path, help="firmware.bin")
    ap.add_argument("--firmware-sig", required=True, type=Path, help="firmware.sig")
    ap.add_argument("--littlefs", required=True, type=Path, help="littlefs.bin")
    ap.add_argument("--littlefs-sig", required=True, type=Path, help="littlefs.sig")
    ap.add_argument("--out", required=True, type=Path, help="output update.lbpack")
    args = ap.parse_args()

    fw = readb(args.firmware)
    fw_sig = readb(args.firmware_sig)
    fs = readb(args.littlefs)
    fs_sig = readb(args.littlefs_sig)

    if len(fw_sig) == 0 or len(fs_sig) == 0:
        raise SystemExit("signature file is empty")
    if len(fw_sig) > 65535 or len(fs_sig) > 65535:
        raise SystemExit("signature too large (max 65535 bytes)")
    if len(fw) == 0 or len(fs) == 0:
        raise SystemExit("binary section is empty")
    if len(fw) > 0xFFFFFFFF or len(fs) > 0xFFFFFFFF:
        raise SystemExit("binary section too large")

    header = struct.pack(
        HEADER_FMT,
        MAGIC,
        VERSION,
        len(fw),
        len(fs),
        len(fw_sig),
        len(fs_sig),
        0,
    )

    args.out.parent.mkdir(parents=True, exist_ok=True)
    with args.out.open("wb") as f:
        f.write(header)
        f.write(fw_sig)
        f.write(fs_sig)
        f.write(fw)
        f.write(fs)

    print(f"[ok] package: {args.out}")
    print(f"[ok] firmware: {args.firmware} ({len(fw)} bytes)")
    print(f"[ok] littlefs: {args.littlefs} ({len(fs)} bytes)")
    print(f"[ok] total: {args.out.stat().st_size} bytes")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
