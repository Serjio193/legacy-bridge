# 🧱 Technical Documentation

## 📁 Project Structure

- `esp32_tes02_ctrl/` — PlatformIO firmware project (ESP32-C3)
- `ui/` — frontend source (synced into LittleFS)
- `security_private.pem` / `security_public.pem` — signing keys
  - private key must never be published

---

## 🔐 Firmware Release Pipeline

- Workflow: `.github/workflows/release_firmware.yml`
- Builds signed `update.lbpack`
- Uploads artifacts to GitHub Releases

Firmware:

- version format: `v1 ... v99999`
- update package is signed with private key
- signature is verified on-device before install

Device:

- reads GitHub Releases
- installs `update.lbpack` via UI

---

## 🌐 USB Flasher (Web)

- Source: `flasher/index.html`
- Deploy: `.github/workflows/pages_flasher.yml`
- URL: https://serjio193.github.io/legacy-bridge/

### Flash image layout

- `bootloader.bin` — `0x0`
- `partitions.bin` — `0x8000`
- `firmware.bin` — `0x10000`
- `littlefs.bin` — `0x238000`
- `recovery.bin` — `0x300000`

### Requirements

- Chromium browser (Chrome / Edge)
- HTTPS context (GitHub Pages)

---

## 📦 Local Build & Packaging

```powershell
powershell -ExecutionPolicy Bypass -File esp32_tes02_ctrl\scripts\make_update_pack.ps1 -Env esp32c3
```
