# Legacy Bridge Workspace

## 🌐 Language / Язык / Мова

Открой нужный язык в блоках ниже.

<details open>
<summary><b>English</b></summary>

## 🔥 Legacy Bridge (LB)

Intelligent bridge between a soldering station and a fume extractor.  
Automation without replacing equipment.

---

### 🚀 What it is

Is your fume extractor not turning on automatically?

Legacy Bridge solves this problem without replacing equipment.

It is an embedded ESP32-based modification for soldering stations (T420D and similar) that adds automatic extractor control based on soldering iron and hot air operation.

---

### 📖 Project story

I use Aixun equipment in daily work.

After buying the Aixun ES02 fume extractor, I found that it did not work with my T420D soldering station because the station had no Wi-Fi module. At that time, I did not know that Wi-Fi versions of this station existed. Like many users, I thought T420D came in only one variant.

It also turned out that the extractor did not work with the H312 hot air tool.

📌 Result: the hardware exists, but there is no proper integration between devices.

First, ES02 Bluetooth control was researched. After command analysis, it became clear that the extractor can be controlled directly.

Then the station USB interface was researched. It was assumed USB could provide runtime status, but in practice USB is used only for flashing and does not provide operating state telemetry.

H312 was also researched via all available interfaces: USB, Wi-Fi, and Bluetooth. Connection worked via all of them, but this project uses wireless integration only as the most practical approach.

Replacing equipment was not the right answer. The problem was not hardware quality, but missing integration logic.

As a result, the most reliable solution was to modify the station itself and add the missing logic.

It is also worth noting that Aixun offers a separate device, BS08 AI Voice Center Control, for voice-based equipment control.

Despite the declared device unification, this remains manual control in practice: the user still has to issue commands (by voice or via UI).

📌 BS08 does not track the actual station working state and cannot automatically react to the real workflow.

So the automation problem remains unresolved; only the control method changes.

This is how Legacy Bridge was created.

### ⚙️ Features

- Automatic fume extractor control
- Reaction to soldering iron state (SENSE)
- Configurable hot air reaction (temperature-based)
- Flexible extractor power control algorithm
- Last set speed memory
- Backlight brightness control
- Flexible backlight behavior based on sensor states
- Automatic time-based stop
- Web interface for setup and control

### 🧠 How it works

- SENSE signal is used to detect iron holder state
- Inputs are processed through RC filters (100k + 100nF)
- ESP32 analyzes input states and external data
- Hot air temperature is used as an additional extractor start condition
- Temperature threshold is configured via web interface
- Extractor control is performed via BLE commands

### 🧩 Hardware

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="System overview" width="300"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="ESP32-C3 module" width="300"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Wires and capacitors on ESP32" width="300"></a>
  
*ESP32 connection (GPIO1, GPIO3, power)*

### 🔌 Wiring

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="SENSE point 1" width="75"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="SENSE point 2" width="75"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="5V point" width="75"></a>
<a href="assets/photos/t420d-case-placement.jpg" target="_blank"><img src="assets/photos/t420d-case-placement.jpg" alt="Example module placement inside T420D case" width="300"></a>
  
*Photo 1: Signal 1 connection*  
*Photo 2: Signal 2 and GND connection*  
*Photo 3: 5V power source point*  
*Photo 4: example module placement inside T420D*

### 🔌 USB connection

ESP32 is connected via USB only for first-time flashing.

Then the system runs as:

- Wi-Fi OTA updates
- autonomous operation without USB

### 🌐 Web interface

The interface is used for logic setup and system monitoring.

---

### Main capabilities

#### 🎛 Control
- extractor power setup  
- backlight brightness setup  

#### 🧠 Logic
- start/stop delays  
- activation conditions (iron / hot air)  
- temperature threshold setup  

#### 📡 Connectivity
- Wi-Fi setup  
- device discovery and connection  

#### 🛠 System
- logs  
- reboot  
- recovery  
- factory reset  

### 🚀 Live Demo

Demo runs in browser and shows UI in emulation mode:

👉 https://serjio193.github.io/legacy-bridge/demo/

---

### 📡 First start

Online USB flasher:  
👉 https://serjio193.github.io/legacy-bridge/

Requires Chromium browser (Chrome / Edge, WebSerial).

### Default credentials

- SSID: `LB-SETUP-XXXXX`
- Password: `lbxxxxx!2026`
- Login: `admin`
- Recovery AP: `LB_RECOVERY`

