# Signing Keys Location

Canonical signing keys are stored in workspace root:

- `../security_private.pem`
- `../security_public.pem`

Do not generate or store new signing keys inside `esp32_tes02_ctrl/`.
Recovery accepts only firmware signed by the matching private key.

## Signing current build

Use:

```powershell
.\scripts\sign_current_build.ps1
```

This signs and verifies:

- `.pio/build/esp32c3/littlefs.bin` -> `littlefs.sig`
- `.pio/build/esp32c3/firmware.bin` -> `firmware.sig`
