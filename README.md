# Legacy Bridge Workspace

## 🌐 Language / Язык / Мова

<details open>
<summary><b>Русский</b></summary>

## 🔥 Legacy Bridge (LB)

Интеллектуальный мост между паяльной станцией и дымоуловителем.  
Автоматизация без замены оборудования.

### 🚀 Что это

Legacy Bridge - внешний модуль на ESP32, который автоматически управляет вытяжкой по состоянию паяльника (SENSE) и фена (Wi-Fi/BLE).

### ⚙️ Функции

- Автовключение и автоотключение дымоуловителя
- Логика по ручкам T420D + температурный триггер по фену H312
- Управление мощностью вытяжки и подсветкой
- Задержки старта/остановки, автозавершение по времени
- Web UI + Recovery + OTA

### 📡 Первый запуск

Онлайн USB flasher: https://serjio193.github.io/legacy-bridge/  
Demo UI: https://serjio193.github.io/legacy-bridge/demo/  
Требуется Chromium браузер (Chrome/Edge, WebSerial).

Данные по умолчанию:
- SSID: `LB-SETUP-XXXXX`
- Пароль: `lbxxxxx!2026`
- Логин: `admin`
- Recovery AP: `LB_RECOVERY`

Где `XXXXX` - последние 5 символов MAC (HEX uppercase), `xxxxx` - те же символы lowercase.

### 🔐 Безопасность

- Пакеты подписаны приватным ключом
- Устройство ставит только валидно подписанные обновления
- Bootloader и Recovery не перезаписываются по Wi-Fi

### ❤️ Поддержка

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

</details>

<details>
<summary><b>English</b></summary>

## 🔥 Legacy Bridge (LB)

An intelligent bridge between a soldering station and a fume extractor.  
Automation without replacing your hardware.

### 🚀 What it is

Legacy Bridge is an external ESP32 module that controls the extractor automatically using soldering iron dock state (SENSE) and hot-air activity (Wi-Fi/BLE).

### ⚙️ Features

- Automatic extractor start/stop
- Logic from T420D handles + H312 temperature trigger
- Extractor speed and light control
- Start/stop delays, timed auto-stop
- Web UI + Recovery + OTA

### 📡 First start

Online USB flasher: https://serjio193.github.io/legacy-bridge/  
Demo UI: https://serjio193.github.io/legacy-bridge/demo/  
Use a Chromium browser (Chrome/Edge, WebSerial support).

Default access:
- SSID: `LB-SETUP-XXXXX`
- Password: `lbxxxxx!2026`
- Web login: `admin`
- Recovery AP: `LB_RECOVERY`

`XXXXX` is the last 5 MAC symbols (HEX uppercase), `xxxxx` is lowercase.

### 🔐 Security

- Firmware packages are signed with a private key
- Device accepts only valid signed updates
- Bootloader and Recovery are protected from Wi-Fi rewriting

### ❤️ Support

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

</details>

<details>
<summary><b>Українська</b></summary>

## 🔥 Legacy Bridge (LB)

Інтелектуальний міст між паяльною станцією та димовловлювачем.  
Автоматизація без заміни обладнання.

### 🚀 Що це

Legacy Bridge - зовнішній модуль на ESP32, який автоматично керує витяжкою за станом паяльника (SENSE) і фена (Wi-Fi/BLE).

### ⚙️ Можливості

- Автоматичне вмикання/вимикання димовловлювача
- Логіка по ручках T420D + температурний тригер фена H312
- Керування потужністю витяжки та підсвіткою
- Затримки старту/зупинки, автостоп за часом
- Web UI + Recovery + OTA

### 📡 Перший запуск

Онлайн USB flasher: https://serjio193.github.io/legacy-bridge/  
Demo UI: https://serjio193.github.io/legacy-bridge/demo/  
Потрібен Chromium браузер (Chrome/Edge, WebSerial).

Дані за замовчуванням:
- SSID: `LB-SETUP-XXXXX`
- Пароль: `lbxxxxx!2026`
- Логін: `admin`
- Recovery AP: `LB_RECOVERY`

`XXXXX` - останні 5 символів MAC (HEX uppercase), `xxxxx` - ті самі символи lowercase.

### 🔐 Безпека

- Пакети прошивки підписані приватним ключем
- Пристрій приймає лише валідно підписані оновлення
- Bootloader та Recovery захищені від перезапису по Wi-Fi

### ❤️ Підтримка

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

</details>

<details id="usdt-trc20">
<summary><b>USDT TRC20</b></summary>