### Password generation

- `XXXXX` — last 5 MAC symbols (HEX, uppercase)  
- `xxxxx` — same symbols in lowercase  

---

### 📡 Network

After setup:

- Access Point is automatically disabled  
- device works in main network  

---

### 🌡 Hot air integration

Connection:

- Wi-Fi  
- Bluetooth (BLE)  

Extractor behavior depends on configured temperature threshold.

---

### 🔐 Security

- Firmware is signed with a private key  
- Device accepts signed updates only  
- Boot and Recovery are protected from Wi-Fi writes  

---

### 📦 Updates

- OTA via Wi-Fi
- Package: `update.lbpack`
- Source: GitHub Releases

### 🧪 Supported equipment

- Aixun T420D
- Aixun H312
- Aixun ES02
- JBC-compatible stations (partial)

### 🚧 Roadmap

- support for ESP32 slave devices  
- integration of additional equipment (Aixun, JCID, etc.)  

### 👨‍🔧 Author

**Serjio193**  
Embedded developer  

The project is based on practical repair workflow and daily equipment use.

### 🎯 Goal

Create a simple and reliable tool that automates workflow and removes unnecessary manual actions.

### ❤️ Support

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20-en">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

<details id="usdt-trc20-en">
<summary>💰 Show USDT (TRC20) wallet</summary>

`TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

<br>
<img src="assets/support/usdt-trc20-qr.jpg" alt="USDT TRC20 QR" width="280">

</details>

</details>

<details>
<summary><b>Українська</b></summary>

## 🔥 Legacy Bridge (LB)

Інтелектуальний міст між паяльною станцією та димовловлювачем.  
Автоматизація без заміни обладнання.

---

### 🚀 Що це таке

Димовловлювач не вмикається автоматично?

Legacy Bridge вирішує цю проблему без заміни обладнання.

Це вбудована модифікація на базі ESP32 для паяльних станцій (T420D та аналогічних), яка додає автоматичне керування димовловлювачем на основі стану паяльника та фена.

---

### 📖 Історія створення

Я використовую обладнання Aixun у роботі.

Після покупки димовловлювача Aixun ES02 з'ясувалося, що він не працює з моєю паяльною станцією T420D, оскільки в ній відсутній Wi-Fi модуль. На той момент я не знав, що існують версії цієї станції з Wi-Fi — як і багато хто, я був упевнений, що T420D буває лише в одному варіанті.

Також виявилося, що димовловлювач не працює і з феном H312.

📌 У підсумку: обладнання є, але нормальної інтеграції між пристроями немає.

Спочатку було досліджено керування димовловлювачем ES02 через Bluetooth. Після аналізу команд стало зрозуміло, що пристроєм можна керувати напряму.

Далі було досліджено USB-інтерфейс паяльної станції. Передбачалося, що через нього можна отримувати інформацію про стан роботи, однак на практиці USB використовується лише для прошивки і не передає робочі статуси.

Також було досліджено фен (H312) через усі доступні інтерфейси: USB, Wi-Fi та Bluetooth. Підключення вдалося встановити через кожен з них, однак у проєкті використовується лише бездротова інтеграція як найбільш практична.

Міняти техніку не було сенсу — проблема була не в залізі, а у відсутності зв'язки між пристроями.

У результаті стало очевидно, що найнадійнішим рішенням буде доопрацювати саму станцію та додати відсутню логіку.

Також варто зазначити, що Aixun пропонує окремий пристрій — BS08 AI Voice Center Control, призначений для керування обладнанням через голосові команди.

Попри заявлену можливість об'єднання пристроїв, це рішення фактично залишається ручним керуванням — користувач, як і раніше, має віддавати команди (голосом або через інтерфейс).

📌 BS08 не відстежує фактичний стан паяльної станції та не здатний автоматично реагувати на процес роботи.

Таким чином, проблема автоматизації залишається невирішеною — змінюється лише спосіб керування.

Так з'явився Legacy Bridge.

### ⚙️ Функціональність

- Автоматичне керування димовловлювачем
- Реакція на стан паяльника (SENSE)
- Налаштовувана реакція на роботу фена (за температурою)
- Гнучке налаштування алгоритму керування потужністю димовловлювача
- Запам'ятовування останньої встановленої швидкості
- Регулювання підсвітки
- Гнучке налаштування поведінки підсвітки залежно від стану датчиків
- Автоматичне вимкнення за часом
- Налаштування і контроль через веб-інтерфейс

### 🧠 Принцип роботи

- Сигнал SENSE використовується для визначення положення паяльника
- Входи обробляються через RC-ланцюги (100k + 100nF)
- ESP32 аналізує стани входів і зовнішні дані
- Температура фена використовується як додаткова умова вмикання димовловлювача
- Поріг температури задається через веб-інтерфейс
- Керування димовловлювачем виконується через BLE-команди

### 🧩 Апаратна частина

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="Загальний вигляд системи" width="300"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="Модуль ESP32-C3" width="300"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Дроти і конденсатори на ESP32" width="300"></a>
  
*Підключення ESP32 (GPIO1, GPIO3, живлення)*

### 🔌 Схема підключення

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="SENSE точка 1" width="75"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="SENSE точка 2" width="75"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="Точка 5V" width="75"></a>
<a href="assets/photos/t420d-case-placement.jpg" target="_blank"><img src="assets/photos/t420d-case-placement.jpg" alt="Приклад розміщення модуля в корпусі T420D" width="300"></a>
  
*Фото 1: підключення Signal 1*  
*Фото 2: підключення Signal 2 і GND*  
*Фото 3: джерело живлення 5V*  
*Фото 4: приклад встановлення модуля всередині T420D*

### 🔌 USB підключення

ESP32 підключається по USB лише для первинної прошивки.

Подальша робота:

- оновлення через Wi-Fi (OTA)
- автономна робота без USB

### 🌐 Веб-інтерфейс

Інтерфейс використовується для налаштування логіки роботи системи та контролю стану.

---

### Основні можливості

#### 🎛 Керування
- налаштування потужності димовловлювача  
- регулювання яскравості підсвітки  

#### 🧠 Логіка
- затримка вмикання та вимикання  
- умови активації (паяльник / фен)  
- налаштування температурних порогів  

#### 📡 Підключення
- налаштування Wi-Fi  
- пошук і підключення пристроїв  

#### 🛠 Система
- перегляд логів  
- reboot  
- recovery  
- скидання до заводських налаштувань  

### 🚀 Live Demo

Демо працює в браузері та показує інтерфейс у режимі емуляції:

👉 https://serjio193.github.io/legacy-bridge/demo/

---

### 📡 Перший запуск

Онлайн USB flasher: https://serjio193.github.io/legacy-bridge/  
Demo UI: https://serjio193.github.io/legacy-bridge/demo/  
Потрібен Chromium-браузер (Chrome / Edge, WebSerial).

### Дані за замовчуванням

- SSID: `LB-SETUP-XXXXX`
- Пароль: `lbxxxxx!2026`
- Логін: `admin`
- Recovery AP: `LB_RECOVERY`

### Генерація пароля

- `XXXXX` — останні 5 символів MAC (HEX, uppercase)  
- `xxxxx` — ті самі символи у lowercase  

---

### 📡 Мережа

Після налаштування:

- точка доступу вимикається  
- пристрій працює в основній мережі  

---

### 🌡 Інтеграція фена

Підключення:

- Wi-Fi  
- Bluetooth (BLE)  

Робота димовловлювача залежить від заданого температурного порога.

---

### 🔐 Безпека

- Пакети прошивки підписані приватним ключем
- Пристрій приймає лише підписані оновлення
- Boot і Recovery захищені від запису по Wi-Fi

---

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

Планується:

- підтримка slave-пристроїв на ESP32  
- інтеграція додаткового обладнання (Aixun, JCID та ін.)  

### 👨‍🔧 Автор

**Serjio193**  
Embedded developer  

Проєкт заснований на практичному досвіді ремонту та щоденній роботі з обладнанням.

### 🎯 Мета проєкту

Створити простий і надійний інструмент, який автоматизує робочий процес і прибирає зайві дії в роботі.

### ❤️ Підтримка

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20-ua">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

<details id="usdt-trc20-ua">
<summary>💰 Показати адресу USDT (TRC20)</summary>

`TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

<br>
<img src="assets/support/usdt-trc20-qr.jpg" alt="USDT TRC20 QR" width="280">

</details>

</details>

<details>
<summary><b>Русский</b></summary>

## 🔥 Legacy Bridge (LB)

