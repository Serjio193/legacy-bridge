# Legacy Bridge Workspace

## Legacy Bridge Overview (RU / EN / UA)

### RU

🔥 **Legacy Bridge (LB)**

Legacy Bridge — это система, которая автоматически управляет дымоуловителем в зависимости от работы паяльной станции и фена.

**Быстрый обзор**

- Автоматически включает вытяжку при работе
- Работает с Aixun / JBC-совместимыми станциями
- Не требует модификации станции
- Простая установка (минимум компонентов)
- Прошивка подписывается ключом: устройство принимает только проверенные обновления

**История проекта**

Я являюсь поклонником оборудования Aixun.

После покупки вытяжки Aixun ES02 я был разочарован тем, что она не работает с моей паяльной станцией T420D, так как в ней отсутствует WiFi модуль. На тот момент я даже не знал, что существует версия с WiFi.

Также оказалось, что вытяжка не работает и с феном H312.

В итоге стало понятно, что оборудование есть, но между ним нет нормальной интеграции.

Поэтому я решил создать внешний модуль управления, который сможет связать устройства между собой.

Так появился Legacy Bridge.

**Что делает Legacy Bridge**

Система автоматически управляет дымоуловителем:

- Паяльник снят с подставки -> вытяжка включается
- Паяльник возвращён в подставку -> вытяжка выключается
- Фен (H312) нагревается -> вытяжка включается

**Пример работы**

- Паяльник снят -> вытяжка включена
- Паяльник в подставке -> вытяжка выключена
- Фен нагревается -> вытяжка включена

**Как устроена система**

Решение максимально простое:

- ESP32 (модуль управления)
- 4 провода
- 2 резистора
- 2 конденсатора

Подключение выполняется без изменения штатной электроники станции.

**Принцип работы**

- Состояние паяльника определяется через линию SENSE
- Состояние фена определяется по температуре (WiFi / BLE)
- Логика обрабатывается на ESP32
- Управление вытяжкой происходит автоматически

**Поддерживаемое оборудование**

- Aixun T420D
- Aixun H312
- Aixun ES02
- JBC-совместимые станции

**Подключение SENSE**

- `SENSE ----[100k]---- GPIO (ESP32)`
- GPIO в режиме `INPUT`
- без `pull-up` / `pull-down`

**Статус**

Проект находится в активной разработке.

Основной функционал уже реализован, продолжается улучшение стабильности и поддержка BLE.

**Поддержка**

