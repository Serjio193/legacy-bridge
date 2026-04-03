# Legacy Bridge (ESP32-C3) - Project Guide

This file is the single source of truth for build/flash/update workflow.

## 1. What this project is

- Main firmware: `src/main.cpp` (normal device mode)
- Recovery firmware: `src/recovery_main.cpp` (fallback/update mode)
- Partition layout: `partitions_4mb_ota_fs_recovery.csv`
- UI files served from LittleFS partition: `data/*`
- Source UI mirror (editable frontend workspace): `../ui/*`

Important: LittleFS image is built from `data`, not from `../ui`.

## 2. Current memory layout

From `partitions_4mb_ota_fs_recovery.csv`:

- `ota_a` at `0x10000` (main app slot, single OTA slot)
- `spiffs` at `0x238000` (LittleFS payload)
- `recovery` at `0x320000` (factory/recovery app)

Recovery image is not writable over Wi-Fi by design.

## 3. Build environments

Defined in `platformio.ini`:

- `esp32c3` -> main firmware
- `esp32c3_hardened` -> hardened marker build
- `esp32c3_recovery` -> recovery firmware

Default upload port currently configured: `COM70`.

## 4. Signing keys

Signing scripts expect keys in workspace root (one level above this folder):

- `../security_private.pem`
- `../security_public.pem`

Do not commit private key to GitHub.

## 5. Canonical build/package command

Use this command for normal release packaging:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\make_update_pack.ps1 -Env esp32c3
```

What it does now (fixed workflow):

1. Syncs UI: `../ui -> data` (`scripts/sync_ui.ps1`)
2. Builds firmware (`pio run -e esp32c3`)
3. Builds LittleFS (`pio run -e esp32c3 -t buildfs`)
4. Signs `firmware.bin` and `littlefs.bin`
5. Builds signed package `update.lbpack`

Output:

- `.pio/build/esp32c3/update.lbpack`

Optional flags:

- `-SkipUiSync` -> skip UI sync step
- `-SkipBuild` -> skip `pio` build steps (not recommended for normal use)

## 6. OTA update flow (main -> recovery -> package flash)

Recommended one-file OTA uses `update.lbpack` and recovery endpoint:

- Enter recovery from main: `POST /api/fw/enter_recovery`
- Flash package in recovery: `POST /api/recovery/flash/allpack` (multipart form field `pack`)

This updates both:

- System (`ota_a`)
- LittleFS (`spiffs`)

## 7. USB flashing

### 7.1 Full flash

```powershell
powershell -ExecutionPolicy Bypass -File scripts\full_flash.ps1 -Port COM70 -Env esp32c3
```

Script now uses fresh recovery binary from `.pio/build/esp32c3_recovery/firmware.bin`
(no stale hardcoded `release\bundle...` path).

### 7.2 Recovery-only flash

```powershell
powershell -ExecutionPolicy Bypass -File scripts\flash_recovery.ps1 -Port COM70
```

Or `scripts/flash_recovery.bat` (also updated to use fresh build artifact).

## 8. Known pitfalls and how to avoid them

1. Stale package artifacts  
   Fixed by default rebuild inside `make_update_pack.ps1`.

2. UI mismatch (`ui` vs `data`)  
   Fixed by default sync step before LittleFS build.

3. Browser/service-worker cache confusion  
   UI uses version query params (`?v=...`) and no-store style patterns; for verification use hard refresh.

4. mDNS instability  
   If `*.local` fails on host machine, use direct IP from `/api/status`.

## 9. Release/GitHub update convention

- Auto-update artifact name: `update.lbpack`
- GitHub UI update logic checks releases and expects `.lbpack` asset
- Keep signatures private; publish only signed artifacts
- Main firmware versioning: numeric sequence `v1 ... v99999`

## 9.1 CI release pipeline (GitHub Actions)

Workflow file: repository root `.github/workflows/release_firmware.yml`

Behavior:

- Trigger by manual run (`workflow_dispatch`) or by tag push `v*`
- Resolves firmware version as `vN` (`1..99999`)
  - uses entered `version_number`, or
  - auto-increments latest numeric release tag
- Builds signed package and publishes GitHub Release
- Uploads `update.lbpack` (and signatures/checksums)

Required repository secrets:

- `LB_SIGN_PRIVATE_PEM` (private signing key PEM)
- `LB_SIGN_PUBLIC_PEM` (public key PEM)

This keeps source open while firmware packages remain signed by your private key.

## 10. Minimal quick checklist before shipping

1. Run `make_update_pack.ps1`
2. Verify package created and signed
3. Flash package to test device via recovery
4. Check `/api/status` and UI elements expected in this release
5. If recovery changed, USB flash recovery and re-test boot-main transition

## 11. Files to read first when resuming work

- `src/main.cpp`
- `src/recovery_main.cpp`
- `data/app.js`
- `data/index.html`
- `scripts/make_update_pack.ps1`
- `scripts/full_flash.ps1`
- `platformio.ini`
- `partitions_4mb_ota_fs_recovery.csv`