Интеллектуальный мост между паяльной станцией и дымоуловителем.  
Автоматизация без замены оборудования.

---

### 🚀 Что это такое

Дымоуловитель не включается автоматически?

Legacy Bridge решает эту проблему без замены оборудования.

Это встраиваемая модификация на базе ESP32 для паяльных станций (T420D и аналогичных), которая добавляет автоматическое управление вытяжкой на основе состояния паяльника и фена.

---

### 📖 История создания

Я использую оборудование Aixun в работе.

После покупки дымоуловителя Aixun ES02 выяснилось, что он не работает с моей паяльной станцией T420D, так как в ней отсутствует Wi-Fi модуль. На тот момент я не знал, что существуют версии этой станции с Wi-Fi — как и многие, я был уверен, что T420D бывает только в одном варианте.

Также оказалось, что вытяжка не работает и с феном H312.

📌 В итоге: оборудование есть, но нормальной интеграции между устройствами нет.

Сначала было изучено управление дымоуловителем ES02 по Bluetooth. После анализа команд стало понятно, что устройством можно управлять напрямую.

Далее был исследован USB-интерфейс паяльной станции. Предполагалось, что через него можно получить информацию о состоянии работы, однако на практике USB используется только для прошивки и не передаёт рабочие статусы.

Также был исследован фен (H312) по всем доступным интерфейсам: USB, Wi-Fi и Bluetooth. Подключение удалось установить по каждому из них, однако в проекте используется только беспроводная интеграция как наиболее практичная.

Менять технику не было смысла — проблема была не в железе, а в отсутствии связки между устройствами.

В результате стало очевидно, что наиболее надёжным решением будет доработать саму станцию и добавить недостающую логику.

Также стоит отметить, что Aixun предлагает отдельное устройство — BS08 AI Voice Center Control, предназначенное для управления оборудованием через голосовые команды.

Несмотря на заявленную возможность объединения устройств, данное решение остаётся по сути ручным управлением — пользователь по-прежнему должен отдавать команды (голосом или через интерфейс).

📌 BS08 не отслеживает фактическое состояние паяльной станции и не способен автоматически реагировать на процесс работы.

Таким образом, проблема автоматизации остаётся нерешённой — меняется лишь способ управления.

Так появился Legacy Bridge.

### ⚙️ Функциональность

- Автоматическое управление дымоуловителем
- Реакция на состояние паяльника (SENSE)
- Настраиваемая реакция на работу фена (по температуре)
- Гибкая настройка алгоритма управления мощностью дымоуловителя
- Запоминание последней установленной скорости
- Регулировка подсветки
- Гибкая настройка поведения подсветки в зависимости от состояния датчиков
- Автоматическое выключение по времени
- Настройка и контроль через веб-интерфейс

### 🧠 Принцип работы

- Сигнал SENSE используется для определения положения паяльника
- Входы обрабатываются через RC-цепи (100k + 100nF)
- ESP32 анализирует состояние входов и внешние данные
- Температура фена используется как дополнительное условие включения вытяжки
- Порог температуры задаётся через веб-интерфейс
- Управление дымоуловителем выполняется по BLE-командам

### 🧩 Аппаратная часть

- ESP32-C3 Pro Mini
- R1, R2 — 100 кОм
- C1, C2 — 100 нФ

<a href="assets/photos/system-overview.jpg" target="_blank"><img src="assets/photos/system-overview.jpg" alt="Общий вид системы: станция, вытяжка и ESP32" width="300"></a>
<a href="assets/photos/esp32-closeup.jpg" target="_blank"><img src="assets/photos/esp32-closeup.jpg" alt="Крупный план ESP32-C3" width="300"></a>
<a href="assets/photos/esp32-wires-capacitors.jpg" target="_blank"><img src="assets/photos/esp32-wires-capacitors.jpg" alt="Подключение проводов и конденсаторов к ESP32" width="300"></a>
  
*Подключение ESP32 (GPIO1, GPIO3, питание)*

### 🔌 Схема подключения

```text
Signal 1 -> R1 100k -> GPIO1
GPIO1    -> C1 100nF -> GND

Signal 2 -> R2 100k -> GPIO3
GPIO3    -> C2 100nF -> GND

5V  -> ESP32 5V
GND -> ESP32 GND
```

