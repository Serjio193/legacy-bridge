# Legacy Bridge Workspace

## First start (new/blank ESP32-C3)

- For first-time flashing of a blank device over USB, use the online flasher page:
  - `https://serjio193.github.io/legacy-bridge/`
- Recommended browser: Chrome/Edge (WebSerial required).

## Structure

- `esp32_tes02_ctrl/` - PlatformIO firmware project (ESP32-C3)
- `ui/` - frontend source mirror used for sync into firmware LittleFS
- `security_private.pem` / `security_public.pem` - signing keys (local only, do not publish private key)

## Firmware release pipeline

- GitHub Actions workflow: `.github/workflows/release_firmware.yml`
- Builds signed `update.lbpack` and uploads it to GitHub Releases
- Main firmware version is numeric: `v1 ... v99999`
- Device auto-update UI reads GitHub Releases and installs `update.lbpack`

## Online USB flasher page (bare ESP32-C3)

- Source: `flasher/index.html`
- Deploy workflow: `.github/workflows/pages_flasher.yml`
- Expected URL after Pages deploy:
  - `https://serjio193.github.io/legacy-bridge/`
- The page pulls versions from GitHub Releases and flashes full image set:
  - `bootloader.bin` (`0x0`)
  - `partitions.bin` (`0x8000`)
  - `firmware.bin` (`0x10000`)
  - `littlefs.bin` (`0x238000`)
  - `recovery.bin` (`0x320000`)
- Browser requirements:
  - Chromium browser (Chrome/Edge)
  - HTTPS context (GitHub Pages)

## Local canonical packaging command

```powershell
powershell -ExecutionPolicy Bypass -File esp32_tes02_ctrl\scripts\make_update_pack.ps1 -Env esp32c3
```

This command syncs `ui -> esp32_tes02_ctrl/data`, rebuilds firmware + LittleFS, signs binaries, and creates package.

## Detailed docs

See full technical guide:

- `esp32_tes02_ctrl/README.md`
