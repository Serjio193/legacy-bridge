Import("env")

import os
import re


def normalize_fw_version(raw: str) -> str:
    s = str(raw or "").strip()
    if not s:
        return ""
    m = re.match(r"^v?(\d{1,5})$", s, flags=re.IGNORECASE)
    if not m:
        return ""
    n = int(m.group(1))
    if n < 1 or n > 99999:
        return ""
    return f"v{n}"


fw_ver = normalize_fw_version(os.getenv("LB_FW_VERSION", ""))
if fw_ver:
    # String define for: #define LB_FW_VERSION "v123"
    env.Append(CPPDEFINES=[("LB_FW_VERSION", '\\"{}\\"'.format(fw_ver))])
    print("[lb] LB_FW_VERSION override:", fw_ver)
