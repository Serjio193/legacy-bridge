# Legacy Bridge Workspace

## 🌐 Language / Язык / Мова

<details open>
<summary><b>Русский</b></summary>

## 🔥 Legacy Bridge (LB)

Интеллектуальный мост между паяльной станцией и дымоуловителем.  
Автоматизация без замены оборудования.

### 🚀 Что это такое

Если вытяжка не включается автоматически, а инструменты работают как отдельные устройства, Legacy Bridge решает эту проблему.

Это внешний модуль на ESP32, который связывает станцию и дымоуловитель в единую систему автоматизации.

### 📖 История

Проект появился из практики работы с оборудованием Aixun: ES02 не давал нормальной интеграции с T420D (без Wi-Fi модуля) и H312.  
Идея Legacy Bridge - добавить совместимость и автоматизацию без замены станции.

### ⚙️ Функциональность

- Автоматическое управление дымоуловителем
- Реакция на состояние паяльника через SENSE
- Реакция на фен по температуре (Wi-Fi / BLE)
- Управление мощностью вытяжки
- Управление белой/зелёной подсветкой
- Задержки включения/выключения
- Веб-интерфейс + Recovery + OTA

### 🧠 Принцип работы

- ESP32 читает состояние ручек и фена
- Логика рассчитывает, когда включать/выключать вытяжку
- Команды отправляются на ES02 автоматически

### 🧩 Аппаратная часть

- ESP32-C3 Pro Mini
- R1, R2 - 100 кОм
- C1, C2 - 100 нФ

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="Общий вид системы" width="220"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="ESP32-C3 модуль" width="180"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Провода и конденсаторы на ESP32" width="220"></a>

### 🔌 Схема подключения

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="SENSE точка 1" width="180"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="SENSE точка 2" width="180"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="Точка 5V" width="180"></a>

### 🌐 Веб-интерфейс

UI сделан для редкой настройки: подключил, выставил логику, дальше работает автономно.

Что это дает на рабочем месте:
- Меньше лишнего шума
- Вытяжка включается только при реальной работе
- Меньше расход электроэнергии
- Не нужно помнить включить/выключить вручную

### 📡 Первый запуск

Онлайн USB flasher: https://serjio193.github.io/legacy-bridge/  
Demo UI: https://serjio193.github.io/legacy-bridge/demo/  
Требуется Chromium браузер (Chrome/Edge, WebSerial).

Данные по умолчанию:
- SSID: `LB-SETUP-XXXXX`
- Пароль: `lbxxxxx!2026`
- Логин: `admin`
- Recovery AP: `LB_RECOVERY`

`XXXXX` - последние 5 символов MAC (HEX uppercase), `xxxxx` - те же символы lowercase.

### 🔐 Безопасность

- Обновления подписаны приватным ключом
- Устройство ставит только валидно подписанные пакеты
- Bootloader и Recovery защищены от записи по Wi-Fi

### 📦 Обновления

- OTA по Wi-Fi
- Пакет: `update.lbpack`
- Источник: GitHub Releases

### 🧪 Поддерживаемое оборудование

- Aixun T420D
- Aixun H312
- Aixun ES02
- JBC-совместимые станции (частично)

### 🚧 План развития

- Поддержка дополнительных slave-устройств на ESP32
- Интеграция дополнительного оборудования (Aixun/JCID и др.)

### 👨‍🔧 Автор

Serjio193, embedded developer.  
Проект основан на реальной практике ремонта и использования оборудования.

### 🎯 Цель проекта

Сделать простой и надежный инструмент, который автоматизирует рабочий процесс без замены существующей техники.

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

If your extractor does not start automatically and your tools work as isolated devices, Legacy Bridge connects them into a single automation system.

Legacy Bridge is an external ESP32 module that controls the fume extractor using soldering iron dock state (SENSE) and hot-air activity.

### 📖 Project story

The project was created from real Aixun workflow issues: ES02 had no practical integration with T420D (without Wi-Fi module) and H312.  
Legacy Bridge was built to add compatibility and automation without replacing the station.

### ⚙️ Features

- Automatic extractor start/stop
- SENSE-based iron state logic
- Hot-air trigger by temperature (Wi-Fi / BLE)
- Extractor speed control
- White/green light behavior control
- Start/stop delays
- Web UI + Recovery + OTA