Wallet: `TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

![USDT TRC20 QR](assets/support/usdt-trc20-qr.jpg)

</details>

## 🧷 Пример установки в T420D

<a href="assets/photos/t420d-case-placement.jpg" target="_blank"><img src="assets/photos/t420d-case-placement.jpg" alt="Пример размещения модуля в корпусе T420D" width="420"></a>

## 🧱 Technical Docs / Техдок / Техдок

<details open>
<summary><b>Русский</b></summary>

### Structure

- `esp32_tes02_ctrl/` - проект прошивки PlatformIO (ESP32-C3)
- `ui/` - исходники frontend, синхронизируются в LittleFS прошивки
- `security_private.pem` / `security_public.pem` - ключи подписи (локально, приватный ключ не публиковать)

### Firmware release pipeline

- Workflow GitHub Actions: `.github/workflows/release_firmware.yml`
- Сборка подписанного `update.lbpack` и публикация в GitHub Releases
- Артефакты релиза подписываются приватным ключом, подпись проверяется на устройстве перед установкой
- Версия основной прошивки числовая: `v1 ... v99999`
- UI автообновления читает GitHub Releases и ставит `update.lbpack`

### Online USB flasher page (bare ESP32-C3)

- Исходник: `flasher/index.html`
- Workflow деплоя: `.github/workflows/pages_flasher.yml`
- Ожидаемый URL после деплоя Pages: `https://serjio193.github.io/legacy-bridge/`
- Страница читает версии из GitHub Releases и прошивает полный набор:
  - `bootloader.bin` (`0x0`)
  - `partitions.bin` (`0x8000`)
  - `firmware.bin` (`0x10000`)
  - `littlefs.bin` (`0x238000`)
  - `recovery.bin` (`0x300000`)
- Требования к браузеру:
  - Chromium (Chrome/Edge)
  - HTTPS-контекст (GitHub Pages)

### Local canonical packaging command

```powershell
powershell -ExecutionPolicy Bypass -File esp32_tes02_ctrl\scripts\make_update_pack.ps1 -Env esp32c3
```

Команда синхронизирует `ui -> esp32_tes02_ctrl/data`, пересобирает firmware + LittleFS, подписывает бинарники и создает пакет.

### Detailed docs

- `esp32_tes02_ctrl/README.md`

</details>

<details>
<summary><b>English</b></summary>

### Structure

- `esp32_tes02_ctrl/` - PlatformIO firmware project (ESP32-C3)
- `ui/` - frontend source mirror used for sync into firmware LittleFS
- `security_private.pem` / `security_public.pem` - signing keys (local only, do not publish private key)

### Firmware release pipeline

- GitHub Actions workflow: `.github/workflows/release_firmware.yml`
- Builds signed `update.lbpack` and uploads it to GitHub Releases
- Release artifacts are signed with a private key, and signature is verified on-device before install
- Main firmware version is numeric: `v1 ... v99999`
- Device auto-update UI reads GitHub Releases and installs `update.lbpack`

### Online USB flasher page (bare ESP32-C3)

- Source: `flasher/index.html`
- Deploy workflow: `.github/workflows/pages_flasher.yml`
- Expected URL after Pages deploy: `https://serjio193.github.io/legacy-bridge/`
- The page pulls versions from GitHub Releases and flashes full image set:
  - `bootloader.bin` (`0x0`)
  - `partitions.bin` (`0x8000`)
  - `firmware.bin` (`0x10000`)
  - `littlefs.bin` (`0x238000`)
  - `recovery.bin` (`0x300000`)
- Browser requirements:
  - Chromium browser (Chrome/Edge)
  - HTTPS context (GitHub Pages)

### Local canonical packaging command

```powershell
powershell -ExecutionPolicy Bypass -File esp32_tes02_ctrl\scripts\make_update_pack.ps1 -Env esp32c3
```

This command syncs `ui -> esp32_tes02_ctrl/data`, rebuilds firmware + LittleFS, signs binaries, and creates package.

### Detailed docs

- `esp32_tes02_ctrl/README.md`

</details>

<details>
<summary><b>Українська</b></summary>

### Structure

- `esp32_tes02_ctrl/` - проєкт прошивки PlatformIO (ESP32-C3)
- `ui/` - вихідний frontend, що синхронізується в LittleFS прошивки
- `security_private.pem` / `security_public.pem` - ключі підпису (локально, приватний ключ не публікувати)

### Firmware release pipeline

- Workflow GitHub Actions: `.github/workflows/release_firmware.yml`
- Збірка підписаного `update.lbpack` і публікація в GitHub Releases
- Артефакти релізу підписуються приватним ключем, підпис перевіряється на пристрої перед встановленням
- Версія основної прошивки числова: `v1 ... v99999`
- UI автооновлення читає GitHub Releases і встановлює `update.lbpack`

### Online USB flasher page (bare ESP32-C3)

- Джерело: `flasher/index.html`
- Workflow деплою: `.github/workflows/pages_flasher.yml`
- Очікуваний URL після деплою Pages: `https://serjio193.github.io/legacy-bridge/`
- Сторінка отримує версії з GitHub Releases та прошиває повний набір:
  - `bootloader.bin` (`0x0`)
  - `partitions.bin` (`0x8000`)
  - `firmware.bin` (`0x10000`)
  - `littlefs.bin` (`0x238000`)
  - `recovery.bin` (`0x300000`)
- Вимоги до браузера:
  - Chromium (Chrome/Edge)
  - HTTPS-контекст (GitHub Pages)

### Local canonical packaging command

```powershell
powershell -ExecutionPolicy Bypass -File esp32_tes02_ctrl\scripts\make_update_pack.ps1 -Env esp32c3
```

Команда синхронізує `ui -> esp32_tes02_ctrl/data`, перебудовує firmware + LittleFS, підписує бінарники та створює пакет.

### Detailed docs

- `esp32_tes02_ctrl/README.md`

</details>
