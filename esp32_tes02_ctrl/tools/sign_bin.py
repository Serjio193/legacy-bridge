#!/usr/bin/env python3
"""
Sign firmware/LittleFS images for LB recovery secure update.

Requires: pycryptodome (Crypto)
"""

import argparse
import hashlib
from pathlib import Path

from Crypto.PublicKey import RSA
from Crypto.Signature import pkcs1_15
from Crypto.Hash import SHA256


def read_bytes(path: Path) -> bytes:
    return path.read_bytes()


def write_bytes(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def gen_keys(priv_path: Path, pub_path: Path, bits: int) -> None:
    key = RSA.generate(bits)
    write_bytes(priv_path, key.export_key("PEM"))
    write_bytes(pub_path, key.publickey().export_key("PEM"))
    print(f"[ok] private: {priv_path}")
    print(f"[ok] public : {pub_path}")


def sign_file(bin_path: Path, priv_path: Path, sig_path: Path) -> None:
    data = read_bytes(bin_path)
    priv = RSA.import_key(read_bytes(priv_path))
    digest = SHA256.new(data)
    sig = pkcs1_15.new(priv).sign(digest)
    write_bytes(sig_path, sig)
    sha_hex = hashlib.sha256(data).hexdigest()
    sha_path = sig_path.with_suffix(sig_path.suffix + ".sha256")
    sha_path.write_text(f"{sha_hex}  {bin_path.name}\n", encoding="utf-8")
    print(f"[ok] signed: {bin_path}")
    print(f"[ok] sig   : {sig_path} ({len(sig)} bytes)")
    print(f"[ok] sha256: {sha_hex}")
    print(f"[ok] hash  : {sha_path}")


def verify_file(bin_path: Path, pub_path: Path, sig_path: Path) -> None:
    data = read_bytes(bin_path)
    pub = RSA.import_key(read_bytes(pub_path))
    sig = read_bytes(sig_path)
    digest = SHA256.new(data)
    pkcs1_15.new(pub).verify(digest, sig)
    print("[ok] signature valid")


def main() -> int:
    ap = argparse.ArgumentParser()
    sub = ap.add_subparsers(dest="cmd", required=True)

    g = sub.add_parser("genkey", help="Generate RSA keypair")
    g.add_argument("--private", required=True, type=Path)
    g.add_argument("--public", required=True, type=Path)
    g.add_argument("--bits", type=int, default=2048)

    s = sub.add_parser("sign", help="Sign .bin and create .sig")
    s.add_argument("--bin", required=True, type=Path)
    s.add_argument("--private", required=True, type=Path)
    s.add_argument("--sig", required=True, type=Path)

    v = sub.add_parser("verify", help="Verify .bin against .sig")
    v.add_argument("--bin", required=True, type=Path)
    v.add_argument("--public", required=True, type=Path)
    v.add_argument("--sig", required=True, type=Path)

    args = ap.parse_args()

    if args.cmd == "genkey":
        gen_keys(args.private, args.public, args.bits)
        return 0
    if args.cmd == "sign":
        sign_file(args.bin, args.private, args.sig)
        return 0
    if args.cmd == "verify":
        verify_file(args.bin, args.public, args.sig)
        return 0
    return 1


if __name__ == "__main__":
    raise SystemExit(main())