[![Support via PayPal](https://img.shields.io/badge/PayPal-Support-0070ba?style=for-the-badge&logo=paypal)](https://paypal.me/SerhiiTarnopovych)
[![Support via Crypto](https://img.shields.io/badge/Crypto-USDT%20TRC20-26a17b?style=for-the-badge&logo=tether)](https://serjio193.github.io/legacy-bridge/support.html)


USDT TRC20 wallet: `TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

![USDT TRC20 QR](assets/support/usdt-trc20-qr.jpg)

Подробнее: https://serjio193.github.io/legacy-bridge/support.html

**Автор**

Serjio193
Embedded developer

Проект основан на реальных задачах ремонта и работы с оборудованием.

**Цель проекта**

Создать простой, надёжный и практичный инструмент, который объединяет оборудование и автоматизирует рабочий процесс.

### EN

🔥 **Legacy Bridge (LB)**

Legacy Bridge is a module that links soldering stations and a fume extractor, adding automation without replacing existing hardware.

**Project background**

I use Aixun equipment. After buying the Aixun ES02 extractor, it became clear that it did not work with my T420D (no WiFi module), and there was no proper integration with the H312 hot-air tool. I decided to modify my T420D by adding a custom control board. That is how Legacy Bridge started.

**What Legacy Bridge does**

- Extractor turns on when the iron is lifted from the stand
- Extractor turns off when the iron is placed back
- Extractor turns on by temperature when H312 is in use
- Firmware is cryptographically signed, and the device installs only verified updates

**Hardware modification**

- ESP32 (control module)
- 4 wires
- 2 resistors
- 2 capacitors

Connection is done without changing the station’s stock electronics.

**How it works**

- Iron state is detected via the SENSE stand line
- Hot-air state is detected by temperature (WiFi / BLE)
- Logic runs on ESP32
- Extractor is controlled automatically

**Supported equipment**

- Aixun T420D
- Aixun H312
- Aixun ES02
- Other JBC-compatible solutions (partial)

**SENSE connection**

- `SENSE ----[100k]---- GPIO (ESP32)`
- GPIO mode: `INPUT`
- no `pull-up` / `pull-down`

**Status**

Project is under active development. Core functionality works; BLE behavior and optimization are still being improved.

**Support**

[![Support via PayPal](https://img.shields.io/badge/PayPal-Support-0070ba?style=for-the-badge&logo=paypal)](https://paypal.me/SerhiiTarnopovych)
[![Support via Crypto](https://img.shields.io/badge/Crypto-USDT%20TRC20-26a17b?style=for-the-badge&logo=tether)](https://serjio193.github.io/legacy-bridge/support.html)


USDT TRC20 wallet: `TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

![USDT TRC20 QR](assets/support/usdt-trc20-qr.jpg)

Details: https://serjio193.github.io/legacy-bridge/support.html

**Author**

Serjio193

**Goal**

Build a simple, reliable, and practical tool that unifies hardware and automates the workflow.

### UA

🔥 **Legacy Bridge (LB)**

Legacy Bridge — це модуль, який поєднує паяльні станції та димовловлювач, додаючи автоматизацію без заміни обладнання.

**Історія створення**

Я використовую обладнання Aixun. Після покупки витяжки Aixun ES02 стало зрозуміло, що вона не працює з T420D без WiFi, а також немає нормальної інтеграції з феном H312. Тому я модифікував T420D, додавши власну плату керування. Так з’явився Legacy Bridge.

**Що робить Legacy Bridge**

- При знятті паяльника з підставки витяжка вмикається
- При поверненні паяльника в підставку витяжка вимикається
- Під час роботи H312 витяжка вмикається за температурою
- Прошивка підписується ключем: пристрій приймає лише перевірені оновлення

**Модифікація**

- ESP32 (модуль керування)
- 4 дроти
- 2 резистори
- 2 конденсатори

Підключення виконується без зміни штатної електроніки станції.

**Як це працює**

- Стан паяльника визначається через лінію SENSE (підставка)
- Стан фена визначається за температурою (WiFi / BLE)
- Логіка працює на ESP32
- Керування витяжкою виконується автоматично

**Підтримуване обладнання**

- Aixun T420D
- Aixun H312
- Aixun ES02
- Інші JBC-сумісні рішення (частково)

**Підключення SENSE**

- `SENSE ----[100k]---- GPIO (ESP32)`
- GPIO у режимі `INPUT`
- без `pull-up` / `pull-down`

**Статус**

Проєкт у активній розробці. Основний функціонал працює, триває доопрацювання BLE та оптимізація.

**Підтримка**

[![Support via PayPal](https://img.shields.io/badge/PayPal-Support-0070ba?style=for-the-badge&logo=paypal)](https://paypal.me/SerhiiTarnopovych)
[![Support via Crypto](https://img.shields.io/badge/Crypto-USDT%20TRC20-26a17b?style=for-the-badge&logo=tether)](https://serjio193.github.io/legacy-bridge/support.html)


USDT TRC20 wallet: `TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

![USDT TRC20 QR](assets/support/usdt-trc20-qr.jpg)

Детальніше: https://serjio193.github.io/legacy-bridge/support.html

**Автор**

Serjio193

**Мета**

Зробити простий, надійний і практичний інструмент для об’єднання обладнання та автоматизації робочого процесу.

## First start (new/blank ESP32-C3)

- For first-time flashing of a blank device over USB, use the online flasher page:
  - [![Open Online USB Flasher](https://img.shields.io/badge/Open%20Online%20USB%20Flasher-2ea44f?style=for-the-badge)](https://serjio193.github.io/legacy-bridge/)
- Direct link:
  - [https://serjio193.github.io/legacy-bridge/](https://serjio193.github.io/legacy-bridge/)
- Recommended browser: Chrome/Edge (WebSerial required).

## Default Wi-Fi and admin access

- `SSID`: `LB-SETUP-XXXXX`
- `Password`: `lbxxxxx!2026`
- `Web login`: `admin`
- `Recovery AP`: `LB_RECOVERY` (no password)

<details>
<summary>How to calculate password and recover access</summary>

- `XXXXX` = last 5 HEX chars of device MAC in uppercase (example: `BB9E4`).
- `xxxxx` = same suffix in lowercase (example: `bb9e4`).
- So default password is `lbxxxxx!2026` (example: `lbbb9e4!2026`).

If credentials were changed and lost:
1. Enter Recovery mode.
2. Press `Reset Main Settings`.
3. Reboot to main firmware.
4. Defaults above are restored.

</details>

## Structure

- `esp32_tes02_ctrl/` - PlatformIO firmware project (ESP32-C3)
- `ui/` - frontend source mirror used for sync into firmware LittleFS
- `security_private.pem` / `security_public.pem` - signing keys (local only, do not publish private key)

## Firmware release pipeline

- GitHub Actions workflow: `.github/workflows/release_firmware.yml`
- Builds signed `update.lbpack` and uploads it to GitHub Releases
- Release artifacts are signed with a private key, and signature is verified on-device before install
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
  - `recovery.bin` (`0x300000`)
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