### 🧠 How it works

- ESP32 reads handle and hot-air state
- Logic decides when extractor must run
- Commands are sent to ES02 automatically

### 🧩 Hardware

- ESP32-C3 Pro Mini
- R1, R2 - 100 kOhm
- C1, C2 - 100 nF

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="System overview" width="220"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="ESP32-C3 module" width="180"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Wires and capacitors on ESP32" width="220"></a>

### 🔌 Wiring

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="SENSE point 1" width="180"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="SENSE point 2" width="180"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="5V point" width="180"></a>

### 🌐 Web interface

The UI is designed for setup-first workflow: configure once, then run daily without manual control.

Practical effect:
- Less unnecessary noise
- Extractor works only when needed
- Lower power waste
- No manual on/off routine

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
- Device installs only valid signed updates
- Bootloader and Recovery are protected from Wi-Fi rewriting

### 📦 Updates

- OTA over Wi-Fi
- Package: `update.lbpack`
- Source: GitHub Releases

### 🧪 Supported equipment

- Aixun T420D
- Aixun H312
- Aixun ES02
- JBC-compatible stations (partial)

### 🚧 Roadmap

- Additional ESP32 slave devices
- Integration with more equipment (Aixun/JCID/etc.)

### 👨‍🔧 Author

Serjio193, embedded developer.  
Built from real repair bench and daily usage requirements.

### 🎯 Goal

Create a simple and reliable tool that automates workflow without replacing existing hardware.

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

### 🚀 Що це таке

Якщо витяжка не вмикається автоматично, а інструменти працюють окремо, Legacy Bridge об'єднує їх в одну систему автоматизації.

Це зовнішній модуль на ESP32, що керує димовловлювачем за станом паяльника (SENSE) і фена.

### 📖 Історія проєкту

Проєкт виник з реальних задач під час роботи з обладнанням Aixun: ES02 не мав зручної інтеграції з T420D (без Wi-Fi модуля) та H312.  
Legacy Bridge створений, щоб додати сумісність і автоматизацію без заміни станції.

### ⚙️ Можливості

- Автоматичне вмикання/вимикання димовловлювача
- Логіка по SENSE для ручок
- Тригер фена по температурі (Wi-Fi / BLE)
- Керування потужністю витяжки
- Керування білою/зеленою підсвіткою
- Затримки старту/зупинки
- Web UI + Recovery + OTA

### 🧠 Принцип роботи

- ESP32 читає стан ручок і фена
- Логіка визначає, коли запускати витяжку
- Команди автоматично надсилаються на ES02

### 🧩 Апаратна частина

- ESP32-C3 Pro Mini
- R1, R2 - 100 кОм
- C1, C2 - 100 нФ

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="Загальний вигляд системи" width="220"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="Модуль ESP32-C3" width="180"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Дроти і конденсатори на ESP32" width="220"></a>

### 🔌 Схема підключення

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="SENSE точка 1" width="180"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="SENSE точка 2" width="180"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="Точка 5V" width="180"></a>

### 🌐 Веб-інтерфейс

UI орієнтований на сценарій "налаштував і працює": один раз виставив логіку, далі система працює автономно.

Що це дає:
- Менше зайвого шуму
- Витяжка працює лише за потреби
- Менше витрат електроенергії
- Не потрібно вручну вмикати/вимикати

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
- Пристрій встановлює лише валідно підписані оновлення
- Bootloader та Recovery захищені від перезапису по Wi-Fi

### 📦 Оновлення

- OTA через Wi-Fi
- Пакет: `update.lbpack`
- Джерело: GitHub Releases

### 🧪 Підтримуване обладнання

- Aixun T420D
- Aixun H312
- Aixun ES02
- JBC-сумісні станції (частково)

### 🚧 План розвитку

- Підтримка додаткових ESP32 slave-пристроїв
- Інтеграція з додатковим обладнанням (Aixun/JCID тощо)

### 👨‍🔧 Автор

Serjio193, embedded developer.  
Проєкт заснований на реальних задачах ремонту та щоденної роботи.

### 🎯 Мета проєкту

Створити простий і надійний інструмент для автоматизації робочого процесу без заміни існуючого обладнання.

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
