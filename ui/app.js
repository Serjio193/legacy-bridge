(function () {
      const STORAGE_KEY = "alb_lang";
      const i18n = {
        en: {
          doc_title: "Legacy Bridge - Station-to-Extractor Integration",
          env: "configuration dashboard",
          lang_label: "Language",
          build: "build",
          hero_title: "Legacy Bridge",
          hero_sub: "Station-to-Extractor Integration",
          hero_small: "Designed for Backward Compatibility",
          k_wifi: "Wi-Fi",
          k_ble: "BLE",
          k_mode: "Mode",
          k_extractor: "Extractor",
          k_handle1: "Handle 1",
          k_handle2: "Handle 2",
          k_hot_air: "Hot Air",
          k_presence: "Presence",
          k_failsafe: "Fail-safe",
          k_firmware: "Firmware",
          k_device: "Device",
          k_bridge_link: "Bridge Link",
          k_security: "Security",
          k_project: "Project",
          k_profile: "Profile",
          k_accent: "Accent",
          v_no_network: "WIFI: DOWN",
          v_enabled: "ENABLED",
          v_ap_fallback: "AP FALLBACK",
          v_not_paired: "PAIR: NONE",
          v_idle: "IDLE",
          v_armed: "ARMED",
          v_ready: "READY",
          v_local_only: "LOCAL-ONLY",
          v_industrial_dark: "Industrial Dark",
          section_network: "Connectivity and Devices",
          pill_connectivity: "Connectivity",
          section_devices: "Devices",
          pill_integration: "Integration",
          label_device_lb: "LB Equipment",
          label_device_third_party: "Third-party Equipment",
          hint_devices: "Enable only the sources you actually use to keep logic deterministic.",
          label_lb_model: "Model",
          label_t420d_handles: "Handles",
          pill_handle1: "Handle 1",
          pill_handle2: "Handle 2",
          label_wifi_scan: "Wi-Fi Scan",
          pill_known: "Known",
          pill_auth: "Auth",
          pill_limited: "Limited",
          btn_scan: "Scan",
          btn_refresh: "Refresh",
          label_wifi_auto_off: "Wi-Fi Auto-Off",
          hint_wifi_auto_off: "0 = never. Turns off Wi-Fi after N minutes to reduce visibility.",
          hint_wifi_pick_network: "Select Wi-Fi network above.",
          hint_wifi_enter_password: "Enter Wi-Fi password.",
          msg_wifi_connecting: "Connecting...",
          msg_wifi_connected: "Connected",
          msg_wifi_opening_page: "Opening local page...",
          msg_wifi_wait_switch: "Connected. Waiting for PC to switch to router...",
          msg_wifi_open_manual: "Could not open automatically. Open manually:",
          wifi_sta_summary_title: "Wi-Fi Connected",
          wifi_sta_summary_text: "STA mode is active. Wi-Fi setup is hidden.",
          label_sta_ssid: "Router SSID",
          label_sta_ip: "IP",
          btn_change_wifi: "Change Wi-Fi",
          btn_wifi_autooff_5: "Auto-Off in 5 min",
          hint_wifi_autooff_locked_hotair_wifi: "Unavailable while hot-air is using Wi-Fi.",
          msg_wifi_autooff5_set: "Wi-Fi auto-off set to 5 min.",
          msg_wifi_autooff5_clear: "Wi-Fi auto-off disabled.",
          msg_wifi_autooff5_fail: "Failed to set Wi-Fi auto-off",
          msg_wifi_shortcut_saved: "Link copied",
          btn_install_app: "Install App",
          btn_download_shortcut: "Add to Desktop",
          btn_open_recovery: "Recovery",
          btn_logout: "Logout",
          msg_logout_fail: "Logout failed",
          msg_shortcut_downloaded: "Link prepared",
          msg_shortcut_open_manual: "Copy link and create shortcut manually",
          msg_shortcut_mobile_note: "On phones use browser menu: Add to Home Screen.",
          msg_shortcut_copied: "Link copied",
          msg_shortcut_copy_failed: "Copy failed. Use this URL manually",
          msg_shortcut_win_steps: "Desktop: Right click -> New -> Shortcut -> Paste link.",
          msg_shortcut_mac_steps: "Drag the URL from browser address bar to Desktop.",
          msg_shortcut_linux_steps: "Create launcher/shortcut and use this URL.",
          msg_shortcut_phone_steps: "Use browser menu: Add to Home Screen.",
          shortcut_offer_title: "Install Desktop Shortcut?",
          shortcut_offer_question: "Install LB-Control shortcut now?",
          shortcut_offer_detected: "Detected OS",
          shortcut_offer_windows_notice: "Windows setup uses PowerShell. Auto mode copies one command. Manual mode copies full script. Browser auto-launch needs one-time protocol registration.",
          shortcut_offer_mobile_hint: "On Android/iOS use browser menu: Add to Home Screen.",
          shortcut_offer_macos_hint: "Run the command in Terminal to create a .webloc on Desktop.",
          shortcut_offer_linux_hint: "Run the command in terminal to create a .desktop link.",
          shortcut_offer_other_hint: "Copy link and create a shortcut manually in your system.",
          shortcut_offer_done: "Done",
          shortcut_offer_auto_copied: "Auto command copied. Open PowerShell and paste it.",
          shortcut_offer_protocol_try: "Trying browser protocol launch...",
          shortcut_offer_script_copied: "Script copied. Paste and run in terminal.",
          shortcut_offer_copy_failed: "Copy blocked. Copy text manually from the field below.",
          shortcut_offer_pwa_started: "If browser supports it, installation prompt is now open.",
          shortcut_offer_pwa_not_available: "Auto install prompt is unavailable here. Use browser menu:",
          shortcut_offer_pwa_hint_win: "Chrome: Menu (three dots) -> Save and share -> Install page as app.",
          shortcut_offer_pwa_hint_macos: "Chrome: Menu (three dots) -> Install page as app.",
          shortcut_offer_pwa_hint_linux: "Chrome: Menu (three dots) -> Install page as app.",
          shortcut_offer_pwa_hint_mobile: "Browser menu -> Add to Home Screen.",
          btn_yes: "Yes",
          btn_no: "No",
          btn_auto_ps: "Auto (PowerShell)",
          btn_copy_script: "Copy Script",
          install_offer_text: "Install LB-Control for quick access.",
          btn_later: "Later",
          ap_switch_title: "Reconnect To Your Router",
          ap_switch_text: "Device is connected. Switch your PC/phone from LB-SETUP to your router Wi-Fi.",
          btn_check_connection: "Check Connection",
          btn_hide: "Hide",
          msg_install_no_prompt: "Automatic PWA install is unavailable in this browser context.",
          msg_install_using_menu: "Use browser menu: Install app / Create shortcut.",
          msg_wifi_failed: "Connection failed",
          msg_wifi_hidden_cannot: "Hidden SSID: enter network name manually.",
          msg_wifi_scan_failed: "Wi-Fi scan failed",
          msg_wifi_scan_unavailable: "Scan is temporarily unavailable",
          msg_wifi_no_networks: "No networks",
          msg_wifi_press_scan: "Press Scan",
          msg_wifi_scanning: "Scanning Wi-Fi...",
          msg_wifi_scan_retry: "Press Scan again",
          unit_min: "min",
          label_ssid: "SSID",
          label_security: "Security",
          opt_wpa2psk: "WPA2-PSK",
          opt_open: "Open",
          opt_wpa2ent: "WPA2-Enterprise",
          label_password: "Password",
          btn_connect: "Connect",
          btn_disconnect: "Disconnect",
          label_ble: "BLE",
          hint_ble_auto: "BLE is used only for extractor control. Auto-connect when configured.",
          opt_enabled: "Enabled",
          opt_disabled: "Disabled",
          label_discovery: "Discovery",
          opt_active: "Active",
          opt_inactive: "Inactive",
          opt_passive: "Passive",
          btn_discover: "Discover",
          btn_rescan: "Rescan",
          section_extractor_reg: "Extractor (ES02)",
          label_extractor: "Extractor",
          label_status: "Status",
          label_found: "Found",
          label_found_devices: "Found devices",
          v_connected: "Connected",
          v_not_found: "Not found",
          btn_find_extractor: "Find Extractor",
          btn_connect_extractor: "Connect",
          btn_save: "Save",
          btn_change: "Change",
          pill_device_binding: "Device Binding",
          label_ble_prefix: "BLE Name Prefix",
          label_locked_addr: "Locked Address (optional)",
          label_discovered_extractors: "Discovered Extractors",
          pill_reachable: "Reachable",
          label_pairing_state: "Pairing State",
          btn_pair: "Pair",
          btn_unpair: "Unpair",
          btn_read_status: "Read Status",
          hint_extractor: "Registration binds a single extractor for deterministic control.",
          section_hotair_bind: "Hot Air (H310/H312)",
          label_device_clock: "Device Clock",
          hotair_state_not_bound: "Not Bound",
          hotair_state_bound: "Bound",
          hotair_state_binding: "Binding...",
          label_hotair_ip: "H310/H312 IP",
          label_hotair_ble_addr: "H310/H312 BLE MAC",
          label_hotair_temp_on: "Temperature Trigger Threshold",
          btn_scan_hotair: "Scan H310/H312",
          btn_scan_hotair_ble: "Scan H310/H312 BLE",
          btn_bind_hotair: "Bind Hot Air",
          btn_bind_hotair_ble: "Bind H310/H312",
          btn_add_hotair: "Add Hot Air",
          btn_remove_hotair: "Remove Hot Air",
          btn_reset_hotair: "Reset H310/H312",
          msg_hotair_reset_confirm: "Reset H310/H312 Wi-Fi and BLE configuration?",
          msg_hotair_reset_working: "Resetting H310/H312 configuration...",
          msg_hotair_reset_ok: "H310/H312 configuration reset",
          msg_hotair_reset_fail: "H310/H312 reset failed",
          section_behavior_logic: "Behavior Logic",
          pill_core: "Core",
          label_power_condition: "Extractor Power Condition",
          label_triggers: "Active Sources",
          default_iron1: "Handle 1",
          default_iron2: "Handle 2",
          default_hot_air: "Hot Air",
          label_iron1_name: "Handle 1 Name (T420D)",
          label_iron2_name: "Handle 2 Name (T420D)",
          label_hot_air_name: "Hot Air Name (H310/H312)",
          label_light_mode: "Light Mode On Activation",
          opt_light_off: "Off",
          opt_light_white: "White",
          opt_light_green: "Green",
          opt_light_last: "Last Enabled",
          label_light_brightness: "Light Brightness",
          value_light_brightness_last: "Last Set Brightness",
          value_light_brightness_manual: "Manual Brightness",
          label_light_brightness_manual: "Manual Brightness (0-100%)",
          label_t420d_h1_gpio: "Handle 1 GPIO (T420D)",
          label_t420d_h2_gpio: "Handle 2 GPIO (T420D)",
          opt_gpio_disabled: "Disabled",
          seg_h1_title: "ON if {h1} removed",
          seg_h2_title: "ON if {h2} removed",
          seg_ha_title: "ON if {ha} removed",
          label_start_delay: "Start Delay",
          label_stop_delay: "Stop Delay",
          fmt_h1: "{h1}",
          fmt_h2: "{h2}",
          fmt_ha: "{ha}",
          label_fail_safe: "Fail-Safe",
          label_kick_start: "Kick Start",
          label_kick_start_level: "Kick Start Level",
          label_speed_restore: "Speed Restore",
          value_last_set_speed: "Last Set Speed",
          value_manual_speed_mode: "Manual Speed",
          label_manual_speed: "Manual Speed",
          label_auto_stop: "Automatic End Of Work",
          label_auto_stop_enable: "Auto Stop",
          label_auto_stop_time: "Time (HH:MM)",
          hint_auto_stop: "At this local time LB turns extractor and light off.",
          btn_apply_logic: "Apply Logic",
          section_iron1: "Iron 1 Pairing and ToF Calibration",
          pill_slot1: "Slot 1",
          section_iron2: "Iron 2 Pairing and ToF Calibration",
          pill_slot2: "Slot 2",
          label_pairing: "Pairing",
          label_presence_sensor: "Presence Sensor",
          opt_tof: "ToF (VL53-class)",
          opt_reed: "Reed Switch",
          opt_current: "Current Sense",
          label_near: "Near (mm)",
          label_far: "Far (mm)",
          label_hyst: "Hysteresis (mm)",
          btn_calibrate: "Calibrate",
          btn_test_presence: "Test Presence",
          section_timing_behavior: "Timing and Behavior",
          pill_tuning: "Tuning",
          label_start_delay_ms: "Start Delay (ms)",
          label_stop_delay_ms: "Stop Delay (ms)",
          label_kick_start_ms: "Kick Start (ms)",
          opt_on: "ON",
          opt_off: "OFF",
          label_max_speed: "Max Speed",
          label_min_speed: "Min Speed",
          btn_apply: "Apply",
          btn_reset_defaults: "Reset Defaults",
          section_apply_diag: "Apply and Diagnostics",
          pill_operations: "Operations",
          btn_fw_update: "Firmware Update",
          fw_menu_title: "Firmware Update",
          fw_menu_text: "Update to latest release or choose a specific version from GitHub releases.",
          btn_fw_latest: "Update To Latest",
          btn_fw_choose_version: "Choose Version",
          btn_fw_online: "Online",
          btn_fw_manual: "Manual",
          btn_fw_github: "Visit GitHub",
          btn_fw_cancel: "OK",
          fw_online_prompt: "Enter firmware URL for online update",
          msg_fw_online_start: "online update started",
          msg_fw_online_ok: "online update done, rebooting",
          msg_fw_online_fail: "online update failed",
          msg_fw_online_working: "downloading and flashing firmware...",
          msg_fw_online_requested: "online update selected (backend not enabled)",
          msg_fw_manual_pick: "manual update: choose firmware file",
          msg_fw_manual_selected: "manual firmware selected",
          msg_fw_manual_upload_start: "starting manual firmware upload",
          msg_fw_manual_uploading: "uploading firmware...",
          msg_fw_manual_finishing: "finalizing flash...",
          msg_fw_manual_ok: "manual update done, rebooting",
          msg_fw_manual_fail: "manual update failed",
          msg_fw_manual_need_recovery: "single-slot OTA layout: switching to Recovery mode for flashing",
          msg_fw_recovery_switch_ok: "recovery mode requested, rebooting",
          msg_fw_recovery_switch_fail: "failed to switch to recovery mode",
          msg_fw_status_idle: "Firmware update idle",
          btn_reboot: "Reboot",
          btn_export_log: "Export Log (.txt)",
          btn_reset_settings: "Reset Settings",
          label_web_auth: "Web Access",
          label_web_user: "Login",
          label_web_pass_new: "New Password",
          label_web_pass_repeat: "Repeat Password",
          btn_web_auth_save: "Save Access",
          btn_web_auth_logout_all: "Logout All Devices",
          msg_web_auth_saved: "Access updated",
          msg_web_auth_failed: "Failed to update access",
          msg_web_auth_pass_mismatch: "Passwords do not match",
          msg_web_auth_pass_short: "Password must be at least 8 characters",
          msg_web_auth_relogin: "Access changed. Reload page and login again.",
          msg_web_auth_logout_all_confirm: "Logout all remembered devices?",
          msg_web_auth_logout_all_ok: "All remembered devices logged out",
          msg_web_auth_logout_all_fail: "Failed to logout remembered devices",
          prompt_confirm_pass: "Enter current web password to continue",
          msg_confirm_pass_required: "Operation canceled: password not provided",
          label_event_log: "Event Log",
          hint_diag: "",
          diag_bg_hue: "Background Hue",
          diag_bg_sat: "Color Amount",
          diag_bg_light: "Brightness",
          diag_bg_angle: "Gradient Angle",
          diag_bg_hex: "Color Code",
          fw_current: "Current FW",
          fw_server: "Server FW",
          footer_github: "Visit GitHub",
          section_fw_map: "Firmware Partition Map",
          pill_service: "Service",
          hint_fw_map: "Shows bootloader, OTA slots, NVS and recovery readiness.",
          btn_switch_ota: "Switch OTA Slot",
          btn_verify_partitions: "Verify Partitions",
          btn_enter_recovery: "Enter Recovery Mode",
          hint_fw_actions: "Entering Recovery will reboot the device.",
        },
        ru: {
          doc_title: "Legacy Bridge - Объединяет станции и вытяжку",
          env: "панель настройки",
          lang_label: "Язык",
          build: "сборка",
          hero_title: "Legacy Bridge",
          hero_sub: "Объединяет станции и вытяжку",
          hero_small: "Спроектировано для обратной совместимости",
          k_wifi: "Wi-Fi",
          k_ble: "BLE",
          k_mode: "Режим",
          k_extractor: "Дымоуловитель",
          k_handle1: "Ручка 1",
          k_handle2: "Ручка 2",
          k_hot_air: "Фен",
          k_presence: "Присутствие",
          k_failsafe: "Аварийный режим",
          k_firmware: "Прошивка",
          k_device: "Устройство",
          k_bridge_link: "Связь моста",
          k_security: "Безопасность",
          k_project: "Проект",
          k_profile: "Профиль",
          k_accent: "Акцент",
          v_no_network: "WIFI: DOWN",
          v_enabled: "ВКЛ",
          v_ap_fallback: "AP РЕЖИМ",
          v_not_paired: "PAIR: NONE",
          v_idle: "ОЖИДАНИЕ",
          v_armed: "АКТИВЕН",
          v_ready: "ГОТОВ",
          v_local_only: "ТОЛЬКО ЛОКАЛЬНО",
          v_industrial_dark: "Индустриальный тёмный",
          section_network: "Связь и устройства",
          pill_connectivity: "Связь",
          section_devices: "Устройства",
          pill_integration: "Интеграция",
          label_device_lb: "LB оборудование",
          label_device_third_party: "Стороннее оборудование",
          hint_devices: "Включай только те источники, которые реально используешь, чтобы логика была предсказуемой.",
          label_lb_model: "Модель",
          label_t420d_handles: "Ручки",
          pill_handle1: "Ручка 1",
          pill_handle2: "Ручка 2",
          label_wifi_scan: "Скан Wi-Fi",
          pill_known: "Известная",
          pill_auth: "Авторизация",
          pill_limited: "Ограничено",
          btn_scan: "Сканировать",
          btn_refresh: "Обновить",
          label_wifi_auto_off: "Выкл Wi-Fi через (мин)",
          hint_wifi_auto_off: "0 = никогда. Выключает Wi-Fi через N минут, чтобы не светиться в сети.",
          hint_wifi_pick_network: "Выберите сеть Wi-Fi выше.",
          hint_wifi_enter_password: "Введите пароль Wi-Fi.",
          msg_wifi_connecting: "Подключение...",
          msg_wifi_connected: "Подключено",
          msg_wifi_opening_page: "Открываю страницу в локальной сети...",
          msg_wifi_wait_switch: "Подключено. Жду, пока ПК переключится на роутер...",
          msg_wifi_open_manual: "Не удалось открыть автоматически. Откройте вручную:",
          wifi_sta_summary_title: "Wi-Fi подключён",
          wifi_sta_summary_text: "Режим STA активен. Настройка Wi-Fi скрыта.",
          label_sta_ssid: "SSID роутера",
          label_sta_ip: "IP",
          btn_change_wifi: "Изменить Wi-Fi",
          btn_wifi_autooff_5: "Автовыкл Wi-Fi через 5 мин",
          hint_wifi_autooff_locked_hotair_wifi: "Недоступно: фен использует Wi-Fi.",
          msg_wifi_autooff5_set: "Автовыключение Wi-Fi установлено: 5 мин.",
          msg_wifi_autooff5_clear: "Автовыключение Wi-Fi отключено.",
          msg_wifi_autooff5_fail: "Не удалось установить автовыключение Wi-Fi",
          msg_wifi_shortcut_saved: "Ссылка скопирована",
          btn_install_app: "Установить",
          btn_download_shortcut: "Добавить на рабочий стол",
          btn_open_recovery: "Recovery",
          btn_logout: "Выход",
          msg_logout_fail: "Не удалось выйти",
          msg_shortcut_downloaded: "Ссылка подготовлена",
          msg_shortcut_open_manual: "Скопируйте ссылку и создайте ярлык вручную",
          msg_shortcut_mobile_note: "На телефоне используйте меню браузера: Добавить на главный экран.",
          msg_shortcut_copied: "Ссылка скопирована",
          msg_shortcut_copy_failed: "Не удалось скопировать. Используйте ссылку вручную",
          msg_shortcut_win_steps: "Рабочий стол: ПКМ -> Создать -> Ярлык -> вставьте ссылку.",
          msg_shortcut_mac_steps: "Перетащите URL из адресной строки браузера на рабочий стол.",
          msg_shortcut_linux_steps: "Создайте launcher/ярлык и укажите этот URL.",
          msg_shortcut_phone_steps: "Используйте меню браузера: Добавить на главный экран.",
          shortcut_offer_title: "Установить ярлык на рабочий стол?",
          shortcut_offer_question: "Установить ярлык LB-Control сейчас?",
          shortcut_offer_detected: "Определена ОС",
          shortcut_offer_windows_notice: "Для Windows используется PowerShell. Авто-режим копирует одну команду. Ручной режим копирует полный скрипт. Запуск прямо из браузера требует одноразовой регистрации протокола.",
          shortcut_offer_mobile_hint: "На Android/iOS используйте меню браузера: Добавить на главный экран.",
          shortcut_offer_macos_hint: "Выполните команду в Terminal, чтобы создать .webloc на рабочем столе.",
          shortcut_offer_linux_hint: "Выполните команду в терминале, чтобы создать .desktop ярлык.",
          shortcut_offer_other_hint: "Скопируйте ссылку и создайте ярлык вручную в вашей системе.",
          shortcut_offer_done: "Готово",
          shortcut_offer_auto_copied: "Команда авто-установки скопирована. Откройте PowerShell и вставьте её.",
          shortcut_offer_protocol_try: "Пробую запуск через браузерный протокол...",
          shortcut_offer_script_copied: "Скрипт скопирован. Вставьте и выполните в терминале.",
          shortcut_offer_copy_failed: "Копирование заблокировано. Скопируйте текст вручную из поля ниже.",
          shortcut_offer_pwa_started: "Если браузер поддерживает установку, окно установки уже открыто.",
          shortcut_offer_pwa_not_available: "Авто-установка недоступна. Используйте меню браузера:",
          shortcut_offer_pwa_hint_win: "Chrome: меню (три точки) -> Сохранить и поделиться -> Установить приложение.",
          shortcut_offer_pwa_hint_macos: "Chrome: меню (три точки) -> Установить приложение.",
          shortcut_offer_pwa_hint_linux: "Chrome: меню (три точки) -> Установить приложение.",
          shortcut_offer_pwa_hint_mobile: "Меню браузера -> Добавить на главный экран.",
          btn_yes: "Да",
          btn_no: "Нет",
          btn_auto_ps: "Авто (PowerShell)",
          btn_copy_script: "Копировать скрипт",
          install_offer_text: "Установите LB-Control для быстрого доступа.",
          btn_later: "Позже",
          ap_switch_title: "ПЕРЕПОДКЛЮЧИТЕСЬ К ВАШЕМУ РОУТЕРУ",
          ap_switch_text: "Устройство подключено. Переключите ПК/телефон с LB-SETUP на Wi-Fi вашего роутера.",
          btn_check_connection: "Проверить подключение",
          btn_hide: "Скрыть",
          msg_install_no_prompt: "Авто-установка PWA недоступна в этом режиме.",
          msg_install_using_menu: "Откройте меню браузера: Установить приложение / Создать ярлык.",
          msg_wifi_failed: "Не удалось подключиться",
          msg_wifi_hidden_cannot: "Скрытая сеть: введите SSID вручную.",
          msg_wifi_scan_failed: "Ошибка сканирования Wi-Fi",
          msg_wifi_scan_unavailable: "Сканирование временно недоступно",
          msg_wifi_no_networks: "Сети не найдены",
          msg_wifi_press_scan: "Нажмите Сканировать",
          msg_wifi_scanning: "Сканирование Wi-Fi...",
          msg_wifi_scan_retry: "Нажмите Сканировать ещё раз",
          unit_min: "мин",
          label_ssid: "SSID",
          label_security: "Защита",
          opt_wpa2psk: "WPA2-PSK",
          opt_open: "Открытая",
          opt_wpa2ent: "WPA2-Enterprise",
          label_password: "Пароль",
          btn_connect: "Подключить",
          btn_disconnect: "Отключить",
          label_ble: "BLE",
          hint_ble_auto: "BLE используется только для управления дымоуловителем. Автоподключение после выбора.",
          opt_enabled: "Вкл",
          opt_disabled: "Выкл",
          label_discovery: "Поиск",
          opt_active: "Активный",
          opt_inactive: "Неактивный",
          opt_passive: "Пассивный",
          btn_discover: "Найти",
          btn_rescan: "Повторить",
          section_extractor_reg: "Дымоуловитель (ES02)",
          label_extractor: "Дымоуловитель",
          label_status: "Статус",
          label_found: "Найден",
          label_found_devices: "Найденные устройства",
          v_connected: "Подключён",
          v_not_found: "Не найден",
          btn_find_extractor: "Найти дымоуловитель",
          btn_connect_extractor: "Подключить",
          btn_save: "Сохранить",
          btn_change: "Изменить",
          pill_device_binding: "Привязка",
          label_ble_prefix: "Префикс BLE имени",
          label_locked_addr: "Фиксированный адрес (опц.)",
          label_discovered_extractors: "Найденные дымоуловители",
          pill_reachable: "Доступен",
          label_pairing_state: "Состояние спаривания",
          btn_pair: "Спарить",
          btn_unpair: "Отвязать",
          btn_read_status: "Статус",
          hint_extractor: "Регистрация привязывает один дымоуловитель для детерминированного управления.",
          section_hotair_bind: "Фен (H310/H312)",
          label_device_clock: "Часы устройства",
          hotair_state_not_bound: "Не привязан",
          hotair_state_bound: "Привязан",
          hotair_state_binding: "Привязка...",
          label_hotair_ip: "IP H310/H312",
          label_hotair_ble_addr: "BLE MAC H310/H312",
          label_hotair_temp_on: "Порог срабатывания температуры",
          btn_scan_hotair: "Скан H310/H312",
          btn_scan_hotair_ble: "Скан BLE H310/H312",
          btn_bind_hotair: "Привязать фен",
          btn_bind_hotair_ble: "Привязать H310/H312",
          btn_add_hotair: "Добавить фен",
          btn_remove_hotair: "Удалить фен",
          btn_reset_hotair: "Сброс H310/H312",
          msg_hotair_reset_confirm: "Сбросить конфигурацию H310/H312 (Wi-Fi и BLE)?",
          msg_hotair_reset_working: "Сброс конфигурации H310/H312...",
          msg_hotair_reset_ok: "Конфигурация H310/H312 сброшена",
          msg_hotair_reset_fail: "Ошибка сброса H310/H312",
          section_behavior_logic: "Логика работы",
          pill_core: "Ядро",
          label_power_condition: "Условие включения",
          label_triggers: "Активные источники",
          default_iron1: "Ручка 1",
          default_iron2: "Ручка 2",
          default_hot_air: "Фен",
          label_iron1_name: "Имя ручки 1 (T420D)",
          label_iron2_name: "Имя ручки 2 (T420D)",
          label_hot_air_name: "Имя фена (H310/H312)",
          label_light_mode: "Подсветка при активации",
          opt_light_off: "Выключено",
          opt_light_white: "Белый",
          opt_light_green: "Зелёный",
          opt_light_last: "Последняя включённая",
          label_light_brightness: "Яркость подсветки",
          value_light_brightness_last: "Последняя заданная яркость",
          value_light_brightness_manual: "Ручная яркость",
          label_light_brightness_manual: "Ручная яркость (0-100%)",
          label_t420d_h1_gpio: "GPIO ручки 1 (T420D)",
          label_t420d_h2_gpio: "GPIO ручки 2 (T420D)",
          opt_gpio_disabled: "Отключено",
          seg_h1_title: "ВКЛ если {h1} снят",
          seg_h2_title: "ВКЛ если {h2} снят",
          seg_ha_title: "ВКЛ если {ha} снят",
          label_start_delay: "Задержка старта",
          label_stop_delay: "Задержка остановки",
          fmt_h1: "{h1}",
          fmt_h2: "{h2}",
          fmt_ha: "{ha}",
          label_fail_safe: "Аварийный режим",
          label_kick_start: "Пусковой импульс",
          label_kick_start_level: "Уровень пуска",
          label_speed_restore: "Восстановление скорости",
          value_last_set_speed: "Последняя заданная скорость",
          value_manual_speed_mode: "Ручная скорость",
          label_manual_speed: "Ручная скорость",
          label_auto_stop: "Автоматическое завершение работы",
          label_auto_stop_enable: "Автовыключение",
          label_auto_stop_time: "Время (ЧЧ:ММ)",
          hint_auto_stop: "В это локальное время LB выключит мотор и подсветку.",
          btn_apply_logic: "Применить",
          section_iron1: "Паяльник 1: спаривание и калибровка ToF",
          pill_slot1: "Слот 1",
          section_iron2: "Паяльник 2: спаривание и калибровка ToF",
          pill_slot2: "Слот 2",
          label_pairing: "Спаривание",
          label_presence_sensor: "Датчик присутствия",
          opt_tof: "ToF (VL53-class)",
          opt_reed: "Геркон",
          opt_current: "Датчик тока",
          label_near: "Близко (мм)",
          label_far: "Далеко (мм)",
          label_hyst: "Гистерезис (мм)",
          btn_calibrate: "Калибровать",
          btn_test_presence: "Проверка",
          section_timing_behavior: "Тайминги и поведение",
          pill_tuning: "Настройка",
          label_start_delay_ms: "Задержка старта (мс)",
          label_stop_delay_ms: "Задержка остановки (мс)",
          label_kick_start_ms: "Пуск (мс)",
          opt_on: "ВКЛ",
          opt_off: "ВЫКЛ",
          label_max_speed: "Макс. скорость",
          label_min_speed: "Мин. скорость",
          btn_apply: "Применить",
          btn_reset_defaults: "Сбросить",
          section_apply_diag: "Применение и диагностика",
          pill_operations: "Операции",
          btn_fw_update: "Обновление ПО",
          fw_menu_title: "Обновление прошивки",
          fw_menu_text: "Обновить до последнего релиза или выбрать конкретную версию из GitHub releases.",
          btn_fw_latest: "Обновить до последней",
          btn_fw_choose_version: "Выбрать версию",
          btn_fw_online: "Онлайн",
          btn_fw_manual: "Вручную",
          btn_fw_github: "Посетить GitHub",
          btn_fw_cancel: "Понятно",
          fw_online_prompt: "Введите URL прошивки для онлайн-обновления",
          msg_fw_online_start: "онлайн-обновление запущено",
          msg_fw_online_ok: "онлайн-обновление завершено, перезагрузка",
          msg_fw_online_fail: "ошибка онлайн-обновления",
          msg_fw_online_working: "скачивание и прошивка...",
          msg_fw_online_requested: "выбрано онлайн-обновление (бэкенд пока не включен)",
          msg_fw_manual_pick: "ручное обновление: выберите файл прошивки",
          msg_fw_manual_selected: "файл прошивки выбран",
          msg_fw_manual_upload_start: "запуск ручной прошивки",
          msg_fw_manual_uploading: "загрузка прошивки...",
          msg_fw_manual_finishing: "завершение записи...",
          msg_fw_manual_ok: "ручное обновление завершено, перезагрузка",
          msg_fw_manual_fail: "ошибка ручного обновления",
          msg_fw_manual_need_recovery: "single-slot OTA: переход в Recovery Mode для прошивки",
          msg_fw_recovery_switch_ok: "запрошен вход в Recovery, перезагрузка",
          msg_fw_recovery_switch_fail: "не удалось перейти в Recovery Mode",
          msg_fw_status_idle: "Прошивка: ожидание",
          btn_reboot: "Перезагрузка",
          btn_export_log: "Экспорт лога (.txt)",
          btn_reset_settings: "Сброс настроек",
          label_web_auth: "Доступ к веб-интерфейсу",
          label_web_user: "Логин",
          label_web_pass_new: "Новый пароль",
          label_web_pass_repeat: "Повтор пароля",
          btn_web_auth_save: "Сохранить доступ",
          btn_web_auth_logout_all: "Разлогинить все устройства",
          msg_web_auth_saved: "Доступ обновлён",
          msg_web_auth_failed: "Не удалось обновить доступ",
          msg_web_auth_pass_mismatch: "Пароли не совпадают",
          msg_web_auth_pass_short: "Пароль должен быть не короче 8 символов",
          msg_web_auth_relogin: "Доступ изменён. Обновите страницу и войдите заново.",
          msg_web_auth_logout_all_confirm: "Разлогинить все запомненные устройства?",
          msg_web_auth_logout_all_ok: "Все запомненные устройства разлогинены",
          msg_web_auth_logout_all_fail: "Не удалось разлогинить устройства",
          prompt_confirm_pass: "Введите текущий пароль веб-доступа",
          msg_confirm_pass_required: "Операция отменена: пароль не введён",
          label_event_log: "Журнал событий",
          hint_diag: "",
          diag_bg_hue: "Оттенок фона (Hue)",
          diag_bg_sat: "Цветность",
          diag_bg_light: "Яркость",
          diag_bg_angle: "Угол градиента",
          diag_bg_hex: "Код цвета",
          fw_current: "Текущее ПО",
          fw_server: "ПО на сервере",
          footer_github: "Посетить GitHub",
          section_fw_map: "Карта разделов прошивки",
          pill_service: "Сервис",
          hint_fw_map: "Показывает bootloader, OTA слоты, NVS и готовность Recovery.",
          btn_switch_ota: "Переключить OTA слот",
          btn_verify_partitions: "Проверить разделы",
          btn_enter_recovery: "Войти в Recovery Mode",
          hint_fw_actions: "Вход в Recovery перезагрузит устройство.",
        },
        uk: {
          doc_title: "Legacy Bridge - Об’єднує станції та витяжку",
          env: "панель налаштувань",
          lang_label: "Мова",
          build: "збірка",
          hero_title: "Legacy Bridge",
          hero_sub: "Об’єднує станції та витяжку",
          hero_small: "Розроблено для зворотної сумісності",
          k_wifi: "Wi-Fi",
          k_ble: "BLE",
          k_mode: "Режим",
          k_extractor: "Димовловлювач",
          k_handle1: "Ручка 1",
          k_handle2: "Ручка 2",
          k_hot_air: "Фен",
          k_presence: "Присутність",
          k_failsafe: "Аварійний режим",
          k_firmware: "Прошивка",
          k_device: "Пристрій",
          k_bridge_link: "Зв'язок мосту",
          k_security: "Безпека",
          k_project: "Проєкт",
          k_profile: "Профіль",
          k_accent: "Акцент",
          v_no_network: "WIFI: DOWN",
          v_enabled: "УВІМК",
          v_ap_fallback: "РЕЖИМ AP",
          v_not_paired: "PAIR: NONE",
          v_idle: "ОЧІКУВАННЯ",
          v_armed: "АКТИВНО",
          v_ready: "ГОТОВО",
          v_local_only: "ЛИШЕ ЛОКАЛЬНО",
          v_industrial_dark: "Індустріальний темний",
          section_network: "Зв'язок і пристрої",
          pill_connectivity: "Зв'язок",
          section_devices: "Пристрої",
          pill_integration: "Інтеграція",
          label_device_lb: "Обладнання LB",
          label_device_third_party: "Стороннє обладнання",
          hint_devices: "Вмикай лише ті джерела, які реально використовуєш, щоб логіка була передбачуваною.",
          label_lb_model: "Модель",
          label_t420d_handles: "Ручки",
          pill_handle1: "Ручка 1",
          pill_handle2: "Ручка 2",
          label_wifi_scan: "Скан Wi-Fi",
          pill_known: "Відома",
          pill_auth: "Авторизація",
          pill_limited: "Обмежено",
          btn_scan: "Сканувати",
          btn_refresh: "Оновити",
          label_wifi_auto_off: "Вимкнути Wi-Fi через (хв)",
          hint_wifi_auto_off: "0 = ніколи. Вимикає Wi-Fi через N хвилин, щоб не світитися в мережі.",
          hint_wifi_pick_network: "Виберіть мережу Wi-Fi вище.",
          hint_wifi_enter_password: "Введіть пароль Wi-Fi.",
          msg_wifi_connecting: "Підключення...",
          msg_wifi_connected: "Підключено",
          msg_wifi_opening_page: "Відкриваю сторінку в локальній мережі...",
          msg_wifi_wait_switch: "Підключено. Чекаю, поки ПК переключиться на роутер...",
          msg_wifi_open_manual: "Не вдалося відкрити автоматично. Відкрийте вручну:",
          wifi_sta_summary_title: "Wi-Fi підключено",
          wifi_sta_summary_text: "Режим STA активний. Налаштування Wi-Fi приховано.",
          label_sta_ssid: "SSID роутера",
          label_sta_ip: "IP",
          btn_change_wifi: "Змінити Wi-Fi",
          btn_wifi_autooff_5: "Автовимк Wi-Fi через 5 хв",
          hint_wifi_autooff_locked_hotair_wifi: "Недоступно: фен використовує Wi-Fi.",
          msg_wifi_autooff5_set: "Автовимкнення Wi-Fi встановлено: 5 хв.",
          msg_wifi_autooff5_clear: "Автовимкнення Wi-Fi вимкнено.",
          msg_wifi_autooff5_fail: "Не вдалося встановити автовимкнення Wi-Fi",
          msg_wifi_shortcut_saved: "Посилання скопійовано",
          btn_install_app: "Встановити",
          btn_download_shortcut: "Додати на робочий стіл",
          btn_open_recovery: "Recovery",
          btn_logout: "Вихід",
          msg_logout_fail: "Не вдалося вийти",
          msg_shortcut_downloaded: "Посилання підготовлено",
          msg_shortcut_open_manual: "Скопіюйте посилання і створіть ярлик вручну",
          msg_shortcut_mobile_note: "На телефоні використайте меню браузера: Додати на головний екран.",
          msg_shortcut_copied: "Посилання скопійовано",
          msg_shortcut_copy_failed: "Не вдалося скопіювати. Використайте посилання вручну",
          msg_shortcut_win_steps: "Робочий стіл: ПКМ -> Створити -> Ярлик -> вставте посилання.",
          msg_shortcut_mac_steps: "Перетягніть URL з адресного рядка браузера на робочий стіл.",
          msg_shortcut_linux_steps: "Створіть launcher/ярлик і вкажіть цей URL.",
          msg_shortcut_phone_steps: "Використайте меню браузера: Додати на головний екран.",
          shortcut_offer_title: "Встановити ярлик на робочий стіл?",
          shortcut_offer_question: "Встановити ярлик LB-Control зараз?",
          shortcut_offer_detected: "Визначена ОС",
          shortcut_offer_windows_notice: "Для Windows використовується PowerShell. Авто-режим копіює одну команду. Ручний режим копіює повний скрипт. Запуск прямо з браузера потребує одноразової реєстрації протоколу.",
          shortcut_offer_mobile_hint: "На Android/iOS використайте меню браузера: Додати на головний екран.",
          shortcut_offer_macos_hint: "Виконайте команду в Terminal, щоб створити .webloc на робочому столі.",
          shortcut_offer_linux_hint: "Виконайте команду в терміналі, щоб створити .desktop ярлик.",
          shortcut_offer_other_hint: "Скопіюйте посилання і створіть ярлик вручну у вашій системі.",
          shortcut_offer_done: "Готово",
          shortcut_offer_auto_copied: "Команду авто-встановлення скопійовано. Відкрийте PowerShell і вставте її.",
          shortcut_offer_protocol_try: "Пробую запуск через браузерний протокол...",
          shortcut_offer_script_copied: "Скрипт скопійовано. Вставте і виконайте в терміналі.",
          shortcut_offer_copy_failed: "Копіювання заблоковано. Скопіюйте текст вручну з поля нижче.",
          shortcut_offer_pwa_started: "Якщо браузер підтримує встановлення, вікно встановлення вже відкрито.",
          shortcut_offer_pwa_not_available: "Авто-встановлення недоступне. Використайте меню браузера:",
          shortcut_offer_pwa_hint_win: "Chrome: меню (три крапки) -> Зберегти і поділитися -> Встановити застосунок.",
          shortcut_offer_pwa_hint_macos: "Chrome: меню (три крапки) -> Встановити застосунок.",
          shortcut_offer_pwa_hint_linux: "Chrome: меню (три крапки) -> Встановити застосунок.",
          shortcut_offer_pwa_hint_mobile: "Меню браузера -> Додати на головний екран.",
          btn_yes: "Так",
          btn_no: "Ні",
          btn_auto_ps: "Авто (PowerShell)",
          btn_copy_script: "Копіювати скрипт",
          install_offer_text: "Встановіть LB-Control для швидкого доступу.",
          btn_later: "Пізніше",
          ap_switch_title: "ПЕРЕПІДКЛЮЧІТЬСЯ ДО ВАШОГО РОУТЕРА",
          ap_switch_text: "Пристрій підключено. Переключіть ПК/телефон з LB-SETUP на Wi-Fi вашого роутера.",
          btn_check_connection: "Перевірити підключення",
          btn_hide: "Сховати",
          msg_install_no_prompt: "Автовстановлення PWA недоступне в цьому режимі.",
          msg_install_using_menu: "Відкрийте меню браузера: Встановити застосунок / Створити ярлик.",
          msg_wifi_failed: "Не вдалося підключитися",
          msg_wifi_hidden_cannot: "Прихована мережа: введіть SSID вручну.",
          msg_wifi_scan_failed: "Помилка сканування Wi-Fi",
          msg_wifi_scan_unavailable: "Сканування тимчасово недоступне",
          msg_wifi_no_networks: "Мережі не знайдено",
          msg_wifi_press_scan: "Натисніть Сканувати",
          msg_wifi_scanning: "Сканування Wi-Fi...",
          msg_wifi_scan_retry: "Натисніть Сканувати ще раз",
          unit_min: "хв",
          label_ssid: "SSID",
          label_security: "Захист",
          opt_wpa2psk: "WPA2-PSK",
          opt_open: "Відкрита",
          opt_wpa2ent: "WPA2-Enterprise",
          label_password: "Пароль",
          btn_connect: "Підключити",
          btn_disconnect: "Відключити",
          label_ble: "BLE",
          hint_ble_auto: "BLE використовується лише для керування димовловлювачем. Автопідключення після вибору.",
          opt_enabled: "Увімк",
          opt_disabled: "Вимк",
          label_discovery: "Пошук",
          opt_active: "Активний",
          opt_inactive: "Неактивний",
          opt_passive: "Пасивний",
          btn_discover: "Знайти",
          btn_rescan: "Повторити",
          section_extractor_reg: "Димовловлювач (ES02)",
          label_extractor: "Димовловлювач",
          label_status: "Статус",
          label_found: "Знайдено",
          label_found_devices: "Знайдені пристрої",
          v_connected: "Підключено",
          v_not_found: "Не знайдено",
          btn_find_extractor: "Знайти димовловлювач",
          btn_connect_extractor: "Підключити",
          btn_save: "Зберегти",
          btn_change: "Змінити",
          pill_device_binding: "Прив'язка",
          label_ble_prefix: "Префікс BLE імені",
          label_locked_addr: "Фіксована адреса (опц.)",
          label_discovered_extractors: "Знайдені димовловлювачі",
          pill_reachable: "Доступний",
          label_pairing_state: "Стан спарювання",
          btn_pair: "Спарити",
          btn_unpair: "Відв'язати",
          btn_read_status: "Статус",
          hint_extractor: "Реєстрація прив'язує один димовловлювач для детермінованого керування.",
          section_hotair_bind: "Фен (H310/H312)",
          label_device_clock: "Годинник пристрою",
          hotair_state_not_bound: "Не прив'язано",
          hotair_state_bound: "Прив'язано",
          hotair_state_binding: "Прив'язка...",
          label_hotair_ip: "IP H310/H312",
          label_hotair_ble_addr: "BLE MAC H310/H312",
          label_hotair_temp_on: "Поріг спрацювання температури",
          btn_scan_hotair: "Скан H310/H312",
          btn_scan_hotair_ble: "Скан BLE H310/H312",
          btn_bind_hotair: "Прив'язати фен",
          btn_bind_hotair_ble: "Прив'язати H310/H312",
          btn_add_hotair: "Додати фен",
          btn_remove_hotair: "Видалити фен",
          btn_reset_hotair: "Скидання H310/H312",
          msg_hotair_reset_confirm: "Скинути конфігурацію H310/H312 (Wi-Fi і BLE)?",
          msg_hotair_reset_working: "Скидання конфігурації H310/H312...",
          msg_hotair_reset_ok: "Конфігурацію H310/H312 скинуто",
          msg_hotair_reset_fail: "Помилка скидання H310/H312",
          section_behavior_logic: "Логіка роботи",
          pill_core: "Ядро",
          label_power_condition: "Умова увімкнення",
          label_triggers: "Активні джерела",
          default_iron1: "Ручка 1",
          default_iron2: "Ручка 2",
          default_hot_air: "Фен",
          label_iron1_name: "Назва ручки 1 (T420D)",
          label_iron2_name: "Назва ручки 2 (T420D)",
          label_hot_air_name: "Назва фена (H310/H312)",
          label_light_mode: "Підсвітка при активації",
          opt_light_off: "Вимкнено",
          opt_light_white: "Білий",
          opt_light_green: "Зелений",
          opt_light_last: "Остання увімкнена",
          label_light_brightness: "Яскравість підсвітки",
          value_light_brightness_last: "Остання задана яскравість",
          value_light_brightness_manual: "Ручна яскравість",
          label_light_brightness_manual: "Ручна яскравість (0-100%)",
          label_t420d_h1_gpio: "GPIO ручки 1 (T420D)",
          label_t420d_h2_gpio: "GPIO ручки 2 (T420D)",
          opt_gpio_disabled: "Вимкнено",
          seg_h1_title: "УВІМК якщо {h1} знято",
          seg_h2_title: "УВІМК якщо {h2} знято",
          seg_ha_title: "УВІМК якщо {ha} знято",
          label_start_delay: "Затримка старту",
          label_stop_delay: "Затримка зупинки",
          fmt_h1: "{h1}",
          fmt_h2: "{h2}",
          fmt_ha: "{ha}",
          label_fail_safe: "Аварійний режим",
          label_kick_start: "Пусковий імпульс",
          label_kick_start_level: "Рівень пуску",
          label_speed_restore: "Відновлення швидкості",
          value_last_set_speed: "Остання задана швидкість",
          value_manual_speed_mode: "Ручна швидкість",
          label_manual_speed: "Ручна швидкість",
          label_auto_stop: "Автоматичне завершення роботи",
          label_auto_stop_enable: "Автовимкнення",
          label_auto_stop_time: "Час (ГГ:ХХ)",
          hint_auto_stop: "У цей локальний час LB вимкне мотор і підсвітку.",
          btn_apply_logic: "Застосувати",
          section_iron1: "Паяльник 1: спарювання та калібрування ToF",
          pill_slot1: "Слот 1",
          section_iron2: "Паяльник 2: спарювання та калібрування ToF",
          pill_slot2: "Слот 2",
          label_pairing: "Спарювання",
          label_presence_sensor: "Датчик присутності",
          opt_tof: "ToF (VL53-class)",
          opt_reed: "Геркон",
          opt_current: "Датчик струму",
          label_near: "Близько (мм)",
          label_far: "Далеко (мм)",
          label_hyst: "Гістерезис (мм)",
          btn_calibrate: "Калібрувати",
          btn_test_presence: "Перевірка",
          section_timing_behavior: "Таймінги та поведінка",
          pill_tuning: "Налаштування",
          label_start_delay_ms: "Затримка старту (мс)",
          label_stop_delay_ms: "Затримка зупинки (мс)",
          label_kick_start_ms: "Пуск (мс)",
          opt_on: "УВІМК",
          opt_off: "ВИМК",
          label_max_speed: "Макс. швидкість",
          label_min_speed: "Мін. швидкість",
          btn_apply: "Застосувати",
          btn_reset_defaults: "Скинути",
          section_apply_diag: "Застосування та діагностика",
          pill_operations: "Операції",
          btn_fw_update: "Оновлення ПЗ",
          fw_menu_title: "Оновлення прошивки",
          fw_menu_text: "Оновити до останнього релізу або вибрати конкретну версію з GitHub releases.",
          btn_fw_latest: "Оновити до останньої",
          btn_fw_choose_version: "Обрати версію",
          btn_fw_online: "Онлайн",
          btn_fw_manual: "Вручну",
          btn_fw_github: "Відвідати GitHub",
          btn_fw_cancel: "Зрозуміло",
          fw_online_prompt: "Вкажіть URL прошивки для онлайн-оновлення",
          msg_fw_online_start: "онлайн-оновлення запущено",
          msg_fw_online_ok: "онлайн-оновлення завершено, перезавантаження",
          msg_fw_online_fail: "помилка онлайн-оновлення",
          msg_fw_online_working: "завантаження і прошивка...",
          msg_fw_online_requested: "обрано онлайн-оновлення (бекенд ще не увімкнений)",
          msg_fw_manual_pick: "ручне оновлення: виберіть файл прошивки",
          msg_fw_manual_selected: "файл прошивки обрано",
          msg_fw_manual_upload_start: "запуск ручного оновлення",
          msg_fw_manual_uploading: "завантаження прошивки...",
          msg_fw_manual_finishing: "завершення запису...",
          msg_fw_manual_ok: "ручне оновлення завершено, перезавантаження",
          msg_fw_manual_fail: "помилка ручного оновлення",
          msg_fw_manual_need_recovery: "single-slot OTA: перехід у Recovery Mode для прошивки",
          msg_fw_recovery_switch_ok: "запитано вхід у Recovery, перезавантаження",
          msg_fw_recovery_switch_fail: "не вдалося перейти у Recovery Mode",
          msg_fw_status_idle: "Прошивка: очікування",
          btn_reboot: "Перезавантаження",
          btn_export_log: "Експорт логу (.txt)",
          btn_reset_settings: "Скидання налаштувань",
          label_web_auth: "Доступ до веб-інтерфейсу",
          label_web_user: "Логін",
          label_web_pass_new: "Новий пароль",
          label_web_pass_repeat: "Повтор пароля",
          btn_web_auth_save: "Зберегти доступ",
          btn_web_auth_logout_all: "Розлогінити всі пристрої",
          msg_web_auth_saved: "Доступ оновлено",
          msg_web_auth_failed: "Не вдалося оновити доступ",
          msg_web_auth_pass_mismatch: "Паролі не співпадають",
          msg_web_auth_pass_short: "Пароль має бути не коротший за 8 символів",
          msg_web_auth_relogin: "Доступ змінено. Оновіть сторінку та увійдіть знову.",
          msg_web_auth_logout_all_confirm: "Розлогінити всі запамʼятані пристрої?",
          msg_web_auth_logout_all_ok: "Усі запамʼятані пристрої розлогінені",
          msg_web_auth_logout_all_fail: "Не вдалося розлогінити пристрої",
          prompt_confirm_pass: "Введіть поточний пароль веб-доступу",
          msg_confirm_pass_required: "Операцію скасовано: пароль не введено",
          label_event_log: "Журнал подій",
          hint_diag: "",
          diag_bg_hue: "Відтінок фону (Hue)",
          diag_bg_sat: "Кольоровість",
          diag_bg_light: "Яскравість",
          diag_bg_angle: "Кут градієнта",
          diag_bg_hex: "Код кольору",
          fw_current: "Поточне ПЗ",
          fw_server: "ПЗ на сервері",
          footer_github: "Відвідати GitHub",
          section_fw_map: "Карта розділів прошивки",
          pill_service: "Сервіс",
          hint_fw_map: "Показує bootloader, OTA слоти, NVS та готовність Recovery.",
          btn_switch_ota: "Перемкнути OTA слот",
          btn_verify_partitions: "Перевірити розділи",
          btn_enter_recovery: "Увійти в Recovery Mode",
          hint_fw_actions: "Вхід у Recovery перезавантажить пристрій.",
        },
      };
      function normalizeLang(lang) {
        if (!lang) return "en";
        const l = String(lang).toLowerCase();
        if (l.startsWith("ru")) return "ru";
        if (l.startsWith("uk") || l.startsWith("ua")) return "uk";
        return "en";
      }
      function applyLang(lang) {
        const pack = i18n[lang] || i18n.en;
        document.documentElement.lang = lang;
        if (pack.doc_title) document.title = pack.doc_title;
        try { localStorage.setItem(STORAGE_KEY, lang); } catch (_) {}
        document.querySelectorAll("[data-i18n]").forEach((el) => {
          const key = el.getAttribute("data-i18n");
          const v = pack[key];
          if (typeof v === "string") el.textContent = v;
        });
        document.querySelectorAll("[data-i18n-value]").forEach((el) => {
          const key = el.getAttribute("data-i18n-value");
          const v = pack[key];
          if (typeof v === "string") el.value = v;
        });
        renderFormatted(pack);
      }
      const sel = document.getElementById("lang");
      const iron1Input = document.getElementById("iron1Name"); // T420D handle 1
      const iron2Input = document.getElementById("iron2Name"); // T420D handle 2
      const hotAirInput = document.getElementById("hotAirName"); // H312
      const initial = normalizeLang((function () {
        try { return localStorage.getItem(STORAGE_KEY); } catch (_) {}
        return navigator.language || "en";
      })());
      function tr(key, fallback) {
        const lang = normalizeLang((sel && sel.value) ? sel.value : initial);
        const pack = i18n[lang] || i18n.en;
        const v = pack[key];
        return (typeof v === "string") ? v : fallback;
      }
      function getStoredName(key) {
        try { return localStorage.getItem(key) || ""; } catch (_) { return ""; }
      }
      function setStoredName(key, value) {
        try { localStorage.setItem(key, value); } catch (_) {}
      }
      function setTogglePressed(btn, on, opts) {
        if (!btn) return;
        const cfg = opts || {};
        const onClass = String(cfg.onClass || "active");
        const offClass = String(cfg.offClass || "");
        btn.classList.toggle(onClass, !!on);
        if (offClass) btn.classList.toggle(offClass, !on);
        btn.setAttribute("aria-pressed", on ? "true" : "false");
      }
      function isTogglePressed(btn, opts) {
        if (!btn) return false;
        const cfg = opts || {};
        const onClass = String(cfg.onClass || "active");
        return btn.classList.contains(onClass);
      }
      function bindMomentaryPressed(el, holdMs) {
        if (!el) return;
        const ms = Math.max(100, Math.min(600, Number(holdMs) || 180));
        let releaseTimer = 0;
        function clearReleaseTimer() {
          if (releaseTimer) {
            clearTimeout(releaseTimer);
            releaseTimer = 0;
          }
        }
        function setPressed(on) {
          el.classList.toggle("is-pressed", !!on);
        }
        function press() {
          clearReleaseTimer();
          setPressed(true);
        }
        function release() {
          clearReleaseTimer();
          releaseTimer = setTimeout(() => {
            releaseTimer = 0;
            setPressed(false);
          }, ms);
        }
        function hardClear() {
          clearReleaseTimer();
          setPressed(false);
        }
        el.addEventListener("pointerdown", press, { passive: true });
        el.addEventListener("pointerup", release, { passive: true });
        el.addEventListener("pointerleave", release, { passive: true });
        el.addEventListener("pointercancel", hardClear, { passive: true });
        el.addEventListener("blur", hardClear);
        el.addEventListener("keydown", (ev) => {
          if (ev.key === "Enter" || ev.key === " ") press();
        });
        el.addEventListener("keyup", (ev) => {
          if (ev.key === "Enter" || ev.key === " ") release();
        });
      }
      function initPremiumRanges(root) {
        const scope = root || document;
        const ranges = scope.querySelectorAll('input[type="range"]');
        const syncOne = (el) => {
          const min = Number(el.min || 0);
          const max = Number(el.max || 100);
          const cur = Number(el.value || 0);
          const span = (max - min) || 1;
          const pct = Math.max(0, Math.min(100, ((cur - min) * 100) / span));
          el.style.setProperty("--range-pct", `${pct}%`);
        };
        ranges.forEach((el) => {
          syncOne(el);
          const onInput = () => syncOne(el);
          el.addEventListener("input", onInput);
          el.addEventListener("change", onInput);
        });
      }
      function renderFormatted(pack) {
        const h1 = (iron1Input && iron1Input.value.trim()) || pack.default_iron1 || "Handle 1";
        const h2 = (iron2Input && iron2Input.value.trim()) || pack.default_iron2 || "Handle 2";
        const ha = (hotAirInput && hotAirInput.value.trim()) || pack.default_hot_air || "Hot Air";
        document.querySelectorAll("[data-i18n-fmt]").forEach((el) => {
          const key = el.getAttribute("data-i18n-fmt");
          const tpl = pack[key];
          if (typeof tpl !== "string") return;
          el.textContent = tpl
            .replaceAll("{h1}", h1)
            .replaceAll("{h2}", h2)
            .replaceAll("{ha}", ha);
        });
      }
      function initWaveClothBackground() {
        const THREE = window.THREE;
        const canvas = document.getElementById("bg3d");
        if (!THREE || !canvas) return;
        function readVar(name, fallback) {
          const v = getComputedStyle(document.documentElement).getPropertyValue(name).trim();
          return v || fallback;
        }
        function hexToRgb01(hex) {
          const s = String(hex || "").trim();
          const m = /^#?([0-9a-f]{6})$/i.exec(s);
          if (!m) return { r: 0, g: 0, b: 0 };
          const n = parseInt(m[1], 16);
          return { r: ((n >> 16) & 255) / 255, g: ((n >> 8) & 255) / 255, b: (n & 255) / 255 };
        }
        function readTintAlpha() {
          const tint = hexToRgb01(readVar("--wave-tint", "#dfe6ee"));
          const alpha = Math.max(0, Math.min(1, parseFloat(readVar("--wave-alpha", "0.55")) || 0.55));
          return { tint, alpha };
        }
        const initTA = readTintAlpha();
        const tint = initTA.tint;
        const alpha = initTA.alpha;
        const waveClothVertexShader = `
          float hash(float n){return fract(sin(n)*1e4);}
          float hash(vec2 p){return fract(1e4*sin(17.*p.x+p.y*.1)*(.1+abs(sin(p.y*13.+p.x))));}
          float noise(vec2 x){
            vec2 i=floor(x);
            vec2 f=fract(x);
            float a=hash(i);
            float b=hash(i+vec2(1.,0.));
            float c=hash(i+vec2(0.,1.));
            float d=hash(i+vec2(1.,1.));
            vec2 u=f*f*(3.-2.*f);
            return mix(a,b,u.x)+(c-a)*u.y*(1.-u.x)+(d-b)*u.x*u.y;
          }
          float noise(vec3 x){
            const vec3 step=vec3(110,241,171);
            vec3 i=floor(x);
            vec3 f=fract(x);
            float n=dot(i,step);
            vec3 u=f*f*(3.-2.*f);
            return mix(
              mix(
                mix(hash(n+dot(step,vec3(0,0,0))),hash(n+dot(step,vec3(1,0,0))),u.x),
                mix(hash(n+dot(step,vec3(0,1,0))),hash(n+dot(step,vec3(1,1,0))),u.x),
                u.y
              ),
              mix(
                mix(hash(n+dot(step,vec3(0,0,1))),hash(n+dot(step,vec3(1,0,1))),u.x),
                mix(hash(n+dot(step,vec3(0,1,1))),hash(n+dot(step,vec3(1,1,1))),u.x),
                u.y
              ),
              u.z
            );
          }
          uniform float uTime;
          varying vec2 vUv;
          varying vec3 vPosition;
          float xmbNoise(vec3 x){
            return cos(x.z*4.)*cos(x.z+uTime/10.+x.x);
          }
          void main(){
            vec3 p=vec3(position.x,0.,position.y);
            p.y=xmbNoise(p)/8.;
            vec3 p2=p;
            p2.x-=uTime/5.;
            p2.x/=4.;
            p2.y-=uTime/100.;
            p2.z-=uTime/10.;
            p.y-=noise(p2*8.)/12.+cos(p.x*2.-uTime/2.)/5.-.3;
            p.z-=noise(p2*8.)/12.;
            vec4 modelPosition=modelMatrix*vec4(p,1.);
            vec4 viewPosition=viewMatrix*modelPosition;
            vec4 projectedPosition=projectionMatrix*viewPosition;
            gl_Position=projectedPosition;
            vUv=uv;
            vPosition=p;
          }
        `;
        const waveClothFragmentShader = `
          precision mediump float;
          vec3 computeNormal(vec3 normal){
            vec3 X=dFdx(normal);
            vec3 Y=dFdy(normal);
            vec3 cNormal=normalize(cross(X,Y));
            return cNormal;
          }
          float fresnel(float bias,float scale,float power,vec3 I,vec3 N){
            return bias+scale*pow(1.+dot(I,N),power);
          }
          uniform float uTime;
          uniform vec2 uResolution;
          uniform vec3 uTint;
          uniform float uAlpha;
          varying vec2 vUv;
          varying vec3 vPosition;
          void main(){
            vec3 cNormal=computeNormal(vPosition);
            vec3 eyeVector=vec3(0.,0.,-1.);
            float F=fresnel(0.,.5,4.,eyeVector,cNormal);
            float rim = pow(F, 1.25);
            float band = 1.0 - abs(vUv.y - 0.5) * 2.0;   // 0 at edges, 1 at center
            band = clamp(band, 0.0, 1.0);
            band = smoothstep(0.0, 1.0, band);
            float a = (rim * 0.85 + band * 0.35) * uAlpha;
            a = clamp(a, 0.0, 1.0);
            gl_FragColor=vec4(uTint, a);
          }
        `;
        let renderer;
        try {
          renderer = new THREE.WebGLRenderer({
            canvas,
            alpha: true,
            antialias: true,
            powerPreference: "high-performance",
          });
        } catch (_) {
          return;
        }
        renderer.setPixelRatio(Math.min(window.devicePixelRatio || 1, 1.5));
        renderer.setSize(window.innerWidth, window.innerHeight, false);
        renderer.setClearColor(0x000000, 0);
        const scene = new THREE.Scene();
        const aspect = (window.innerWidth || 1) / (window.innerHeight || 1);
        const camera = new THREE.OrthographicCamera(-aspect, aspect, 1, -1, -100, 1000);
        camera.position.set(0, 0, 2);
        camera.lookAt(0, 0, 0);
        const mat = new THREE.ShaderMaterial({
          vertexShader: waveClothVertexShader,
          fragmentShader: waveClothFragmentShader,
          side: THREE.DoubleSide,
          transparent: true,
          depthTest: false,
          uniforms: {
            uTime: { value: 0 },
            uResolution: { value: new THREE.Vector2(window.innerWidth, window.innerHeight) },
            uTint: { value: new THREE.Vector3(tint.r, tint.g, tint.b) },
            uAlpha: { value: alpha },
          },
        });
        function refreshStyleTint() {
          const ta = readTintAlpha();
          mat.uniforms.uTint.value.set(ta.tint.r, ta.tint.g, ta.tint.b);
          mat.uniforms.uAlpha.value = ta.alpha;
        }
        const geometry = new THREE.PlaneGeometry(20, 3, 256, 128);
        const mesh = new THREE.Mesh(geometry, mat);
        mesh.position.y = 0.02;
        scene.add(mesh);
        const reduceMotion = !!(window.matchMedia && window.matchMedia("(prefers-reduced-motion: reduce)").matches);
        const start = performance.now();
        let raf = 0;
        let running = !reduceMotion;
        function render(now) {
          const t = (now - start) * 0.001;
          mat.uniforms.uTime.value = t;
          renderer.render(scene, camera);
          if (running) raf = requestAnimationFrame(render);
        }
        function resize() {
          const w = window.innerWidth || 1;
          const h = window.innerHeight || 1;
          const a = w / h;
          renderer.setPixelRatio(Math.min(window.devicePixelRatio || 1, 1.5));
          renderer.setSize(w, h, false);
          mat.uniforms.uResolution.value.set(w, h);
          camera.left = -a;
          camera.right = a;
          camera.top = 1;
          camera.bottom = -1;
          camera.updateProjectionMatrix();
          renderer.render(scene, camera);
        }
        window.addEventListener("resize", resize, { passive: true });
        window.addEventListener("alb-theme", refreshStyleTint);
        document.addEventListener("visibilitychange", () => {
          if (reduceMotion) return;
          if (document.hidden) {
            running = false;
            if (raf) cancelAnimationFrame(raf);
            raf = 0;
          } else if (!running) {
            running = true;
            raf = requestAnimationFrame(render);
          }
        });
        renderer.render(scene, camera);
        if (!reduceMotion) raf = requestAnimationFrame(render);
      }
      function initNames(pack) {
        if (iron1Input) {
          const s1 = getStoredName("alb_h1_name") || getStoredName("alb_iron1_name");
          iron1Input.value = s1 ? s1 : (pack.default_iron1 || "Handle 1");
          iron1Input.addEventListener("input", () => {
            setStoredName("alb_h1_name", iron1Input.value);
            renderFormatted(i18n[normalizeLang(sel.value)] || i18n.en);
          });
        }
        if (iron2Input) {
          const s2 = getStoredName("alb_h2_name") || getStoredName("alb_iron2_name");
          iron2Input.value = s2 ? s2 : (pack.default_iron2 || "Handle 2");
          iron2Input.addEventListener("input", () => {
            setStoredName("alb_h2_name", iron2Input.value);
            renderFormatted(i18n[normalizeLang(sel.value)] || i18n.en);
          });
        }
        if (hotAirInput) {
          const s3 = getStoredName("alb_ha_name");
          hotAirInput.value = s3 ? s3 : (pack.default_hot_air || "Hot Air");
          hotAirInput.addEventListener("input", () => {
            setStoredName("alb_ha_name", hotAirInput.value);
            renderFormatted(i18n[normalizeLang(sel.value)] || i18n.en);
          });
        }
      }
      function initDeviceTile() {
        const lbActive = document.getElementById("devLBActive");
        const lbInactive = document.getElementById("devLBInactive");
        const thirdActive = document.getElementById("devThirdActive");
        const thirdInactive = document.getElementById("devThirdInactive");
        const modelRow = document.getElementById("lbModelRow");
        const extractorTile = document.getElementById("extractorTile");
        const hotAirTile = document.getElementById("hotAirTile");
        const btnES02 = document.getElementById("lbES02");
        const btnT420D = document.getElementById("lbT420D");
        const btnH312 = document.getElementById("lbH312");
        const h1LiftPill = document.getElementById("h1LiftPill");
        const h2LiftPill = document.getElementById("h2LiftPill");
        const btnCalH1 = document.getElementById("btnCalH1");
        const btnCalH2 = document.getElementById("btnCalH2");
        const hasLBToggle = !!(lbActive && lbInactive);
        let handleLiftInFlight = false;
        let handleCalInFlight = false;
        function isLBEnabled() {
          return hasLBToggle ? lbActive.classList.contains("active") : true;
        }
        function load(key, defaultVal) {
          try {
            const v = localStorage.getItem(key);
            return (v === "0" || v === "1") ? v : defaultVal;
          } catch (_) {
            return defaultVal;
          }
        }
        function loadStr(key, defaultVal) {
          try { return localStorage.getItem(key) || defaultVal; } catch (_) { return defaultVal; }
        }
        function save(key, v) {
          try { localStorage.setItem(key, v); } catch (_) {}
        }
        function renderLBButtons(es02On, t420dOn, h312On) {
          if (btnES02) {
            setTogglePressed(btnES02, !!es02On);
          }
          if (btnT420D) {
            setTogglePressed(btnT420D, !!t420dOn);
          }
          if (btnH312) {
            setTogglePressed(btnH312, !!h312On);
          }
        }
        function renderLBModelVisibility() {
          if (!modelRow) return;
          modelRow.classList.toggle("hidden", !isLBEnabled());
        }
        function renderDeviceTileVisibility() {
          const lbEnabled = isLBEnabled();
          const es02Enabled = !!(btnES02 && btnES02.classList.contains("active"));
          const h312Enabled = !!(btnH312 && btnH312.classList.contains("active"));
          if (extractorTile) extractorTile.classList.toggle("hidden", !lbEnabled || !es02Enabled);
          if (hotAirTile) hotAirTile.classList.toggle("hidden", !lbEnabled || !h312Enabled);
        }
        function renderLogicVisibility() {
          const logicCard = document.getElementById("logicCard");
          const w1 = document.getElementById("logicIron1Wrap");
          const w2 = document.getElementById("logicIron2Wrap");
          const wHa = document.getElementById("logicHotAirWrap");
          const wHaTemp = document.getElementById("logicHotAirTempWrap");
          const b1 = document.getElementById("senseH1");
          const b2 = document.getElementById("senseH2");
          const bHa = document.getElementById("senseHA");
          const sdH1 = document.getElementById("sdH1Wrap");
          const sdH2 = document.getElementById("sdH2Wrap");
          const sdHA = document.getElementById("sdHAWrap");
          const stH1 = document.getElementById("stH1Wrap");
          const stH2 = document.getElementById("stH2Wrap");
          const stHA = document.getElementById("stHAWrap");
          const ltH1 = document.getElementById("lightH1Wrap");
          const ltH2 = document.getElementById("lightH2Wrap");
          const ltHA = document.getElementById("lightHAWrap");
          const lbH1 = document.getElementById("lbH1Wrap");
          const lbH2 = document.getElementById("lbH2Wrap");
          const lbHA = document.getElementById("lbHAWrap");
          const gpioH1 = document.getElementById("gpioH1Wrap");
          const gpioH2 = document.getElementById("gpioH2Wrap");
          if (!logicCard) return;
          const lbEnabled = isLBEnabled();
          const t420dEnabled = !!(btnT420D && btnT420D.classList.contains("active"));
          const h312Enabled = !!(btnH312 && btnH312.classList.contains("active"));
          const h1Enabled = t420dEnabled;
          const h2Enabled = t420dEnabled;
          const sources = (t420dEnabled ? 2 : 0) + (h312Enabled ? 1 : 0);
          logicCard.classList.toggle("hidden", !lbEnabled || sources === 0);
          if (!lbEnabled) return;
          if (w1) w1.classList.toggle("hidden", !h1Enabled);
          if (b1) b1.classList.toggle("hidden", !h1Enabled);
          if (w2) w2.classList.toggle("hidden", !h2Enabled);
          if (b2) b2.classList.toggle("hidden", !h2Enabled);
          if (wHa) wHa.classList.toggle("hidden", !h312Enabled);
          if (wHaTemp) wHaTemp.classList.toggle("hidden", !h312Enabled);
          if (bHa) bHa.classList.toggle("hidden", !h312Enabled);
          if (sdH1) sdH1.classList.toggle("hidden", !h1Enabled);
          if (sdH2) sdH2.classList.toggle("hidden", !h2Enabled);
          if (sdHA) sdHA.classList.toggle("hidden", !h312Enabled);
          if (stH1) stH1.classList.toggle("hidden", !h1Enabled);
          if (stH2) stH2.classList.toggle("hidden", !h2Enabled);
          if (stHA) stHA.classList.toggle("hidden", !h312Enabled);
          if (ltH1) ltH1.classList.toggle("hidden", !h1Enabled);
          if (ltH2) ltH2.classList.toggle("hidden", !h2Enabled);
          if (ltHA) ltHA.classList.toggle("hidden", !h312Enabled);
          if (lbH1) lbH1.classList.toggle("hidden", !h1Enabled);
          if (lbH2) lbH2.classList.toggle("hidden", !h2Enabled);
          if (lbHA) lbHA.classList.toggle("hidden", !h312Enabled);
          if (gpioH1) gpioH1.classList.toggle("hidden", !h1Enabled);
          if (gpioH2) gpioH2.classList.toggle("hidden", !h2Enabled);
          if (typeof window.__refreshManualSpeedVisibility === "function") {
            window.__refreshManualSpeedVisibility();
          }
          if (typeof window.__refreshLightBrightnessVisibility === "function") {
            window.__refreshLightBrightnessVisibility();
          }
          renderDeviceTileVisibility();
        }
        function renderTofVisibility() {
          const enabled = !!(thirdActive && thirdActive.classList.contains("active"));
          document.querySelectorAll(".tofOnly").forEach((el) => el.classList.toggle("hidden", !enabled));
          if (!enabled) {
            const iron1Card = document.getElementById("iron1Card");
            const iron2Card = document.getElementById("iron2Card");
            if (iron1Card) iron1Card.classList.add("hidden");
            if (iron2Card) iron2Card.classList.add("hidden");
          }
          document.querySelectorAll("select.presenceSel").forEach((selEl) => {
            const tofOpt = selEl.querySelector("option[data-tof-option='1']");
            if (tofOpt) tofOpt.hidden = !enabled;
            if (!enabled && tofOpt && selEl.value === tofOpt.value) {
              const reed = Array.from(selEl.options).find((o) => !o.hidden && /reed/i.test(o.textContent || ""));
              if (reed) selEl.value = reed.value;
              else {
                const firstVisible = Array.from(selEl.options).find((o) => !o.hidden);
                if (firstVisible) selEl.value = firstVisible.value;
              }
            }
          });
        }
        const a = load("alb_dev_lb", "1");
        function setLBEnabled(on) {
          if (!hasLBToggle) return;
          const v = !!on;
          setTogglePressed(lbActive, v);
          setTogglePressed(lbInactive, !v);
        }
        if (hasLBToggle) setLBEnabled(a !== "0");
        else save("alb_dev_lb", "1");
        const t = load("alb_dev_third", "1");
        function setThirdEnabled(on) {
          if (!thirdActive || !thirdInactive) return;
          const v = !!on;
          setTogglePressed(thirdActive, v);
          setTogglePressed(thirdInactive, !v);
        }
        if (thirdActive && thirdInactive) setThirdEnabled(t !== "0");
        let es02 = loadStr("alb_lb_es02", "");
        let t420d = loadStr("alb_lb_t420d", "");
        let h312 = loadStr("alb_lb_h312", "");
        const legacy = loadStr("alb_lb_model", "T420D");
        if (es02 !== "0" && es02 !== "1") {
          es02 = "1";
        }
        if (t420d !== "0" && t420d !== "1") {
          t420d = (legacy === "H312") ? "0" : "1";
        }
        if (h312 !== "0" && h312 !== "1") {
          h312 = (legacy === "H312") ? "1" : "0";
        }
        if (es02 !== "0" && es02 !== "1") es02 = "1";
        if (t420d !== "0" && t420d !== "1") t420d = "1";
        if (h312 !== "0" && h312 !== "1") h312 = "0";
        save("alb_lb_es02", es02);
        save("alb_lb_t420d", t420d);
        save("alb_lb_h312", h312);
        renderLBButtons(es02 === "1", t420d === "1", h312 === "1");
        if (btnES02) {
          btnES02.addEventListener("click", () => {
            const next = btnES02.classList.contains("active") ? "0" : "1";
            save("alb_lb_es02", next);
            renderLBButtons(
              next === "1",
              (loadStr("alb_lb_t420d", t420d) === "1"),
              (loadStr("alb_lb_h312", h312) === "1")
            );
            renderLBModelVisibility();
            renderLogicVisibility();
          });
        }
        function toRaised(v) {
          if (typeof v === "boolean") return v;
          if (typeof v === "number") return v !== 0;
          if (typeof v === "string") {
            const s = v.trim().toLowerCase();
            return s === "1" || s === "true" || s === "up" || s === "raised" || s === "on";
          }
          return false;
        }
        function setLiftPill(el, raised, idx) {
          if (!el) return;
          el.className = `pill ${raised ? "good" : "warn"}`;
          if (idx === 1) {
            el.textContent = raised
              ? tr("msg_handle1_up", "Ручка 1: поднята")
              : tr("msg_handle1_down", "Ручка 1: опущена");
          } else {
            el.textContent = raised
              ? tr("msg_handle2_up", "Ручка 2: поднята")
              : tr("msg_handle2_down", "Ручка 2: опущена");
          }
        }
        function updateHandleLiftPills(status) {
          if (!h1LiftPill && !h2LiftPill) return;
          const ok = !!(status && status.ok);
          const h1 = ok ? toRaised(status.h1_raised) : false;
          const h2 = ok ? toRaised(status.h2_raised) : false;
          setLiftPill(h1LiftPill, h1, 1);
          setLiftPill(h2LiftPill, h2, 2);
        }
        async function refreshHandleLiftPills() {
          if (handleLiftInFlight) return;
          handleLiftInFlight = true;
          try {
            const s = await apiJson("/api/status", { method: "GET", timeout_ms: 2200 });
            updateHandleLiftPills(s);
          } catch (_) {
          } finally {
            handleLiftInFlight = false;
          }
        }
        function sleepMs(ms) {
          return new Promise((resolve) => setTimeout(resolve, Math.max(0, Number(ms) || 0)));
        }
        function parseLevel(v) {
          if (v === 0 || v === 1) return v;
          if (typeof v === "number" && Number.isFinite(v)) {
            const n = Math.round(v);
            if (n === 0 || n === 1) return n;
          }
          if (typeof v === "string") {
            const s = v.trim();
            if (s === "0" || s === "1") return Number(s);
          }
          return null;
        }
        function calibrationDialog(opts) {
          const o = Object.assign({
            title: "",
            text: "",
            primaryText: "Далее",
            secondaryText: "Отмена",
            tertiaryText: ""
          }, opts || {});
          return new Promise((resolve) => {
            let done = false;
            const overlay = document.createElement("div");
            overlay.className = "fwUpdateMenuModal";
            overlay.setAttribute("role", "dialog");
            overlay.setAttribute("aria-modal", "true");
            const box = document.createElement("div");
            box.className = "fwUpdateMenuBox";
            box.style.width = "min(620px, 96vw)";
            const titleEl = document.createElement("div");
            titleEl.className = "fwUpdateMenuTitle";
            titleEl.style.textTransform = "none";
            titleEl.style.fontSize = "clamp(17px, 2vw, 24px)";
            titleEl.textContent = String(o.title || "");
            box.appendChild(titleEl);
            const textEl = document.createElement("div");
            textEl.className = "fwUpdateMenuText";
            textEl.style.whiteSpace = "pre-wrap";
            textEl.textContent = String(o.text || "");
            box.appendChild(textEl);
            const row = document.createElement("div");
            row.className = "btnRow";
            row.style.justifyContent = "center";
            row.style.marginTop = "12px";
            const btnPrimary = document.createElement("button");
            btnPrimary.type = "button";
            btnPrimary.className = "primary";
            btnPrimary.textContent = String(o.primaryText || "Далее");
            row.appendChild(btnPrimary);
            if (o.tertiaryText) {
              const btnTertiary = document.createElement("button");
              btnTertiary.type = "button";
              btnTertiary.className = "pillBtn gray";
              btnTertiary.textContent = String(o.tertiaryText);
              btnTertiary.addEventListener("click", () => finish("tertiary"));
              row.appendChild(btnTertiary);
            }
            const btnSecondary = document.createElement("button");
            btnSecondary.type = "button";
            btnSecondary.textContent = String(o.secondaryText || "Отмена");
            row.appendChild(btnSecondary);
            box.appendChild(row);
            overlay.appendChild(box);
            function finish(v) {
              if (done) return;
              done = true;
              try { document.removeEventListener("keydown", onKey, true); } catch (_) {}
              try { overlay.remove(); } catch (_) {}
              resolve(v);
            }
            function onKey(ev) {
              if (!ev) return;
              if (ev.key === "Escape") {
                ev.preventDefault();
                finish("secondary");
              } else if (ev.key === "Enter") {
                ev.preventDefault();
                finish("primary");
              }
            }
            btnPrimary.addEventListener("click", () => finish("primary"));
            btnSecondary.addEventListener("click", () => finish("secondary"));
            overlay.addEventListener("click", (ev) => { if (ev.target === overlay) finish("secondary"); });
            document.addEventListener("keydown", onKey, true);
            document.body.appendChild(overlay);
          });
        }
        async function sampleHandleLevel(handleIdx, rounds) {
          const keyRaw = (handleIdx === 1) ? "h1_level_raw" : "h2_level_raw";
          const keyStable = (handleIdx === 1) ? "h1_level" : "h2_level";
          let hi = 0;
          let lo = 0;
          for (let i = 0; i < rounds; i++) {
            const s = await apiJson("/api/status", { method: "GET", timeout_ms: 2400 });
            const rawLevel = parseLevel(s ? s[keyRaw] : null);
            const stableLevel = parseLevel(s ? s[keyStable] : null);
            const lv = (rawLevel == null) ? stableLevel : rawLevel;
            if (lv === 1) hi++;
            else if (lv === 0) lo++;
            if (i + 1 < rounds) await sleepMs(120);
          }
          if (hi === 0 && lo === 0) throw new Error("нет валидного сигнала GPIO");
          return (hi >= lo) ? 1 : 0;
        }
        async function saveHandleCalibration(handleIdx, raisedOnHigh) {
          const body = (handleIdx === 1)
            ? { t420d_h1_raised_on_high: !!raisedOnHigh }
            : { t420d_h2_raised_on_high: !!raisedOnHigh };
          return apiJson("/api/config", {
            method: "POST",
            body: JSON.stringify(body),
            timeout_ms: 5000
          });
        }
        async function runHandleCalibration(handleIdx) {
          if (handleCalInFlight) return;
          handleCalInFlight = true;
          const handleTxt = (handleIdx === 1) ? "Ручка 1" : "Ручка 2";
          try {
            while (true) {
              const s1 = await calibrationDialog({
                title: `${handleTxt}: калибровка`,
                text: "Убедитесь, что ручка вставлена в держатель, и нажмите \"Далее\".",
                primaryText: "Далее",
                secondaryText: "Отмена"
              });
              if (s1 !== "primary") break;
              const standLevel = await sampleHandleLevel(handleIdx, 8);

              const s2 = await calibrationDialog({
                title: `${handleTxt}: калибровка`,
                text: "Поднимите ручку и нажмите \"Далее\".",
                primaryText: "Далее",
                secondaryText: "Отмена"
              });
              if (s2 !== "primary") break;
              const raisedLevel = await sampleHandleLevel(handleIdx, 8);

              if (standLevel === raisedLevel) {
                const again = await calibrationDialog({
                  title: `${handleTxt}: калибровка`,
                  text: `Не удалось различить состояния (оба шага дали уровень ${standLevel}).\nПроверьте контакт и повторите.`,
                  primaryText: "Перекалибровать",
                  secondaryText: "Отмена"
                });
                if (again === "primary") continue;
                break;
              }

              const raisedOnHigh = (raisedLevel === 1);
              const preview = await calibrationDialog({
                title: `${handleTxt}: проверка`,
                text:
                  `В держателе: уровень ${standLevel}\n` +
                  `Снята: уровень ${raisedLevel}\n` +
                  `Результат: ${raisedOnHigh ? "поднята = HIGH, в держателе = LOW" : "поднята = LOW, в держателе = HIGH"}`,
                primaryText: "Сохранить",
                secondaryText: "Отмена",
                tertiaryText: "Перекалибровать"
              });
              if (preview === "tertiary") continue;
              if (preview !== "primary") break;

              const saved = await saveHandleCalibration(handleIdx, raisedOnHigh);
              if (!saved || !saved.ok) {
                await calibrationDialog({
                  title: `${handleTxt}: ошибка`,
                  text: `Не удалось сохранить калибровку: ${saved && saved.error ? saved.error : "unknown"}`,
                  primaryText: "Перекалибровать",
                  secondaryText: "Закрыть"
                });
                continue;
              }
              await refreshHandleLiftPills();
              await calibrationDialog({
                title: `${handleTxt}: готово`,
                text: "Калибровка сохранена.",
                primaryText: "OK",
                secondaryText: "Закрыть"
              });
              break;
            }
          } catch (e) {
            await calibrationDialog({
              title: `${handleTxt}: ошибка`,
              text: `Калибровка прервана: ${String((e && e.message) ? e.message : e)}`,
              primaryText: "OK",
              secondaryText: "Закрыть"
            });
          } finally {
            handleCalInFlight = false;
          }
        }
        if (btnCalH1) btnCalH1.addEventListener("click", () => { runHandleCalibration(1).catch(() => {}); });
        if (btnCalH2) btnCalH2.addEventListener("click", () => { runHandleCalibration(2).catch(() => {}); });
        if (btnT420D) {
          btnT420D.addEventListener("click", () => {
            const next = btnT420D.classList.contains("active") ? "0" : "1";
            save("alb_lb_t420d", next);
            renderLBButtons(
              (loadStr("alb_lb_es02", es02) === "1"),
              next === "1",
              (loadStr("alb_lb_h312", h312) === "1")
            );
            renderLBModelVisibility();
            renderLogicVisibility();
          });
        }
        if (btnH312) {
          btnH312.addEventListener("click", () => {
            const next = btnH312.classList.contains("active") ? "0" : "1";
            save("alb_lb_h312", next);
            renderLBButtons(
              (loadStr("alb_lb_es02", es02) === "1"),
              (loadStr("alb_lb_t420d", t420d) === "1"),
              next === "1"
            );
            renderLBModelVisibility();
            renderLogicVisibility();
          });
        }
        if (hasLBToggle) {
          lbActive.addEventListener("click", () => {
            setLBEnabled(true);
            save("alb_dev_lb", "1");
            renderLBModelVisibility();
            renderLogicVisibility();
          });
          lbInactive.addEventListener("click", () => {
            setLBEnabled(false);
            save("alb_dev_lb", "0");
            renderLBModelVisibility();
            renderLogicVisibility();
          });
        }
        if (thirdActive && thirdInactive) {
          thirdActive.addEventListener("click", () => {
            setThirdEnabled(true);
            save("alb_dev_third", "1");
            renderTofVisibility();
          });
          thirdInactive.addEventListener("click", () => {
            setThirdEnabled(false);
            save("alb_dev_third", "0");
            renderTofVisibility();
          });
        }
        renderLBModelVisibility();
        renderDeviceTileVisibility();
        renderLogicVisibility();
        renderTofVisibility();
        updateHandleLiftPills(null);
        refreshHandleLiftPills().catch(() => {});
        setInterval(() => {
          refreshHandleLiftPills().catch(() => {});
        }, 2000);
      }
      async function apiJson(path, opts) {
        const init = Object.assign({}, opts || {});
        init.headers = Object.assign({}, init.headers || {});
        if (!Object.prototype.hasOwnProperty.call(init, "credentials")) init.credentials = "same-origin";
        if (init.body != null && !init.headers["Content-Type"]) init.headers["Content-Type"] = "application/json";
        const timeoutMs = (typeof init.timeout_ms === "number" && init.timeout_ms > 0) ? Math.round(init.timeout_ms) : 0;
        delete init.timeout_ms;
        let timer = 0;
        let ctrl = null;
        if (timeoutMs > 0 && !init.signal && typeof AbortController !== "undefined") {
          ctrl = new AbortController();
          init.signal = ctrl.signal;
          timer = setTimeout(() => {
            try { ctrl.abort(); } catch (_) {}
          }, timeoutMs);
        }
        try {
          const r = await fetch(path, init);
          const t = await r.text();
          try {
            const j = JSON.parse(t || "{}");
            if (j && typeof j === "object") return j;
          } catch (_) {}
          return { ok: false, error: `bad json (${r.status})`, status: r.status, raw: t };
        } catch (e) {
          return { ok: false, error: String((e && e.message) ? e.message : e) };
        } finally {
          if (timer) clearTimeout(timer);
        }
      }
      function askPasswordModal(opts) {
        const o = Object.assign({
          title: "",
          message: "",
          okText: "OK",
          cancelText: "Cancel",
          placeholder: ""
        }, opts || {});
        return new Promise((resolve) => {
          let done = false;
          const overlay = document.createElement("div");
          overlay.className = "fwUpdateMenuModal";
          overlay.setAttribute("role", "dialog");
          overlay.setAttribute("aria-modal", "true");
          const box = document.createElement("div");
          box.className = "fwUpdateMenuBox";
          box.style.width = "min(560px, 96vw)";
          const titleEl = document.createElement("div");
          titleEl.className = "fwUpdateMenuTitle";
          titleEl.style.textTransform = "none";
          titleEl.style.fontSize = "clamp(18px, 2.1vw, 24px)";
          titleEl.textContent = String(o.title || o.message || "");
          box.appendChild(titleEl);
          if (o.title && o.message) {
            const msgEl = document.createElement("div");
            msgEl.className = "fwUpdateMenuText";
            msgEl.textContent = String(o.message || "");
            box.appendChild(msgEl);
          }
          const input = document.createElement("input");
          input.type = "password";
          input.autocomplete = "current-password";
          input.placeholder = String(o.placeholder || "");
          input.style.marginTop = "12px";
          box.appendChild(input);
          const row = document.createElement("div");
          row.className = "btnRow";
          row.style.justifyContent = "center";
          row.style.marginTop = "12px";
          const btnOk = document.createElement("button");
          btnOk.type = "button";
          btnOk.className = "primary";
          btnOk.textContent = String(o.okText || "OK");
          const btnCancel = document.createElement("button");
          btnCancel.type = "button";
          btnCancel.textContent = String(o.cancelText || "Cancel");
          row.appendChild(btnOk);
          row.appendChild(btnCancel);
          box.appendChild(row);
          overlay.appendChild(box);
          function finish(value) {
            if (done) return;
            done = true;
            try { document.removeEventListener("keydown", onKey, true); } catch (_) {}
            try { overlay.remove(); } catch (_) {}
            resolve(value);
          }
          function submit() {
            const pass = String(input.value || "").trim();
            finish(pass || null);
          }
          function onKey(ev) {
            if (done) return;
            if (ev.key === "Escape" || ev.key === "Esc") {
              ev.preventDefault();
              finish(null);
              return;
            }
            if (ev.key === "Enter") {
              ev.preventDefault();
              submit();
            }
          }
          btnOk.addEventListener("click", submit);
          btnCancel.addEventListener("click", () => finish(null));
          overlay.addEventListener("click", (ev) => {
            if (ev.target === overlay) finish(null);
          });
          document.addEventListener("keydown", onKey, true);
          document.body.appendChild(overlay);
          setTimeout(() => {
            try { input.focus(); } catch (_) {}
          }, 0);
        });
      }
      function scheduleRecoveryAutoOpen(primaryUrl, fallbackUrl) {
        const primaryDefault = String(primaryUrl || `${location.protocol}//${location.host}/recovery.html`);
        const fallbackDefault = String(fallbackUrl || "http://lb-bridge.local/recovery.html");
        const query = (() => {
          try { return String(new URL(primaryDefault).search || ""); } catch (_) { return ""; }
        })();
        const suffix = `/recovery.html${query}`;
        const mkRecoveryUrl = (host) => `http://${host}${suffix}`;
        const hostFromUrl = (u) => {
          try { return String(new URL(String(u || "")).hostname || "").trim(); } catch (_) { return ""; }
        };
        const isIp = (h) => /^\d{1,3}(?:\.\d{1,3}){3}$/.test(String(h || ""));
        const uniq = (arr) => {
          const out = [];
          const seen = new Set();
          (arr || []).forEach((u) => {
            const s = String(u || "").trim();
            if (!s || seen.has(s)) return;
            seen.add(s);
            out.push(s);
          });
          return out;
        };
        const nav = (url) => {
          try { window.location.assign(url); } catch (_) {
            try { window.location.href = url; } catch (_) {}
          }
        };
        const probeReachable = async (url, timeoutMs = 2800) => {
          const ctrl = (typeof AbortController !== "undefined") ? new AbortController() : null;
          let timer = null;
          try {
            if (ctrl) timer = setTimeout(() => { try { ctrl.abort(); } catch (_) {} }, timeoutMs);
            await fetch(String(url || ""), {
              method: "GET",
              mode: "no-cors",
              cache: "no-store",
              credentials: "omit",
              signal: ctrl ? ctrl.signal : undefined
            });
            return true;
          } catch (_) {
            return false;
          } finally {
            if (timer) clearTimeout(timer);
          }
        };
        const buildCandidates = async () => {
          const candidates = [];
          let statusIp = "";
          let statusMdnsHost = "";
          try {
            const r = await fetch("/api/status", { method: "GET", credentials: "same-origin", cache: "no-store" });
            if (r && r.ok) {
              const j = await r.json();
              statusIp = String((j && j.ip) ? j.ip : "").trim();
              statusMdnsHost = String((j && j.mdns_host) ? j.mdns_host : "").trim();
            }
          } catch (_) {}
          if (statusIp && isIp(statusIp)) candidates.push(mkRecoveryUrl(statusIp));
          const curHost = String(location.hostname || "").trim();
          if (curHost && isIp(curHost)) candidates.push(mkRecoveryUrl(curHost));
          if (statusMdnsHost) candidates.push(mkRecoveryUrl(statusMdnsHost));
          if (primaryDefault) candidates.push(primaryDefault);
          if (curHost && !isIp(curHost)) candidates.push(mkRecoveryUrl(curHost));
          const fallbackHost = hostFromUrl(fallbackDefault);
          if (fallbackHost) candidates.push(mkRecoveryUrl(fallbackHost));
          candidates.push("http://192.168.4.1/recovery.html");
          return uniq(candidates);
        };
        buildCandidates().then((candidates) => {
          if (!candidates.length) return nav(primaryDefault);
          // Wait reboot window, then probe candidates in rounds until recovery is back.
          setTimeout(async () => {
            const deadline = Date.now() + 70000;
            while (Date.now() < deadline) {
              for (let i = 0; i < candidates.length; i++) {
                const target = candidates[i];
                const ok = await probeReachable(target);
                if (ok) {
                  nav(target);
                  return;
                }
              }
              await new Promise((r) => setTimeout(r, 1300));
            }
            // Last resort: still open AP default.
            nav(`http://192.168.4.1${suffix}`);
          }, 8500);
        }).catch(() => {
          setTimeout(() => nav(primaryDefault), 8000);
          setTimeout(() => nav(fallbackDefault), 18000);
        });
      }
      function extractFwVersion(raw) {
        const s = String(raw || "").trim();
        if (!s) return "";
        const semver = s.match(/v?\d+\.\d+\.\d+(?:[-+][0-9A-Za-z.-]+)?/i);
        if (semver) return `v${String(semver[0]).replace(/^v/i, "")}`;
        const numeric = s.match(/v?(\d{1,5})\b/i);
        if (numeric && numeric[1]) return `v${numeric[1]}`;
        return "";
      }
      function compareFwVersions(aRaw, bRaw) {
        const a = extractFwVersion(aRaw);
        const b = extractFwVersion(bRaw);
        if (!a || !b) return 0;
        const parse = (v) => {
          const clean = String(v).replace(/^v/i, "");
          const main = clean.split(/[+-]/)[0];
          const p = main.split(".").map((x) => Number.parseInt(x, 10) || 0);
          return { major: p[0] || 0, minor: p[1] || 0, patch: p[2] || 0 };
        };
        const av = parse(a);
        const bv = parse(b);
        if (av.major !== bv.major) return av.major > bv.major ? 1 : -1;
        if (av.minor !== bv.minor) return av.minor > bv.minor ? 1 : -1;
        if (av.patch !== bv.patch) return av.patch > bv.patch ? 1 : -1;
        return 0;
      }
      function initApplyDiag() {
        const logEl = document.getElementById("eventLog");
        const btnFw = document.getElementById("btnFwUpdate");
        const btnReboot = document.getElementById("btnReboot");
        const btnEnterRecovery = document.getElementById("btnEnterRecovery");
        const btnExport = document.getElementById("btnExportLog");
        const btnReset = document.getElementById("btnResetSettings");
        const fwCurrentEl = document.getElementById("fwCurrent");
        const fwServerEl = document.getElementById("fwServer");
        const fwMenuModal = document.getElementById("fwUpdateMenuModal");
        const btnFwLatest = document.getElementById("btnFwUpdateLatest");
        const btnFwChoose = document.getElementById("btnFwUpdateChoose");
        const btnFwManual = document.getElementById("btnFwUpdateManual");
        const btnFwGithub = document.getElementById("btnFwUpdateGithub");
        const btnFwCancel = document.getElementById("btnFwUpdateCancel");
        const fwManualPicker = document.getElementById("fwUpdatePicker");
        const fwStatusWrap = document.getElementById("fwUpdateStatusWrap");
        const fwStatusTextEl = document.getElementById("fwUpdateStatusText");
        const fwStatusPctEl = document.getElementById("fwUpdateStatusPct");
        const fwStatusFillEl = document.getElementById("fwUpdateStatusFill");
        const fwStatusBarEl = fwStatusFillEl ? fwStatusFillEl.parentElement : null;
        const webUserEl = document.getElementById("webAuthUser");
        const webPassEl = document.getElementById("webAuthPass");
        const webPass2El = document.getElementById("webAuthPass2");
        const btnWebAuthSave = document.getElementById("btnWebAuthSave");
        const btnWebAuthLogoutAll = document.getElementById("btnWebAuthLogoutAll");
        const webAuthHintEl = document.getElementById("webAuthHint");
        if (!logEl) return;
        function ts() {
          const d = new Date();
          const p2 = (n) => String(n).padStart(2, "0");
          return `${d.getFullYear()}-${p2(d.getMonth() + 1)}-${p2(d.getDate())} ${p2(d.getHours())}:${p2(d.getMinutes())}:${p2(d.getSeconds())}`;
        }
        function append(line) {
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          const next = `${cur}\n[ui] ${ts()} ${line}`;
          const rows = next.split("\n");
          const keep = 220;
          logEl.textContent = rows.length > keep ? rows.slice(rows.length - keep).join("\n") : next;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function setWebAuthHint(text, cls) {
          if (!webAuthHintEl) return;
          webAuthHintEl.classList.remove("ok", "err");
          if (cls) webAuthHintEl.classList.add(cls);
          webAuthHintEl.textContent = String(text || "");
        }
        function isValidWebUserName(user) {
          const s = String(user || "").trim();
          return /^[A-Za-z0-9_.-]{3,24}$/.test(s);
        }
        async function askCriticalPassword() {
          const pass = await askPasswordModal({
            title: tr("prompt_confirm_pass", "Enter current web password to continue"),
            okText: "OK",
            cancelText: tr("btn_no", "Cancel")
          });
          if (!pass) {
            setWebAuthHint(tr("msg_confirm_pass_required", "Operation canceled: password not provided"), "err");
            return null;
          }
          return pass;
        }
        function criticalHeaderValue(pass) {
          return encodeURIComponent(String(pass || ""));
        }
        function criticalHeaders(pass) {
          return { "X-LB-Confirm-Pass-Enc": criticalHeaderValue(pass) };
        }
        function downloadText(filename, text) {
          const blob = new Blob([String(text || "").replace(/\n/g, "\r\n")], { type: "text/plain;charset=utf-8" });
          const url = URL.createObjectURL(blob);
          const a = document.createElement("a");
          a.href = url;
          a.download = filename;
          document.body.appendChild(a);
          a.click();
          a.remove();
          setTimeout(() => URL.revokeObjectURL(url), 0);
        }
        let fwUpdateBusy = false;
        let fwVersionCurrent = "";
        let fwReleaseMeta = { checked: false, available: false, version: "", packUrl: "", pageUrl: "", error: "" };
        let fwReleaseCheckBusy = false;
        let fwReleaseCheckedAt = 0;
        let fwStatusHideTimer = 0;
        const kFwRepo = "Serjio193/legacy-bridge";
        const kFwReleaseApi = `https://api.github.com/repos/${kFwRepo}/releases/latest`;
        const kFwReleasesApi = `https://api.github.com/repos/${kFwRepo}/releases?per_page=30`;
        const kFwMirrorBase = "https://serjio193.github.io/legacy-bridge/";
        const kFwMirrorIndex = `${kFwMirrorBase}releases/index.json`;
        function normalizeFwTag(version) {
          const v = String(version || "").trim().toLowerCase().replace(/^v/, "");
          return v ? `v${v}` : "";
        }
        function buildMirrorPackUrl(version) {
          const tag = normalizeFwTag(version);
          if (!tag) return "";
          return `${kFwMirrorBase}releases/${encodeURIComponent(tag)}/update.lbpack`;
        }
        async function fetchMirrorPackMap() {
          const out = {};
          try {
            const r = await fetch(kFwMirrorIndex, { cache: "no-store" });
            if (!r || !r.ok) return out;
            const arr = await r.json();
            const src = Array.isArray(arr) ? arr : [];
            src.forEach((item) => {
              const tag = normalizeFwTag(item && item.tag_name);
              if (!tag) return;
              const assets = (item && item.assets && typeof item.assets === "object") ? item.assets : {};
              const rel = String(assets["update.lbpack"] || "").trim();
              if (!rel) return;
              out[tag] = new URL(rel, kFwMirrorBase).href;
            });
          } catch (_) {}
          return out;
        }
        function pickLbpackAsset(release) {
          const assets = Array.isArray(release && release.assets) ? release.assets : [];
          if (!assets.length) return null;
          const exact = assets.find((a) => String(a && a.name || "").toLowerCase() === "update.lbpack");
          if (exact && exact.browser_download_url) return exact;
          const anyPack = assets.find((a) => String(a && a.name || "").toLowerCase().endsWith(".lbpack"));
          if (anyPack && anyPack.browser_download_url) return anyPack;
          return null;
        }
        function applyFwReleaseUi() {
          if (fwServerEl) {
            fwServerEl.textContent = fwReleaseMeta.version || "n/a";
          }
          if (btnFw) {
            btnFw.classList.toggle("fwUpdateAvailable", !!fwReleaseMeta.available);
            if (fwReleaseMeta.available && fwReleaseMeta.version) {
              btnFw.title = `New firmware available: ${fwReleaseMeta.version}`;
            } else if (fwReleaseMeta.error) {
              btnFw.title = `Firmware check: ${fwReleaseMeta.error}`;
            } else {
              btnFw.title = "Firmware update";
            }
          }
        }
        async function checkGithubFirmwareUpdate(force) {
          const now = Date.now();
          if (!force && fwReleaseMeta.checked && (now - fwReleaseCheckedAt) < (5 * 60 * 1000)) {
            applyFwReleaseUi();
            return fwReleaseMeta;
          }
          if (fwReleaseCheckBusy) return fwReleaseMeta;
          fwReleaseCheckBusy = true;
          try {
            const mirrorMap = await fetchMirrorPackMap();
            const r = await fetch(kFwReleaseApi, { cache: "no-store" });
            if (!r || !r.ok) throw new Error(`github ${r ? r.status : "fetch failed"}`);
            const j = await r.json();
            const version = extractFwVersion((j && j.tag_name) || (j && j.name) || "");
            const asset = pickLbpackAsset(j);
            const tag = normalizeFwTag(version);
            const mirrorPackUrl = String(mirrorMap[tag] || buildMirrorPackUrl(version)).trim();
            const fallbackPackUrl = String(asset && asset.browser_download_url ? asset.browser_download_url : "").trim();
            const packUrl = fallbackPackUrl || mirrorPackUrl;
            const pageUrl = String(j && j.html_url ? j.html_url : "").trim();
            const cmp = compareFwVersions(version, fwVersionCurrent);
            fwReleaseMeta = {
              checked: true,
              available: !!(version && packUrl && cmp > 0),
              version,
              packUrl,
              pageUrl,
              error: ""
            };
            fwReleaseCheckedAt = now;
            if (fwReleaseMeta.available) {
              append(`firmware update available: current=${fwVersionCurrent || "?"} latest=${fwReleaseMeta.version}`);
            }
          } catch (e) {
            fwReleaseMeta = {
              checked: true,
              available: false,
              version: "",
              packUrl: "",
              pageUrl: "",
              error: String((e && e.message) ? e.message : e)
            };
            fwReleaseCheckedAt = now;
          } finally {
            fwReleaseCheckBusy = false;
            applyFwReleaseUi();
          }
          return fwReleaseMeta;
        }
        async function fetchGithubReleasesList() {
          const mirrorMap = await fetchMirrorPackMap();
          const r = await fetch(kFwReleasesApi, { cache: "no-store" });
          if (!r || !r.ok) throw new Error(`github ${r ? r.status : "fetch failed"}`);
          const arr = await r.json();
          const src = Array.isArray(arr) ? arr : [];
          const out = [];
          src.forEach((rel) => {
            if (!rel || rel.draft) return;
            const version = extractFwVersion((rel && rel.tag_name) || (rel && rel.name) || "");
            const asset = pickLbpackAsset(rel);
            const tag = normalizeFwTag(version);
            const mirrorPackUrl = String(mirrorMap[tag] || buildMirrorPackUrl(version)).trim();
            const fallbackPackUrl = String(asset && asset.browser_download_url ? asset.browser_download_url : "").trim();
            const packUrl = fallbackPackUrl || mirrorPackUrl;
            if (!version || !packUrl) return;
            out.push({
              version,
              packUrl,
              pageUrl: String(rel && rel.html_url ? rel.html_url : "").trim(),
              prerelease: !!rel.prerelease,
              publishedAt: String(rel && rel.published_at ? rel.published_at : "")
            });
          });
          return out;
        }
        function setFwStatus(text, pct, indeterminate) {
          if (!fwStatusWrap) return;
          fwStatusWrap.classList.remove("hidden");
          if (fwStatusTextEl) fwStatusTextEl.textContent = String(text || "");
          const p = Number.isFinite(pct) ? Math.max(0, Math.min(100, Number(pct))) : null;
          if (fwStatusFillEl) {
            fwStatusFillEl.classList.toggle("isIndeterminate", !!indeterminate);
            if (!indeterminate) fwStatusFillEl.style.width = `${(p == null ? 0 : p).toFixed(1)}%`;
          }
          if (fwStatusPctEl) {
            fwStatusPctEl.textContent = indeterminate ? "..." : `${Math.round(p == null ? 0 : p)}%`;
          }
          if (fwStatusBarEl) fwStatusBarEl.setAttribute("aria-valuenow", String(Math.round(p == null ? 0 : p)));
        }
        function clearFwStatus() {
          if (fwStatusHideTimer) {
            clearTimeout(fwStatusHideTimer);
            fwStatusHideTimer = 0;
          }
          if (!fwStatusWrap) return;
          if (fwStatusTextEl) fwStatusTextEl.textContent = tr("msg_fw_status_idle", "Firmware update idle");
          if (fwStatusPctEl) fwStatusPctEl.textContent = "0%";
          if (fwStatusFillEl) {
            fwStatusFillEl.classList.remove("isIndeterminate");
            fwStatusFillEl.style.width = "0%";
          }
          if (fwStatusBarEl) fwStatusBarEl.setAttribute("aria-valuenow", "0");
          fwStatusWrap.classList.add("hidden");
        }
        function hideFwStatusLater(ms) {
          if (!fwStatusWrap) return;
          if (fwStatusHideTimer) clearTimeout(fwStatusHideTimer);
          fwStatusHideTimer = setTimeout(() => {
            fwStatusHideTimer = 0;
            clearFwStatus();
          }, Math.max(800, Number(ms) || 4500));
        }
        function setFwUpdateBusy(on) {
          const v = !!on;
          fwUpdateBusy = v;
          if (btnFw) {
            btnFw.disabled = v;
            btnFw.classList.toggle("fwBusyBtn", v);
            if (v) btnFw.classList.remove("fwUpdateAvailable");
          }
          if (btnFwLatest) {
            btnFwLatest.disabled = v;
            btnFwLatest.classList.toggle("fwBusyBtn", v);
          }
          if (btnFwChoose) {
            btnFwChoose.disabled = v;
            btnFwChoose.classList.toggle("fwBusyBtn", v);
          }
          if (btnFwManual) {
            btnFwManual.disabled = v;
            btnFwManual.classList.toggle("fwBusyBtn", v);
          }
          if (btnFwGithub) btnFwGithub.disabled = v;
          if (btnFwCancel) btnFwCancel.disabled = v;
          if (!v) applyFwReleaseUi();
        }
        function uploadManualFirmware(file, confirmPass) {
          return new Promise((resolve) => {
            if (!file) {
              resolve({ ok: false, error: "no file selected", progress: 0 });
              return;
            }
            let lastPct = 0;
            const xhr = new XMLHttpRequest();
            const form = new FormData();
            form.append("fw", file, file.name || "firmware.bin");
            xhr.timeout = 300000;
            xhr.upload.addEventListener("progress", (ev) => {
              if (!ev.lengthComputable || ev.total <= 0) return;
              lastPct = Math.max(0, Math.min(100, (ev.loaded / ev.total) * 100));
              setFwStatus(tr("msg_fw_manual_uploading", "Uploading firmware..."), lastPct, false);
            });
            xhr.upload.addEventListener("load", () => {
              setFwStatus(tr("msg_fw_manual_finishing", "Finalizing flash..."), 99, false);
            });
            xhr.addEventListener("load", () => {
              let j = null;
              try { j = JSON.parse(xhr.responseText || "{}"); } catch (_) {}
              if (j && typeof j === "object") resolve(Object.assign({ progress: 100 }, j));
              else resolve({ ok: false, error: xhr.responseText || "bad json", progress: 100 });
            });
            xhr.addEventListener("error", () => resolve({ ok: false, error: "network error", progress: lastPct }));
            xhr.addEventListener("abort", () => resolve({ ok: false, error: "aborted", progress: lastPct }));
            xhr.addEventListener("timeout", () => resolve({ ok: false, error: "timeout", progress: lastPct }));
            try {
              xhr.open("POST", "/api/fw/update_manual", true);
              if (confirmPass) xhr.setRequestHeader("X-LB-Confirm-Pass-Enc", criticalHeaderValue(confirmPass));
              xhr.send(form);
            } catch (e) {
              resolve({ ok: false, error: String((e && e.message) ? e.message : e), progress: lastPct });
            }
          });
        }
        function isOtaPartitionConflict(err) {
          const s = String(err || "").toLowerCase();
          return s.includes("5377") || s.includes("partition conflict") || s.includes("single-slot");
        }
        async function enterRecoveryForUpdate() {
          const pass = await askCriticalPassword();
          if (!pass) return false;
          const recoveryUrl = `${location.protocol}//${location.host}/recovery.html`;
          append(tr("msg_fw_manual_need_recovery", "single-slot OTA layout: switching to Recovery mode for flashing"));
          append(`${tr("msg_fw_recovery_hint", "after reboot open")}: ${recoveryUrl} (fallback: http://lb-bridge.local/recovery.html)`);
          setFwStatus(tr("msg_fw_manual_need_recovery", "single-slot OTA layout: switching to Recovery mode for flashing"), null, true);
          const rr = await apiJson("/api/fw/enter_recovery", { method: "POST", body: "{}", headers: criticalHeaders(pass) });
          if (rr && rr.ok) {
            append(tr("msg_fw_recovery_switch_ok", "recovery mode requested, rebooting"));
            setFwStatus(tr("msg_fw_recovery_switch_ok", "recovery mode requested, rebooting"), 100, false);
            scheduleRecoveryAutoOpen(recoveryUrl, "http://lb-bridge.local/recovery.html");
            hideFwStatusLater(12000);
            return true;
          }
          append(`${tr("msg_fw_recovery_switch_fail", "failed to switch to recovery mode")}: ${(rr && rr.error) ? rr.error : "unknown"}`);
          setFwStatus(tr("msg_fw_recovery_switch_fail", "failed to switch to recovery mode"), 0, false);
          hideFwStatusLater(9000);
          return false;
        }
        clearFwStatus();
        function filenameNow() {
          const d = new Date();
          const p2 = (n) => String(n).padStart(2, "0");
          return `lb-legacy-bridge-log-${d.getFullYear()}${p2(d.getMonth() + 1)}${p2(d.getDate())}-${p2(d.getHours())}${p2(d.getMinutes())}${p2(d.getSeconds())}.txt`;
        }
        function clearLocalStateAfterFactoryReset() {
          try {
            for (let i = localStorage.length - 1; i >= 0; i--) {
              const key = String(localStorage.key(i) || "");
              if (key.startsWith("alb_")) localStorage.removeItem(key);
            }
          } catch (_) {}
        }
        function closeFwMenu() {
          if (fwMenuModal) fwMenuModal.classList.add("hidden");
        }
        async function requestEnterRecoveryFromMain() {
          if (fwUpdateBusy) return;
          const pass = await askCriticalPassword();
          if (!pass) return;
          const recoveryUrl = `${location.protocol}//${location.host}/recovery.html?autopack=latest`;
          setFwUpdateBusy(true);
          setFwStatus("Firmware update: switching to Recovery mode...", 30, false);
          append("firmware update: switch to recovery requested");
          const rr = await apiJson("/api/fw/enter_recovery", { method: "POST", body: "{}", headers: criticalHeaders(pass) });
          if (rr && rr.ok) {
            const autoPrimary = String((rr && rr.recovery_url) ? rr.recovery_url : recoveryUrl);
            const autoFallback = String((rr && rr.recovery_fallback) ? rr.recovery_fallback : "http://lb-bridge.local/recovery.html?autopack=latest");
            append("firmware update: recovery requested, rebooting");
            setFwStatus("Firmware update: opening Recovery and starting latest update...", 65, false);
            scheduleRecoveryAutoOpen(autoPrimary, autoFallback);
            hideFwStatusLater(15000);
            return;
          }
          append(`firmware update failed: ${(rr && rr.error) ? rr.error : "unknown"}`);
          setFwStatus(tr("msg_fw_recovery_switch_fail", "failed to switch to recovery mode"), 0, false);
          hideFwStatusLater(9000);
          setFwUpdateBusy(false);
        }
        if (btnFw) btnFw.addEventListener("click", async () => {
          await checkGithubFirmwareUpdate(false);
          await requestEnterRecoveryFromMain();
        });
        function formatFwCurrentLabel(st) {
          const fw = String((st && st.fw) ? st.fw : "").trim();
          if (!fw) return "";
          const rawProfile = String((st && st.fw_profile) ? st.fw_profile : "").trim().toLowerCase();
          const hardened = !!(st && st.fw_hardened);
          const profile = rawProfile || (hardened ? "hardened" : "standard");
          const title = (profile === "hardened") ? "Hardened" : "Standard";
          return `${fw} (${title})`;
        }
        let currentWebUser = "admin";
        async function refreshWebAuthMeta() {
          if (!webUserEl) return;
          const r = await apiJson("/api/config", { method: "GET", timeout_ms: 7000 });
          if (!r || !r.ok) return;
          const u = String(r.web_auth_user || "admin").trim() || "admin";
          currentWebUser = u;
          webUserEl.value = u;
        }
        async function refreshDiagMeta() {
          const s = await apiJson("/api/status");
          if (!s || !s.ok) return;
          fwVersionCurrent = extractFwVersion(String((s && s.fw) ? s.fw : "")) || String((s && s.fw) ? s.fw : "").trim();
          if (fwCurrentEl) {
            const label = formatFwCurrentLabel(s);
            if (label) fwCurrentEl.textContent = label;
          }
          await checkGithubFirmwareUpdate(false);
        }
        let cmdLogSince = 0;
        let cmdLogBusy = false;
        async function pollExtractorCmdLog() {
          if (cmdLogBusy) return;
          cmdLogBusy = true;
          try {
            const r = await apiJson(`/api/extractor/cmdlog?since=${cmdLogSince}`, { method: "GET", timeout_ms: 7000 });
            if (!r || !r.ok) return;
            const items = Array.isArray(r.items) ? r.items : [];
            items.forEach((it) => {
              const seq = Number(it && it.seq);
              const line = String((it && it.line) || "").trim();
              if (Number.isFinite(seq) && seq > cmdLogSince) cmdLogSince = seq;
              if (line) append(`[ble] ${line}`);
            });
            const nextSeq = Number(r.next_seq);
            if (Number.isFinite(nextSeq) && nextSeq > cmdLogSince && items.length === 0) cmdLogSince = nextSeq;
          } catch (_) {
          } finally {
            cmdLogBusy = false;
          }
        }
        refreshDiagMeta().catch(() => {});
        refreshWebAuthMeta().catch(() => {});
        apiJson("/api/auth/remember", { method: "POST", body: "{}", timeout_ms: 5000 }).catch(() => {});
        setInterval(() => {
          refreshDiagMeta().catch(() => {});
        }, 5000);
        setInterval(() => {
          checkGithubFirmwareUpdate(false).catch(() => {});
        }, 120000);
        pollExtractorCmdLog().catch(() => {});
        setInterval(() => {
          pollExtractorCmdLog().catch(() => {});
        }, 3000);
        if (btnReboot) btnReboot.addEventListener("click", async () => {
          const pass = await askCriticalPassword();
          if (!pass) return;
          append("reboot requested");
          const r = await apiJson("/api/reboot", { method: "POST", body: "{}", headers: criticalHeaders(pass) });
          if (!r || !r.ok) append(`reboot failed: ${(r && r.error) ? r.error : "unknown"}`);
        });
        if (btnEnterRecovery) btnEnterRecovery.addEventListener("click", async () => {
          await enterRecoveryForUpdate();
        });
        if (btnReset) btnReset.addEventListener("click", async () => {
          const pass = await askCriticalPassword();
          if (!pass) return;
          append("reset settings requested");
          const r = await apiJson("/api/factory_reset", { method: "POST", body: "{}", headers: criticalHeaders(pass) });
          if (!r || !r.ok) {
            append(`reset failed: ${(r && r.error) ? r.error : "unknown"}`);
            return;
          }
          clearLocalStateAfterFactoryReset();
          append("reset ok: local cache cleared");
          setTimeout(() => {
            try { window.location.reload(); } catch (_) {}
          }, 1200);
        });
        if (btnExport) btnExport.addEventListener("click", () => {
          downloadText(filenameNow(), logEl.textContent || "");
          append("event log exported");
        });
        if (btnWebAuthSave) btnWebAuthSave.addEventListener("click", async () => {
          const nextUser = String(webUserEl && webUserEl.value || "").trim();
          const p1 = String(webPassEl && webPassEl.value || "");
          const p2 = String(webPass2El && webPass2El.value || "");
          if (!isValidWebUserName(nextUser)) {
            setWebAuthHint("login must be 3..24 (a-zA-Z0-9_.-)", "err");
            return;
          }
          if ((p1 || p2) && p1 !== p2) {
            setWebAuthHint(tr("msg_web_auth_pass_mismatch", "Passwords do not match"), "err");
            return;
          }
          if (p1 && p1.length < 8) {
            setWebAuthHint(tr("msg_web_auth_pass_short", "Password must be at least 8 characters"), "err");
            return;
          }
          const body = { web_user: nextUser };
          if (p1) body.web_pass = p1;
          const changed = (nextUser !== currentWebUser) || !!p1;
          let confirmPass = null;
          if (changed) {
            confirmPass = await askCriticalPassword();
            if (!confirmPass) return;
          }
          btnWebAuthSave.disabled = true;
          setWebAuthHint("saving...", "");
          const r = await apiJson("/api/config", {
            method: "POST",
            body: JSON.stringify(body),
            headers: confirmPass ? criticalHeaders(confirmPass) : {},
            timeout_ms: 12000
          });
          btnWebAuthSave.disabled = false;
          if (!r || !r.ok) {
            setWebAuthHint(`${tr("msg_web_auth_failed", "Failed to update access")}: ${(r && r.error) ? r.error : "unknown"}`, "err");
            return;
          }
          append(`[security] web auth updated user=${nextUser} pass=${p1 ? 1 : 0}`);
          setWebAuthHint(tr("msg_web_auth_saved", "Access updated"), "ok");
          if (webPassEl) webPassEl.value = "";
          if (webPass2El) webPass2El.value = "";
          currentWebUser = nextUser;
          if (changed) {
            setWebAuthHint(tr("msg_web_auth_relogin", "Access changed. Reload page and login again."), "ok");
            setTimeout(() => {
              try { window.location.reload(); } catch (_) {}
            }, 900);
          }
        });
        if (btnWebAuthLogoutAll) btnWebAuthLogoutAll.addEventListener("click", async () => {
          if (!window.confirm(tr("msg_web_auth_logout_all_confirm", "Logout all remembered devices?"))) return;
          const pass = await askCriticalPassword();
          if (!pass) return;
          const r = await apiJson("/api/auth/revoke_all", {
            method: "POST",
            body: "{}",
            headers: criticalHeaders(pass),
            timeout_ms: 10000
          });
          if (!r || !r.ok) {
            setWebAuthHint(`${tr("msg_web_auth_logout_all_fail", "Failed to logout remembered devices")}: ${(r && r.error) ? r.error : "unknown"}`, "err");
            return;
          }
          setWebAuthHint(tr("msg_web_auth_logout_all_ok", "All remembered devices logged out"), "ok");
        });
      }
      function initWifiAutoOff() {
        const el = document.getElementById("wifiAutoOffMin");
        if (!el) return;
        const key = "alb_wifi_autooff_min";
        let saveTimer = 0;
        let lastSent = null;
        function load(def) {
          try { return localStorage.getItem(key) || def; } catch (_) { return def; }
        }
        function save(v) {
          try { localStorage.setItem(key, String(v)); } catch (_) {}
        }
        function norm(raw) {
          return Math.max(0, Math.min(1440, parseInt(String(raw || "0"), 10) || 0));
        }
        async function pushToDevice(v) {
          const n = norm(v);
          if (lastSent === n) return;
          const r = await apiJson("/api/config", {
            method: "POST",
            body: JSON.stringify({ wifi_autooff_min: n })
          });
          if (r && r.ok) lastSent = n;
        }
        function schedulePush(v) {
          if (saveTimer) clearTimeout(saveTimer);
          saveTimer = setTimeout(() => {
            saveTimer = 0;
            pushToDevice(v).catch(() => {});
          }, 350);
        }
        const raw = load(String(el.value || "10"));
        const n = norm(raw);
        el.value = String(n);
        apiJson("/api/config", { method: "GET" }).then((r) => {
          if (!r || !r.ok) return;
          if (typeof r.wifi_autooff_min === "undefined") return;
          const fromDevice = norm(r.wifi_autooff_min);
          el.value = String(fromDevice);
          save(fromDevice);
          lastSent = fromDevice;
        }).catch(() => {});
        el.addEventListener("input", () => {
          const next = norm(el.value);
          el.value = String(next);
          save(next);
          schedulePush(next);
        });
        el.addEventListener("change", () => {
          const next = norm(el.value);
          el.value = String(next);
          save(next);
          schedulePush(next);
        });
        el.addEventListener("blur", () => {
          const next = norm(el.value);
          el.value = String(next);
          save(next);
          schedulePush(next);
        });
      }
      function detectShortcutOs() {
        const ua = String(navigator.userAgent || "").toLowerCase();
        const platform = String((navigator.userAgentData && navigator.userAgentData.platform) || navigator.platform || "").toLowerCase();
        const src = `${platform} ${ua}`;
        if (src.includes("android") || src.includes("iphone") || src.includes("ipad")) return "mobile";
        if (src.includes("win")) return "windows";
        if (src.includes("mac")) return "macos";
        if (src.includes("linux") || src.includes("x11")) return "linux";
        return "other";
      }
      function getShortcutTargetUrl() {
        let target = "";
        try { target = localStorage.getItem("alb_last_open_url") || ""; } catch (_) {}
        if (!/^https?:\/\//i.test(target)) target = `${window.location.origin}/`;
        if (!target.endsWith("/")) target += "/";
        return target;
      }
      function legacyCopyText(text) {
        try {
          const ta = document.createElement("textarea");
          ta.value = String(text || "");
          ta.setAttribute("readonly", "");
          ta.style.position = "fixed";
          ta.style.left = "-9999px";
          ta.style.top = "0";
          document.body.appendChild(ta);
          ta.focus();
          ta.select();
          const ok = !!document.execCommand && document.execCommand("copy");
          document.body.removeChild(ta);
          return ok;
        } catch (_) {
          return false;
        }
      }
      async function copyTextSmart(text) {
        let copied = false;
        try {
          if (navigator.clipboard && navigator.clipboard.writeText) {
            await navigator.clipboard.writeText(String(text || ""));
            copied = true;
          }
        } catch (_) {
          copied = false;
        }
        if (!copied) copied = legacyCopyText(text);
        return copied;
      }
      function scriptTargetUrl(url) {
        const raw = String(url || getShortcutTargetUrl() || "").trim();
        if (/^https?:\/\//i.test(raw)) return raw.endsWith("/") ? raw : `${raw}/`;
        return getShortcutTargetUrl();
      }
      function buildWindowsShortcutScript(url) {
        const target = scriptTargetUrl(url).replace(/'/g, "''");
        return [
          `$url = '${target}'`,
          "$base = Join-Path $env:LOCALAPPDATA 'LB-Control'",
          "New-Item -ItemType Directory -Force -Path $base | Out-Null",
          "$icon = Join-Path $base 'LB.ico'",
          "try { Invoke-WebRequest -Uri ($url + 'LB.ico') -UseBasicParsing -OutFile $icon -TimeoutSec 8 } catch {}",
          "$desktop = [Environment]::GetFolderPath('Desktop')",
          "$shortcut = Join-Path $desktop 'LB-Control.lnk'",
          "$ws = New-Object -ComObject WScript.Shell",
          "$sc = $ws.CreateShortcut($shortcut)",
          "$sc.TargetPath = \"$env:WINDIR\\explorer.exe\"",
          "$sc.Arguments = $url",
          "if (Test-Path $icon) { $sc.IconLocation = \"$icon,0\" }",
          "$sc.WorkingDirectory = $desktop",
          "$sc.Description = 'LB Control'",
          "$sc.Save()",
          "Write-Host ('Created: ' + $shortcut)",
        ].join("\n");
      }
      function buildWindowsAutoCommand(script) {
        const oneLine = String(script || "")
          .replace(/\r/g, "")
          .split("\n")
          .map((s) => s.trim())
          .filter((s) => !!s)
          .join("; ");
        const escaped = oneLine.replace(/"/g, "`\"");
        return `powershell -NoProfile -ExecutionPolicy Bypass -Command "${escaped}"`;
      }
      function buildWindowsProtocolUri(url, name) {
        const target = scriptTargetUrl(url);
        const nm = (String(name || "LB-Control").trim() || "LB-Control").replace(/[^a-zA-Z0-9._-]/g, "-");
        return `lb-shortcut://install?url=${target}&name=${nm}`;
      }
      function tryWindowsProtocolLaunch(url, name) {
        try {
          window.location.href = buildWindowsProtocolUri(url, name);
          return true;
        } catch (_) {
          return false;
        }
      }
      function buildLinuxShortcutScript(url) {
        const target = scriptTargetUrl(url).replace(/"/g, '\\"');
        return [
          `URL="${target}"`,
          'ICON_DIR="$HOME/.local/share/icons"',
          'ICON_PATH="$ICON_DIR/lb-control.png"',
          'DESKTOP_FILE="$HOME/Desktop/LB-Control.desktop"',
          'mkdir -p "$ICON_DIR"',
          'curl -fsSL "${URL}LB.png" -o "$ICON_PATH" || true',
          'cat > "$DESKTOP_FILE" <<EOF',
          "[Desktop Entry]",
          "Type=Link",
          "Name=LB-Control",
          "URL=$URL",
          "Icon=$ICON_PATH",
          "Terminal=false",
          "EOF",
          'chmod +x "$DESKTOP_FILE"',
          'echo "Created: $DESKTOP_FILE"',
        ].join("\n");
      }
      function buildMacShortcutScript(url) {
        const target = scriptTargetUrl(url).replace(/"/g, '\\"');
        return [
          `URL="${target}"`,
          'OUT="$HOME/Desktop/LB-Control.webloc"',
          'cat > "$OUT" <<EOF',
          '<?xml version="1.0" encoding="UTF-8"?>',
          '<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">',
          '<plist version="1.0">',
          "<dict>",
          "  <key>URL</key>",
          "  <string>$URL</string>",
          "</dict>",
          "</plist>",
          "EOF",
          'echo "Created: $OUT"',
        ].join("\n");
      }
      let deferredInstallPrompt = null;
      function initPwaInstallCapture() {
        if (typeof window === "undefined") return;
        window.addEventListener("beforeinstallprompt", (ev) => {
          try { ev.preventDefault(); } catch (_) {}
          deferredInstallPrompt = ev;
        });
        window.addEventListener("appinstalled", () => {
          deferredInstallPrompt = null;
        });
      }
      function pwaHintForOs(os) {
        if (os === "windows") return tr("shortcut_offer_pwa_hint_win", "Chrome: Menu (three dots) -> Save and share -> Install page as app.");
        if (os === "macos") return tr("shortcut_offer_pwa_hint_macos", "Chrome: Menu (three dots) -> Install page as app.");
        if (os === "linux") return tr("shortcut_offer_pwa_hint_linux", "Chrome: Menu (three dots) -> Install page as app.");
        return tr("shortcut_offer_pwa_hint_mobile", "Browser menu -> Add to Home Screen.");
      }
      async function tryPwaInstallFlow(os) {
        if (deferredInstallPrompt && typeof deferredInstallPrompt.prompt === "function") {
          try {
            await deferredInstallPrompt.prompt();
            if (deferredInstallPrompt.userChoice) await deferredInstallPrompt.userChoice;
          } catch (_) {}
          alert(tr("shortcut_offer_pwa_started", "If browser supports it, installation prompt is now open."));
          return;
        }
        alert(`${tr("shortcut_offer_pwa_not_available", "Auto install prompt is unavailable here. Use browser menu:")}\n${pwaHintForOs(os || detectShortcutOs())}`);
      }
      function initShortcutOffer() {
        const modal = document.getElementById("shortcutOfferModal");
        const question = document.getElementById("shortcutOfferQuestion");
        const choiceRow = document.getElementById("shortcutOfferChoiceRow");
        const actions = document.getElementById("shortcutOfferActions");
        const detectedEl = document.getElementById("shortcutOfferDetected");
        const hintEl = document.getElementById("shortcutOfferHint");
        const scriptEl = document.getElementById("shortcutOfferScript");
        const btnYes = document.getElementById("btnShortcutOfferYes");
        const btnNo = document.getElementById("btnShortcutOfferNo");
        const btnClose = document.getElementById("btnShortcutOfferClose");
        const btnPwa = document.getElementById("btnShortcutOfferPwa");
        const btnCopyLink = document.getElementById("btnShortcutOfferCopyLink");
        const btnAuto = document.getElementById("btnShortcutOfferAuto");
        const btnCopyScript = document.getElementById("btnShortcutOfferCopyScript");
        if (!modal || !question || !choiceRow || !actions || !detectedEl || !hintEl || !scriptEl || !btnYes || !btnNo || !btnClose || !btnPwa || !btnCopyLink || !btnAuto || !btnCopyScript) {
          return { openFromUrl: () => {}, offerOnce: () => {} };
        }
        let activeUrl = "";
        let activeScript = "";
        let activeAutoCommand = "";
        let activeDoneKey = "";
        let activeOs = "other";
        function osLabel(os) {
          if (os === "windows") return "Windows";
          if (os === "macos") return "macOS";
          if (os === "linux") return "Linux";
          if (os === "mobile") return "Android / iOS";
          return "Other";
        }
        function closeModal() {
          modal.classList.add("hidden");
          actions.classList.add("hidden");
          choiceRow.classList.remove("hidden");
          question.classList.remove("hidden");
          activeScript = "";
          activeAutoCommand = "";
          scriptEl.value = "";
        }
        function markDone() {
          if (!activeDoneKey) return;
          try { localStorage.setItem(activeDoneKey, "1"); } catch (_) {}
        }
        function keyForUrl(url) {
          try {
            const host = String((new URL(scriptTargetUrl(url))).hostname || "unknown").toLowerCase();
            return `alb_shortcut_offer_done_v1_${host}`;
          } catch (_) {
            return "alb_shortcut_offer_done_v1_default";
          }
        }
        function setupForDetectedOs() {
          activeOs = detectShortcutOs();
          detectedEl.textContent = `${tr("shortcut_offer_detected", "Detected OS")}: ${osLabel(activeOs)}`;
          if (activeOs === "windows") {
            activeScript = buildWindowsShortcutScript(activeUrl);
            activeAutoCommand = buildWindowsAutoCommand(activeScript);
            hintEl.textContent = tr("shortcut_offer_windows_notice", "Windows setup uses PowerShell. Auto mode copies one command. Manual mode copies full script.");
            btnAuto.classList.remove("hidden");
            btnCopyScript.classList.remove("hidden");
            scriptEl.classList.remove("hidden");
            scriptEl.value = activeScript;
            return;
          }
          btnAuto.classList.add("hidden");
          btnCopyScript.classList.remove("hidden");
          if (activeOs === "linux") {
            activeScript = buildLinuxShortcutScript(activeUrl);
            hintEl.textContent = tr("shortcut_offer_linux_hint", "Run the command in terminal to create a .desktop link.");
            scriptEl.classList.remove("hidden");
            scriptEl.value = activeScript;
            return;
          }
          if (activeOs === "macos") {
            activeScript = buildMacShortcutScript(activeUrl);
            hintEl.textContent = tr("shortcut_offer_macos_hint", "Run the command in Terminal to create a .webloc on Desktop.");
            scriptEl.classList.remove("hidden");
            scriptEl.value = activeScript;
            return;
          }
          activeScript = "";
          scriptEl.value = "";
          scriptEl.classList.add("hidden");
          btnCopyScript.classList.add("hidden");
          hintEl.textContent = (activeOs === "mobile")
            ? tr("shortcut_offer_mobile_hint", "On Android/iOS use browser menu: Add to Home Screen.")
            : tr("shortcut_offer_other_hint", "Copy link and create a shortcut manually in your system.");
        }
        function openModal(url, onlyIfFirst) {
          activeUrl = scriptTargetUrl(url);
          activeDoneKey = keyForUrl(activeUrl);
          if (onlyIfFirst) {
            try {
              if (localStorage.getItem(activeDoneKey) === "1") return;
            } catch (_) {}
          }
          question.textContent = tr("shortcut_offer_question", "Install LB-Control shortcut now?");
          choiceRow.classList.remove("hidden");
          question.classList.remove("hidden");
          actions.classList.add("hidden");
          modal.classList.remove("hidden");
        }
        btnYes.addEventListener("click", () => {
          markDone();
          setupForDetectedOs();
          choiceRow.classList.add("hidden");
          question.classList.add("hidden");
          actions.classList.remove("hidden");
        });
        btnNo.addEventListener("click", () => {
          markDone();
          closeModal();
        });
        btnClose.addEventListener("click", () => closeModal());
        btnPwa.addEventListener("click", async () => {
          await tryPwaInstallFlow(activeOs || detectShortcutOs());
        });
        btnCopyLink.addEventListener("click", async () => {
          const copied = await copyTextSmart(activeUrl || getShortcutTargetUrl());
          if (!copied) {
            try { window.prompt(tr("shortcut_offer_copy_failed", "Copy blocked. Copy text manually from the field below."), activeUrl || getShortcutTargetUrl()); } catch (_) {}
            return;
          }
          alert(`${tr("msg_shortcut_copied", "Link copied")}: ${activeUrl || getShortcutTargetUrl()}`);
        });
        btnAuto.addEventListener("click", async () => {
          if (!activeAutoCommand) return;
          let launched = false;
          if (activeOs === "windows") {
            launched = tryWindowsProtocolLaunch(activeUrl, "LB-Control");
          }
          const copied = await copyTextSmart(activeAutoCommand);
          if (copied) {
            const prefix = launched ? `${tr("shortcut_offer_protocol_try", "Trying browser protocol launch...")}\n` : "";
            alert(`${prefix}${tr("shortcut_offer_auto_copied", "Auto command copied. Open PowerShell and paste it.")}`);
          }
          else {
            scriptEl.classList.remove("hidden");
            scriptEl.value = activeAutoCommand;
            scriptEl.focus();
            scriptEl.select();
            alert(tr("shortcut_offer_copy_failed", "Copy blocked. Copy text manually from the field below."));
          }
        });
        btnCopyScript.addEventListener("click", async () => {
          if (!activeScript) return;
          const copied = await copyTextSmart(activeScript);
          if (copied) alert(tr("shortcut_offer_script_copied", "Script copied. Paste and run in terminal."));
          else {
            scriptEl.focus();
            scriptEl.select();
            alert(tr("shortcut_offer_copy_failed", "Copy blocked. Copy text manually from the field below."));
          }
        });
        return {
          openFromUrl: (url) => openModal(url, false),
          offerOnce: (url) => openModal(url, true),
        };
      }
      async function downloadShortcutFromUi() {
        const url = getShortcutTargetUrl();
        const os = detectShortcutOs();
        const copied = await copyTextSmart(url);
        if (!copied) {
          try {
            window.prompt(tr("msg_shortcut_copy_failed", "Copy failed. Use this URL manually"), url);
          } catch (_) {}
        }
        let msg = copied
          ? `${tr("msg_shortcut_copied", "Link copied")}: ${url}`
          : `${tr("msg_shortcut_copy_failed", "Copy failed. Use this URL manually")}: ${url}`;
        if (os === "windows") msg += `\n${tr("msg_shortcut_win_steps", "Desktop: Right click -> New -> Shortcut -> Paste link.")}`;
        else if (os === "macos") msg += `\n${tr("msg_shortcut_mac_steps", "Drag the URL from browser address bar to Desktop.")}`;
        else if (os === "linux") msg += `\n${tr("msg_shortcut_linux_steps", "Create launcher/shortcut and use this URL.")}`;
        else msg += `\n${tr("msg_shortcut_phone_steps", "Use browser menu: Add to Home Screen.")}`;
        alert(msg);
      }
      let shortcutOfferApi = { openFromUrl: () => {}, offerOnce: () => {} };
      function initWifiScan() {
        const listEl = document.getElementById("wifiScanList");
        const btn = document.getElementById("btnWifiScan");
        const setupPanel = document.getElementById("wifiSetupPanel");
        const staSummary = document.getElementById("wifiStaSummary");
        const staSummaryText = document.getElementById("wifiStaSummaryText");
        const staSummaryMode = document.getElementById("wifiStaSummaryMode");
        const wifiSetupMode = document.getElementById("wifiSetupMode");
        const staSummarySsid = document.getElementById("wifiStaSummarySsid");
        const staSummaryIp = document.getElementById("wifiStaSummaryIp");
        const staSignal = document.getElementById("wifiStaSignal");
        const btnWifiChangeMode = document.getElementById("btnWifiChangeMode");
        const btnWifiAutoOff5 = document.getElementById("btnWifiAutoOff5");
        const wifiAutoOffInput = document.getElementById("wifiAutoOffMin");
        const wifiConnectBox = document.getElementById("wifiConnectBox");
        const selectedEl = document.getElementById("wifiSelectedSsid");
        const passEl = document.getElementById("wifiPassword");
        const btnConnect = document.getElementById("btnWifiConnect");
        const hintEl = document.getElementById("wifiConnectHint");
        const apNotice = document.getElementById("apSwitchNotice");
        const apTitle = document.getElementById("apSwitchTitle");
        const apText = document.getElementById("apSwitchText");
        const apBtnDownload = document.getElementById("btnApSwitchDownload");
        const apBtnCheck = document.getElementById("btnApSwitchCheck");
        const apBtnHide = document.getElementById("btnApSwitchHide");
        if (!listEl || !btn || !selectedEl || !passEl || !btnConnect || !hintEl) return;
        const KEY_WIFI_SELECTED_NET = "alb_wifi_selected_net";
        const KEY_WIFI_FORCE_SETUP = "alb_wifi_force_setup";
        function saveJson(key, obj) {
          try { localStorage.setItem(key, JSON.stringify(obj)); } catch (_) {}
        }
        function loadJson(key, def) {
          try {
            const raw = localStorage.getItem(key);
            if (!raw) return def;
            const obj = JSON.parse(raw);
            return (obj && typeof obj === "object") ? obj : def;
          } catch (_) {
            return def;
          }
        }
        function saveFlag(key, on) {
          try { localStorage.setItem(key, on ? "1" : "0"); } catch (_) {}
        }
        function loadFlag(key, def) {
          try {
            const raw = localStorage.getItem(key);
            if (raw === "1") return true;
            if (raw === "0") return false;
          } catch (_) {}
          return !!def;
        }
        let selectedNet = loadJson(KEY_WIFI_SELECTED_NET, null);
        if (!selectedNet || typeof selectedNet.ssid !== "string") selectedNet = null;
        let openPollTimer = 0;
        let lastOpenCandidates = [];
        let forceShowSetup = loadFlag(KEY_WIFI_FORCE_SETUP, false);
        let wifiModeNow = "off";
        function pack() {
          return i18n[normalizeLang(sel.value)] || i18n.en;
        }
        function t(key, fallback) {
          const p = pack();
          const v = p && p[key];
          return (typeof v === "string") ? v : (fallback || key);
        }
        function setHint(text, cls) {
          hintEl.classList.remove("ok", "err");
          if (cls) hintEl.classList.add(cls);
          hintEl.textContent = text;
        }
        function setConnectBoxVisible(show) {
          if (!wifiConnectBox) return;
          wifiConnectBox.classList.toggle("hidden", !show);
        }
        function rssiToLevel(raw) {
          const r = Number(raw);
          if (!Number.isFinite(r)) return 0;
          if (r >= -55) return 4;
          if (r >= -67) return 3;
          if (r >= -78) return 2;
          if (r >= -88) return 1;
          return 0;
        }
        function isHotAirWifiLocked(status) {
          if (!status || !status.ok) return false;
          const enabled = !!status.h312_enabled;
          const mode = String(status.h312_mode || "").toLowerCase();
          if (!enabled || mode !== "wifi") return false;
          const connected = !!status.h312_connected;
          return connected;
        }
        function normAutoOffMin(raw) {
          return Math.max(0, Math.min(1440, parseInt(String(raw || "0"), 10) || 0));
        }
        function getQuickAutoOffMin(status) {
          if (status && status.ok && typeof status.wifi_autooff_min !== "undefined") {
            return normAutoOffMin(status.wifi_autooff_min);
          }
          if (wifiAutoOffInput) return normAutoOffMin(wifiAutoOffInput.value);
          return 0;
        }
        function updateAutoOffQuickButton(status) {
          if (!btnWifiAutoOff5) return;
          const locked = isHotAirWifiLocked(status);
          const current = getQuickAutoOffMin(status);
          const on = (current === 5);
          const stateText = on ? t("opt_on", "ON") : t("opt_off", "OFF");
          const baseText = t("btn_wifi_autooff_5", "Auto-Off in 5 min");
          btnWifiAutoOff5.textContent = `${baseText}: ${stateText}`;
          setTogglePressed(btnWifiAutoOff5, on);
          btnWifiAutoOff5.disabled = locked;
          btnWifiAutoOff5.title = locked
            ? t("hint_wifi_autooff_locked_hotair_wifi", "Unavailable while hot-air is using Wi-Fi.")
            : "";
        }
        function setWifiSignal(level, state) {
          if (!staSignal) return;
          const bars = staSignal.querySelectorAll(".bar");
          const lvl = Math.max(0, Math.min(4, Number(level) || 0));
          staSignal.classList.remove("on", "warn", "off", "live", "l1", "l2", "l3", "l4");
          if (state === "on") {
            staSignal.classList.add("on", "live");
          } else if (state === "warn") {
            staSignal.classList.add("warn", "live");
          } else {
            staSignal.classList.add("off");
          }
          if (lvl > 0) staSignal.classList.add(`l${lvl}`);
          bars.forEach((bar, idx) => bar.classList.toggle("on", idx < lvl));
        }
        function isStableSta(status) {
          if (!status || !status.ok) return false;
          const mode = String(status.wifi_mode || "").toLowerCase();
          const ip = String(status.ip || "").trim();
          const staSsid = String(status.sta_ssid || "").trim();
          const staConnected = !!status.sta_connected;
          const hasIp = !!ip && ip !== "0.0.0.0";
          if (!hasIp) return false;
          if (mode === "sta") return true;
          if (mode === "apsta") {
            if (staConnected) return true;
            if (!staSsid) return false;
            if (ip === "192.168.4.1") return false;
            return true;
          }
          return false;
        }
        function formatWifiMode(modeRaw) {
          const mode = String(modeRaw || "").toLowerCase();
          if (mode === "apsta") return { text: "AP+STA", cls: "pill cyan" };
          if (mode === "sta") return { text: "STA", cls: "pill good" };
          if (mode === "ap") return { text: "AP", cls: "pill warn" };
          return { text: "OFF", cls: "pill" };
        }
        function applyWifiMode(status) {
          if (!setupPanel || !staSummary) return;
          updateAutoOffQuickButton(status);
          const stableSta = isStableSta(status);
          const mode = String((status && status.wifi_mode) ? status.wifi_mode : "off").toLowerCase();
          wifiModeNow = mode;
          const modeUi = formatWifiMode(mode);
          if (wifiSetupMode) {
            wifiSetupMode.className = modeUi.cls;
            wifiSetupMode.textContent = modeUi.text;
          }
          const lvl = rssiToLevel(status ? status.sta_rssi : null);
          if (stableSta) setWifiSignal(lvl > 0 ? lvl : ((mode === "apsta") ? 3 : 4), lvl >= 2 ? "on" : "warn");
          else if (mode === "apsta") setWifiSignal(2, "warn");
          else if (mode === "ap") setWifiSignal(1, "warn");
          else setWifiSignal(0, "off");
          const showSummary = stableSta && !forceShowSetup;
          setupPanel.classList.toggle("hidden", showSummary);
          staSummary.classList.toggle("hidden", !showSummary);
          if (listEl) {
            if (mode === "off") {
              listEl.innerHTML = "";
              listEl.classList.add("hidden");
              setConnectBoxVisible(false);
            } else {
              listEl.classList.remove("hidden");
              setConnectBoxVisible(!!selectedNet);
            }
          }
          if (!showSummary) return;
          if (staSummaryMode) {
            staSummaryMode.className = modeUi.cls;
            staSummaryMode.textContent = modeUi.text;
          }
          if (staSummaryText) staSummaryText.textContent = t("wifi_sta_summary_text", "STA mode is active. Wi-Fi setup is hidden.");
          if (staSummarySsid) staSummarySsid.textContent = String(status.sta_ssid || "-");
          if (staSummaryIp) staSummaryIp.textContent = String(status.ip || "-");
        }
        async function refreshWifiMode() {
          try {
            const s = await apiJson("/api/status");
            if (s && s.ok) applyWifiMode(s);
            else applyWifiMode(null);
            return s;
          } catch (_) {
            applyWifiMode(null);
            return null;
          }
        }
        function showApSwitchNotice(extraLine) {
          if (!apNotice || !apTitle || !apText) return;
          apTitle.textContent = t("ap_switch_title", "Reconnect To Your Router");
          const base = t("ap_switch_text", "Device is connected. Switch your PC/phone from LB-SETUP to your router Wi-Fi.");
          apText.textContent = extraLine ? `${base} ${extraLine}` : base;
          apNotice.classList.remove("hidden");
        }
        function hideApSwitchNotice() {
          if (!apNotice) return;
          apNotice.classList.add("hidden");
        }
        function saveLastOpenUrl(url) {
          try { localStorage.setItem("alb_last_open_url", String(url || "")); } catch (_) {}
        }
        function clearOpenPoll() {
          if (!openPollTimer) return;
          clearTimeout(openPollTimer);
          openPollTimer = 0;
        }
        function uniqueUrls(arr) {
          const out = [];
          const seen = Object.create(null);
          (arr || []).forEach((u) => {
            const s = String(u || "").trim();
            if (!/^https?:\/\//i.test(s)) return;
            if (seen[s]) return;
            seen[s] = true;
            out.push(s.endsWith("/") ? s : `${s}/`);
          });
          return out;
        }
        function probeBaseUrl(baseUrl, timeoutMs) {
          return new Promise((resolve) => {
            let done = false;
            const img = new Image();
            const finish = (ok) => {
              if (done) return;
              done = true;
              clearTimeout(timer);
              img.onload = null;
              img.onerror = null;
              resolve(!!ok);
            };
            const timer = setTimeout(() => finish(false), Math.max(500, timeoutMs | 0));
            img.onload = () => finish(true);
            img.onerror = () => finish(false);
            img.src = `${baseUrl}LB.png?probe=${Date.now()}`;
          });
        }
        function startAutoOpen(urls) {
          const targets = uniqueUrls(urls);
          if (!targets.length) return;
          clearOpenPoll();
          lastOpenCandidates = targets;
          const startedAt = Date.now();
          const maxWaitMs = 120000;
          const tick = async () => {
            for (const url of targets) {
              const ok = await probeBaseUrl(url, 1300);
              if (!ok) continue;
              saveLastOpenUrl(url);
              setHint(`${t("msg_wifi_opening_page", "Opening local page...")} ${url}`, "ok");
              window.location.href = url;
              return;
            }
            if (Date.now() - startedAt >= maxWaitMs) {
              setHint(`${t("msg_wifi_open_manual", "Could not open automatically. Open manually:")} ${targets.join("  |  ")}`, "err");
              clearOpenPoll();
              return;
            }
            openPollTimer = setTimeout(tick, 2200);
          };
          openPollTimer = setTimeout(tick, 2600);
        }
        function signalLevel(rssi) {
          const n = (typeof rssi === "number") ? rssi : -100;
          if (n >= -55) return 4;
          if (n >= -67) return 3;
          if (n >= -75) return 2;
          if (n >= -85) return 1;
          return 0;
        }
        function isHidden(net) {
          return !net || !net.ssid || !String(net.ssid).trim();
        }
        function needsPassword(net) {
          const auth = String((net && net.auth) ? net.auth : "").toUpperCase();
          return auth !== "OPEN";
        }
        function pillFor(net) {
          const auth = String((net && net.auth) ? net.auth : "").toUpperCase();
          const known = !!(net && net.known);
          if (known) return { cls: "pill cyan", text: (i18n[normalizeLang(sel.value)] || i18n.en).pill_known || "Known" };
          if (auth.includes("ENT")) return { cls: "pill warn", text: "ENT" };
          if (auth === "OPEN") return { cls: "pill", text: "OPEN" };
          if (auth.includes("WPA3")) return { cls: "pill cyan", text: "WPA3" };
          if (auth.includes("WPA2")) return { cls: "pill cyan", text: "WPA2" };
          if (auth.includes("WPA")) return { cls: "pill cyan", text: "WPA" };
          if (auth.includes("WEP")) return { cls: "pill warn", text: "WEP" };
          return { cls: "pill", text: auth || "SEC" };
        }
        function updateSelectionUi() {
          const ssid = selectedNet ? String(selectedNet.ssid || "") : "";
          selectedEl.textContent = ssid || "-";
          if (!selectedNet) {
            setConnectBoxVisible(false);
            btnConnect.disabled = true;
            setHint(t("hint_wifi_pick_network", "Select Wi-Fi network above."));
            return;
          }
          setConnectBoxVisible(true);
          btnConnect.disabled = false;
          if (isHidden(selectedNet)) {
            setHint(t("msg_wifi_hidden_cannot", "Hidden SSID: enter network name manually."), "err");
          } else if (needsPassword(selectedNet)) {
            setHint(t("hint_wifi_enter_password", "Enter Wi-Fi password."));
          } else {
            setHint(t("hint_wifi_pick_network", "Select Wi-Fi network above."));
          }
        }
        function makeSignal(level) {
          const wrap = document.createElement("div");
          wrap.className = "wifiSig";
          wrap.setAttribute("aria-hidden", "true");
          for (let i = 1; i <= 4; i++) {
            const b = document.createElement("span");
            b.className = `bar b${i}${i <= level ? " on" : ""}`;
            wrap.appendChild(b);
          }
          return wrap;
        }
        function renderLoading() {
          listEl.innerHTML = "";
          const it = document.createElement("div");
          it.className = "listItem state";
          const left = document.createElement("div");
          left.className = "liLeft";
          const nm = document.createElement("div");
          nm.className = "liName";
          nm.textContent = t("msg_wifi_scanning", "Scanning Wi-Fi...");
          const meta = document.createElement("div");
          meta.className = "liMeta";
          meta.textContent = t("msg_wifi_press_scan", "Press Scan");
          left.appendChild(nm);
          left.appendChild(meta);
          it.appendChild(left);
          const p = document.createElement("span");
          p.className = "pill cyan";
          p.textContent = "SCAN";
          it.appendChild(p);
          listEl.appendChild(it);
        }
        function render(nets, err) {
          const arr = Array.isArray(nets) ? nets : [];
          listEl.innerHTML = "";
          if (wifiModeNow === "off") {
            listEl.classList.add("hidden");
            return;
          }
          listEl.classList.remove("hidden");
          if (err) {
            const it = document.createElement("div");
            it.className = "listItem state";
            const left = document.createElement("div");
            left.className = "liLeft";
            const nm = document.createElement("div");
            nm.className = "liName";
            nm.textContent = t("msg_wifi_scan_unavailable", "Scan is temporarily unavailable");
            const meta = document.createElement("div");
            meta.className = "liMeta";
            meta.textContent = t("msg_wifi_scan_retry", "Press Scan again");
            left.appendChild(nm);
            left.appendChild(meta);
            it.appendChild(left);
            const p = document.createElement("span");
            p.className = "pill warn";
            p.textContent = "RETRY";
            it.appendChild(p);
            listEl.appendChild(it);
            try { console.warn("[wifi-scan]", err); } catch (_) {}
            return;
          }
          if (!arr.length) {
            const it = document.createElement("div");
            it.className = "listItem";
            const left = document.createElement("div");
            left.className = "liLeft";
            const nm = document.createElement("div");
            nm.className = "liName";
            nm.textContent = t("msg_wifi_no_networks", "No networks");
            const meta = document.createElement("div");
            meta.className = "liMeta";
            meta.textContent = t("msg_wifi_press_scan", "Press Scan");
            left.appendChild(nm);
            left.appendChild(meta);
            it.appendChild(left);
            listEl.appendChild(it);
            selectedNet = null;
            saveJson(KEY_WIFI_SELECTED_NET, null);
            updateSelectionUi();
            return;
          }
          const sorted = arr.slice().sort((a, b) => {
            const ar = (typeof a.rssi === "number") ? a.rssi : -1000;
            const br = (typeof b.rssi === "number") ? b.rssi : -1000;
            return br - ar;
          });
          let stillSelected = false;
          sorted.forEach((net) => {
            const ssid = String((net && net.ssid) ? net.ssid : "");
            const rssi = (net && typeof net.rssi === "number") ? net.rssi : null;
            const auth = String((net && net.auth) ? net.auth : "");
            const level = signalLevel(rssi);
            const it = document.createElement("div");
            it.className = "listItem selectable";
            const left = document.createElement("div");
            left.className = "liLeft";
            const nm = document.createElement("div");
            nm.className = "liName";
            nm.textContent = ssid || "(hidden)";
            const meta = document.createElement("div");
            meta.className = "liMeta";
            meta.textContent = auth || "?";
            left.appendChild(nm);
            left.appendChild(meta);
            it.appendChild(left);
            const right = document.createElement("div");
            right.className = "wifiRight";
            right.appendChild(makeSignal(level));
            const pill = pillFor(net);
            const p = document.createElement("span");
            p.className = pill.cls;
            p.textContent = pill.text;
            right.appendChild(p);
            it.appendChild(right);
            const selected = !!(selectedNet && String(selectedNet.ssid || "") === ssid && String(selectedNet.auth || "") === auth);
            if (selected) {
              it.classList.add("selected");
              stillSelected = true;
            }
            it.addEventListener("click", () => {
              selectedNet = {
                ssid: String((net && net.ssid) ? net.ssid : ""),
                auth: String((net && net.auth) ? net.auth : "")
              };
              saveJson(KEY_WIFI_SELECTED_NET, selectedNet);
              render(sorted, null);
            });
            listEl.appendChild(it);
          });
          if (!stillSelected) {
            selectedNet = null;
            saveJson(KEY_WIFI_SELECTED_NET, null);
          }
          updateSelectionUi();
        }
        async function scan() {
          btn.disabled = true;
          renderLoading();
          try {
            const r = await apiJson("/api/wifi/scan", { method: "POST", body: "{}" });
            if (r && r.ok && Array.isArray(r.nets)) render(r.nets, null);
            else render([], (r && r.error) ? String(r.error) : t("msg_wifi_scan_failed", "Wi-Fi scan failed"));
          } catch (e) {
            render([], t("msg_wifi_scan_failed", "Wi-Fi scan failed"));
          } finally {
            btn.disabled = false;
          }
        }
        async function connectSelected() {
          if (!selectedNet) {
            setHint(t("hint_wifi_pick_network", "Select Wi-Fi network above."), "err");
            return;
          }
          const ssid = String(selectedNet.ssid || "");
          if (!ssid) {
            setHint(t("msg_wifi_hidden_cannot", "Hidden SSID: enter network name manually."), "err");
            return;
          }
          const pass = String(passEl.value || "");
          if (needsPassword(selectedNet) && !pass) {
            setHint(t("hint_wifi_enter_password", "Enter Wi-Fi password."), "err");
            passEl.focus();
            return;
          }
          btn.disabled = true;
          btnConnect.disabled = true;
          setHint(t("msg_wifi_connecting", "Connecting..."));
          clearOpenPoll();
          hideApSwitchNotice();
          try {
            const r = await apiJson("/api/wifi/connect", { method: "POST", body: JSON.stringify({ ssid, pass }) });
            if (r && r.ok) {
              const host = (r.ip && String(r.ip)) ? String(r.ip) : "";
              const localUrl = (r.local_url && String(r.local_url)) ? String(r.local_url) : "";
              const ipUrl = host ? `http://${host}/` : "";
              const openUrl = /^https?:\/\//i.test(localUrl) ? localUrl : ipUrl;
              const ipInfo = host ? ` (${host})` : "";
              forceShowSetup = false;
              saveFlag(KEY_WIFI_FORCE_SETUP, false);
              setHint(`${t("msg_wifi_connected", "Connected")}${ipInfo}`, "ok");
              if (openUrl) {
                saveLastOpenUrl(openUrl);
                setHint(`${t("msg_wifi_wait_switch", "Connected. Waiting for PC to switch to router...")} ${openUrl}`, "ok");
                showApSwitchNotice(openUrl);
                if (shortcutOfferApi && shortcutOfferApi.offerOnce) shortcutOfferApi.offerOnce(openUrl);
                startAutoOpen([localUrl, ipUrl]);
              }
            } else {
              const err = (r && r.error) ? String(r.error) : "unknown";
              setHint(`${t("msg_wifi_failed", "Connection failed")}: ${err}`, "err");
            }
          } catch (e) {
            setHint(`${t("msg_wifi_failed", "Connection failed")}: ${String((e && e.message) ? e.message : e)}`, "err");
          } finally {
            btn.disabled = false;
            btnConnect.disabled = false;
          }
        }
        btn.addEventListener("click", scan);
        btnConnect.addEventListener("click", connectSelected);
        if (apBtnDownload) apBtnDownload.addEventListener("click", () => {
          if (shortcutOfferApi && shortcutOfferApi.openFromUrl) shortcutOfferApi.openFromUrl(getShortcutTargetUrl());
          else downloadShortcutFromUi();
        });
        if (apBtnCheck) apBtnCheck.addEventListener("click", () => {
          if (lastOpenCandidates.length) startAutoOpen(lastOpenCandidates);
        });
        if (apBtnHide) apBtnHide.addEventListener("click", () => hideApSwitchNotice());
        if (btnWifiChangeMode) btnWifiChangeMode.addEventListener("click", () => {
          forceShowSetup = true;
          saveFlag(KEY_WIFI_FORCE_SETUP, true);
          applyWifiMode(null);
          render([], t("msg_wifi_press_scan", "Press Scan"));
        });
        if (btnWifiAutoOff5) btnWifiAutoOff5.addEventListener("click", async () => {
          if (btnWifiAutoOff5.disabled) return;
          const current = getQuickAutoOffMin(null);
          const next = (current === 5) ? 0 : 5;
          btnWifiAutoOff5.disabled = true;
          try {
            const r = await apiJson("/api/config", {
              method: "POST",
              body: JSON.stringify({ wifi_autooff_min: next })
            });
            if (r && r.ok) {
              if (wifiAutoOffInput) wifiAutoOffInput.value = String(next);
              try { localStorage.setItem("alb_wifi_autooff_min", String(next)); } catch (_) {}
              setHint(
                next === 5
                  ? t("msg_wifi_autooff5_set", "Wi-Fi auto-off set to 5 min.")
                  : t("msg_wifi_autooff5_clear", "Wi-Fi auto-off disabled."),
                "ok"
              );
            } else {
              const err = (r && r.error) ? String(r.error) : "unknown";
              setHint(`${t("msg_wifi_autooff5_fail", "Failed to set Wi-Fi auto-off")}: ${err}`, "err");
            }
          } catch (e) {
            setHint(`${t("msg_wifi_autooff5_fail", "Failed to set Wi-Fi auto-off")}: ${String((e && e.message) ? e.message : e)}`, "err");
          } finally {
            const s = await refreshWifiMode();
            updateAutoOffQuickButton(s);
          }
        });
        passEl.addEventListener("keydown", (ev) => {
          if (ev.key === "Enter") {
            ev.preventDefault();
            connectSelected();
          }
        });
        updateSelectionUi();
        (async () => {
          const s = await refreshWifiMode();
          if (!isStableSta(s) || forceShowSetup) {
            render([], t("msg_wifi_press_scan", "Press Scan"));
          }
        })();
        setInterval(() => {
          if (forceShowSetup) return;
          refreshWifiMode();
        }, 8000);
      }
      function initPwaInstall() {
        const btn = document.getElementById("btnInstallPwa");
        if (!btn) return;
        btn.classList.remove("hidden");
        btn.addEventListener("click", () => {
          if (shortcutOfferApi && shortcutOfferApi.openFromUrl) shortcutOfferApi.openFromUrl(getShortcutTargetUrl());
          else downloadShortcutFromUi();
        });
      }
      function initTopbarRecovery() {
        const btn = document.getElementById("btnOpenRecovery");
        if (!btn) return;
        btn.addEventListener("click", () => {
          window.location.href = `${location.protocol}//${location.host}/recovery.html`;
        });
      }
      function initTopbarLogout() {
        const btn = document.getElementById("btnLogout");
        if (!btn) return;
        btn.addEventListener("click", async () => {
          if (btn.disabled) return;
          btn.disabled = true;
          try {
            const r = await apiJson("/api/auth/logout", {
              method: "POST",
              body: "{}",
              timeout_ms: 7000
            });
            if (r && r.ok) {
              try { window.location.reload(); } catch (_) { window.location.href = "/"; }
              return;
            }
            const err = (r && r.error) ? String(r.error) : "unknown";
            window.alert(`${tr("msg_logout_fail", "Logout failed")}: ${err}`);
          } catch (e) {
            window.alert(`${tr("msg_logout_fail", "Logout failed")}: ${String((e && e.message) ? e.message : e)}`);
          } finally {
            btn.disabled = false;
          }
        });
      }
      function initExtractorPicker() {
        const KEY_NAME = "alb_extractor_name";
        const KEY_ADDR = "alb_extractor_addr";
        const KEY_LAST_SCAN = "alb_extractor_last_scan";
        const KEY_LAST_SCAN_DEVICES = "alb_extractor_last_scan_devices";
        const KEY_LAST_SCAN_VALID = "alb_extractor_last_scan_valid";
        const KEY_FIND_TOGGLE = "alb_extractor_find_toggle";
        const KEY_VIEW = "alb_extractor_view";
        const savedWrap = document.getElementById("extractorSavedWrap");
        const searchWrap = document.getElementById("extractorSearchWrap");
        const resultsWrap = document.getElementById("extractorResultsWrap");
        const singleWrap = document.getElementById("extractorSingleWrap");
        const multiWrap = document.getElementById("extractorMultiWrap");
        const savedNameEl = document.getElementById("extractorSavedName");
        const singleNameEl = document.getElementById("extractorSingleName");
        const pickListEl = document.getElementById("extractorPickList");
        const speedEl = document.getElementById("extractorSpeedPill");
        const bleSignalEl = document.getElementById("extractorBleSignal");
        const btnFind = document.getElementById("btnExtractorFind");
        const btnChange = document.getElementById("btnExtractorChange");
        const btnConnect = document.getElementById("btnExtractorConnect");
        const btnSave = document.getElementById("btnExtractorSave");
        let speedTimer = null;
        let speedInFlight = false;
        let speedNeedConnect = true;
        if (!savedWrap || !searchWrap || !resultsWrap || !singleWrap || !multiWrap || !btnFind) return;
        function pack() {
          return i18n[normalizeLang(sel.value)] || i18n.en;
        }
        function t(key, fallback) {
          const p = pack();
          const v = p && p[key];
          return (typeof v === "string") ? v : (fallback || key);
        }
        function loadStr(key, def) {
          try { return localStorage.getItem(key) || def; } catch (_) { return def; }
        }
        function saveStr(key, v) {
          try { localStorage.setItem(key, String(v)); } catch (_) {}
        }
        function dropExtractorLocalBinding() {
          try {
            localStorage.removeItem(KEY_NAME);
            localStorage.removeItem(KEY_ADDR);
            localStorage.removeItem(KEY_LAST_SCAN);
            localStorage.removeItem(KEY_LAST_SCAN_DEVICES);
            localStorage.removeItem(KEY_LAST_SCAN_VALID);
            localStorage.removeItem(KEY_VIEW);
          } catch (_) {}
        }
        function publicExtractorName(raw) {
          const s = String(raw || "").trim();
          if (!s) return "ES02";
          const up = s.toUpperCase();
          if (up.startsWith("TES02") || up.startsWith("ES02")) return "ES02";
          return s;
        }
        async function syncExtractorBindingFromDevice() {
          const cfg = await apiJson("/api/config", { method: "GET", timeout_ms: 5000 });
          if (!cfg || !cfg.ok) return;
          const bound = !!cfg.ble_bound;
          const devName = String(cfg.ble_locked_name || "").trim();
          const devAddr = String(cfg.ble_locked_addr || "").trim();
          if (!bound || (!devName && !devAddr)) {
            dropExtractorLocalBinding();
            return;
          }
          if (devName) saveStr(KEY_NAME, devName);
          if (devAddr) saveStr(KEY_ADDR, devAddr);
        }
        function normPct(raw) {
          const n = Number(raw);
          if (!Number.isFinite(n)) return null;
          return Math.max(0, Math.min(100, Math.round(n)));
        }
        function loadFlag(key) {
          try { return localStorage.getItem(key) === "1"; } catch (_) { return false; }
        }
        function isAnySourceActive() {
          return loadFlag("alb_presence_h1") || loadFlag("alb_presence_h2") || loadFlag("alb_presence_ha");
        }
        function setExtractorSpeed(text, kind) {
          if (!speedEl) return;
          speedEl.classList.remove("good", "warn", "cyan");
          if (kind === "ok") speedEl.classList.add("good");
          else if (kind === "err") speedEl.classList.add("warn");
          else if (kind === "wait") speedEl.classList.add("cyan");
          speedEl.textContent = text;
        }
        function rssiToLevel(raw) {
          const r = Number(raw);
          if (!Number.isFinite(r)) return 0;
          if (r >= -55) return 4;
          if (r >= -67) return 3;
          if (r >= -78) return 2;
          if (r >= -88) return 1;
          return 0;
        }
        function setBleSignal(state, level) {
          if (!bleSignalEl) return;
          bleSignalEl.classList.remove("on", "warn", "off", "live", "l1", "l2", "l3", "l4");
          const lvl = Math.max(0, Math.min(4, Number(level) || 0));
          if (state === "on") bleSignalEl.classList.add("on", "live");
          else if (state === "warn") bleSignalEl.classList.add("warn", "live");
          else bleSignalEl.classList.add("off");
          if (lvl > 0) bleSignalEl.classList.add(`l${lvl}`);
        }
        async function refreshExtractorSpeed() {
          if (!speedEl) return;
          if (speedInFlight) return;
          const savedName = String(loadStr(KEY_NAME, "") || "").trim();
          if (!savedName) {
            speedNeedConnect = true;
            setExtractorSpeed("--%", "");
            setBleSignal("off");
            return;
          }
          const activeByUi = isAnySourceActive();
          if (activeByUi && speedNeedConnect) setBleSignal("warn");
          else if (!activeByUi) setBleSignal("off");
          speedInFlight = true;
          try {
            const statusPath = (activeByUi && speedNeedConnect) ? "/api/extractor/status?connect=1" : "/api/extractor/status";
            const r = await apiJson(statusPath, { method: "GET", timeout_ms: 2600 });
            if (!r || !r.ok) {
              if (activeByUi) speedNeedConnect = true;
              setExtractorSpeed("--%", "err");
              setBleSignal(activeByUi ? "warn" : "off");
              return;
            }
            const cur = normPct(r.speed);
            const lvl = rssiToLevel(r.rssi);
            if (r.ble_ok && cur != null) {
              if (activeByUi) speedNeedConnect = false;
              setExtractorSpeed(`${cur}%`, "ok");
              setBleSignal("on", lvl);
              return;
            }
            if (!activeByUi) {
              setExtractorSpeed("0%", "");
              setBleSignal("off", lvl);
              return;
            }
            speedNeedConnect = true;
            setExtractorSpeed("--%", "err");
            setBleSignal("warn", lvl);
          } finally {
            speedInFlight = false;
          }
        }
        function ensureExtractorSpeedLoop() {
          if (!speedEl || speedTimer) return;
          speedTimer = setInterval(() => {
            refreshExtractorSpeed().catch(() => {});
          }, 4000);
        }
        function loadJsonDevices(key) {
          try {
            const raw = localStorage.getItem(key) || "";
            const v = JSON.parse(raw);
            return Array.isArray(v) ? v : [];
          } catch (_) {
            return [];
          }
        }
        function showSaved(name) {
          if (savedNameEl) savedNameEl.textContent = publicExtractorName(name);
          savedWrap.classList.remove("hidden");
          searchWrap.classList.add("hidden");
          resultsWrap.classList.add("hidden");
          saveStr(KEY_VIEW, "saved");
          setBleSignal("warn");
          refreshExtractorSpeed().catch(() => {});
        }
        function showSearch() {
          savedWrap.classList.add("hidden");
          searchWrap.classList.remove("hidden");
          resultsWrap.classList.add("hidden");
          saveStr(KEY_VIEW, "search");
          setExtractorSpeed("--%", "");
          setBleSignal("off");
        }
        function mockFind() {
          const prev = parseInt(loadStr(KEY_FIND_TOGGLE, "0"), 10) || 0;
          const next = (prev + 1) % 2;
          saveStr(KEY_FIND_TOGGLE, String(next));
          if (next === 0) return ["ES02"];
          return ["ES02", "ES02"];
        }
        async function selectDevice(dev) {
          const name = dev && dev.name ? String(dev.name) : "";
          const addr = dev && dev.addr ? String(dev.addr) : "";
          if (name) saveStr(KEY_NAME, name);
          if (addr) saveStr(KEY_ADDR, addr);
          const r = await apiJson("/api/extractor/select", {
            method: "POST",
            body: JSON.stringify({ addr, name })
          });
          if (!r || !r.ok) return { ok: false, error: (r && r.error) ? r.error : "select failed" };
          return { ok: true };
        }
        function normDeviceList(found) {
          if (!Array.isArray(found)) return [];
          if (!found.length) return [];
          if (typeof found[0] === "string") return found.map((s) => ({ name: String(s), addr: "" }));
          return found.map((d) => ({
            name: String((d && d.name) ? d.name : ""),
            addr: String((d && d.addr) ? d.addr : ""),
            rssi: (d && typeof d.rssi === "number") ? d.rssi : null,
          })).filter((d) => d.name || d.addr);
        }
        function renderResults(found) {
          const devices = normDeviceList(found);
          saveStr(KEY_LAST_SCAN, JSON.stringify(devices.map((d) => d.name || d.addr)));
          saveStr(KEY_LAST_SCAN_DEVICES, JSON.stringify(devices));
          saveStr(KEY_LAST_SCAN_VALID, "1");
          saveStr(KEY_VIEW, "results");
          resultsWrap.classList.remove("hidden");
          singleWrap.classList.add("hidden");
          multiWrap.classList.add("hidden");
          if (devices.length <= 0) {
            const cur = loadStr(KEY_NAME, "");
            if (cur) showSaved(cur);
            return;
          }
          if (devices.length === 1) {
            singleWrap.classList.remove("hidden");
            if (singleNameEl) singleNameEl.textContent = publicExtractorName(devices[0].name || devices[0].addr || "ES02");
            if (btnConnect) {
              btnConnect.onclick = async () => {
                const r = await selectDevice(devices[0]);
                if (!r.ok) return;
                showSaved(devices[0].name || devices[0].addr || "ES02");
              };
            }
            return;
          }
          multiWrap.classList.remove("hidden");
          if (!pickListEl || !btnSave) return;
          pickListEl.innerHTML = "";
          let selected = null;
          function syncSave() {
            btnSave.disabled = !selected;
          }
          devices.forEach((dev) => {
            const name = publicExtractorName(dev.name || dev.addr || "ES02");
            const row = document.createElement("label");
            row.className = "pickItem";
            const left = document.createElement("div");
            left.className = "pickLeft";
            const cb = document.createElement("input");
            cb.type = "checkbox";
            cb.className = "pickCheck";
            cb.setAttribute("aria-label", name);
            const nm = document.createElement("div");
            nm.className = "pickName";
            nm.textContent = name;
            left.appendChild(cb);
            left.appendChild(nm);
            row.appendChild(left);
            cb.addEventListener("change", () => {
              if (cb.checked) {
                selected = dev;
                pickListEl.querySelectorAll("input.pickCheck").forEach((el) => {
                  if (el !== cb) el.checked = false;
                });
              } else {
                if (selected === dev) selected = null;
              }
              syncSave();
            });
            pickListEl.appendChild(row);
          });
          btnSave.disabled = true;
          btnSave.onclick = async () => {
            if (!selected) return;
            const r = await selectDevice(selected);
            if (!r.ok) return;
            showSaved(selected.name || selected.addr || "ES02");
          };
        }
        btnFind.addEventListener("click", async () => {
          btnFind.disabled = true;
          try {
            const r = await apiJson("/api/extractor/scan", { method: "POST", body: JSON.stringify({ scan_ms: 4000 }) });
            if (r && r.ok && Array.isArray(r.devices)) renderResults(r.devices);
            else renderResults(mockFind());
          } finally {
            btnFind.disabled = false;
          }
        });
        if (btnChange) btnChange.addEventListener("click", (ev) => {
          ev.preventDefault();
          ev.stopPropagation();
          showSearch();
        });
        (async () => {
          try { await syncExtractorBindingFromDevice(); } catch (_) {}
          const cur = loadStr(KEY_NAME, "");
          const view = loadStr(KEY_VIEW, "");
          if (view === "results") {
            const lastDevices = loadJsonDevices(KEY_LAST_SCAN_DEVICES);
            if (lastDevices.length > 0) {
              renderResults(lastDevices);
            } else if (cur) {
              showSaved(cur);
            } else {
              showSearch();
            }
          } else if (view === "search") {
            showSearch();
          } else if (cur) {
            showSaved(cur);
          } else {
            showSearch();
          }
        })();
        ensureExtractorSpeedLoop();
        refreshExtractorSpeed().catch(() => {});
      }
      function initHotAirBinding() {
        const ipEl = document.getElementById("hotAirBindIp");
        const tempEl = document.getElementById("hotAirTempOn");
        const tempLblEl = document.getElementById("hotAirTempOnLbl");
        const btnScan = document.getElementById("btnHotAirScan");
        const btnBind = document.getElementById("btnHotAirBind");
        const btnAdd = document.getElementById("btnHotAirAdd");
        const btnRemove = document.getElementById("btnHotAirRemove");
        const btnResetHotAir = document.getElementById("btnHotAirReset");
        const modeWifiBtn = document.getElementById("hotAirModeWifi");
        const modeBleBtn = document.getElementById("hotAirModeBle");
        const btnApplyLogic = document.getElementById("btnApplyLogic");
        const slot1Btn = document.getElementById("hotAirSlot1");
        const slot2Btn = document.getElementById("hotAirSlot2");
        const slot3Btn = document.getElementById("hotAirSlot3");
        const bindAddrLabelEl = document.getElementById("hotAirBindAddrLabel");
        const bindEditorEl = document.getElementById("hotAirBindEditor");
        const bindActionsEl = document.getElementById("hotAirBindActions");
        const scanListEl = document.getElementById("hotAirScanList");
        const rtEl = document.getElementById("hotAirBindRt");
        const stateEl = document.getElementById("hotAirBindState");
        const hintEl = document.getElementById("hotAirBindHint");
        const logEl = document.getElementById("eventLog");
        if (!ipEl || !tempEl || !btnBind || !scanListEl || !stateEl || !hintEl) return;
        const KEY_IP = "alb_h312_ip";
        const KEY_BLE_ADDR = "alb_h312_ble_addr";
        const KEY_BLE_NAME = "alb_h312_ble_name";
        const KEY_MODE = "alb_h312_mode";
        const KEY_TEMP_ON = "alb_h312_t_on";
        const SLOT_COUNT = 3;
        const KEY_SLOT_ACTIVE = "alb_h312_slot_active";
        const KEY_SLOT_COUNT = "alb_h312_slot_count";
        const keySlotIp = (slot) => `alb_h312_slot_${slot}_ip`;
        const keySlotTempOn = (slot) => `alb_h312_slot_${slot}_t_on`;
        const keySlotBound = (slot) => `alb_h312_slot_${slot}_bound`;
        const slotButtons = [slot1Btn, slot2Btn, slot3Btn].filter(Boolean);
        let activeSlot = 1;
        let slotCount = 1;
        let hotAirLiveTimer = null;
        let hotAirLiveInFlight = false;
        function loadStr(key, def) {
          try { return localStorage.getItem(key) || def; } catch (_) { return def; }
        }
        function saveStr(key, v) {
          try { localStorage.setItem(key, String(v)); } catch (_) {}
        }
        function ts() {
          const d = new Date();
          const p2 = (n) => String(n).padStart(2, "0");
          return `${d.getFullYear()}-${p2(d.getMonth() + 1)}-${p2(d.getDate())} ${p2(d.getHours())}:${p2(d.getMinutes())}:${p2(d.getSeconds())}`;
        }
        function append(line) {
          if (!logEl) return;
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          logEl.textContent = `${cur}\n[ui] ${ts()} ${line}`;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function setRt(text, kind) {
          if (!rtEl) return;
          rtEl.classList.remove("good", "warn", "cyan");
          if (kind === "ok") rtEl.classList.add("good");
          else if (kind === "err") rtEl.classList.add("warn");
          else if (kind === "wait") rtEl.classList.add("cyan");
          rtEl.textContent = text;
        }
        function syncBindControls(kind) {
          const isBound = String(kind || stateEl.dataset.state || "not_bound") === "bound";
          if (bindEditorEl) bindEditorEl.classList.toggle("hidden", isBound);
          if (bindActionsEl) bindActionsEl.classList.toggle("hidden", isBound);
          if (hintEl) hintEl.classList.toggle("hidden", isBound);
          if (isBound) {
            scanListEl.classList.add("hidden");
            scanListEl.innerHTML = "";
            setHint("");
          }
        }
        function setState(kind) {
          stateEl.dataset.state = kind;
          stateEl.classList.remove("warn", "good", "cyan");
          if (kind === "bound") {
            stateEl.classList.add("good");
            stateEl.textContent = tr("hotair_state_bound", "Bound");
            setRt("...°C", "wait");
            syncBindControls(kind);
            return;
          }
          if (kind === "binding") {
            stateEl.classList.add("cyan");
            stateEl.textContent = tr("hotair_state_binding", "Binding...");
            setRt("...°C", "wait");
            syncBindControls(kind);
            return;
          }
          stateEl.classList.add("warn");
          stateEl.textContent = tr("hotair_state_not_bound", "Not Bound");
          setRt("--°C", "");
          syncBindControls(kind);
        }
        function setHint(text, cls) {
          hintEl.classList.remove("ok", "err");
          if (cls) hintEl.classList.add(cls);
          hintEl.textContent = text;
        }
        async function refreshHotAirLiveTemp() {
          const stateKind = String(stateEl.dataset.state || "not_bound");
          if (stateKind !== "bound") return;
          if (hotAirLiveInFlight) return;
          hotAirLiveInFlight = true;
          try {
            const r = await apiJson("/api/status", { method: "GET", timeout_ms: 2800 });
            if (!r || !r.ok) return;
            const rawRt = r.h312_real_temp;
            const hasRt = ((typeof rawRt === "number" && Number.isFinite(rawRt)) ||
                           (typeof rawRt === "string" && rawRt.trim() !== "" && Number.isFinite(Number(rawRt))));
            const rt = hasRt ? Number(rawRt) : NaN;
            const connected = !!r.h312_connected;
            const lastJsonMs = Number(r.h312_last_json_ms || 0);
            const wifiMode = String(r.wifi_mode || "");
            const h312Mode = String(r.h312_mode || "wifi").toLowerCase();
            const herr = String(r.h312_error || "").trim();
            if (h312Mode !== "ble" && (wifiMode === "ap" || wifiMode === "off")) {
              setRt("--°C", "err");
              setHint("No STA link (AP only)", "err");
              return;
            }

            if (hasRt && rt > -100) {
              setRt(`${Math.round(rt)}°C`, "ok");
              if (!connected && herr) {
                setHint(`Stream unstable: ${herr}`, "err");
              } else {
                setHint("");
              }
              return;
            }

            if (!connected) {
              if (herr) {
                setRt("--°C", "err");
                setHint(`Stream error: ${herr}`, "err");
              } else {
                setRt("--°C", "err");
                setHint("No stream", "err");
              }
              return;
            }

            if (!Number.isFinite(lastJsonMs) || lastJsonMs <= 0) {
              setRt("--°C", "err");
              setHint("No telemetry frames", "err");
              return;
            }
            setRt("...°C", "wait");
            setHint("");
          } finally {
            hotAirLiveInFlight = false;
          }
        }
        function ensureHotAirLiveLoop() {
          if (hotAirLiveTimer) return;
          hotAirLiveTimer = setInterval(() => {
            refreshHotAirLiveTemp().catch(() => {});
          }, 2000);
        }
        function isIpv4(ip) {
          const s = String(ip || "").trim();
          const m = /^(\d{1,3})(\.\d{1,3}){3}$/.exec(s);
          if (!m) return false;
          return s.split(".").every((x) => {
            const n = parseInt(x, 10);
            return Number.isFinite(n) && n >= 0 && n <= 255;
          });
        }
        function parseTempOn(v) {
          const n = parseInt(String(v || "").trim(), 10);
          if (!Number.isFinite(n)) return 140;
          return Math.max(30, Math.min(500, n));
        }
        function normHotAirMode(raw) {
          const m = String(raw || "").trim().toLowerCase();
          return m === "ble" ? "ble" : "wifi";
        }
        function currentMode() {
          return normHotAirMode(loadStr(KEY_MODE, "wifi"));
        }
        function normBleAddr(raw) {
          const s = String(raw || "").trim().toUpperCase();
          if (!s) return "";
          if (/^[0-9A-F]{12}$/.test(s)) {
            return `${s.slice(0, 2)}:${s.slice(2, 4)}:${s.slice(4, 6)}:${s.slice(6, 8)}:${s.slice(8, 10)}:${s.slice(10, 12)}`;
          }
          return s;
        }
        function isBleAddr(raw) {
          const s = normBleAddr(raw);
          return /^[0-9A-F]{2}(:[0-9A-F]{2}){5}$/.test(s);
        }
        function setModeButtons(mode) {
          const wifiOn = mode !== "ble";
          setTogglePressed(modeWifiBtn, wifiOn);
          setTogglePressed(modeBleBtn, !wifiOn);
          if (bindAddrLabelEl) {
            bindAddrLabelEl.textContent = wifiOn
              ? tr("label_hotair_ip", "H310/H312 IP")
              : tr("label_hotair_ble_addr", "H310/H312 BLE MAC");
          }
          ipEl.placeholder = wifiOn ? "192.168.1.122" : "";
          if (btnScan) btnScan.textContent = wifiOn
            ? tr("btn_scan_hotair", "Scan H310/H312")
            : tr("btn_scan_hotair_ble", "Scan H310/H312 BLE");
          if (btnBind) btnBind.textContent = wifiOn
            ? tr("btn_bind_hotair", "Bind Hot Air")
            : tr("btn_bind_hotair_ble", "Bind H310/H312");
        }
        function updateHotAirTempLabel(v) {
          if (!tempLblEl) return;
          const n = parseTempOn(v == null ? tempEl.value : v);
          const base = tr("label_hotair_temp_on", "Temperature Trigger Threshold");
          tempLblEl.textContent = `${base} (${n}°C)`;
        }
        function calcTempOff(tOn) {
          let tOff = Math.round(Number(tOn) - 20);
          if (!Number.isFinite(tOff)) tOff = 90;
          if (tOff < -40) tOff = -40;
          if (tOff > 599) tOff = 599;
          if (tOff >= tOn) tOff = tOn - 1;
          return tOff;
        }
        function parseSlot(v) {
          const n = parseInt(String(v || "").trim(), 10);
          if (!Number.isFinite(n)) return 1;
          return Math.max(1, Math.min(SLOT_COUNT, n));
        }
        function parseSlotCount(v) {
          const n = parseInt(String(v || "").trim(), 10);
          if (!Number.isFinite(n)) return 1;
          return Math.max(1, Math.min(SLOT_COUNT, n));
        }
        function slotTitle(slot) {
          return `Hot Air ${slot}`;
        }
        function detectSlotCount() {
          let maxSlot = 1;
          for (let s = 1; s <= SLOT_COUNT; s++) {
            const ip = String(loadStr(keySlotIp(s), "") || "").trim();
            const bound = (loadStr(keySlotBound(s), "0") === "1");
            if (ip || bound) maxSlot = s;
          }
          return maxSlot;
        }
        function setSlotCount(next) {
          slotCount = parseSlotCount(next);
          saveStr(KEY_SLOT_COUNT, String(slotCount));
        }
        function renderSlotControls() {
          slotButtons.forEach((btn, idx) => {
            const visible = (idx + 1) <= slotCount;
            btn.classList.toggle("hidden", !visible);
          });
          if (btnAdd) {
            const full = slotCount >= SLOT_COUNT;
            btnAdd.classList.toggle("hidden", full);
            btnAdd.disabled = full;
            btnAdd.setAttribute("aria-disabled", full ? "true" : "false");
          }
          if (btnRemove) {
            const minReached = slotCount <= 1;
            btnRemove.classList.toggle("hidden", minReached);
            btnRemove.disabled = minReached;
            btnRemove.setAttribute("aria-disabled", minReached ? "true" : "false");
          }
        }
        function loadSlotProfile(slot) {
          const s = parseSlot(slot);
          let ip = loadStr(keySlotIp(s), "");
          let tOnRaw = loadStr(keySlotTempOn(s), "");
          let bound = (loadStr(keySlotBound(s), "0") === "1");
          if (s === 1) {
            if (!ip) ip = loadStr(KEY_IP, "");
            if (!tOnRaw) tOnRaw = loadStr(KEY_TEMP_ON, "");
            if (!bound && ip) bound = true;
          }
          const tOn = parseTempOn(tOnRaw || "140");
          return { ip: String(ip || "").trim(), tOn, bound: !!bound };
        }
        function saveSlotProfile(slot, profile) {
          const s = parseSlot(slot);
          const ip = String((profile && profile.ip) ? profile.ip : "").trim();
          const tOn = parseTempOn((profile && profile.tOn) != null ? profile.tOn : "140");
          const bound = !!(profile && profile.bound);
          saveStr(keySlotIp(s), ip);
          saveStr(keySlotTempOn(s), String(tOn));
          saveStr(keySlotBound(s), bound ? "1" : "0");
          if (s === 1) {
            saveStr(KEY_IP, ip);
            saveStr(KEY_TEMP_ON, String(tOn));
          }
        }
        function clearSlotProfile(slot) {
          const s = parseSlot(slot);
          saveStr(keySlotIp(s), "");
          saveStr(keySlotTempOn(s), "140");
          saveStr(keySlotBound(s), "0");
          if (s === 1) {
            saveStr(KEY_IP, "");
            saveStr(KEY_TEMP_ON, "140");
          }
        }
        function clearHotAirLocalCache() {
          try {
            for (let i = localStorage.length - 1; i >= 0; i--) {
              const key = String(localStorage.key(i) || "");
              if (key.startsWith("alb_h312_")) localStorage.removeItem(key);
            }
            localStorage.removeItem("alb_ha_name");
          } catch (_) {}
        }
        function setHotAirBusyControls(disabled) {
          [btnResetHotAir, btnScan, btnBind, btnAdd, btnRemove, modeWifiBtn, modeBleBtn]
            .filter(Boolean)
            .forEach((el) => { el.disabled = !!disabled; });
        }
        async function resetHotAirConfig() {
          const ask = tr("msg_hotair_reset_confirm", "Reset H310/H312 Wi-Fi and BLE configuration?");
          let confirmed = true;
          try { confirmed = window.confirm(ask); } catch (_) { confirmed = true; }
          if (!confirmed) return;
          setHotAirBusyControls(true);
          scanListEl.classList.add("hidden");
          scanListEl.innerHTML = "";
          setHint(tr("msg_hotair_reset_working", "Resetting H310/H312 configuration..."));
          append("[h312] reset requested");
          try {
            const r = await apiJson("/api/config", {
              method: "POST",
              body: JSON.stringify({
                h312_enabled: false,
                h312_mode: "wifi",
                h312_ip: "",
                h312_ble_addr: "",
                h312_ble_name: "",
                h312_port: 6666,
                h312_t_on: 140,
                h312_t_off: 90,
                h312_slot: 1
              })
            });
            if (!r || !r.ok) {
              const err = (r && r.error) ? String(r.error) : "unknown";
              setHint(`${tr("msg_hotair_reset_fail", "H310/H312 reset failed")}: ${err}`, "err");
              append(`[h312] reset failed: ${err}`);
              return;
            }
            clearHotAirLocalCache();
            for (let s = 1; s <= SLOT_COUNT; s++) clearSlotProfile(s);
            setSlotCount(1);
            activeSlot = 1;
            saveStr(KEY_SLOT_ACTIVE, "1");
            saveStr(KEY_MODE, "wifi");
            saveStr(KEY_BLE_ADDR, "");
            saveStr(KEY_BLE_NAME, "");
            ipEl.value = "";
            tempEl.value = "140";
            updateHotAirTempLabel(140);
            setState("not_bound");
            setRt("--°C", "");
            applySlotToUi(1, tr("msg_hotair_reset_ok", "H310/H312 configuration reset"));
            append("[h312] reset ok: wifi+ble config cleared");
          } catch (e) {
            const err = String((e && e.message) ? e.message : e);
            setHint(`${tr("msg_hotair_reset_fail", "H310/H312 reset failed")}: ${err}`, "err");
            append(`[h312] reset error: ${err}`);
          } finally {
            setHotAirBusyControls(false);
            refreshFromDevice().catch(() => {});
          }
        }
        function saveCurrentUiToSlot(slot) {
          saveSlotProfile(slot, {
            ip: String(ipEl.value || "").trim(),
            tOn: parseTempOn(tempEl.value),
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
        }
        function renderSlotButtons(slot) {
          const s = Math.min(parseSlot(slot), slotCount);
          activeSlot = s;
          renderSlotControls();
          slotButtons.forEach((btn, idx) => {
            const on = (idx + 1) === s;
            setTogglePressed(btn, on);
            btn.textContent = slotTitle(idx + 1);
          });
        }
        function applySlotToUi(slot, hintText) {
          const s = parseSlot(slot);
          const p = loadSlotProfile(s);
          const mode = currentMode();
          const hotAirNameEl = document.getElementById("hotAirName");
          setModeButtons(mode);
          if (mode === "ble") {
            ipEl.value = normBleAddr(loadStr(KEY_BLE_ADDR, ""));
          } else {
            ipEl.value = p.ip;
          }
          tempEl.value = String(p.tOn);
          updateHotAirTempLabel(p.tOn);
          if (hotAirNameEl) {
            hotAirNameEl.value = slotTitle(s);
            saveStr("alb_ha_name", slotTitle(s));
          }
          const bound = (mode === "ble")
            ? (p.bound && isBleAddr(loadStr(KEY_BLE_ADDR, "")))
            : (p.bound && isIpv4(p.ip));
          setState(bound ? "bound" : "not_bound");
          if (hintText != null) {
            setHint(hintText);
          } else if (bound) {
            setHint(`${slotTitle(s)} selected`);
          } else {
            setHint(mode === "ble" ? "H310/H312 BLE MAC." : "H310/H312 IP.");
          }
          renderSlotButtons(s);
        }
        async function saveHotAirConfig(reason) {
          const mode = currentMode();
          const ip = String(ipEl.value || "").trim();
          const bleAddr = normBleAddr(ip || loadStr(KEY_BLE_ADDR, ""));
          const bleName = String(loadStr(KEY_BLE_NAME, "") || "").trim();
          const tOn = parseTempOn(tempEl.value);
          const tOff = calcTempOff(tOn);
          tempEl.value = String(tOn);
          updateHotAirTempLabel(tOn);
          saveSlotProfile(activeSlot, {
            ip: (mode === "ble") ? "" : ip,
            tOn,
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
          if (mode === "ble") {
            if (!isBleAddr(bleAddr)) {
              append(`[h312] ${reason}: skip save (invalid ble addr)`);
              return { ok: false, skipped: true, error: "invalid_ble_addr" };
            }
          } else if (!isIpv4(ip)) {
            append(`[h312] ${reason}: skip save (invalid ip)`);
            return { ok: false, skipped: true, error: "invalid_ip" };
          }
          const hasTarget = (mode === "ble") ? isBleAddr(bleAddr) : isIpv4(ip);
          const wantEnabled = ((String(stateEl.dataset.state || "") === "bound") && hasTarget);
          let enabled = wantEnabled;
          let port = 6666;
          const cfg = await apiJson("/api/config", { method: "GET" });
          if (cfg && cfg.ok) {
            if (!wantEnabled && typeof cfg.h312_enabled !== "undefined") enabled = !!cfg.h312_enabled;
            const p = Number(cfg.h312_port || 6666);
            if (Number.isFinite(p) && p > 0 && p <= 65535) port = Math.round(p);
          }
          const body = {
            h312_enabled: enabled,
            h312_mode: mode,
            h312_ip: (mode === "ble") ? "" : ip,
            h312_ble_addr: (mode === "ble") ? bleAddr : "",
            h312_ble_name: (mode === "ble") ? bleName : "",
            h312_port: port,
            h312_t_on: tOn,
            h312_t_off: tOff,
            h312_slot: activeSlot
          };
          const r = await apiJson("/api/config", { method: "POST", body: JSON.stringify(body) });
          if (r && r.ok) {
            if (mode === "ble") {
              saveStr(KEY_BLE_ADDR, bleAddr);
              saveStr(KEY_BLE_NAME, bleName);
              saveSlotProfile(activeSlot, { ip: "", tOn, bound: true });
              append(`[h312] ${reason}: saved ble=${bleAddr} t_on=${tOn} t_off=${tOff}`);
            } else {
              saveSlotProfile(activeSlot, { ip, tOn, bound: true });
              append(`[h312] ${reason}: saved ip=${ip} t_on=${tOn} t_off=${tOff}`);
            }
            return { ok: true };
          }
          const err = (r && r.error) ? String(r.error) : "unknown";
          append(`[h312] ${reason}: save failed: ${err}`);
          return { ok: false, error: err };
        }
        function syncLocalUi() {
          const p = loadSlotProfile(activeSlot);
          const mode = currentMode();
          const ip = (mode === "ble") ? normBleAddr(loadStr(KEY_BLE_ADDR, "")) : p.ip;
          const tOn = p.tOn;
          if (ip && !ipEl.value.trim()) ipEl.value = ip;
          tempEl.value = String(tOn);
          updateHotAirTempLabel(tOn);
        }
        function renderScanResults(devices) {
          const src = Array.isArray(devices) ? devices : [];
          const map = new Map();
          src.forEach((dev) => {
            const ip = String((dev && dev.ip) ? dev.ip : "").trim();
            if (!ip) return;
            const prev = map.get(ip);
            const prevTemp = (prev && typeof prev.realTimeTemp === "number") ? prev.realTimeTemp : null;
            const curTemp = (dev && typeof dev.realTimeTemp === "number") ? dev.realTimeTemp : null;
            if (!prev || (prevTemp == null && curTemp != null)) map.set(ip, dev);
          });
          const arr = Array.from(map.values()).sort((a, b) => {
            const ai = String((a && a.ip) ? a.ip : "").split(".").map((x) => parseInt(x, 10) || 0);
            const bi = String((b && b.ip) ? b.ip : "").split(".").map((x) => parseInt(x, 10) || 0);
            for (let i = 0; i < 4; i++) {
              if (ai[i] !== bi[i]) return ai[i] - bi[i];
            }
            return 0;
          });
          scanListEl.innerHTML = "";
          if (!arr.length) {
            scanListEl.classList.add("hidden");
            return;
          }
          arr.forEach((dev) => {
            const ip = String((dev && dev.ip) ? dev.ip : "").trim();
            if (!ip) return;
            const name = String((dev && dev.name) ? dev.name : "H31X");
            const model = String((dev && dev.model) ? dev.model : "").trim();
            const sn = String((dev && dev.sn) ? dev.sn : "").trim();
            const ws = (dev && typeof dev.workStatus === "number") ? dev.workStatus : null;
            const rt = (dev && typeof dev.realTimeTemp === "number") ? dev.realTimeTemp : null;
            const row = document.createElement("div");
            row.className = "listItem";
            row.style.cursor = "pointer";
            const left = document.createElement("div");
            left.className = "liLeft";
            const nm = document.createElement("div");
            nm.className = "liName";
            const titleParts = [name];
            if (model) titleParts.push(model);
            nm.textContent = `${titleParts.join(" / ")} (${ip})`;
            const meta = document.createElement("div");
            meta.className = "liMeta";
            const wsTxt = (ws == null) ? "-" : String(ws);
            const rtTxt = (rt == null) ? "-" : String(rt);
            const serialTxt = sn ? ` sn=${sn}` : "";
            meta.textContent = `work=${wsTxt} temp=${rtTxt}${serialTxt}`;
            left.appendChild(nm);
            left.appendChild(meta);
            const pill = document.createElement("span");
            pill.className = "pill cyan";
            pill.textContent = "Select";
            row.appendChild(left);
            row.appendChild(pill);
            row.addEventListener("click", () => {
              ipEl.value = ip;
              const info = [name];
              if (model) info.push(`model=${model}`);
              if (sn) info.push(`sn=${sn}`);
              setHint(`Selected: ${info.join(" ")} ${ip}`);
            });
            scanListEl.appendChild(row);
          });
          scanListEl.classList.remove("hidden");
        }
        async function refreshFromDevice() {
          const r = await apiJson("/api/config", { method: "GET" });
          if (!r || !r.ok) {
            applySlotToUi(activeSlot, null);
            return;
          }
          const mode = normHotAirMode(r.h312_mode || loadStr(KEY_MODE, "wifi"));
          saveStr(KEY_MODE, mode);
          const cfgBleAddr = normBleAddr(String(r.h312_ble_addr || ""));
          const cfgBleName = String(r.h312_ble_name || "").trim();
          saveStr(KEY_BLE_ADDR, cfgBleAddr);
          saveStr(KEY_BLE_NAME, cfgBleName);
          const cfgIp = String(r.h312_ip || "").trim();
          const cfgOn = parseTempOn(r.h312_t_on);
          const enabled = !!r.h312_enabled;
          const bound = (mode === "ble") ? (enabled && isBleAddr(cfgBleAddr)) : (enabled && !!cfgIp);
          saveSlotProfile(activeSlot, { ip: (mode === "ble") ? "" : cfgIp, tOn: cfgOn, bound });
          applySlotToUi(activeSlot, null);
          const hasTarget = (mode === "ble") ? isBleAddr(cfgBleAddr) : !!cfgIp;
          if (enabled && hasTarget) {
            setHint("");
            await refreshHotAirLiveTemp();
          } else {
            setHint(mode === "ble" ? "H310/H312 BLE MAC." : "H310/H312 IP.");
          }
        }
        async function scanHotAir() {
          btnScan.disabled = true;
          btnBind.disabled = true;
          scanListEl.classList.add("hidden");
          scanListEl.innerHTML = "";
          const mode = currentMode();
          if (mode === "ble") {
            setHint("Scanning H310/H312 BLE...");
            append("[h312] ble scan started");
            try {
              const r = await apiJson("/api/h312/ble_scan", {
                method: "POST",
                body: JSON.stringify({ scan_ms: 5000 }),
                timeout_ms: 25000
              });
              if (!(r && r.ok)) {
                const err = (r && r.error) ? String(r.error) : "unknown";
                setHint(`BLE scan failed: ${err}`, "err");
                append(`[h312] ble scan failed: ${err}`);
                return;
              }
              const arr = Array.isArray(r.devices) ? r.devices : [];
              scanListEl.innerHTML = "";
              if (!arr.length) {
                setHint("No H310/H312 BLE devices found", "err");
                append("[h312] ble scan: no devices");
                return;
              }
              arr.forEach((dev) => {
                const addr = normBleAddr(dev && dev.addr);
                if (!isBleAddr(addr)) return;
                const name = String((dev && dev.name) ? dev.name : "H312");
                const rssi = Number((dev && dev.rssi) || 0);
                const row = document.createElement("div");
                row.className = "listItem";
                row.style.cursor = "pointer";
                const left = document.createElement("div");
                left.className = "liLeft";
                const nm = document.createElement("div");
                nm.className = "liName";
                nm.textContent = `${name} (${addr})`;
                const meta = document.createElement("div");
                meta.className = "liMeta";
                meta.textContent = `rssi=${rssi} dBm`;
                left.appendChild(nm);
                left.appendChild(meta);
                const pill = document.createElement("span");
                pill.className = "pill cyan";
                pill.textContent = "Select";
                row.appendChild(left);
                row.appendChild(pill);
                row.addEventListener("click", () => {
                  ipEl.value = addr;
                  ipEl.dataset.bleName = name;
                  setHint(`Selected: ${name} ${addr}`, "ok");
                });
                scanListEl.appendChild(row);
              });
              scanListEl.classList.remove("hidden");
              if (scanListEl.children.length === 1) {
                const first = scanListEl.querySelector(".listItem");
                if (first) first.click();
              } else {
                setHint(`Found ${scanListEl.children.length} BLE devices. Select and bind.`, "ok");
              }
              append(`[h312] ble scan found=${scanListEl.children.length}`);
            } finally {
              btnScan.disabled = false;
              btnBind.disabled = false;
            }
            return;
          }

          function hostOffsetByOrder(orderIdx, hostCount) {
            if (hostCount >= 100) {
              const firstStart = 100;
              const firstEnd = Math.min(hostCount, 199);
              const firstLen = (hostCount >= firstStart) ? (firstEnd - firstStart + 1) : 0;
              const secondLen = Math.min(hostCount, 99);
              if (orderIdx < firstLen) return firstStart + orderIdx;
              if (orderIdx < (firstLen + secondLen)) return 1 + (orderIdx - firstLen);
              return 200 + (orderIdx - firstLen - secondLen);
            }
            return orderIdx + 1;
          }
          function buildScanTargets(prefix, selfHost, hostCount) {
            const out = [];
            for (let orderIdx = 0; orderIdx < hostCount; orderIdx++) {
              const h = hostOffsetByOrder(orderIdx, hostCount);
              if (h === selfHost) continue;
              out.push(`${prefix}${h}`);
            }
            return out;
          }

          const scanBody = {
            port: 6666,
            timeout_ms: 900,
            read_ms: 1250,
            probe_timeout_ms: 1200,
            probe_read_ms: 500,
            startup_delay_ms: 20,
            keepalive_after_ms: 700,
            max_hosts: 254,
            retries: 2,
            workers: 10,
            stop_on_first: false,
            include_weak: false,
            debug: false,
            max_debug_hosts: 32
          };

          let prefix = "192.168.1.";
          let selfHost = -1;
          try {
            const st = await apiJson("/api/status", { method: "GET", timeout_ms: 10000 });
            if (st && st.ok && st.ip && /^\d+\.\d+\.\d+\.\d+$/.test(String(st.ip))) {
              const p = String(st.ip).split(".");
              if (p.length === 4) {
                prefix = `${p[0]}.${p[1]}.${p[2]}.`;
                selfHost = Number(p[3]) || -1;
              }
            }
          } catch (_) {}

          const fastHostCount = Math.max(1, Number(scanBody.max_hosts || 254));
          const fastTargets = buildScanTargets(prefix, selfHost, fastHostCount);
          const estimatedHostMs = Math.max(
            650,
            Math.round((Number(scanBody.probe_timeout_ms || 1200) + 120) * Math.max(1, Number(scanBody.retries || 1)))
          );

          const scanState = {
            phase: "fast",
            found: 0,
            scanned: 0,
            total: Math.max(1, fastTargets.length),
            currentIp: fastTargets.length ? fastTargets[0] : "-",
            startedAt: Date.now(),
            estimatedHostMs,
            fastTargets
          };

          function getFastEstimate() {
            const elapsedMs = Math.max(0, Date.now() - scanState.startedAt);
            const workerRate = Math.max(1, Number(scanBody.workers || 1));
            const estScanned = Math.max(
              Number(scanState.scanned || 0),
              Math.min(scanState.total, Math.floor((elapsedMs * workerRate) / Math.max(1, scanState.estimatedHostMs || 1000)))
            );
            const pct = Math.max(0, Math.min(100, Math.round((estScanned / Math.max(1, scanState.total)) * 100)));
            const ip = (estScanned < scanState.fastTargets.length) ? scanState.fastTargets[estScanned] : "done";
            const left = Math.max(0, scanState.total - estScanned);
            const etaSec = Math.ceil((left * Math.max(1, scanState.estimatedHostMs || 1000)) / (1000 * workerRate));
            return { scanned: estScanned, pct, ip, etaSec };
          }

          function renderScanAnimation() {
            scanListEl.classList.remove("hidden");
            const fastEst = getFastEstimate();
            const phaseText = (scanState.phase === "deep") ? "Deep analyzer" : "Fast scan (estimated)";
            const displayScanned = (scanState.phase === "deep") ? Number(scanState.scanned || 0) : fastEst.scanned;
            const displayTotal = Math.max(1, Number(scanState.total || 0));
            const displayPct = Math.max(0, Math.min(100, Math.round((displayScanned / displayTotal) * 100)));
            const shownCounter = Math.min(displayTotal, Math.max(1, displayScanned));
            scanListEl.innerHTML =
              `<div class="scanAnimCard">` +
              `<div class="scanAnimHead"><span class="scanAnimSpinner"></span><span>${phaseText}</span><span class="pill cyan">SCAN</span></div>` +
              `<div class="scanAnimBar"><div class="scanAnimFill" style="width:${displayPct}%"></div></div>` +
              `<div class="scanAnimMeta">Scan ${shownCounter}/${displayTotal}</div>` +
              `</div>`;
          }

          renderScanAnimation();
          setHint("Scanning H31x");
          append("[h312] network scan started");
          const waitTimer = setInterval(() => {
            renderScanAnimation();
            if (scanState.phase === "deep" && Number.isFinite(scanState.scanned)) {
              const shown = Math.min(Math.max(1, Number(scanState.total || 1)), Math.max(1, Number(scanState.scanned || 0)));
              setHint(`Scan ${shown}/${scanState.total}`);
            } else {
              const est = getFastEstimate();
              const shown = Math.min(Math.max(1, Number(scanState.total || 1)), Math.max(1, Number(est.scanned || 0)));
              setHint(`Scan ${shown}/${scanState.total}`);
            }
          }, 250);

          async function playScanTrace(debugArr, foundCount, hostTotal, skipCount, elapsedMs) {
            if (!Array.isArray(debugArr) || !debugArr.length) return;
            const total = Math.max(1, Number(hostTotal || debugArr.length));
            const basePace = (elapsedMs > 0 && total > 0) ? Math.round(elapsedMs / total) : 20;
            // Keep playback readable for humans even when real scan finishes very quickly.
            const paceMs = Math.max(20, Math.min(80, basePace));
            for (let i = 0; i < debugArr.length; i++) {
              const d = debugArr[i] || {};
              const ip = (d && d.ip) ? d.ip : "?";
              const reason = (d && d.reason) ? d.reason : "unknown";
              const rxBytes = (d && typeof d.rxBytes === "number") ? d.rxBytes : 0;
              append(`[h312] ${ip} ${reason} rx=${rxBytes}`);
              const scannedNow = Math.min(total, i + 1);
              setHint(`Found ${foundCount}, scanned ${scannedNow}/${total} (skip ${skipCount || 0}) | ${ip}`);
              await new Promise((resolve) => setTimeout(resolve, paceMs));
            }
          }

          try {
            let r = await apiJson("/api/h312/scan", {
              method: "POST",
              body: JSON.stringify(scanBody),
              timeout_ms: 480000
            });
            clearInterval(waitTimer);
            if (r && r.ok) {
              let arr = Array.isArray(r.devices) ? r.devices : [];
              scanState.found = arr.length;
              scanState.scanned = Number(r.scanned || 0);
              scanState.total = Number(r.host_total || 254);
              scanState.currentIp = "done";
              const debugArr = Array.isArray(r.debug) ? r.debug : [];
              renderScanResults(arr);
              await playScanTrace(debugArr, arr.length, r.host_total, r.skipped_self, Number(r.ms || 0));
              if (!arr.length) {
                append("[h312] fast scan empty -> stability pass (wk=5 r=2)");
                setHint("Stability pass H31x");
                const stableBody = {
                  ...scanBody,
                  workers: 5,
                  retries: Math.max(2, Number(scanBody.retries || 1)),
                  probe_timeout_ms: Math.max(1500, Number(scanBody.probe_timeout_ms || 1200)),
                  probe_read_ms: Math.max(800, Number(scanBody.probe_read_ms || 500))
                };
                const stableResp = await apiJson("/api/h312/scan", {
                  method: "POST",
                  body: JSON.stringify(stableBody),
                  timeout_ms: 480000
                });
                if (stableResp && stableResp.ok) {
                  r = stableResp;
                  arr = Array.isArray(r.devices) ? r.devices : [];
                  scanState.found = arr.length;
                  scanState.scanned = Number(r.scanned || 0);
                  scanState.total = Number(r.host_total || scanState.total || 254);
                  scanState.currentIp = "done";
                  renderScanResults(arr);
                  append(`[h312] stability f=${arr.length} p=${r.found_probe || 0} w=${r.found_weak || 0} x=${r.failed || 0} s=${r.scanned || 0}/${r.host_total || 0} skip=${r.skipped_self || 0} r=${r.retries || 1} wk=${r.workers || 1} par=${r.parallel ? 1 : 0} stop=${r.stopped ? 1 : 0} ord=${r.order_processed || 0}/${r.order_target || 0} oc=${r.order_complete ? 1 : 0} ms=${r.ms || 0}`);
                } else {
                  append(`[h312] stability failed: ${(stableResp && stableResp.error) ? stableResp.error : "unknown"}`);
                }
              }
              if (!arr.length) {
                append("[h312] fast+stability empty -> deep analyzer");
                scanState.phase = "deep";
                scanState.scanned = 0;
                scanState.currentIp = "-";
                setHint("Deep analyzing H31x");
                const deepRanges = [
                  [100, 130],
                  [2, 40],
                  [131, 199],
                  [200, 254],
                  [41, 99],
                  [1, 1]
                ];
                const deepArr = [];
                const reasonStats = Object.create(null);
                const d0 = Date.now();
                let deepScanned = 0;
                let deepTotal = 0;
                for (const rg of deepRanges) {
                  deepTotal += (rg[1] - rg[0] + 1);
                }
                if (selfHost >= 0) deepTotal = Math.max(1, deepTotal - 1);
                scanState.total = deepTotal;
                scanState.scanned = 0;
                for (const rg of deepRanges) {
                  append(`[h312] deep range ${rg[0]}-${rg[1]} started`);
                  for (let h = rg[0]; h <= rg[1]; h++) {
                    if (h === selfHost) continue;
                    const dip = `${prefix}${h}`;
                    scanState.currentIp = dip;
                    let pr = null;
                    for (let pa = 0; pa < 2; pa++) {
                      pr = await apiJson("/api/h312/probe", {
                        method: "POST",
                        body: JSON.stringify({
                          ip: dip,
                          port: 6666,
                          attempts: 2,
                          connect_timeout_ms: 1200,
                          read_ms: 800,
                          startup_delay_ms: 20,
                          keepalive_after_ms: 700
                        }),
                        timeout_ms: 16000
                      });
                      // Retry once for transport-level fetch errors.
                      if (!(pr && !pr.ok && String(pr.error || "").includes("Failed to fetch"))) break;
                      await new Promise((resolve) => setTimeout(resolve, 180));
                    }
                    deepScanned++;
                    scanState.scanned = deepScanned;
                    if (pr && pr.ok) {
                      deepArr.push({
                        ip: dip,
                        name: String(pr.name || "H31X"),
                        model: String(pr.model || ""),
                        sn: String(pr.sn || ""),
                        workStatus: (typeof pr.workStatus === "number") ? pr.workStatus : undefined,
                        realTimeTemp: (typeof pr.realTimeTemp === "number") ? pr.realTimeTemp : undefined,
                        heatFlag: (typeof pr.heatFlag === "number") ? pr.heatFlag : undefined
                      });
                      scanState.found = deepArr.length;
                    }
                    const reason = String((pr && pr.reason) ? pr.reason : ((pr && pr.error) ? pr.error : "unknown"));
                    reasonStats[reason] = (Number(reasonStats[reason] || 0) + 1);
                    if (deepScanned % 5 === 0) {
                      setHint(`Deep analyzing H31x ${deepScanned}/${deepTotal} | found ${deepArr.length}`);
                    }
                  }
                }
                if (deepArr.length) {
                  arr = deepArr;
                  renderScanResults(arr);
                }
                scanState.currentIp = "done";
                const topReasons = Object.keys(reasonStats)
                  .sort((a, b) => Number(reasonStats[b] || 0) - Number(reasonStats[a] || 0))
                  .slice(0, 3)
                  .map((k) => `${k}:${reasonStats[k]}`)
                  .join(", ");
                append(`[h312] deep f=${deepArr.length} s=${deepScanned}/${deepTotal} ranges=100-130,2-40,131-199,200-254,41-99,1 reasons=${topReasons || "-"} ms=${Date.now() - d0}`);
                if (!deepArr.length) {
                  const connectFailCount = Number(reasonStats["connect_fail"] || 0);
                  if (deepScanned > 0 && connectFailCount === deepScanned) {
                    append("[h312] analyzer: all probes connect_fail -> no reachable TCP:6666 on subnet");
                    setHint("No TCP:6666 host. Check H310/H312 power/LAN mode/IP", "err");
                  } else {
                    setHint(`Not found (${topReasons || "unknown"})`, "err");
                  }
                }
              }
              if (arr.length === 1 && arr[0] && arr[0].ip) {
                ipEl.value = String(arr[0].ip);
                const oneName = String(arr[0].name || "H31X");
                const oneModel = String(arr[0].model || "");
                const oneSn = String(arr[0].sn || "");
                const info = [oneName];
                if (oneModel) info.push(`model=${oneModel}`);
                if (oneSn) info.push(`sn=${oneSn}`);
                setHint(`Found: ${info.join(" ")} ${ipEl.value}`);
              } else if (arr.length > 1) {
                setHint(`Found ${arr.length} devices. Select IP and press Bind.`, "ok");
              } else {
                setHint(`Found ${arr.length}, scanned ${r.scanned || 0}/${r.host_total || 0} (skip ${r.skipped_self || 0})`);
              }
              append(`[h312] f=${arr.length} p=${r.found_probe || 0} w=${r.found_weak || 0} x=${r.failed || 0} s=${r.scanned || 0}/${r.host_total || 0} skip=${r.skipped_self || 0} r=${r.retries || 1} wk=${r.workers || 1} par=${r.parallel ? 1 : 0} stop=${r.stopped ? 1 : 0} ord=${r.order_processed || 0}/${r.order_target || 0} oc=${r.order_complete ? 1 : 0} ms=${r.ms || 0}`);
            } else {
              setHint(`Scan failed: ${(r && r.error) ? r.error : "unknown"}`, "err");
              append(`[h312] scan failed: ${(r && r.error) ? r.error : "unknown"}`);
            }
          } finally {
            clearInterval(waitTimer);
            btnScan.disabled = false;
            btnBind.disabled = false;
          }
        }
        async function bindHotAir() {
          const mode = currentMode();
          const ip = String(ipEl.value || "").trim();
          const bleAddr = normBleAddr(ip);
          const bleName = String((ipEl.dataset && ipEl.dataset.bleName) ? ipEl.dataset.bleName : loadStr(KEY_BLE_NAME, "")).trim();
          const tOn = parseTempOn(tempEl.value);
          const tOff = calcTempOff(tOn);
          if (mode === "ble") {
            if (!isBleAddr(bleAddr)) {
              setState("not_bound");
              setHint("Invalid H310/H312 BLE MAC", "err");
              return;
            }
          } else if (!isIpv4(ip)) {
            setState("not_bound");
            setHint("Invalid H310/H312 IP", "err");
            return;
          }
          btnBind.disabled = true;
          setState("binding");
          try {
            if (mode === "ble") {
              const body = {
                h312_enabled: true,
                h312_mode: "ble",
                h312_ip: "",
                h312_ble_addr: bleAddr,
                h312_ble_name: bleName,
                h312_port: 6666,
                h312_t_on: tOn,
                h312_t_off: tOff,
                h312_slot: activeSlot
              };
              const r = await apiJson("/api/config", { method: "POST", body: JSON.stringify(body) });
              if (r && r.ok) {
                saveStr(KEY_MODE, "ble");
                saveStr(KEY_BLE_ADDR, bleAddr);
                saveStr(KEY_BLE_NAME, bleName);
                saveSlotProfile(activeSlot, { ip: "", tOn, bound: true });
                setState("bound");
                setHint(`${slotTitle(activeSlot)} bound: ${bleName || "H312"} ${bleAddr}`, "ok");
                append(`[h312] ble bind saved slot=${activeSlot} addr=${bleAddr} name=${bleName || "-"}`);
                await refreshHotAirLiveTemp();
              } else {
                setState("not_bound");
                setHint(`Bind failed: ${(r && r.error) ? r.error : "unknown"}`, "err");
                append(`[h312] ble bind failed: ${(r && r.error) ? r.error : "unknown"}`);
              }
            } else {
              const probe = await apiJson("/api/h312/probe", {
                method: "POST",
                body: JSON.stringify({ ip, port: 6666 }),
                timeout_ms: 20000
              });
              const probeOk = !!(probe && probe.ok);
              if (!probeOk) {
                append(`[h312] probe weak for ${ip}: ${(probe && probe.error) ? probe.error : "unknown"}; saving manual bind`);
              }
              const body = {
                h312_enabled: true,
                h312_mode: "wifi",
                h312_ip: ip,
                h312_ble_addr: "",
                h312_ble_name: "",
                h312_port: 6666,
                h312_t_on: tOn,
                h312_t_off: tOff,
              };
              const r = await apiJson("/api/config", { method: "POST", body: JSON.stringify(body) });
              if (r && r.ok) {
                saveStr(KEY_MODE, "wifi");
                saveStr(KEY_BLE_ADDR, "");
                saveStr(KEY_BLE_NAME, "");
                saveSlotProfile(activeSlot, { ip, tOn, bound: true });
                setState("bound");
                const pname = String((probeOk && probe && probe.name) ? probe.name : "H31X");
                const pmodel = String((probeOk && probe && probe.model) ? probe.model : "").trim();
                const psn = String((probeOk && probe && probe.sn) ? probe.sn : "").trim();
                const info = [pname];
                if (pmodel) info.push(`model=${pmodel}`);
                if (psn) info.push(`sn=${psn}`);
                if (probeOk) {
                  setHint(`${slotTitle(activeSlot)} bound: ${info.join(" ")} ${ip}`, "ok");
                } else {
                  setHint(`${slotTitle(activeSlot)} bound (manual): ${ip}`, "ok");
                }
                append(`[h312] bind saved slot=${activeSlot} name=${pname} model=${pmodel || "-"} sn=${psn || "-"} ip=${ip} t_on=${tOn} t_off=${tOff}`);
                await refreshHotAirLiveTemp();
              } else {
                setState("not_bound");
                setHint(`Bind failed: ${(r && r.error) ? r.error : "unknown"}`, "err");
                append(`[h312] bind failed: ${(r && r.error) ? r.error : "unknown"}`);
              }
            }
          } finally {
            btnBind.disabled = false;
          }
        }
        async function switchHotAirMode(nextMode) {
          const mode = normHotAirMode(nextMode);
          if (mode === currentMode()) {
            setModeButtons(mode);
            return;
          }
          saveCurrentUiToSlot(activeSlot);
          saveStr(KEY_MODE, mode);
          if (mode === "ble") {
            ipEl.value = normBleAddr(loadStr(KEY_BLE_ADDR, ""));
            setHint("BLE mode selected. Scan and bind.", "ok");
          } else {
            ipEl.value = String(loadSlotProfile(activeSlot).ip || "");
            setHint("Wi-Fi mode selected. Scan and bind.", "ok");
          }
          setState("not_bound");
          setModeButtons(mode);
          try {
            await apiJson("/api/config", {
              method: "POST",
              body: JSON.stringify({
                h312_enabled: false,
                h312_mode: mode,
                h312_slot: activeSlot
              })
            });
            append(`[h312] mode switched: ${mode}`);
          } catch (_) {}
        }
        if (modeWifiBtn) modeWifiBtn.addEventListener("click", () => { switchHotAirMode("wifi").catch(() => {}); });
        if (modeBleBtn) modeBleBtn.addEventListener("click", () => { switchHotAirMode("ble").catch(() => {}); });
        btnBind.addEventListener("click", bindHotAir);
        btnScan.addEventListener("click", scanHotAir);
        async function activateSlot(slot) {
          const next = parseSlot(slot);
          if (next > slotCount) return;
          if (next === activeSlot) return;
          saveCurrentUiToSlot(activeSlot);
          applySlotToUi(next, `${slotTitle(next)} selected`);
          saveStr(KEY_SLOT_ACTIVE, String(next));
          const p = loadSlotProfile(next);
          const mode = currentMode();
          const hasTarget = (mode === "ble") ? isBleAddr(loadStr(KEY_BLE_ADDR, "")) : isIpv4(p.ip);
          if (p.bound && hasTarget) {
            const saved = await saveHotAirConfig("slot_switch");
            if (!saved.ok && !saved.skipped) setHint(`Slot switch failed: ${saved.error || "unknown"}`, "err");
            await refreshFromDevice();
          }
        }
        if (slot1Btn) slot1Btn.addEventListener("click", () => { activateSlot(1).catch(() => {}); });
        if (slot2Btn) slot2Btn.addEventListener("click", () => { activateSlot(2).catch(() => {}); });
        if (slot3Btn) slot3Btn.addEventListener("click", () => { activateSlot(3).catch(() => {}); });
        if (btnAdd) {
          btnAdd.addEventListener("click", () => {
            if (slotCount >= SLOT_COUNT) return;
            saveCurrentUiToSlot(activeSlot);
            setSlotCount(slotCount + 1);
            renderSlotControls();
            const next = slotCount;
            activateSlot(next).catch(() => {});
            setHint(`${slotTitle(next)} added. Scan and bind.`, "ok");
          });
        }
        if (btnRemove) {
          btnRemove.addEventListener("click", async () => {
            if (slotCount <= 1) return;
            saveCurrentUiToSlot(activeSlot);
            const removedSlot = slotCount;
            clearSlotProfile(removedSlot);
            setSlotCount(slotCount - 1);
            const next = Math.min(activeSlot, slotCount);
            saveStr(KEY_SLOT_ACTIVE, String(next));
            applySlotToUi(next, `${slotTitle(removedSlot)} removed`);
            append(`[h312] slot removed: ${slotTitle(removedSlot)}`);
            const p = loadSlotProfile(next);
            if (p.bound && isIpv4(p.ip)) {
              const saved = await saveHotAirConfig("slot_remove");
              if (!saved.ok && !saved.skipped) setHint(`Slot remove sync failed: ${saved.error || "unknown"}`, "err");
              await refreshFromDevice();
            }
          });
        }
        if (btnResetHotAir) {
          btnResetHotAir.addEventListener("click", () => {
            resetHotAirConfig().catch(() => {});
          });
        }
        if (btnApplyLogic) {
          btnApplyLogic.addEventListener("click", async () => {
            const saved = await saveHotAirConfig("apply_logic");
            if (!saved || saved.skipped) return;
            if (!saved.ok) setHint(`Hot Air apply failed: ${saved.error || "unknown"}`, "err");
          });
        }
        if (sel) {
          sel.addEventListener("change", () => {
            const stateKind = String(stateEl.dataset.state || "not_bound");
            setState(stateKind);
            setTimeout(() => updateHotAirTempLabel(), 0);
          });
        }
        ipEl.addEventListener("keydown", (ev) => {
          if (ev.key === "Enter") {
            ev.preventDefault();
            bindHotAir();
          }
        });
        tempEl.addEventListener("keydown", (ev) => {
          if (ev.key === "Enter") {
            ev.preventDefault();
            bindHotAir();
          }
        });
        tempEl.addEventListener("input", () => {
          const tOn = parseTempOn(tempEl.value);
          updateHotAirTempLabel(tOn);
          saveSlotProfile(activeSlot, {
            ip: String(ipEl.value || "").trim(),
            tOn,
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
        });
        tempEl.addEventListener("change", () => {
          const n = parseTempOn(tempEl.value);
          tempEl.value = String(n);
          updateHotAirTempLabel(n);
          saveSlotProfile(activeSlot, {
            ip: String(ipEl.value || "").trim(),
            tOn: n,
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
        });
        tempEl.addEventListener("blur", () => {
          const n = parseTempOn(tempEl.value);
          tempEl.value = String(n);
          updateHotAirTempLabel(n);
          saveSlotProfile(activeSlot, {
            ip: String(ipEl.value || "").trim(),
            tOn: n,
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
        });
        ipEl.addEventListener("input", () => {
          saveSlotProfile(activeSlot, {
            ip: String(ipEl.value || "").trim(),
            tOn: parseTempOn(tempEl.value),
            bound: (String(stateEl.dataset.state || "not_bound") === "bound")
          });
        });
        activeSlot = parseSlot(loadStr(KEY_SLOT_ACTIVE, "1"));
        const legacyIp = loadStr(KEY_IP, "");
        const legacyOn = parseTempOn(loadStr(KEY_TEMP_ON, String(tempEl.value || "140")));
        const slot1 = loadSlotProfile(1);
        if (!slot1.ip && legacyIp) saveSlotProfile(1, { ip: legacyIp, tOn: legacyOn, bound: true });
        const storedSlotCount = parseSlotCount(loadStr(KEY_SLOT_COUNT, "1"));
        const detectedSlotCount = detectSlotCount();
        setSlotCount(Math.max(storedSlotCount, detectedSlotCount, activeSlot));
        activeSlot = Math.min(activeSlot, slotCount);
        saveStr(KEY_SLOT_ACTIVE, String(activeSlot));
        renderSlotButtons(activeSlot);
        applySlotToUi(activeSlot, null);
        ensureHotAirLiveLoop();
        refreshFromDevice();
      }
      function initT420dGpioConfig() {
        const selH1 = document.getElementById("gpioH1");
        const selH2 = document.getElementById("gpioH2");
        const btnApplyLogic = document.getElementById("btnApplyLogic");
        const logEl = document.getElementById("eventLog");
        if (!selH1 || !selH2) return;
        const KEY_H1 = "alb_t420d_h1_gpio";
        const KEY_H2 = "alb_t420d_h2_gpio";
        const FIXED_H1 = 1;
        const FIXED_H2 = 3;
        const allowed = [FIXED_H1, FIXED_H2];
        function append(line) {
          if (!logEl) return;
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          logEl.textContent = `${cur}\n[ui] ${line}`;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function parseGpio(v) {
          const n = parseInt(String(v || "").trim(), 10);
          if (!Number.isFinite(n)) return FIXED_H1;
          if (!allowed.includes(n)) return FIXED_H1;
          return n;
        }
        function gpioText(v) {
          return `GPIO${v}`;
        }
        function loadInt(key, def) {
          try {
            const raw = localStorage.getItem(key);
            if (raw == null || raw === "") return def;
            return parseGpio(raw);
          } catch (_) {
            return def;
          }
        }
        function saveInt(key, v) {
          try { localStorage.setItem(key, String(parseGpio(v))); } catch (_) {}
        }
        function fillSelect(sel, fixed) {
          sel.innerHTML = "";
          const opt = document.createElement("option");
          opt.value = String(fixed);
          opt.textContent = gpioText(fixed);
          opt.selected = true;
          sel.appendChild(opt);
        }
        function setSelectValue(sel, v) {
          const n = (sel === selH1) ? FIXED_H1 : FIXED_H2;
          sel.value = String(n);
        }
        async function applyToDevice(reason) {
          const h1 = FIXED_H1;
          const h2 = FIXED_H2;
          setSelectValue(selH1, h1);
          setSelectValue(selH2, h2);
          saveInt(KEY_H1, h1);
          saveInt(KEY_H2, h2);
          const r = await apiJson("/api/config", {
            method: "POST",
            body: JSON.stringify({
              t420d_h1_gpio: h1,
              t420d_h2_gpio: h2
            })
          });
          if (r && r.ok) {
            append(`[gpio] ${reason}: saved h1=${gpioText(h1)} h2=${gpioText(h2)}`);
            return true;
          }
          append(`[gpio] ${reason}: save failed: ${(r && r.error) ? r.error : "unknown"}`);
          return false;
        }
        fillSelect(selH1, FIXED_H1);
        fillSelect(selH2, FIXED_H2);
        setSelectValue(selH1, loadInt(KEY_H1, FIXED_H1));
        setSelectValue(selH2, loadInt(KEY_H2, FIXED_H2));
        selH1.disabled = true;
        selH2.disabled = true;
        saveInt(KEY_H1, FIXED_H1);
        saveInt(KEY_H2, FIXED_H2);
        if (btnApplyLogic) {
          btnApplyLogic.addEventListener("click", () => {
            applyToDevice("apply_logic").catch(() => {});
          });
        }
        apiJson("/api/config", { method: "GET" }).then((r) => {
          if (!r || !r.ok) return;
          setSelectValue(selH1, FIXED_H1);
          setSelectValue(selH2, FIXED_H2);
          saveInt(KEY_H1, FIXED_H1);
          saveInt(KEY_H2, FIXED_H2);
        }).catch(() => {});
      }
      function initSpeedRestore() {
        const wrap = document.getElementById("manualSpeedWrap");
        const btnMode = document.getElementById("btnSpeedRestoreMode");
        const h1Wrap = document.getElementById("msH1Wrap");
        const h2Wrap = document.getElementById("msH2Wrap");
        const haWrap = document.getElementById("msHAWrap");
        const h1Lbl = document.getElementById("msH1SpeedLbl");
        const h2Lbl = document.getElementById("msH2SpeedLbl");
        const haLbl = document.getElementById("msHASpeedLbl");
        const h1El = document.getElementById("manualSpeedH1");
        const h2El = document.getElementById("manualSpeedH2");
        const haEl = document.getElementById("manualSpeedHA");
        if (!wrap || !h1Wrap || !h2Wrap || !haWrap || !h1Lbl || !h2Lbl || !haLbl || !h1El || !h2El || !haEl || !btnMode) return;

        const KEY_H1 = "alb_manual_speed_h1";
        const KEY_H2 = "alb_manual_speed_h2";
        const KEY_HA = "alb_manual_speed_ha";
        const KEY_MODE = "alb_speed_restore_mode";

        function loadStr(key, def) {
          try { return localStorage.getItem(key) || def; } catch (_) { return def; }
        }
        function saveStr(key, v) {
          try { localStorage.setItem(key, String(v)); } catch (_) {}
        }
        function normMode(raw) {
          const m = String(raw || "").trim().toLowerCase();
          return (m === "manual") ? "manual" : "last_set";
        }
        function getMode() {
          return normMode(loadStr(KEY_MODE, "last_set"));
        }
        function setMode(mode) {
          const next = normMode(mode);
          saveStr(KEY_MODE, next);
          const lastSet = next === "last_set";
          setTogglePressed(btnMode, lastSet, { onClass: "active", offClass: "off" });
          btnMode.setAttribute("data-i18n", lastSet ? "value_last_set_speed" : "value_manual_speed_mode");
          btnMode.textContent = tr(lastSet ? "value_last_set_speed" : "value_manual_speed_mode",
                                   lastSet ? "Last Set Speed" : "Manual Speed");
          refreshManualSpeedVisibility();
        }
        function normSpeed(raw, def) {
          const n = parseInt(String(raw == null ? "" : raw).trim(), 10);
          if (!Number.isFinite(n)) return def;
          return Math.max(0, Math.min(100, n));
        }
        function sourceVisible(id) {
          const el = document.getElementById(id);
          return !!(el && !el.classList.contains("hidden"));
        }
        function refreshManualSpeedVisibility() {
          const h1On = sourceVisible("logicIron1Wrap");
          const h2On = sourceVisible("logicIron2Wrap");
          const haOn = sourceVisible("logicHotAirWrap");
          h1Wrap.classList.toggle("hidden", !h1On);
          h2Wrap.classList.toggle("hidden", !h2On);
          haWrap.classList.toggle("hidden", !haOn);
          const anyVisible = (h1On || h2On || haOn);
          const manualMode = getMode() === "manual";
          wrap.classList.toggle("hidden", !(anyVisible && manualMode));
        }
        function updateManualSpeedLabels() {
          const base = tr("label_manual_speed", "Manual Speed");
          h1Lbl.textContent = `${base} (${normSpeed(h1El.value, 35)}%)`;
          h2Lbl.textContent = `${base} (${normSpeed(h2El.value, 35)}%)`;
          haLbl.textContent = `${base} (${normSpeed(haEl.value, 35)}%)`;
        }
        function bindManualInput(el, key, def) {
          const init = normSpeed(loadStr(key, String(def)), def);
          el.value = String(init);
          const saveNow = () => {
            const n = normSpeed(el.value, def);
            el.value = String(n);
            saveStr(key, String(n));
            updateManualSpeedLabels();
          };
          el.addEventListener("input", saveNow);
          el.addEventListener("change", saveNow);
          el.addEventListener("blur", saveNow);
        }
        function getManualSpeedBySource(sourceKey) {
          if (String(sourceKey || "") === "h1") return normSpeed(loadStr(KEY_H1, h1El.value), 35);
          if (String(sourceKey || "") === "h2") return normSpeed(loadStr(KEY_H2, h2El.value), 35);
          if (String(sourceKey || "") === "ha") return normSpeed(loadStr(KEY_HA, haEl.value), 35);
          return normSpeed(loadStr(KEY_H1, h1El.value), 35);
        }

        bindManualInput(h1El, KEY_H1, 35);
        bindManualInput(h2El, KEY_H2, 35);
        bindManualInput(haEl, KEY_HA, 35);
        btnMode.addEventListener("click", () => {
          const next = (getMode() === "last_set") ? "manual" : "last_set";
          setMode(next);
        });

        window.__refreshManualSpeedVisibility = refreshManualSpeedVisibility;
        window.__getSpeedRestoreMode = () => getMode();
        window.__getManualSpeedBySource = getManualSpeedBySource;
        window.__refreshSpeedRestoreModeLabel = () => setMode(getMode());
        if (sel) sel.addEventListener("change", () => setTimeout(updateManualSpeedLabels, 0));
        updateManualSpeedLabels();
        setMode(getMode());
      }
      function initDeviceClock() {
        const pillEl = document.getElementById("deviceClockPill");
        const hintEl = document.getElementById("deviceClockHint");
        if (!pillEl || !hintEl) return;

        let baseUtcMs = 0;
        let baseClientMs = 0;
        let tzOffsetMin = 0;
        let valid = false;

        function pad2(n) { return String(n).padStart(2, "0"); }
        function formatLocal(utcMs, offsetMin) {
          const localMs = Number(utcMs || 0) + Number(offsetMin || 0) * 60000;
          const d = new Date(localMs);
          return `${pad2(d.getUTCHours())}:${pad2(d.getUTCMinutes())}:${pad2(d.getUTCSeconds())}`;
        }
        function setHint(text, cls) {
          hintEl.textContent = String(text || "");
          hintEl.classList.remove("ok", "err");
          if (cls) hintEl.classList.add(cls);
        }
        function renderTick() {
          if (!valid || !baseUtcMs) {
            pillEl.textContent = "--:--:--";
            return;
          }
          const utcNow = baseUtcMs + (Date.now() - baseClientMs);
          pillEl.textContent = formatLocal(utcNow, tzOffsetMin);
        }
        function applyStatus(st) {
          if (!st || !st.ok) return false;
          const utc = Number(st.utc_epoch_ms || 0);
          const off = Number(st.tz_offset_min || 0);
          valid = !!st.valid && Number.isFinite(utc) && utc > 0;
          if (valid) {
            baseUtcMs = utc;
            baseClientMs = Date.now();
            tzOffsetMin = Number.isFinite(off) ? off : 0;
            const src = String(st.last_sync_source || "-");
            setHint(`TZ ${tzOffsetMin >= 0 ? "+" : ""}${tzOffsetMin} min | sync: ${src}`, "ok");
          } else {
            setHint("Clock not set yet", "err");
          }
          return true;
        }
        async function fetchInternetUtcMs() {
          try {
            const ctrl = (typeof AbortController !== "undefined") ? new AbortController() : null;
            const t = ctrl ? setTimeout(() => { try { ctrl.abort(); } catch (_) {} }, 2800) : 0;
            const r = await fetch("https://worldtimeapi.org/api/ip", {
              method: "GET",
              signal: ctrl ? ctrl.signal : undefined,
              cache: "no-store"
            });
            if (t) clearTimeout(t);
            const j = await r.json();
            const unixtime = Number(j && j.unixtime);
            if (!Number.isFinite(unixtime) || unixtime <= 0) return null;
            return Math.round(unixtime * 1000);
          } catch (_) {
            return null;
          }
        }
        async function setFromBrowserTime(tzOffsetOverride, tzNameOverride) {
          const off = Number.isFinite(tzOffsetOverride) ? Number(tzOffsetOverride) : -new Date().getTimezoneOffset();
          const tzName = String(tzNameOverride || (Intl.DateTimeFormat().resolvedOptions().timeZone || "UTC"));
          const payload = {
            epoch_ms: Date.now(),
            tz_offset_min: off,
            tz_name: tzName
          };
          return apiJson("/api/time/set", { method: "POST", body: JSON.stringify(payload), timeout_ms: 7000 });
        }
        async function bootstrapClock() {
          let st = await apiJson("/api/time/status", { method: "GET", timeout_ms: 7000 });
          applyStatus(st);

          const localOffset = -new Date().getTimezoneOffset();
          const localTzName = String(Intl.DateTimeFormat().resolvedOptions().timeZone || "UTC");
          if (!st || !st.ok || !st.tz_configured || !st.valid) {
            const internetUtc = await fetchInternetUtcMs();
            const localUtc = Date.now();
            if (internetUtc != null) {
              const driftSec = Math.round((localUtc - internetUtc) / 1000);
              setHint(`First sync: local/internet drift ${driftSec}s`, "ok");
            } else {
              setHint("First sync: internet time unavailable, using local clock", "err");
            }
            await setFromBrowserTime(localOffset, localTzName);
            st = await apiJson("/api/time/status", { method: "GET", timeout_ms: 7000 });
            applyStatus(st);
            return;
          }

          const syncResp = await apiJson("/api/time/sync", { method: "POST", body: "{}", timeout_ms: 8000 });
          if (!syncResp || !syncResp.ok) {
            const devOffset = Number(st.tz_offset_min);
            const useOff = Number.isFinite(devOffset) ? devOffset : localOffset;
            const devTzName = String(st.tz_name || localTzName);
            await setFromBrowserTime(useOff, devTzName);
          }
          st = await apiJson("/api/time/status", { method: "GET", timeout_ms: 7000 });
          applyStatus(st);
        }

        renderTick();
        bootstrapClock().catch(() => {
          setHint("Clock sync failed", "err");
        });
        setInterval(renderTick, 1000);
        setInterval(async () => {
          const st = await apiJson("/api/time/status", { method: "GET", timeout_ms: 7000 });
          applyStatus(st);
        }, 30000);
      }
      function initLightScheduleConfig() {
        const lightH1 = document.getElementById("lightH1");
        const lightH2 = document.getElementById("lightH2");
        const lightHA = document.getElementById("lightHA");
        const btnLightBrightnessModeH1 = document.getElementById("btnLightBrightnessModeH1");
        const btnLightBrightnessModeH2 = document.getElementById("btnLightBrightnessModeH2");
        const btnLightBrightnessModeHA = document.getElementById("btnLightBrightnessModeHA");
        const lightBrightnessH1Wrap = document.getElementById("lightBrightnessH1Wrap");
        const lightBrightnessH2Wrap = document.getElementById("lightBrightnessH2Wrap");
        const lightBrightnessHAWrap = document.getElementById("lightBrightnessHAWrap");
        const lightBrightnessH1Lbl = document.getElementById("lightBrightnessH1Lbl");
        const lightBrightnessH2Lbl = document.getElementById("lightBrightnessH2Lbl");
        const lightBrightnessHALbl = document.getElementById("lightBrightnessHALbl");
        const lightBrightnessH1 = document.getElementById("lightBrightnessH1");
        const lightBrightnessH2 = document.getElementById("lightBrightnessH2");
        const lightBrightnessHA = document.getElementById("lightBrightnessHA");
        const lbH1Wrap = document.getElementById("lbH1Wrap");
        const lbH2Wrap = document.getElementById("lbH2Wrap");
        const lbHAWrap = document.getElementById("lbHAWrap");
        const autoOn = document.getElementById("autoStopOn");
        const autoOff = document.getElementById("autoStopOff");
        const autoTime = document.getElementById("autoStopTime");
        const btnApplyLogic = document.getElementById("btnApplyLogic");
        const logEl = document.getElementById("eventLog");
        if (!lightH1 || !lightH2 || !lightHA || !btnLightBrightnessModeH1 || !btnLightBrightnessModeH2 || !btnLightBrightnessModeHA ||
            !lightBrightnessH1Wrap || !lightBrightnessH2Wrap || !lightBrightnessHAWrap || !lightBrightnessH1 || !lightBrightnessH2 ||
            !lightBrightnessHA || !lightBrightnessH1Lbl || !lightBrightnessH2Lbl || !lightBrightnessHALbl || !autoOn || !autoOff || !autoTime ||
            !btnApplyLogic) return;

        const K_LH1 = "alb_light_h1";
        const K_LH2 = "alb_light_h2";
        const K_LHA_PREFIX = "alb_light_ha_";
        const K_LBM_LEGACY = "alb_light_bri_mode";
        const K_LBMH1 = "alb_light_bri_mode_h1";
        const K_LBMH2 = "alb_light_bri_mode_h2";
        const K_LBMHA_PREFIX = "alb_light_bri_mode_ha_";
        const K_LBVH1 = "alb_light_bri_h1";
        const K_LBVH2 = "alb_light_bri_h2";
        const K_LBVHA_PREFIX = "alb_light_bri_ha_";
        const K_AEN = "alb_auto_stop_en";
        const K_ATM = "alb_auto_stop_time";
        const slotBtns = [
          document.getElementById("hotAirSlot1"),
          document.getElementById("hotAirSlot2"),
          document.getElementById("hotAirSlot3")
        ];

        function append(line) {
          if (!logEl) return;
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          logEl.textContent = `${cur}\n[ui] ${line}`;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function loadStr(key, def) {
          try { return localStorage.getItem(key) || def; } catch (_) { return def; }
        }
        function saveStr(key, v) {
          try { localStorage.setItem(key, String(v)); } catch (_) {}
        }
        function activeHotAirSlot() {
          try {
            const n = parseInt(String(localStorage.getItem("alb_h312_slot_active") || "1"), 10);
            if (!Number.isFinite(n)) return 1;
            return Math.max(1, Math.min(3, n));
          } catch (_) {
            return 1;
          }
        }
        function keyHa(slot) {
          const n = Math.max(1, Math.min(3, parseInt(String(slot || "1"), 10) || 1));
          return `${K_LHA_PREFIX}${n}`;
        }
        function keyBriHa(slot) {
          const n = Math.max(1, Math.min(3, parseInt(String(slot || "1"), 10) || 1));
          return `${K_LBVHA_PREFIX}${n}`;
        }
        function keyBriModeHa(slot) {
          const n = Math.max(1, Math.min(3, parseInt(String(slot || "1"), 10) || 1));
          return `${K_LBMHA_PREFIX}${n}`;
        }
        function normLight(v, def = 0) {
          const n = parseInt(String(v == null ? "" : v), 10);
          if (!Number.isFinite(n)) return def;
          return Math.max(0, Math.min(3, n));
        }
        function normBriMode(v) {
          const s = String(v == null ? "" : v).toLowerCase();
          if (s === "manual" || s === "1") return "manual";
          return "last";
        }
        function normBrightness(v, def = 60) {
          const n = parseInt(String(v == null ? "" : v), 10);
          if (!Number.isFinite(n)) return def;
          return Math.max(0, Math.min(100, n));
        }
        function sourceVisible(id) {
          const el = document.getElementById(id);
          return !!(el && !el.classList.contains("hidden"));
        }
        function updateBriLabels() {
          lightBrightnessH1Lbl.textContent = `${normBrightness(lightBrightnessH1.value, 60)}%`;
          lightBrightnessH2Lbl.textContent = `${normBrightness(lightBrightnessH2.value, 60)}%`;
          lightBrightnessHALbl.textContent = `${normBrightness(lightBrightnessHA.value, 60)}%`;
        }
        function readModeH1() {
          return normBriMode(loadStr(K_LBMH1, loadStr(K_LBM_LEGACY, "last")));
        }
        function readModeH2() {
          return normBriMode(loadStr(K_LBMH2, loadStr(K_LBM_LEGACY, "last")));
        }
        function readModeHa(slot) {
          return normBriMode(loadStr(keyBriModeHa(slot), loadStr(K_LBM_LEGACY, "last")));
        }
        function writeModeH1(mode) {
          saveStr(K_LBMH1, normBriMode(mode));
        }
        function writeModeH2(mode) {
          saveStr(K_LBMH2, normBriMode(mode));
        }
        function writeModeHa(slot, mode) {
          saveStr(keyBriModeHa(slot), normBriMode(mode));
        }
        function applyModeButton(btn, mode) {
          const isLast = normBriMode(mode) === "last";
          setTogglePressed(btn, isLast, { onClass: "active", offClass: "off" });
          btn.setAttribute("data-i18n", isLast ? "value_light_brightness_last" : "value_light_brightness_manual");
          btn.textContent = tr(
            isLast ? "value_light_brightness_last" : "value_light_brightness_manual",
            isLast ? "Last Set Brightness" : "Manual Brightness"
          );
        }
        function refreshLightBrightnessVisibility() {
          const h1On = sourceVisible("logicIron1Wrap");
          const h2On = sourceVisible("logicIron2Wrap");
          const haOn = sourceVisible("logicHotAirWrap");
          const modeH1 = readModeH1();
          const modeH2 = readModeH2();
          const modeHA = readModeHa(activeHotAirSlot());
          if (lbH1Wrap) lbH1Wrap.classList.toggle("hidden", !h1On);
          if (lbH2Wrap) lbH2Wrap.classList.toggle("hidden", !h2On);
          if (lbHAWrap) lbHAWrap.classList.toggle("hidden", !haOn);
          applyModeButton(btnLightBrightnessModeH1, modeH1);
          applyModeButton(btnLightBrightnessModeH2, modeH2);
          applyModeButton(btnLightBrightnessModeHA, modeHA);
          lightBrightnessH1Wrap.classList.toggle("hidden", !h1On || modeH1 !== "manual");
          lightBrightnessH2Wrap.classList.toggle("hidden", !h2On || modeH2 !== "manual");
          lightBrightnessHAWrap.classList.toggle("hidden", !haOn || modeHA !== "manual");
        }
        function parseHm(s) {
          const m = String(s || "").trim().match(/^(\d{1,2}):(\d{1,2})$/);
          if (!m) return 0;
          const hh = Math.max(0, Math.min(23, parseInt(m[1], 10) || 0));
          const mm = Math.max(0, Math.min(59, parseInt(m[2], 10) || 0));
          return hh * 60 + mm;
        }
        function fmtHm(minute) {
          const m = Math.max(0, Math.min(1439, parseInt(String(minute || 0), 10) || 0));
          const hh = String(Math.floor(m / 60)).padStart(2, "0");
          const mm = String(m % 60).padStart(2, "0");
          return `${hh}:${mm}`;
        }
        function setAutoEnabled(on) {
          setTogglePressed(autoOn, !!on);
          setTogglePressed(autoOff, !on);
          autoTime.disabled = !on;
        }
        function getAutoEnabled() {
          return isTogglePressed(autoOn);
        }
        function localApply() {
          lightH1.value = String(normLight(loadStr(K_LH1, lightH1.value || "0"), 0));
          lightH2.value = String(normLight(loadStr(K_LH2, lightH2.value || "0"), 0));
          lightHA.value = String(normLight(loadStr(keyHa(activeHotAirSlot()), lightHA.value || "0"), 0));
          lightBrightnessH1.value = String(normBrightness(loadStr(K_LBVH1, lightBrightnessH1.value || "60"), 60));
          lightBrightnessH2.value = String(normBrightness(loadStr(K_LBVH2, lightBrightnessH2.value || "60"), 60));
          lightBrightnessHA.value = String(normBrightness(loadStr(keyBriHa(activeHotAirSlot()), lightBrightnessHA.value || "60"), 60));
          updateBriLabels();
          setAutoEnabled(loadStr(K_AEN, "0") === "1");
          autoTime.value = fmtHm(loadStr(K_ATM, autoTime.value || "00:00"));
          refreshLightBrightnessVisibility();
        }
        function localSave() {
          saveStr(K_LH1, normLight(lightH1.value, 0));
          saveStr(K_LH2, normLight(lightH2.value, 0));
          saveStr(keyHa(activeHotAirSlot()), normLight(lightHA.value, 0));
          saveStr(K_LBVH1, normBrightness(lightBrightnessH1.value, 60));
          saveStr(K_LBVH2, normBrightness(lightBrightnessH2.value, 60));
          saveStr(keyBriHa(activeHotAirSlot()), normBrightness(lightBrightnessHA.value, 60));
          updateBriLabels();
          saveStr(K_AEN, getAutoEnabled() ? "1" : "0");
          saveStr(K_ATM, fmtHm(autoTime.value));
          refreshLightBrightnessVisibility();
        }
        async function pushToDevice(reason) {
          localSave();
          const ha1 = normLight(loadStr(keyHa(1), "0"), 0);
          const ha2 = normLight(loadStr(keyHa(2), "0"), 0);
          const ha3 = normLight(loadStr(keyHa(3), "0"), 0);
          const modeH1 = readModeH1() === "manual" ? 1 : 0;
          const modeH2 = readModeH2() === "manual" ? 1 : 0;
          const modeHa1 = readModeHa(1) === "manual" ? 1 : 0;
          const modeHa2 = readModeHa(2) === "manual" ? 1 : 0;
          const modeHa3 = readModeHa(3) === "manual" ? 1 : 0;
          const briHa1 = normBrightness(loadStr(keyBriHa(1), "60"), 60);
          const briHa2 = normBrightness(loadStr(keyBriHa(2), "60"), 60);
          const briHa3 = normBrightness(loadStr(keyBriHa(3), "60"), 60);
          const body = {
            light_h1: normLight(lightH1.value, 0),
            light_h2: normLight(lightH2.value, 0),
            light_ha1: ha1,
            light_ha2: ha2,
            light_ha3: ha3,
            light_bri_mode_h1: modeH1,
            light_bri_mode_h2: modeH2,
            light_bri_mode_ha1: modeHa1,
            light_bri_mode_ha2: modeHa2,
            light_bri_mode_ha3: modeHa3,
            light_bri_manual_h1: normBrightness(lightBrightnessH1.value, 60),
            light_bri_manual_h2: normBrightness(lightBrightnessH2.value, 60),
            light_bri_manual_ha1: briHa1,
            light_bri_manual_ha2: briHa2,
            light_bri_manual_ha3: briHa3,
            auto_stop_enabled: getAutoEnabled(),
            auto_stop_minute: parseHm(autoTime.value)
          };
          const r = await apiJson("/api/config", { method: "POST", body: JSON.stringify(body) });
          if (r && r.ok) append(`[logic] ${reason}: light/schedule saved`);
          else append(`[logic] ${reason}: light/schedule save failed: ${(r && r.error) ? r.error : "unknown"}`);
        }
        async function pullFromDevice() {
          const r = await apiJson("/api/config", { method: "GET", timeout_ms: 7000 });
          if (!r || !r.ok) return;
          lightH1.value = String(normLight(r.light_h1, normLight(lightH1.value, 0)));
          lightH2.value = String(normLight(r.light_h2, normLight(lightH2.value, 0)));
          saveStr(keyHa(1), normLight(r.light_ha1, normLight(loadStr(keyHa(1), "0"), 0)));
          saveStr(keyHa(2), normLight(r.light_ha2, normLight(loadStr(keyHa(2), "0"), 0)));
          saveStr(keyHa(3), normLight(r.light_ha3, normLight(loadStr(keyHa(3), "0"), 0)));
          writeModeH1(parseInt(String(r.light_bri_mode_h1 ?? 0), 10) === 1 ? "manual" : "last");
          writeModeH2(parseInt(String(r.light_bri_mode_h2 ?? 0), 10) === 1 ? "manual" : "last");
          writeModeHa(1, parseInt(String(r.light_bri_mode_ha1 ?? 0), 10) === 1 ? "manual" : "last");
          writeModeHa(2, parseInt(String(r.light_bri_mode_ha2 ?? 0), 10) === 1 ? "manual" : "last");
          writeModeHa(3, parseInt(String(r.light_bri_mode_ha3 ?? 0), 10) === 1 ? "manual" : "last");
          saveStr(K_LBVH1, normBrightness(r.light_bri_manual_h1, normBrightness(loadStr(K_LBVH1, "60"), 60)));
          saveStr(K_LBVH2, normBrightness(r.light_bri_manual_h2, normBrightness(loadStr(K_LBVH2, "60"), 60)));
          saveStr(keyBriHa(1), normBrightness(r.light_bri_manual_ha1, normBrightness(loadStr(keyBriHa(1), "60"), 60)));
          saveStr(keyBriHa(2), normBrightness(r.light_bri_manual_ha2, normBrightness(loadStr(keyBriHa(2), "60"), 60)));
          saveStr(keyBriHa(3), normBrightness(r.light_bri_manual_ha3, normBrightness(loadStr(keyBriHa(3), "60"), 60)));
          lightHA.value = String(normLight(loadStr(keyHa(activeHotAirSlot()), lightHA.value || "0"), 0));
          lightBrightnessH1.value = String(normBrightness(loadStr(K_LBVH1, lightBrightnessH1.value || "60"), 60));
          lightBrightnessH2.value = String(normBrightness(loadStr(K_LBVH2, lightBrightnessH2.value || "60"), 60));
          lightBrightnessHA.value = String(normBrightness(loadStr(keyBriHa(activeHotAirSlot()), lightBrightnessHA.value || "60"), 60));
          updateBriLabels();
          setAutoEnabled(!!r.auto_stop_enabled);
          autoTime.value = fmtHm(r.auto_stop_minute);
          localSave();
        }

        localApply();
        window.__refreshLightBrightnessVisibility = refreshLightBrightnessVisibility;
        pullFromDevice().catch(() => {});
        lightH1.addEventListener("change", localSave);
        lightH2.addEventListener("change", localSave);
        lightHA.addEventListener("change", localSave);
        btnLightBrightnessModeH1.addEventListener("click", () => {
          writeModeH1(readModeH1() === "last" ? "manual" : "last");
          localSave();
        });
        btnLightBrightnessModeH2.addEventListener("click", () => {
          writeModeH2(readModeH2() === "last" ? "manual" : "last");
          localSave();
        });
        btnLightBrightnessModeHA.addEventListener("click", () => {
          const slot = activeHotAirSlot();
          writeModeHa(slot, readModeHa(slot) === "last" ? "manual" : "last");
          localSave();
        });
        lightBrightnessH1.addEventListener("input", localSave);
        lightBrightnessH1.addEventListener("change", localSave);
        lightBrightnessH2.addEventListener("input", localSave);
        lightBrightnessH2.addEventListener("change", localSave);
        lightBrightnessHA.addEventListener("input", localSave);
        lightBrightnessHA.addEventListener("change", localSave);
        autoTime.addEventListener("change", () => { autoTime.value = fmtHm(autoTime.value); localSave(); });
        autoOn.addEventListener("click", () => { setAutoEnabled(true); localSave(); });
        autoOff.addEventListener("click", () => { setAutoEnabled(false); localSave(); });
        slotBtns.forEach((btn) => {
          if (!btn) return;
          btn.addEventListener("click", () => {
            setTimeout(() => {
              lightHA.value = String(normLight(loadStr(keyHa(activeHotAirSlot()), lightHA.value || "0"), 0));
              lightBrightnessHA.value = String(normBrightness(loadStr(keyBriHa(activeHotAirSlot()), lightBrightnessHA.value || "60"), 60));
              localSave();
            }, 0);
          });
        });
        if (sel) sel.addEventListener("change", () => setTimeout(updateBriLabels, 0));
        btnApplyLogic.addEventListener("click", () => {
          pushToDevice("apply_logic").catch(() => {});
        });
      }
      function initFirmwarePartitionMap() {
        const listEl = document.getElementById("fwLayoutList");
        const btnRecovery = document.getElementById("btnFwRecovery");
        if (!listEl) return;
        const logEl = document.getElementById("eventLog");
        function ts() {
          const d = new Date();
          const p2 = (n) => String(n).padStart(2, "0");
          return `${d.getFullYear()}-${p2(d.getMonth() + 1)}-${p2(d.getDate())} ${p2(d.getHours())}:${p2(d.getMinutes())}:${p2(d.getSeconds())}`;
        }
        function append(line) {
          if (!logEl) return;
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          logEl.textContent = `${cur}\n[ui] ${ts()} ${line}`;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function pillClass(status) {
          const s = String(status || "").toUpperCase();
          if (s === "OK" || s === "READY" || s === "ACTIVE") return "pill good";
          if (s === "STANDBY") return "pill cyan";
          if (s === "N/A" || s === "MISSING") return "pill warn";
          if (s === "FAIL" || s === "ERROR") return "pill warn";
          return "pill";
        }
        function render(rows) {
          const r = Array.isArray(rows) ? rows : [];
          listEl.innerHTML = "";
          r.forEach((row) => {
            const it = document.createElement("div");
            it.className = "listItem";
            const left = document.createElement("div");
            left.className = "liLeft";
            const nm = document.createElement("div");
            nm.className = "liName";
            nm.textContent = String(row.name || "");
            const meta = document.createElement("div");
            meta.className = "liMeta";
            meta.textContent = String(row.detail || "");
            left.appendChild(nm);
            left.appendChild(meta);
            const st = document.createElement("span");
            st.className = pillClass(row.status);
            st.textContent = String(row.status || "");
            it.appendChild(left);
            it.appendChild(st);
            listEl.appendChild(it);
          });
        }
        let lastSafe = "";
        let lastVerifySig = "";
        let lastVerifyAtMs = 0;
        async function verifyAuto() {
          const r = await apiJson("/api/fw/verify", { method: "POST", body: "{}" });
          if (!r || !r.ok || !Array.isArray(r.checks)) return;
          const checks = r.checks
            .filter((c) => String(c && c.name ? c.name : "").trim().toUpperCase() !== "OTA_B")
            .map((c) => {
              const out = Object.assign({}, c || {});
              const nm = String(out.name || "").trim().toUpperCase();
              if (nm === "OTA_A") out.name = "System";
              return out;
            });
          const sig = checks.map((c) => `${c.name || "?"}:${c.ok ? 1 : 0}:${c.detail || ""}`).join("|");
          if (!sig || sig === lastVerifySig) return;
          lastVerifySig = sig;
          checks.forEach((c) => append(`[fw] check ${c.name || "?"}: ${c.ok ? "OK" : "FAIL"} ${c.detail || ""}`));
        }
        async function refresh() {
          const r = await apiJson("/api/fw/layout");
          if (!r || !r.ok) {
            render([{ name: "Firmware Layout", status: "N/A", detail: (r && r.error) ? r.error : "unavailable" }]);
            return;
          }
          const rawRows = Array.isArray(r.layout) ? r.layout : [];
          const viewRows = rawRows
            .filter((row) => String(row && row.name ? row.name : "").trim().toUpperCase() !== "OTA_B")
            .map((row) => {
              const out = Object.assign({}, row || {});
              const nm = String(out.name || "").trim().toUpperCase();
              if (nm === "OTA_A") out.name = "System";
              if (String(out.detail || "").trim().toLowerCase() === "ota_a") out.detail = "system";
              return out;
            });
          render(viewRows);
          const safeKey = r.safe_mode ? `1:${r.safe_cause || "unknown"}` : "0";
          if (safeKey !== lastSafe) {
            lastSafe = safeKey;
            if (r.safe_mode) append(`SYSTEM STATUS: SAFE MODE; CAUSE: ${r.safe_cause || "unknown"}`);
            else append("SYSTEM STATUS: NORMAL");
          }
          const now = Date.now();
          if ((now - lastVerifyAtMs) >= 60000) {
            lastVerifyAtMs = now;
            verifyAuto().catch(() => {});
          }
        }
        if (btnRecovery) btnRecovery.addEventListener("click", async () => {
          const recoveryUrl = `${location.protocol}//${location.host}/recovery.html`;
          const pass = await askPasswordModal({
            title: tr("prompt_confirm_pass", "Enter current web password to continue"),
            okText: "OK",
            cancelText: tr("btn_no", "Cancel")
          });
          if (!pass) {
            append(tr("msg_confirm_pass_required", "Operation canceled: password not provided"));
            return;
          }
          append("enter recovery requested");
          append(`${tr("msg_fw_recovery_hint", "after reboot open")}: ${recoveryUrl} (fallback: http://lb-bridge.local/recovery.html)`);
          const r = await apiJson("/api/fw/enter_recovery", {
            method: "POST",
            body: "{}",
            headers: { "X-LB-Confirm-Pass-Enc": criticalHeaderValue(pass) }
          });
          if (!r || !r.ok) append(`recovery failed: ${(r && r.error) ? r.error : "unknown"}`);
          else scheduleRecoveryAutoOpen(recoveryUrl, "http://lb-bridge.local/recovery.html");
        });
        refresh();
        setInterval(refresh, 5000);
      }
      function initPresenceDemo() {
        const b1 = document.getElementById("senseH1");
        const b2 = document.getElementById("senseH2");
        const b3 = document.getElementById("senseHA");
        const btnApplyLogic = document.getElementById("btnApplyLogic");
        const sd1 = document.getElementById("startDelayH1") || document.querySelector("#sdH1Wrap input");
        const sd2 = document.getElementById("startDelayH2") || document.querySelector("#sdH2Wrap input");
        const sd3 = document.getElementById("startDelayHA") || document.querySelector("#sdHAWrap input");
        const td1 = document.getElementById("stopDelayH1") || document.querySelector("#stH1Wrap input");
        const td2 = document.getElementById("stopDelayH2") || document.querySelector("#stH2Wrap input");
        const td3 = document.getElementById("stopDelayHA") || document.querySelector("#stHAWrap input");
        const logEl = document.getElementById("eventLog");
        const hasPresenceButtons = !!(b1 && b2 && b3);
        function load(key) {
          try { return localStorage.getItem(key) === "1"; } catch (_) { return false; }
        }
        function save(key, on) {
          try { localStorage.setItem(key, on ? "1" : "0"); } catch (_) {}
        }
        function setBtn(btn, on) {
          if (!btn) return;
          setTogglePressed(btn, !!on, { onClass: "on" });
        }
        function toggle(btn, key) {
          if (!btn) return false;
          const next = !isTogglePressed(btn, { onClass: "on" });
          setBtn(btn, next);
          save(key, next);
          return next;
        }
        function isOn(btn) {
          return isTogglePressed(btn, { onClass: "on" });
        }
        function anyOn() {
          return isOn(b1) || isOn(b2) || isOn(b3);
        }
        function getSpeedRestoreMode() {
          try {
            if (typeof window.__getSpeedRestoreMode === "function") {
              const mode = String(window.__getSpeedRestoreMode() || "").toLowerCase();
              return mode === "manual" ? "manual" : "last_set";
            }
            return "last_set";
          } catch (_) {
            return "last_set";
          }
        }
        function getManualSpeedForSource(sourceKey) {
          try {
            if (typeof window.__getManualSpeedBySource === "function") {
              const v = Number(window.__getManualSpeedBySource(sourceKey));
              if (Number.isFinite(v)) return Math.max(0, Math.min(100, Math.round(v)));
            }
          } catch (_) {}
          let key = "alb_manual_speed_h1";
          if (sourceKey === "h2") key = "alb_manual_speed_h2";
          else if (sourceKey === "ha") key = "alb_manual_speed_ha";
          try {
            const n = parseInt(String(localStorage.getItem(key) || "35"), 10);
            if (!Number.isFinite(n)) return 35;
            return Math.max(0, Math.min(100, n));
          } catch (_) {
            return 35;
          }
        }
        function getActiveHotAirSlot() {
          try {
            const n = parseInt(String(localStorage.getItem("alb_h312_slot_active") || "1"), 10);
            if (!Number.isFinite(n)) return 1;
            return Math.max(1, Math.min(3, n));
          } catch (_) {
            return 1;
          }
        }
        function mapSourceKeyToApiSource(sourceKey) {
          const k = String(sourceKey || "").trim().toLowerCase();
          if (k === "h1") return "h1";
          if (k === "h2") return "h2";
          if (k === "ha") {
            const slot = getActiveHotAirSlot();
            return `ha${slot}`;
          }
          return "";
        }
        function pickOnSource(changedKey) {
          if (changedKey === "h1" && isOn(b1)) return "h1";
          if (changedKey === "h2" && isOn(b2)) return "h2";
          if (changedKey === "ha" && isOn(b3)) return "ha";
          if (isOn(b1)) return "h1";
          if (isOn(b2)) return "h2";
          if (isOn(b3)) return "ha";
          return null;
        }
        function ts() {
          const d = new Date();
          const p2 = (n) => String(n).padStart(2, "0");
          return `${p2(d.getHours())}:${p2(d.getMinutes())}:${p2(d.getSeconds())}`;
        }
        function append(line) {
          if (!logEl) return;
          const cur = (logEl.textContent || "").replace(/\s+$/, "");
          logEl.textContent = `${cur}\n[logic] ${ts()} ${line}`;
          logEl.scrollTop = logEl.scrollHeight;
        }
        function parseDelayMs(raw, fallbackMs) {
          const s = String(raw || "").trim().toLowerCase().replace(",", ".");
          const m = s.match(/(\d+(?:\.\d+)?)/);
          if (!m) return fallbackMs;
          let n = Number(m[1]);
          if (!Number.isFinite(n) || n < 0) return fallbackMs;
          if (s.includes("ms")) return Math.max(0, Math.min(600000, Math.round(n)));
          return Math.max(0, Math.min(600000, Math.round(n * 1000)));
        }
        function getStartDelayMs(key) {
          if (key === "h1") return parseDelayMs(sd1 ? sd1.value : "5 s", 5000);
          if (key === "h2") return parseDelayMs(sd2 ? sd2.value : "5 s", 5000);
          return parseDelayMs(sd3 ? sd3.value : "5 s", 5000);
        }
        function getStopDelayMs(key) {
          if (key === "h1") return parseDelayMs(td1 ? td1.value : "20 s", 20000);
          if (key === "h2") return parseDelayMs(td2 ? td2.value : "20 s", 20000);
          return parseDelayMs(td3 ? td3.value : "20 s", 20000);
        }
        const delayKeys = {
          sd1: "alb_delay_start_h1",
          sd2: "alb_delay_start_h2",
          sd3: "alb_delay_start_ha",
          td1: "alb_delay_stop_h1",
          td2: "alb_delay_stop_h2",
          td3: "alb_delay_stop_ha",
        };
        function loadText(key, def) {
          try {
            const v = localStorage.getItem(key);
            if (v == null || String(v).trim() === "") return def;
            return String(v);
          } catch (_) {
            return def;
          }
        }
        function saveText(key, value) {
          try { localStorage.setItem(key, String(value == null ? "" : value)); } catch (_) {}
        }
        function bindDelayPersist(el, key) {
          if (!el || !key) return;
          el.value = loadText(key, String(el.value || ""));
          const saveNow = () => saveText(key, el.value);
          el.addEventListener("input", saveNow);
          el.addEventListener("change", saveNow);
          el.addEventListener("blur", saveNow);
        }
        function saveAllDelayFields() {
          if (sd1) saveText(delayKeys.sd1, sd1.value);
          if (sd2) saveText(delayKeys.sd2, sd2.value);
          if (sd3) saveText(delayKeys.sd3, sd3.value);
          if (td1) saveText(delayKeys.td1, td1.value);
          if (td2) saveText(delayKeys.td2, td2.value);
          if (td3) saveText(delayKeys.td3, td3.value);
        }
        function delayTextFromMs(v, fallback) {
          const n = parseInt(String(v == null ? "" : v), 10);
          if (!Number.isFinite(n) || n < 0) return String(fallback || "5 s");
          if (n % 1000 === 0) return `${Math.round(n / 1000)} s`;
          return `${n} ms`;
        }
        async function syncDelayFieldsFromDevice() {
          const r = await apiJson("/api/config", { method: "GET", timeout_ms: 7000 });
          if (!r || !r.ok) return false;
          if (sd1 && r.t420d_h1_on_ms != null) sd1.value = delayTextFromMs(r.t420d_h1_on_ms, sd1.value || "5 s");
          if (sd2 && r.t420d_h2_on_ms != null) sd2.value = delayTextFromMs(r.t420d_h2_on_ms, sd2.value || "5 s");
          if (td1 && r.t420d_h1_off_ms != null) td1.value = delayTextFromMs(r.t420d_h1_off_ms, td1.value || "20 s");
          if (td2 && r.t420d_h2_off_ms != null) td2.value = delayTextFromMs(r.t420d_h2_off_ms, td2.value || "20 s");
          saveAllDelayFields();
          return true;
        }
        async function pushDelayConfigToDevice(reason) {
          const body = {
            t420d_logic_enabled: true,
            t420d_h1_on_ms: getStartDelayMs("h1"),
            t420d_h2_on_ms: getStartDelayMs("h2"),
            t420d_h1_off_ms: getStopDelayMs("h1"),
            t420d_h2_off_ms: getStopDelayMs("h2")
          };
          const r = await apiJson("/api/config", { method: "POST", body: JSON.stringify(body), timeout_ms: 9000 });
          if (r && r.ok) {
            append(`[t420d] ${reason}: delays saved on=${body.t420d_h1_on_ms}/${body.t420d_h2_on_ms} off=${body.t420d_h1_off_ms}/${body.t420d_h2_off_ms}`);
            return true;
          }
          append(`[t420d] ${reason}: delay save failed: ${(r && r.error) ? r.error : "unknown"}`);
          return false;
        }
        bindDelayPersist(sd1, delayKeys.sd1);
        bindDelayPersist(sd2, delayKeys.sd2);
        bindDelayPersist(sd3, delayKeys.sd3);
        bindDelayPersist(td1, delayKeys.td1);
        bindDelayPersist(td2, delayKeys.td2);
        bindDelayPersist(td3, delayKeys.td3);
        syncDelayFieldsFromDevice().catch(() => {});
        function delayForOn(changedKey) {
          if (changedKey === "h1" && isOn(b1)) return getStartDelayMs("h1");
          if (changedKey === "h2" && isOn(b2)) return getStartDelayMs("h2");
          if (changedKey === "ha" && isOn(b3)) return getStartDelayMs("ha");
          const arr = [];
          if (isOn(b1)) arr.push(getStartDelayMs("h1"));
          if (isOn(b2)) arr.push(getStartDelayMs("h2"));
          if (isOn(b3)) arr.push(getStartDelayMs("ha"));
          return arr.length ? Math.min(...arr) : 0;
        }
        function delayForOff(changedKey) {
          if (changedKey === "h1" && !isOn(b1)) return getStopDelayMs("h1");
          if (changedKey === "h2" && !isOn(b2)) return getStopDelayMs("h2");
          if (changedKey === "ha" && !isOn(b3)) return getStopDelayMs("ha");
          return Math.max(getStopDelayMs("h1"), getStopDelayMs("h2"), getStopDelayMs("ha"));
        }
        let timerId = 0;
        let pendingTarget = null;
        let pendingSource = null;
        function clearPending() {
          if (timerId) {
            clearTimeout(timerId);
            timerId = 0;
          }
          pendingTarget = null;
          pendingSource = null;
        }
        async function sendPower(on, reason, sourceKey) {
          append(`${reason}: sending extractor ${on ? "ON" : "OFF"}...`);
          const apiSource = mapSourceKeyToApiSource(sourceKey);
          const powerBody = { on };
          if (apiSource) powerBody.source = apiSource;
          const r = await apiJson("/api/extractor/power", {
            method: "POST",
            body: JSON.stringify(powerBody),
            timeout_ms: 45000
          });
          if (r && r.ok) {
            append(`${reason}: extractor ${on ? "ON" : "OFF"} ok`);
            const meta = [
              `target=${(r && r.target != null) ? r.target : "?"}`,
              `src=${(r && r.connect_source) ? r.connect_source : "-"}`,
              `addr=${(r && r.addr) ? r.addr : "-"}`,
              `tx=${(r && r.tx) ? r.tx : "-"}`,
              `dt=${(r && r.duration_ms != null) ? r.duration_ms : "?"}ms`
            ].join(" ");
            append(`${reason}: ${meta}`);
            if (r && r.warn) append(`${reason}: warn: ${r.warn}`);
            if (on && getSpeedRestoreMode() === "manual") {
              const src = sourceKey || pickOnSource("manual");
              if (src) {
                const manualSpeed = getManualSpeedForSource(src);
                const speedSource = mapSourceKeyToApiSource(src);
                const speedBody = { speed: manualSpeed };
                if (speedSource) speedBody.source = speedSource;
                const sr = await apiJson("/api/extractor/speed", {
                  method: "POST",
                  body: JSON.stringify(speedBody),
                  timeout_ms: 25000
                });
                if (sr && sr.ok) append(`${reason}: manual speed ${manualSpeed}% applied (${src})`);
                else append(`${reason}: manual speed apply failed (${src}): ${(sr && sr.error) ? sr.error : "unknown"}`);
              }
            }
            setTimeout(async () => {
              const st = await apiJson("/api/extractor/status", { method: "GET", timeout_ms: 25000 });
              if (st && st.ok) {
                append(`${reason}: status ble_ok=${st.ble_ok ? 1 : 0} speed=${st.speed} saved=${st.saved}`);
              } else {
                append(`${reason}: status read failed: ${(st && st.error) ? st.error : "unknown"}`);
              }
            }, 1200);
            return;
          }
          const err = (r && r.error) ? r.error : "unknown";
          append(`${reason}: extractor command failed: ${err}`);
          if (r && typeof r === "object") {
            const metaFail = [
              `target=${(r.target != null) ? r.target : "?"}`,
              `src=${r.connect_source || "-"}`,
              `addr=${r.addr || "-"}`,
              `tx=${r.tx || "-"}`,
              `dt=${(r.duration_ms != null) ? r.duration_ms : "?"}ms`
            ].join(" ");
            append(`${reason}: ${metaFail}`);
          }
        }
        function schedulePower(on, delayMs, reason, sourceKey) {
          clearPending();
          pendingTarget = !!on;
          pendingSource = sourceKey || null;
          if (delayMs <= 0) {
            void sendPower(!!on, reason, pendingSource);
            return;
          }
          const sec = (delayMs / 1000).toFixed((delayMs % 1000) ? 1 : 0);
          append(`${reason}: ${on ? "start" : "stop"} in ${sec}s`);
          timerId = setTimeout(() => {
            timerId = 0;
            const target = pendingTarget;
            const source = pendingSource;
            pendingTarget = null;
            pendingSource = null;
            append(`${reason}/timer: fired`);
            void sendPower(!!target, `${reason}/timer`, source);
          }, delayMs);
        }
        function evaluate(changedKey) {
          if (anyOn()) {
            const onSource = pickOnSource(changedKey);
            schedulePower(true, delayForOn(changedKey), "presence_on", onSource);
          } else {
            schedulePower(false, delayForOff(changedKey), "presence_off", changedKey || null);
          }
        }
        if (hasPresenceButtons) {
          setBtn(b1, load("alb_presence_h1"));
          setBtn(b2, load("alb_presence_h2"));
          setBtn(b3, load("alb_presence_ha"));
          b1.addEventListener("click", () => { toggle(b1, "alb_presence_h1"); evaluate("h1"); });
          b2.addEventListener("click", () => { toggle(b2, "alb_presence_h2"); evaluate("h2"); });
          b3.addEventListener("click", () => { toggle(b3, "alb_presence_ha"); evaluate("ha"); });
        }
        if (btnApplyLogic) {
          btnApplyLogic.addEventListener("click", async (ev) => {
            ev.preventDefault();
            ev.stopPropagation();
            saveAllDelayFields();
            await pushDelayConfigToDevice("apply_logic");
            clearPending();
            if (hasPresenceButtons) {
              const src = pickOnSource("manual");
              await sendPower(anyOn(), "apply_logic", src);
            }
            else append("apply_logic: values saved");
          });
        }
      }
      sel.value = initial;
      initWaveClothBackground();
      initNames(i18n[initial] || i18n.en);
      initDeviceTile();
      initPwaInstallCapture();
      shortcutOfferApi = initShortcutOffer();
      initPwaInstall();
      initTopbarRecovery();
      initTopbarLogout();
      initWifiScan();
      initWifiAutoOff();
      initExtractorPicker();
      initHotAirBinding();
      initDeviceClock();
      initT420dGpioConfig();
      initSpeedRestore();
      initLightScheduleConfig();
      initApplyDiag();
      initFirmwarePartitionMap();
      initPresenceDemo();
      initPremiumRanges(document);
      bindMomentaryPressed(document.getElementById("githubLink"), 220);
      sel.addEventListener("change", () => applyLang(normalizeLang(sel.value)));
      applyLang(initial);
    })();