<a href="assets/photos/t420d-sense-point-1.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-1.jpg" alt="Подключение SENSE к плате T420D (точка 1)" width="75"></a>
<a href="assets/photos/t420d-sense-point-2.jpg" target="_blank"><img src="assets/photos/t420d-sense-point-2.jpg" alt="Подключение SENSE к плате T420D (точка 2)" width="75"></a>
<a href="assets/photos/t420d-5v-point.jpg" target="_blank"><img src="assets/photos/t420d-5v-point.jpg" alt="Точка подключения 5V на плате T420D" width="75"></a>
<a href="assets/photos/t420d-case-placement.jpg" target="_blank"><img src="assets/photos/t420d-case-placement.jpg" alt="Пример размещения модуля в корпусе T420D" width="300"></a>
  
*Фото 1: подключение Signal 1*  
*Фото 2: подключение Signal 2 и GND*  
*Фото 3: источник питания 5V*  
*Фото 4: пример установки модуля внутри T420D*

### 🔌 USB подключение

ESP32 подключается по USB только для первичной прошивки.

Дальнейшая работа:

- обновление по Wi-Fi (OTA)
- автономная работа без USB

### 🌐 Веб-интерфейс

Интерфейс используется для настройки логики работы системы и контроля состояния.

---

### Основные возможности

#### 🎛 Управление
- настройка мощности дымоуловителя  
- регулировка яркости подсветки  

#### 🧠 Логика
- задержка включения и выключения  
- условия активации (паяльник / фен)  
- настройка температурных порогов  

#### 📡 Подключение
- настройка Wi-Fi  
- поиск и подключение устройств  

#### 🛠 Система
- просмотр логов  
- reboot  
- recovery  
- сброс настроек  

### 🚀 Live Demo

Демо работает в браузере и показывает интерфейс в режиме эмуляции:

👉 https://serjio193.github.io/legacy-bridge/demo/

---

### 📡 Первый запуск

Онлайн USB flasher:  
👉 https://serjio193.github.io/legacy-bridge/

Требуется Chromium-браузер (Chrome / Edge, WebSerial).

### Данные по умолчанию

- SSID: `LB-SETUP-XXXXX`  
- Пароль: `lbxxxxx!2026`  
- Логин: `admin`  
- Recovery AP: `LB_RECOVERY`  

### Генерация пароля

- `XXXXX` — последние 5 символов MAC (HEX, uppercase)  
- `xxxxx` — те же символы в lowercase  

---

### 📡 Сеть

После настройки:

- точка доступа отключается  
- устройство работает в основной сети  

---

### 🌡 Интеграция фена

Подключение:

- Wi-Fi  
- Bluetooth (BLE)  

Работа вытяжки зависит от заданного температурного порога.

---

### 🔐 Безопасность

- Прошивка подписана приватным ключом  
- Устройство принимает только подписанные обновления  
- Boot и Recovery защищены от записи по Wi-Fi  

---

### 📦 Обновления

- OTA через Wi-Fi  
- Пакет: `update.lbpack`  
- Источник: GitHub Releases  

---

### 🧪 Поддерживаемое оборудование

- Aixun T420D  
- Aixun H312  
- Aixun ES02  
- JBC-совместимые станции (частично)  

---

### 🚧 План развития

Планируется:

- поддержка slave-устройств на ESP32  
- интеграция дополнительного оборудования (Aixun, JCID и др.)  

### 👨‍🔧 Автор

**Serjio193**  
Embedded developer  

Проект основан на практическом опыте ремонта и повседневной работе с оборудованием.

---

### 🎯 Цель проекта

Создать простой и надёжный инструмент, который автоматизирует рабочий процесс и убирает лишние действия в работе.

---

### ❤️ Поддержка

<a href="https://paypal.me/SerhiiTarnopovych" target="_blank">
  <img src="assets/support/paypal-logo.png" alt="PayPal" height="34">
</a>
&nbsp;&nbsp;
<a href="#usdt-trc20-ru">
  <img src="assets/support/usdt-trc20-logo.png" alt="USDT TRC20" height="34">
</a>

<details id="usdt-trc20-ru">
<summary>💰 Показать адрес USDT (TRC20)</summary>

`TB4kzsHL3emLtdvDroNE9dEpMhUW6r3bTL`

<br>
<img src="assets/support/usdt-trc20-qr.jpg" alt="USDT TRC20 QR" width="280">

</details>

</details>

## 🧱 Technical Documentation

- See [TECHNICAL.md](TECHNICAL.md)
