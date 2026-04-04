#include <Arduino.h>
#if defined(ARDUINO_USB_MODE) && defined(ARDUINO_USB_CDC_ON_BOOT)
#if ARDUINO_USB_MODE && ARDUINO_USB_CDC_ON_BOOT
#include <HWCDC.h>
#endif
#endif
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WebServer.h>
#include <time.h>
#include <stdarg.h>
#include <math.h>
#include <ctype.h>
#include <string.h>
#include <mbedtls/sha256.h>

#include <NimBLEDevice.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include "drivers/h312_driver.h"
#include "drivers/tes02_driver.h"

extern "C" {
#include "esp_app_format.h"
#include "esp_err.h"
#include "esp_heap_caps.h"
#include "esp_mac.h"
#include "esp_netif.h"
#include "mdns.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "esp_system.h"
#include "esp_wifi.h"
}

#ifndef INPUT_PULLDOWN
#define INPUT_PULLDOWN INPUT
#endif

#ifndef LB_SERIAL_LOG
#define LB_SERIAL_LOG 0
#endif

#if LB_SERIAL_LOG
#define LB_SERIAL_BEGIN(...) Serial.begin(__VA_ARGS__)
#define LB_SERIAL_PRINT(...) Serial.print(__VA_ARGS__)
#define LB_SERIAL_PRINTLN(...) Serial.println(__VA_ARGS__)
#define LB_SERIAL_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
#define LB_SERIAL_BEGIN(...) do {} while (0)
#define LB_SERIAL_PRINT(...) do {} while (0)
#define LB_SERIAL_PRINTLN(...) do {} while (0)
#define LB_SERIAL_PRINTF(...) do {} while (0)
#endif

#ifndef LB_FW_VERSION
#define LB_FW_VERSION "v1"
#endif
static const char *kFwVersion = LB_FW_VERSION;

#ifndef LB_FW_PROFILE
#define LB_FW_PROFILE "standard"
#endif
#ifndef LB_HARDENED_BUILD
#define LB_HARDENED_BUILD 0
#endif

static const char *kFwProfile = LB_FW_PROFILE;
static const bool kFwHardenedBuild = (LB_HARDENED_BUILD != 0);

static const char *kFwOnlineDefaultUrl = "";
static const uint32_t kFwOnlineConnectTimeoutMs = 12000;
static const uint32_t kFwOnlineReadTimeoutMs = 20000;
static const uint8_t kAutoOnSpeedCap = 30;
static const char *kFwOnlineAllowedHost = "";
static const char *kFwOnlineRootCaPem = "";

static const char *kApSsidPrefix = "LB-SETUP-";
static const char *kPrefsNs = "tes02ctrl";
static const char *kWebAuthUserDefault = "admin";
static const uint8_t kWebAuthFailMax = 5;
static const uint32_t kWebAuthBlockMs = 5UL * 60UL * 1000UL;
static const uint8_t kRememberTokenSlots = 6;
static const uint32_t kRememberTokenMaxAgeSec = 30UL * 24UL * 60UL * 60UL;
static const char *kRememberCookieName = "lb_rt";
static const char *kCriticalPassHeader = "X-LB-Confirm-Pass";
static const char *kCriticalPassHeaderEnc = "X-LB-Confirm-Pass-Enc";
static const bool kFwOnlineUpdateEnabled = false;
static const bool kBleBootPulseEnabled = true;
static const uint8_t kBleBootPulseSpeed = 35;
static const uint16_t kBleBootPulseOnMs = 1000;
static const uint32_t kBleBootPulseScanMs = 9000;
static const uint32_t kClockNtpRetryMs = 120000;
static const uint32_t kClockNtpResyncMs = 6UL * 60UL * 60UL * 1000UL;

static Preferences prefs;
static WebServer server(80);

static String wifiSsid;
static String wifiPass;
static String bleNamePrefix;
static String bleLockedAddr;  // optional, makes control deterministic
static String bleLockedName;  // display name selected during scan
static bool bleBound = false; // explicit binding state; no auto-connect when false
static uint16_t wifiAutoOffMin = 0;
enum ExtractorSource : uint8_t { SRC_H1 = 0, SRC_H2 = 1, SRC_HA1 = 2, SRC_HA2 = 3, SRC_HA3 = 4, SRC_COUNT = 5 };
static uint8_t gSourceLastSpeed[SRC_COUNT] = {30, 30, 30, 30, 30};
enum ExtractorLightMode : uint8_t { LIGHT_OFF = 0, LIGHT_WHITE = 1, LIGHT_GREEN = 2, LIGHT_LAST = 3 };
static uint8_t gSourceLightMode[SRC_COUNT] = {LIGHT_OFF, LIGHT_OFF, LIGHT_OFF, LIGHT_OFF, LIGHT_OFF};
static uint8_t gSourceLastEnabledLightMode[SRC_COUNT] = {LIGHT_WHITE, LIGHT_WHITE, LIGHT_WHITE, LIGHT_WHITE, LIGHT_WHITE};
enum ExtractorBrightnessMode : uint8_t { BRI_LAST = 0, BRI_MANUAL = 1 };
static uint8_t gSourceBrightnessMode[SRC_COUNT] = {BRI_LAST, BRI_LAST, BRI_LAST, BRI_LAST, BRI_LAST};
static uint8_t gSourceManualBrightness[SRC_COUNT] = {60, 60, 60, 60, 60};
static uint8_t gSourceLastBrightness[SRC_COUNT] = {60, 60, 60, 60, 60};
static uint8_t lastOnSpeed = 30; // active source speed, kept for legacy API field "saved"
static ExtractorSource gActiveSource = SRC_H1;
enum H312TransportMode : uint8_t { H312_MODE_WIFI = 0, H312_MODE_BLE = 1 };
static bool h312Enabled = false;
static H312TransportMode h312Mode = H312_MODE_WIFI;
static String h312Ip;
static String h312BleAddr;
static String h312BleName;
static uint16_t h312Port = 6666;
static int16_t h312TempOn = 140;
static int16_t h312TempOff = 90;
static uint16_t h312DebounceMs = 1200;
static uint32_t h312MinOnMs = 5000;
static uint8_t h312Slot = 1; // 1..3, maps H312 telemetry logic to HA1..HA3 speed profile
static int8_t t420dH1Gpio = -1;
static int8_t t420dH2Gpio = -1;
static bool t420dH1RaisedOnHigh = true;
static bool t420dH2RaisedOnHigh = true;
static bool t420dLogicEnabled = true;
static uint32_t t420dH1OnDelayMs = 1000;
static uint32_t t420dH2OnDelayMs = 1000;
static uint32_t t420dH1OffDelayMs = 20000;
static uint32_t t420dH2OffDelayMs = 20000;
static int16_t tzOffsetMin = 0;
static String tzName = "UTC";
static bool tzConfigured = false;
static bool clockValid = false;
static uint64_t clockBaseEpochMs = 0;
static uint32_t clockBaseUptimeMs = 0;
static uint32_t clockLastSetMs = 0;
static String clockLastSyncSource = "none";
static uint32_t clockNtpLastTryMs = 0;
static uint32_t clockLastNtpSyncMs = 0;
static bool autoStopEnabled = false;
static uint16_t autoStopMinute = 0; // local minute-of-day, 0..1439
static uint32_t autoStopLastYmd = 0;

static bool gSafeMode = false;
static String gSafeCause;
static String gStaHostname = "lb-bridge-00000";
static String gApSsid = "LB-SETUP-00000";
static String gApPass = "lb00000!2026";
static String gWebAuthUser = "admin";
static String gWebAuthPass = "lb00000!2026";
static String gMacText = "00:00:00:00:00:00";
static String gMacSuffixLower = "00000";
static String gMacSuffixUpper = "00000";
static char gMdnsIdTxt[20] = "LB-00000";
static bool gMdnsStarted = false;
static uint32_t gApDisableAtMs = 0;
static IPAddress gWebAuthFailIp(0, 0, 0, 0);
static uint8_t gWebAuthFailCount = 0;
static uint32_t gWebAuthBlockUntilMs = 0;
static bool gReqAuthedByToken = false;
static bool gReqAuthedByBasic = false;
static String gReqTokenHash;
static h312::H312Driver gH312Driver;
static tes02::Tes02Driver gTes02Driver;
static NimBLEClient *gH312BleClient = nullptr;
static NimBLERemoteCharacteristic *gH312BleCharBb02 = nullptr;
static NimBLERemoteCharacteristic *gH312BleCharFf01 = nullptr;
static uint8_t gH312BleCharKind = 2; // 2=bb02(json), 1=bb01(approx)
static String gH312BleRuntimeAddr;
static int8_t gH312BleRuntimeAddrType = -1; // BLE_ADDR_* from NimBLE (0..3), -1 unknown
static String gH312BleRuntimeIp;
static String gH312BleLastFailedAddr;
static uint32_t gH312BleLastFailedMs = 0;
static uint32_t gH312BleLastIpReadMs = 0;
static int8_t gH312BleFailStreak = 0;
static int16_t gH312BlePrevRt = -1000;
static uint32_t gH312BlePrevRtMs = 0;
static uint8_t gH312BleSecLikeStreak = 0;
static uint8_t gH312BleJsonWaitStreak = 0;
static std::string gH312BleJsonBuf;
static std::string gH312BleNotifyBuf;
static portMUX_TYPE gH312BleNotifyMux = portMUX_INITIALIZER_UNLOCKED;
static bool gH312BleNotifySubscribed = false;
static uint32_t gH312BleNextPollMs = 0;
static bool gH312BleLinkWasUp = false;
static bool gH312BleConnectPending = false;

static String gBleLastAddr = "";
static String gBleLastName = "";
static int gBleLastRssi = -999;
static String gBleConnectSource = "none";
static String gBleLastTx = "";
static int gBleLastSetBefore = -1;
static int gBleLastSetAfter = -1;
static int gBleLastSetTarget = -1;
static uint32_t gBleLastSetMs = 0;
static uint32_t gLockedBackoffUntilMs = 0;
static uint32_t gBleConnectedAtMs = 0;
static uint32_t gBleLastActivityMs = 0;
static const uint32_t kBleIdleDisconnectMs = 12000;
static const bool kBleReleaseAfterOn = true; // release BLE lock after ON so manual panel control stays responsive
static bool gExtractorSessionActive = false;
static ExtractorSource gExtractorSessionSource = SRC_H1;
static bool gExtractorSessionHadPreLightOn = false;
static uint32_t gH312LastJsonMs = 0;
static bool gH312SeenTelemetry = false;
static int gH312WorkStatus = -1;
static int gH312HeatFlag = -1;
static int gH312RealTimeTemp = -1000;
static int gH312SetTemp = -1;
static int gH312RealFan = -1;
static int gH312SetFan = -1;
static bool gH312ExtractorOn = false;
static uint32_t gH312ExtractorOnSinceMs = 0;
static uint32_t gH312ExtractorOffSinceMs = 0;
static bool gH312PendingValid = false;
static bool gH312PendingOn = false;
static uint32_t gH312PendingSinceMs = 0;
static bool gT420dExtractorOn = false;
static bool gT420dPendingValid = false;
static bool gT420dPendingOn = false;
static uint32_t gT420dPendingSinceMs = 0;
static uint32_t gT420dPendingDelayMs = 0;
static ExtractorSource gT420dPendingSource = SRC_H1;
static bool gT420dLastH1Raised = false;
static bool gT420dLastH2Raised = false;
static bool gT420dDebouncedH1Raised = false;
static bool gT420dDebouncedH2Raised = false;
static bool gT420dDebounceCandH1Raised = false;
static bool gT420dDebounceCandH2Raised = false;
static uint32_t gT420dDebounceH1SinceMs = 0;
static uint32_t gT420dDebounceH2SinceMs = 0;
static String gT420dLastLogic = "idle";
static String gT420dLastError = "";
static uint32_t gT420dLastEvalMs = 0;
static bool gH312DemandOn = false;
static bool gT420dDemandOn = false;
static uint32_t gGlobalAllDemandOffSinceMs = 0;
static String gH312LastError;
static String gH312LastLogic = "idle";
static const uint32_t kH312PollIdleMs = 2000;                         // fixed: 2s temperature polling cadence
static const uint32_t kH312PollHotMs = 2000;                          // fixed: 2s temperature polling cadence
static const uint32_t kH312BlePollMs = 7000;
static const uint32_t kH312BleScanMs = 900;
static const uint32_t kH312BleRetryFailMs = 15000;
static const uint32_t kH312BleReconnectAttemptMinMs = 8000;
static const uint32_t kH312BleReconnectLogMinMs = 30000;
static const uint32_t kH312BleYieldAfterTes02Ms = 1800;
static const uint8_t kH312BleConnectTimeoutSec = 1;
static const uint32_t kH312BleYieldAfterHttpMs = 250;
static const uint32_t kH312BleFailedAddrTtlMs = 180000;
static const uint32_t kH312BleIpRefreshMs = 12000;
static const uint16_t kH312BleTcpConnectMs = 3200;
static const uint16_t kH312BleTcpReadMs = 4200;
static const uint16_t kH312BleTcpStartupDelayMs = 40;
static const uint16_t kH312BleTcpKeepaliveAfterMs = 900;
static const uint32_t kH312BleTcpFailBackoffMs = 6000;
static const bool kH312BleRuntimeClientEnabled = true;
static const bool kH312BleDiagLogs = false;
static const uint32_t kH312BleDiagSampleMs = 10000;
static const uint32_t kH312TempLogMinGapMs = 4000;
static const uint32_t kH312TelemetryLossForceOffMs = 30000;           // fail-safe off when telemetry is lost too long
static const uint32_t kH312FailSafeRetryMs = 5000;                    // spacing between fail-safe off attempts
static const uint32_t kH312PostSwitchGuardMs = 7000;                  // suppress immediate opposite action after switch
static const uint32_t kH312MinOffMs = 7000;                           // minimum off-hold before auto ON
static const uint32_t kT420dLogicTickMs = 220;
static const uint32_t kT420dGpioDebounceMs = 700;
static const int kHandleMvHighThreshold = 1800;
static const int kHandleMvLowThreshold = 1200;
static const uint32_t kGlobalOffDemandConfirmMs = 2500;
static const bool kUartDiagEnabled = (LB_SERIAL_LOG != 0);
static const uint32_t kUartDiagTickMs = 2000;
static const uint32_t kUiPerfLogTickMs = 30000;
static const uint8_t kUiPerfDeltaPct = 5;
static const bool kUiLogVerbose = false;
static const bool kWifiTxAutoTuneEnabled = true;
static const uint32_t kWifiTxTunePeriodMs = 15000;
static const int8_t kWifiTxQdbmMin = 58;    // 14.5 dBm floor to avoid over-tuning down
static const int8_t kWifiTxQdbmMax = 78;    // 19.5 dBm
static const int8_t kWifiTxQdbmStep = 4;    // 1.0 dBm
static const int8_t kWifiTxRssiLowTune = -65;
static const int8_t kWifiTxRssiHighTune = -55;
static const uint8_t kWifiTxConfirmCycles = 3;
static const uint32_t kWifiTxStepMinGapMs = 60000;
static const uint32_t kWifiTxStepWindowMs = 300000;
static const uint8_t kWifiTxStepWindowMax = 3;
static const uint32_t kFailsafeApBootPinMs = 120000;
static const uint32_t kFailsafeStaLostToApMs = 12000;
static const uint32_t kFailsafeServiceStallUs = 5000000;
static const uint16_t kRecoveryHandleSampleMs = 450;
static const uint16_t kRecoveryHandleSampleStepMs = 15;
static const uint32_t kRecoveryUnlockWindowMs = 10UL * 60UL * 1000UL;
static const bool kHandleGpioActiveHigh = true; // trigger when signal appears on input
static const int8_t kT420dHandle1GpioFixed = 1;
static const int8_t kT420dHandle2GpioFixed = 3;
static const uint16_t kHandleStatusSampleMs = 24;
static const uint16_t kHandleStatusSampleStepMs = 4;
static const uint32_t kHandleDiagSampleMs = 120;
static const uint32_t kHandleBounceWindowMs = 1200;
static const uint8_t kHandleBounceBurstWarn = 4;
static uint32_t gUartDiagLastMs = 0;
static uint32_t gUiPerfLastMs = 0;
static uint32_t gWebReqLastMs = 0;
static uint32_t gWifiTxLastTuneMs = 0;
static uint32_t gWifiTxLastStepMs = 0;
static uint32_t gH312BleDiagLastMs = 0;
static int gH312BleDiagLastTemp = -10000;
static uint32_t gH312BleSkipDiagMs = 0;
static uint32_t gH312BleLastReconnectLogMs = 0;
static uint32_t gH312BleLastConnectAttemptMs = 0;
static uint32_t gH312BleLastReconnectAttemptUiLogMs = 0;
static uint32_t gH312BleLastReconnectLinkUiLogMs = 0;
static uint32_t gWifiTxStepWindowStartMs = 0;
static int8_t gWifiTxLastQdbm = -1;
static uint8_t gWifiTxStepWindowCount = 0;
static uint8_t gWifiTxNeedUpCycles = 0;
static uint8_t gWifiTxNeedDownCycles = 0;
static bool gUiPerfPrevValid = false;
static uint8_t gUiPerfPrevCpuPct = 0;
static uint8_t gUiPerfPrevCorePct = 0;
static uint8_t gUiPerfPrevH312Pct = 0;
static uint8_t gUiPerfPrevSessPct = 0;
static uint8_t gUiPerfPrevBlePct = 0;
static uint8_t gUiPerfPrevWebPct = 0;
static uint32_t gH312LastFailSafeTryMs = 0;
static bool gDiagPrevH312Connected = false;
static bool gDiagPrevBleReady = false;
static bool gDiagPrevExtractorOn = false;
static String gDiagPrevLogic;
static String gDiagPrevH312Error;
static String gDiagPrevH312Gate;
static uint32_t gH312DiagBeatMs = 0;
static bool gH312UiErrActive[3] = {false, false, false};
static uint32_t gH312UiLastTempLogMs[3] = {0, 0, 0};
static int16_t gH312UiLastTempLogC[3] = {-1000, -1000, -1000};
static bool gFailsafeApPinned = false;
static uint32_t gFailsafeApPinUntilMs = 0;
static uint32_t gStaLostSinceMs = 0;
static bool gRecoveryUnlockByHandles = false;
static uint32_t gRecoveryUnlockUntilMs = 0;
static uint32_t gHandleDiagLastMs = 0;
static int gHandleDiagPrevH1 = -2;
static int gHandleDiagPrevH2 = -2;
static uint32_t gHandleDiagLastEdgeH1Ms = 0;
static uint32_t gHandleDiagLastEdgeH2Ms = 0;
static uint32_t gHandleDiagBurstWinH1Ms = 0;
static uint32_t gHandleDiagBurstWinH2Ms = 0;
static uint8_t gHandleDiagBurstH1 = 0;
static uint8_t gHandleDiagBurstH2 = 0;

// Lightweight runtime profiler (windowed, low overhead).
static uint32_t gPerfWindowStartUs = 0;
static uint32_t gPerfLastSnapMs = 0;
static uint64_t gPerfAccBusyUs = 0;
static uint64_t gPerfAccCoreUs = 0;
static uint64_t gPerfAccH312Us = 0;
static uint64_t gPerfAccSessUs = 0;
static uint64_t gPerfAccBleUs = 0;
static uint64_t gPerfAccDiagUs = 0;
static uint64_t gPerfAccWebUs = 0;
static uint32_t gPerfAccLoops = 0;
static uint32_t gPerfAccLoopMaxUs = 0;
static uint8_t gPerfCpuPct = 0;
static uint8_t gPerfCorePct = 0;
static uint8_t gPerfH312Pct = 0;
static uint8_t gPerfSessPct = 0;
static uint8_t gPerfBlePct = 0;
static uint8_t gPerfDiagPct = 0;
static uint8_t gPerfWebPct = 0;
static uint32_t gPerfLoopMaxUs = 0;
static uint32_t gPerfLoops = 0;

static inline uint8_t perfPct(uint64_t partUs, uint32_t windowUs) {
  if (windowUs == 0) return 0;
  uint64_t v = (partUs * 100ULL) / (uint64_t)windowUs;
  if (v > 100ULL) v = 100ULL;
  return (uint8_t)v;
}

static void perfMaybeSnap(uint32_t nowMs) {
  if (gPerfWindowStartUs == 0) gPerfWindowStartUs = (uint32_t)micros();
  if (gPerfLastSnapMs != 0 && (nowMs - gPerfLastSnapMs) < kUartDiagTickMs) return;

  uint32_t nowUs = (uint32_t)micros();
  uint32_t windowUs = nowUs - gPerfWindowStartUs;
  gPerfCpuPct = perfPct(gPerfAccBusyUs, windowUs);
  gPerfCorePct = perfPct(gPerfAccCoreUs, windowUs);
  gPerfH312Pct = perfPct(gPerfAccH312Us, windowUs);
  gPerfSessPct = perfPct(gPerfAccSessUs, windowUs);
  gPerfBlePct = perfPct(gPerfAccBleUs, windowUs);
  gPerfDiagPct = perfPct(gPerfAccDiagUs, windowUs);
  gPerfWebPct = perfPct(gPerfAccWebUs, windowUs);
  gPerfLoopMaxUs = gPerfAccLoopMaxUs;
  gPerfLoops = gPerfAccLoops;

  gPerfWindowStartUs = nowUs;
  gPerfLastSnapMs = nowMs;
  gPerfAccBusyUs = 0;
  gPerfAccCoreUs = 0;
  gPerfAccH312Us = 0;
  gPerfAccSessUs = 0;
  gPerfAccBleUs = 0;
  gPerfAccDiagUs = 0;
  gPerfAccWebUs = 0;
  gPerfAccLoops = 0;
  gPerfAccLoopMaxUs = 0;
}

struct ExtractorCmdLogEntry {
  uint32_t seq;
  uint32_t ms;
  char line[160];
};

static const uint16_t kExtractorCmdLogCap = 96;
static ExtractorCmdLogEntry gExtractorCmdLog[kExtractorCmdLogCap];
static uint16_t gExtractorCmdLogHead = 0;   // oldest entry index
static uint16_t gExtractorCmdLogCount = 0;  // valid entries
static uint32_t gExtractorCmdLogSeq = 0;    // global monotonically increasing sequence

static void extractorCmdHex(const uint8_t *data, size_t len, char *out, size_t outSize) {
  if (!out || outSize == 0) return;
  out[0] = '\0';
  if (!data || len == 0) return;
  size_t pos = 0;
  for (size_t i = 0; i < len; i++) {
    int w = snprintf(out + pos, (pos < outSize) ? (outSize - pos) : 0, (i == 0) ? "%02X" : " %02X", (unsigned)data[i]);
    if (w <= 0) break;
    if ((size_t)w >= (outSize - pos)) {
      pos = outSize - 1;
      break;
    }
    pos += (size_t)w;
  }
  out[pos] = '\0';
}

static void extractorCmdLogPush(const char *line) {
  if (!line) return;
  if (!kUiLogVerbose) {
    if (strncmp(line, "[perf]", 6) == 0) return;
    if (strncmp(line, "[wifi] tx auto ", 15) == 0) return;
    if (strstr(line, "setspeed begin ") != nullptr) return;
    if (strstr(line, "setspeed ok ") != nullptr) return;
    if (strstr(line, "setlight begin ") != nullptr) return;
    if (strstr(line, "setlight ok ") != nullptr) return;
    if (strstr(line, "setbri begin ") != nullptr) return;
    if (strstr(line, "setbri ok ") != nullptr) return;
    if (strstr(line, "prime echo ") != nullptr) return;
    if (strstr(line, "tx resp len=") != nullptr) return;
    if (strstr(line, "tx noresp len=") != nullptr) return;
    if (strstr(line, "tx fallback len=") != nullptr) return;
  }
  uint16_t idx = 0;
  if (gExtractorCmdLogCount < kExtractorCmdLogCap) {
    idx = (uint16_t)((gExtractorCmdLogHead + gExtractorCmdLogCount) % kExtractorCmdLogCap);
    gExtractorCmdLogCount++;
  } else {
    idx = gExtractorCmdLogHead;
    gExtractorCmdLogHead = (uint16_t)((gExtractorCmdLogHead + 1) % kExtractorCmdLogCap);
  }

  ExtractorCmdLogEntry &e = gExtractorCmdLog[idx];
  e.seq = ++gExtractorCmdLogSeq;
  e.ms = (uint32_t)millis();
  snprintf(e.line, sizeof(e.line), "%s", line);
}

static void extractorCmdLogPushf(const char *fmt, ...) {
  if (!fmt) return;
  char msg[160];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(msg, sizeof(msg), fmt, ap);
  va_end(ap);
  extractorCmdLogPush(msg);
}

static inline uint8_t perfDelta(uint8_t a, uint8_t b) { return (a >= b) ? (a - b) : (b - a); }

static void uiPerfMaybeLog(uint32_t nowMs) {
  if (gUiPerfLastMs != 0 && (nowMs - gUiPerfLastMs) < kUiPerfLogTickMs) return;
  gUiPerfLastMs = nowMs;
  bool changed = !gUiPerfPrevValid;
  if (!changed) {
    changed = (perfDelta(gPerfCpuPct, gUiPerfPrevCpuPct) >= kUiPerfDeltaPct) ||
              (perfDelta(gPerfCorePct, gUiPerfPrevCorePct) >= kUiPerfDeltaPct) ||
              (perfDelta(gPerfH312Pct, gUiPerfPrevH312Pct) >= kUiPerfDeltaPct) ||
              (perfDelta(gPerfSessPct, gUiPerfPrevSessPct) >= kUiPerfDeltaPct) ||
              (perfDelta(gPerfBlePct, gUiPerfPrevBlePct) >= kUiPerfDeltaPct) ||
              (perfDelta(gPerfWebPct, gUiPerfPrevWebPct) >= kUiPerfDeltaPct);
  }
  if (!changed) return;

  gUiPerfPrevValid = true;
  gUiPerfPrevCpuPct = gPerfCpuPct;
  gUiPerfPrevCorePct = gPerfCorePct;
  gUiPerfPrevH312Pct = gPerfH312Pct;
  gUiPerfPrevSessPct = gPerfSessPct;
  gUiPerfPrevBlePct = gPerfBlePct;
  gUiPerfPrevWebPct = gPerfWebPct;
  extractorCmdLogPushf("[perf] cpu=%u core=%u h312=%u sess=%u ble=%u web=%u lmax=%luus loops=%lu",
                       (unsigned int)gPerfCpuPct, (unsigned int)gPerfCorePct, (unsigned int)gPerfH312Pct,
                       (unsigned int)gPerfSessPct, (unsigned int)gPerfBlePct, (unsigned int)gPerfWebPct,
                       (unsigned long)gPerfLoopMaxUs, (unsigned long)gPerfLoops);
}

static inline int8_t clampWifiTxQdbm(int8_t v) {
  if (v < kWifiTxQdbmMin) return kWifiTxQdbmMin;
  if (v > kWifiTxQdbmMax) return kWifiTxQdbmMax;
  return v;
}

static void wifiTxAutoTuneLoop(uint32_t nowMs) {
  if (!kWifiTxAutoTuneEnabled) return;
  if (gWifiTxLastTuneMs != 0 && (nowMs - gWifiTxLastTuneMs) < kWifiTxTunePeriodMs) return;
  gWifiTxLastTuneMs = nowMs;

  if (WiFi.status() != WL_CONNECTED) return;
  wifi_mode_t m = WiFi.getMode();
  if (m != WIFI_STA) {
    gWifiTxNeedUpCycles = 0;
    gWifiTxNeedDownCycles = 0;
    return;
  }

  int32_t rssi = WiFi.RSSI();
  int8_t dir = 0;
  if (rssi <= kWifiTxRssiLowTune) dir = 1;      // weak signal: increase TX
  else if (rssi >= kWifiTxRssiHighTune) dir = -1; // too strong: decrease TX
  if (dir == 0) {
    gWifiTxNeedUpCycles = 0;
    gWifiTxNeedDownCycles = 0;
    return;
  }

  if (dir > 0) {
    if (gWifiTxNeedUpCycles < 250) gWifiTxNeedUpCycles++;
    gWifiTxNeedDownCycles = 0;
    if (gWifiTxNeedUpCycles < kWifiTxConfirmCycles) return;
  } else {
    if (gWifiTxNeedDownCycles < 250) gWifiTxNeedDownCycles++;
    gWifiTxNeedUpCycles = 0;
    if (gWifiTxNeedDownCycles < kWifiTxConfirmCycles) return;
  }

  if (gWifiTxLastStepMs != 0 && (nowMs - gWifiTxLastStepMs) < kWifiTxStepMinGapMs) return;
  if (gWifiTxStepWindowStartMs == 0 || (nowMs - gWifiTxStepWindowStartMs) > kWifiTxStepWindowMs) {
    gWifiTxStepWindowStartMs = nowMs;
    gWifiTxStepWindowCount = 0;
  }
  if (gWifiTxStepWindowCount >= kWifiTxStepWindowMax) return;

  int8_t curQdbm = 0;
  if (esp_wifi_get_max_tx_power(&curQdbm) != ESP_OK) return;
  curQdbm = clampWifiTxQdbm(curQdbm);

  int8_t nextQdbm = clampWifiTxQdbm((int8_t)(curQdbm + (dir * kWifiTxQdbmStep)));
  if (nextQdbm == curQdbm) return;
  if (esp_wifi_set_max_tx_power(nextQdbm) != ESP_OK) return;

  gWifiTxLastQdbm = nextQdbm;
  gWifiTxLastStepMs = nowMs;
  if (gWifiTxStepWindowCount < 250) gWifiTxStepWindowCount++;
  gWifiTxNeedUpCycles = 0;
  gWifiTxNeedDownCycles = 0;
  extractorCmdLogPushf("[wifi] tx auto rssi=%ld qdbm=%d->%d mode=sta", (long)rssi, (int)curQdbm, (int)nextQdbm);
}

static uint8_t h312NormSlot(uint8_t slot) {
  if (slot < 1 || slot > 3) return 1;
  return slot;
}

static const char *h312ModeText(H312TransportMode mode) {
  return (mode == H312_MODE_BLE) ? "ble" : "wifi";
}

static H312TransportMode h312ModeFromText(const String &raw) {
  String s = raw;
  s.trim();
  s.toLowerCase();
  if (s == "ble" || s == "bluetooth") return H312_MODE_BLE;
  return H312_MODE_WIFI;
}

static bool h312BleAddrLooksValid(const String &addr) {
  String s = addr;
  s.trim();
  if (s.length() != 17) return false;
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if ((i % 3) == 2) {
      if (c != ':') return false;
    } else {
      bool hex = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
      if (!hex) return false;
    }
  }
  return true;
}

static bool h312TargetConfigured() {
  if (!h312Enabled) return false;
  if (h312Mode == H312_MODE_BLE) {
    return h312BleAddrLooksValid(h312BleAddr) || !h312BleName.isEmpty();
  }
  return !h312Ip.isEmpty();
}

static void h312BleLogEvent(const char *eventName, uint8_t slot, int tempC = -1000, const char *note = nullptr) {
  const uint8_t s = h312NormSlot(slot);
  if (eventName && strcmp(eventName, "H312_BLE_RECONNECT") == 0) {
    uint32_t nowMs = (uint32_t)millis();
    if (note && strcmp(note, "attempt") == 0) {
      if (gH312BleLastReconnectAttemptUiLogMs != 0 &&
          (uint32_t)(nowMs - gH312BleLastReconnectAttemptUiLogMs) < kH312BleReconnectLogMinMs) {
        return;
      }
      gH312BleLastReconnectAttemptUiLogMs = nowMs;
    } else if (note && strcmp(note, "link_lost") == 0) {
      if (gH312BleLastReconnectLinkUiLogMs != 0 &&
          (uint32_t)(nowMs - gH312BleLastReconnectLinkUiLogMs) < kH312BleReconnectLogMinMs) {
        return;
      }
      gH312BleLastReconnectLinkUiLogMs = nowMs;
    }
  }
  if (tempC > -100) {
    if (note && note[0]) extractorCmdLogPushf("[h312] %s slot=%u temp=%d note=%s", eventName, (unsigned int)s, tempC, note);
    else extractorCmdLogPushf("[h312] %s slot=%u temp=%d", eventName, (unsigned int)s, tempC);
    return;
  }
  if (note && note[0]) extractorCmdLogPushf("[h312] %s slot=%u note=%s", eventName, (unsigned int)s, note);
  else extractorCmdLogPushf("[h312] %s slot=%u", eventName, (unsigned int)s);
}

static void h312BleBb02NotifyCb(NimBLERemoteCharacteristic *chr, uint8_t *data, size_t len, bool isNotify) {
  (void)chr;
  (void)isNotify;
  if (!data || len == 0) return;
  portENTER_CRITICAL(&gH312BleNotifyMux);
  if (gH312BleNotifyBuf.size() > 16384) gH312BleNotifyBuf.erase(0, gH312BleNotifyBuf.size() - 4096);
  gH312BleNotifyBuf.append((const char *)data, len);
  portEXIT_CRITICAL(&gH312BleNotifyMux);
}

static bool h312BleEnableNotify(NimBLERemoteCharacteristic *chr) {
  if (!chr || !chr->canNotify()) return false;
  if (gH312BleNotifySubscribed) return true;
  if (!chr->subscribe(true, h312BleBb02NotifyCb, true)) return false;
  gH312BleNotifySubscribed = true;
  return true;
}

static std::string h312BleTakeNotifyBuffer() {
  std::string out;
  portENTER_CRITICAL(&gH312BleNotifyMux);
  out.swap(gH312BleNotifyBuf);
  gH312BleNotifyBuf.clear();
  portEXIT_CRITICAL(&gH312BleNotifyMux);
  return out;
}

static void h312BleDropClientHard() {
  if (gH312BleClient) {
    if (gH312BleNotifySubscribed && gH312BleCharBb02 && gH312BleCharBb02->canNotify()) {
      (void)gH312BleCharBb02->unsubscribe(true);
    }
    if (gH312BleClient->isConnected()) gH312BleClient->disconnect();
    NimBLEDevice::deleteClient(gH312BleClient);
  }
  gH312BleClient = nullptr;
  gH312BleNotifySubscribed = false;
  gH312BleLinkWasUp = false;
  gH312BleConnectPending = false;
}

static void h312BleReleaseClient() {
  if (gH312BleClient && gH312BleClient->isConnected()) {
    if (gH312BleNotifySubscribed && gH312BleCharBb02 && gH312BleCharBb02->canNotify()) {
      (void)gH312BleCharBb02->unsubscribe(true);
    }
    gH312BleClient->disconnect();
  }
  gH312BleCharBb02 = nullptr;
  gH312BleCharFf01 = nullptr;
  gH312BleCharKind = 2;
  gH312BleRuntimeIp = "";
  gH312BleLastIpReadMs = 0;
  gH312BleJsonWaitStreak = 0;
  gH312BleJsonBuf.clear();
  portENTER_CRITICAL(&gH312BleNotifyMux);
  gH312BleNotifyBuf.clear();
  portEXIT_CRITICAL(&gH312BleNotifyMux);
  gH312BleNotifySubscribed = false;
  gH312BleLinkWasUp = false;
  gH312BleConnectPending = false;
}

static void h312BleDisconnectKeepSeed() {
  if (gH312BleClient && gH312BleClient->isConnected()) {
    if (gH312BleNotifySubscribed && gH312BleCharBb02 && gH312BleCharBb02->canNotify()) {
      (void)gH312BleCharBb02->unsubscribe(true);
    }
    gH312BleClient->disconnect();
  }
  gH312BleCharBb02 = nullptr;
  gH312BleCharFf01 = nullptr;
  gH312BleCharKind = 2;
  gH312BleJsonWaitStreak = 0;
  gH312BleJsonBuf.clear();
  portENTER_CRITICAL(&gH312BleNotifyMux);
  gH312BleNotifyBuf.clear();
  portEXIT_CRITICAL(&gH312BleNotifyMux);
  gH312BleNotifySubscribed = false;
  gH312BleLinkWasUp = false;
  gH312BleConnectPending = false;
}

static const char *h312BleAddrTypeText(int type) {
  switch (type) {
    case BLE_ADDR_PUBLIC:
      return "public";
    case BLE_ADDR_RANDOM:
      return "random";
    case BLE_ADDR_PUBLIC_ID:
      return "public_id";
    case BLE_ADDR_RANDOM_ID:
      return "random_id";
    default:
      return "unknown";
  }
}

static bool h312BleConnectWithTypeFallback(NimBLEClient *client, const String &addrText, int8_t preferredType, uint8_t *outUsedType) {
  if (!client || !h312BleAddrLooksValid(addrText)) return false;
  bool tried[4] = {false, false, false, false};
  uint8_t order[4];
  uint8_t count = 0;
  if (preferredType >= 0 && preferredType <= 3) {
    order[count++] = (uint8_t)preferredType;
    tried[(uint8_t)preferredType] = true;
  } else {
    // Unknown addr type is common with some H312 adverts.
    // Keep attempts short to avoid long blocking connect loops.
    order[count++] = BLE_ADDR_RANDOM;
    tried[BLE_ADDR_RANDOM] = true;
    order[count++] = BLE_ADDR_PUBLIC;
    tried[BLE_ADDR_PUBLIC] = true;
  }
  const uint8_t defaults[4] = {BLE_ADDR_PUBLIC, BLE_ADDR_RANDOM, BLE_ADDR_PUBLIC_ID, BLE_ADDR_RANDOM_ID};
  for (uint8_t i = 0; i < 4; i++) {
    uint8_t t = defaults[i];
    if (tried[t]) continue;
    order[count++] = t;
    tried[t] = true;
  }
  const uint8_t hardMaxAttempts = 4;
  uint8_t attempts = 0;
  for (uint8_t i = 0; i < count; i++) {
    if (attempts >= hardMaxAttempts) break;
    const uint8_t t = order[i];
    NimBLEAddress addr(addrText.c_str(), t);
    if (client->connect(addr, false)) {
      if (outUsedType) *outUsedType = t;
      return true;
    }
    attempts++;
    if (client->isConnected()) client->disconnect();
    delay(1);
  }
  return false;
}

static NimBLEClient *h312BleAcquireClient() {
  if (gH312BleClient) return gH312BleClient;
  NimBLEClient *c = NimBLEDevice::getDisconnectedClient();
  if (!c) c = NimBLEDevice::createClient();
  if (!c) return nullptr;
  c->setConnectTimeout(kH312BleConnectTimeoutSec);
  gH312BleClient = c;
  return gH312BleClient;
}

static bool h312BleLooksLikeH312Name(const String &name) {
  String up = name;
  up.toUpperCase();
  return (up.indexOf("H312") >= 0 || up.indexOf("HOT") >= 0 || up.indexOf("AIR") >= 0);
}

static bool h312BleScanForTarget(uint32_t scanMs, String *outAddr, String *outName, int *outRssi, int8_t *outAddrType) {
  if (!outAddr) return false;
  h312BleLogEvent("H312_BLE_SCAN", h312Slot);
  NimBLEScan *scan = NimBLEDevice::getScan();
  if (!scan) return false;
  scan->setActiveScan(true);
  scan->setInterval(45);
  scan->setWindow(15);
  scan->setDuplicateFilter(true);
  scan->setMaxResults(40);

  uint32_t durSec = (scanMs + 999) / 1000;
  if (durSec < 1) durSec = 1;
  if (durSec > 12) durSec = 12;

  NimBLEScanResults res = scan->start(durSec, false);
  int bestRssi = -999;
  String bestAddr;
  String bestName;
  int8_t bestAddrType = -1;

  for (int i = 0; i < res.getCount(); i++) {
    NimBLEAdvertisedDevice dev = res.getDevice((uint32_t)i);
    String addr = String(dev.getAddress().toString().c_str());
    const bool addrPinnedMatch = (h312BleAddrLooksValid(h312BleAddr) && addr.equalsIgnoreCase(h312BleAddr));

    std::string rawName = dev.getName();
    String name(rawName.c_str());
    if (!addrPinnedMatch) {
      if (rawName.empty()) continue;
      if (!h312BleLooksLikeH312Name(name)) continue;
      if (!h312BleName.isEmpty() && !name.equalsIgnoreCase(h312BleName)) continue;
    }

    int rssi = dev.getRSSI();
    if (bestAddr.isEmpty() || rssi > bestRssi) {
      bestRssi = rssi;
      bestAddr = addr;
      bestName = name;
      bestAddrType = (int8_t)dev.getAddressType();
    }
  }
  scan->clearResults();
  if (bestAddr.isEmpty()) return false;
  *outAddr = bestAddr;
  if (outName) *outName = bestName;
  if (outRssi) *outRssi = bestRssi;
  if (outAddrType) *outAddrType = bestAddrType;
  return true;
}

static bool h312BleResolveTarget(uint32_t scanMs, String *outAddr, String *outName, int *outRssi, int8_t *outAddrType) {
  if (!outAddr) return false;
  const bool hasCfgAddr = h312BleAddrLooksValid(h312BleAddr);
  const bool runtimeReady = !gH312BleRuntimeAddr.isEmpty() && h312BleAddrLooksValid(gH312BleRuntimeAddr) &&
                            (gH312BleRuntimeAddrType >= 0 || !hasCfgAddr);
  if (runtimeReady) {
    *outAddr = gH312BleRuntimeAddr;
    if (outName) *outName = h312BleName;
    if (outRssi) *outRssi = -999;
    if (outAddrType) *outAddrType = gH312BleRuntimeAddrType;
    return true;
  }

  bool preferScan = false;
  if (hasCfgAddr && gH312BleRuntimeAddrType < 0) {
    // Address type is unknown for a pinned device. Force one scan pass to learn
    // actual addrType from advertisement and avoid repeated connect-fail loops.
    preferScan = true;
  }
  if (hasCfgAddr && !h312BleName.isEmpty() && !gH312BleLastFailedAddr.isEmpty() &&
      h312BleAddr.equalsIgnoreCase(gH312BleLastFailedAddr)) {
    const uint32_t now = (uint32_t)millis();
    if (now >= gH312BleLastFailedMs && (now - gH312BleLastFailedMs) <= kH312BleFailedAddrTtlMs) {
      preferScan = true;
    }
  }

  if (hasCfgAddr && !preferScan) {
    if (!gH312BleRuntimeAddr.equalsIgnoreCase(h312BleAddr)) gH312BleRuntimeAddrType = -1;
    gH312BleRuntimeAddr = h312BleAddr;
    *outAddr = gH312BleRuntimeAddr;
    if (outName) *outName = h312BleName;
    if (outRssi) *outRssi = -999;
    if (outAddrType) *outAddrType = (gH312BleRuntimeAddrType >= 0) ? gH312BleRuntimeAddrType : -1;
    return true;
  }
  String scannedAddr;
  String scannedName;
  int scannedRssi = -999;
  int8_t scannedAddrType = -1;
  if (!h312BleScanForTarget(scanMs, &scannedAddr, &scannedName, &scannedRssi, &scannedAddrType)) {
    // If scan is required to learn address type for pinned MAC, do not fall back
    // to unknown type connect attempts: keep runtime empty and retry later.
    if (hasCfgAddr && preferScan) {
      gH312BleRuntimeAddr = "";
      gH312BleRuntimeAddrType = -1;
      return false;
    }
    if (hasCfgAddr) {
      if (!gH312BleRuntimeAddr.equalsIgnoreCase(h312BleAddr)) gH312BleRuntimeAddrType = -1;
      gH312BleRuntimeAddr = h312BleAddr;
      *outAddr = gH312BleRuntimeAddr;
      if (outName) *outName = h312BleName;
      if (outRssi) *outRssi = -999;
      if (outAddrType) *outAddrType = (gH312BleRuntimeAddrType >= 0) ? gH312BleRuntimeAddrType : -1;
      return true;
    }
    return false;
  }
  gH312BleRuntimeAddr = scannedAddr;
  gH312BleRuntimeAddrType = scannedAddrType;
  *outAddr = scannedAddr;
  if (outName) *outName = scannedName;
  if (outRssi) *outRssi = scannedRssi;
  if (outAddrType) *outAddrType = scannedAddrType;
  if (hasCfgAddr && !scannedAddr.equalsIgnoreCase(h312BleAddr)) {
    extractorCmdLogPushf("[h312] ble runtime addr=%s (cfg=%s name=%s)", scannedAddr.c_str(), h312BleAddr.c_str(), h312BleName.c_str());
  }
  return true;
}

static String h312BleUuidUpper(const std::string &in) {
  String s(in.c_str());
  s.toUpperCase();
  return s;
}

static bool h312BleUuidLooksLike(const std::string &uuid, const char *needleHex4) {
  if (!needleHex4 || !needleHex4[0]) return false;
  String u = h312BleUuidUpper(uuid);
  String n = String(needleHex4);
  n.toUpperCase();
  return (u.indexOf(n) >= 0);
}

static NimBLERemoteCharacteristic *h312BleFindCharacteristicAnywhere(NimBLEClient *client, const char *shortHex4,
                                                                     String *outSvcUuid, String *outCharUuid,
                                                                     bool requireReadable) {
  if (!client || !shortHex4) return nullptr;
  std::vector<NimBLERemoteService*> *svcs = client->getServices(true);
  if (!svcs) return nullptr;
  for (NimBLERemoteService *svc : *svcs) {
    if (!svc) continue;
    std::vector<NimBLERemoteCharacteristic*> *chars = svc->getCharacteristics(true);
    if (!chars) continue;
    for (NimBLERemoteCharacteristic *ch : *chars) {
      if (!ch) continue;
      std::string cu = ch->getUUID().toString();
      if (!h312BleUuidLooksLike(cu, shortHex4)) continue;
      if (requireReadable && !ch->canRead()) continue;
      if (outSvcUuid) *outSvcUuid = String(svc->getUUID().toString().c_str());
      if (outCharUuid) *outCharUuid = String(cu.c_str());
      return ch;
    }
  }
  return nullptr;
}

static void h312BleLogGattMap(NimBLEClient *client, uint8_t slot) {
  if (!kH312BleDiagLogs || !client) return;
  std::vector<NimBLERemoteService*> *svcs = client->getServices(true);
  if (!svcs) {
    extractorCmdLogPushf("[h312] ble gatt slot=%u: no services", (unsigned int)slot);
    return;
  }
  extractorCmdLogPushf("[h312] ble gatt slot=%u services=%u", (unsigned int)slot, (unsigned int)svcs->size());
  for (NimBLERemoteService *svc : *svcs) {
    if (!svc) continue;
    String su = String(svc->getUUID().toString().c_str());
    std::vector<NimBLERemoteCharacteristic*> *chars = svc->getCharacteristics(true);
    const unsigned int cc = chars ? (unsigned int)chars->size() : 0U;
    extractorCmdLogPushf("[h312] ble svc=%s chars=%u", su.c_str(), cc);
    if (!chars) continue;
    for (NimBLERemoteCharacteristic *ch : *chars) {
      if (!ch) continue;
      String cu = String(ch->getUUID().toString().c_str());
      extractorCmdLogPushf("[h312] ble   chr=%s rd=%u ntf=%u ind=%u", cu.c_str(),
                           ch->canRead() ? 1U : 0U, ch->canNotify() ? 1U : 0U, ch->canIndicate() ? 1U : 0U);
    }
  }
}

static String h312BleHexPreview(const std::string &raw, size_t maxBytes) {
  const size_t n = (raw.size() < maxBytes) ? raw.size() : maxBytes;
  String s;
  s.reserve((n * 3) + 8);
  for (size_t i = 0; i < n; i++) {
    if (i) s += ' ';
    char b[3];
    snprintf(b, sizeof(b), "%02X", (uint8_t)raw[i]);
    s += b;
  }
  if (raw.size() > n) s += " ...";
  return s;
}

static String h312BleAsciiPreview(const std::string &raw, size_t maxBytes) {
  const size_t n = (raw.size() < maxBytes) ? raw.size() : maxBytes;
  String s;
  s.reserve(n + 8);
  for (size_t i = 0; i < n; i++) {
    const char c = raw[i];
    s += (isprint((unsigned char)c) ? c : '.');
  }
  if (raw.size() > n) s += "...";
  return s;
}

static bool h312BleHasKey(const std::string &raw, const char *key) {
  if (!key) return false;
  std::string pat = "\"";
  pat += key;
  pat += "\"";
  return raw.find(pat) != std::string::npos;
}

static bool h312BleExtractJsonObject(const std::string &raw, std::string *outJson) {
  if (!outJson) return false;
  const size_t start = raw.find('{');
  if (start == std::string::npos) return false;

  bool inStr = false;
  bool esc = false;
  int depth = 0;
  size_t end = std::string::npos;

  for (size_t i = start; i < raw.size(); i++) {
    const char c = raw[i];
    if (inStr) {
      if (esc) {
        esc = false;
      } else if (c == '\\') {
        esc = true;
      } else if (c == '"') {
        inStr = false;
      }
      continue;
    }

    if (c == '"') {
      inStr = true;
      continue;
    }
    if (c == '{') {
      depth++;
      continue;
    }
    if (c == '}') {
      depth--;
      if (depth == 0) {
        end = i;
        break;
      }
      if (depth < 0) return false;
    }
  }

  if (end == std::string::npos || end <= start) return false;
  *outJson = raw.substr(start, (end - start) + 1);
  return true;
}

static bool h312BleTakeLatestJsonFromStream(const std::string &chunk, std::string *ioBuf, std::string *outJson) {
  if (!ioBuf || !outJson) return false;
  if (!chunk.empty()) ioBuf->append(chunk);
  if (ioBuf->size() > 16384) {
    ioBuf->erase(0, ioBuf->size() - 4096);
  }

  std::string latest;
  while (true) {
    size_t start = ioBuf->find('{');
    if (start == std::string::npos) {
      // Keep a short tail only.
      if (ioBuf->size() > 2048) ioBuf->erase(0, ioBuf->size() - 2048);
      break;
    }
    if (start > 0) ioBuf->erase(0, start);

    bool inStr = false;
    bool esc = false;
    int depth = 0;
    size_t end = std::string::npos;
    for (size_t i = 0; i < ioBuf->size(); i++) {
      const char c = (*ioBuf)[i];
      if (inStr) {
        if (esc) {
          esc = false;
        } else if (c == '\\') {
          esc = true;
        } else if (c == '"') {
          inStr = false;
        }
        continue;
      }
      if (c == '"') {
        inStr = true;
        continue;
      }
      if (c == '{') {
        depth++;
        continue;
      }
      if (c == '}') {
        depth--;
        if (depth == 0) {
          end = i;
          break;
        }
      }
    }

    if (end == std::string::npos) {
      // Wait for more data to complete this object.
      if (ioBuf->size() > 8192) ioBuf->erase(0, ioBuf->size() - 2048);
      break;
    }

    const std::string obj = ioBuf->substr(0, end + 1);
    ioBuf->erase(0, end + 1);
    latest = obj;
  }

  if (latest.empty()) return false;
  *outJson = latest;
  return true;
}

static bool h312BleLooksLikeBb02AckFrame(const std::string &raw) {
  if (raw.size() != 9) return false;
  const uint8_t *p = (const uint8_t *)raw.data();
  // Known ACK-only variants observed on H312:
  // 1) 01 00 09 00 00 00 00 55 xx
  // 2) C2 AA 09 00 00 00 00 01 xx
  const bool v1 = (p[0] == 0x01 && p[1] == 0x00 && p[2] == 0x09 && p[3] == 0x00 && p[7] == 0x55);
  const bool v2 = (p[0] == 0xC2 && p[1] == 0xAA && p[2] == 0x09 && p[3] == 0x00);
  return (v1 || v2);
}

static bool h312BleExtractIpFromFf01(const std::string &raw, String *outIp) {
  if (!outIp || raw.size() < 4) return false;
  size_t i = 0;
  while ((i + 2) <= raw.size()) {
    const uint8_t tag = (uint8_t)raw[i];
    const uint8_t len = (uint8_t)raw[i + 1];
    i += 2;
    if ((i + len) > raw.size()) break;
    if (tag == 0x54 && len >= 7 && len <= 18) {
      String ip;
      ip.reserve((unsigned int)len + 2U);
      for (uint8_t j = 0; j < len; j++) {
        const char c = (char)raw[i + j];
        if ((c >= '0' && c <= '9') || c == '.') ip += c;
      }
      ip.trim();
      IPAddress p;
      if (p.fromString(ip)) {
        *outIp = ip;
        return true;
      }
    }
    i += len;
  }
  return false;
}

static void h312BleMaybeRefreshRuntimeIp(uint32_t nowMs) {
  if (!gH312BleCharFf01) return;
  if (!gH312BleRuntimeIp.isEmpty() && gH312BleLastIpReadMs > 0 && nowMs >= gH312BleLastIpReadMs &&
      (nowMs - gH312BleLastIpReadMs) < kH312BleIpRefreshMs) {
    return;
  }
  std::string raw = gH312BleCharFf01->readValue();
  gH312BleLastIpReadMs = nowMs;
  if (raw.empty()) return;
  String ip;
  if (!h312BleExtractIpFromFf01(raw, &ip)) return;
  if (ip == gH312BleRuntimeIp) return;
  gH312BleRuntimeIp = ip;
  if (kH312BleDiagLogs) {
    extractorCmdLogPushf("[h312] ble ff01 ip=%s", gH312BleRuntimeIp.c_str());
  }
}

static char h312BleLowerAscii(char c) {
  if (c >= 'A' && c <= 'Z') return (char)(c + ('a' - 'A'));
  return c;
}

static bool h312BleIsNumTokChar(char c) {
  return (c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.';
}

static size_t h312BleFindQuotedKeyPosCI(const std::string &raw, const char *key) {
  if (!key) return std::string::npos;
  const size_t keyLen = strlen(key);
  if (keyLen == 0 || raw.size() < (keyLen + 2)) return std::string::npos;

  const size_t need = keyLen + 2; // "key"
  size_t pos = raw.size();
  while (pos >= need) {
    const size_t i = pos - need;
    if (raw[i] == '"' && raw[i + need - 1] == '"') {
      bool match = true;
      for (size_t k = 0; k < keyLen; k++) {
        if (h312BleLowerAscii(raw[i + 1 + k]) != h312BleLowerAscii(key[k])) {
          match = false;
          break;
        }
      }
      if (match) return i;
    }
    if (i == 0) break;
    pos--;
  }
  return std::string::npos;
}

static bool h312BleExtractIntField(const std::string &raw, const char *key, int *outValue) {
  if (!outValue || !key) return false;
  size_t keyPos = h312BleFindQuotedKeyPosCI(raw, key);
  if (keyPos == std::string::npos) return false;
  size_t pos = raw.find(':', keyPos + strlen(key) + 2);
  if (pos == std::string::npos) return false;
  pos++;
  while (pos < raw.size() && (raw[pos] == ' ' || raw[pos] == '\t' || raw[pos] == '\r' || raw[pos] == '\n')) pos++;

  bool quoted = false;
  if (pos < raw.size() && (raw[pos] == '"' || raw[pos] == '\'')) {
    quoted = true;
    pos++;
    while (pos < raw.size() && (raw[pos] == ' ' || raw[pos] == '\t')) pos++;
  }

  size_t start = pos;
  while (pos < raw.size() && h312BleIsNumTokChar(raw[pos])) pos++;
  if (start == pos) return false;

  if (quoted) {
    size_t tail = pos;
    while (tail < raw.size() && (raw[tail] == ' ' || raw[tail] == '\t' || raw[tail] == '\r' || raw[tail] == '\n')) tail++;
    if (tail >= raw.size()) return false;
    const char q = raw[tail];
    // Reject values like "21:17:07" or "182sec": only pure numeric token is allowed.
    if (!(q == '"' || q == '\'')) return false;
  } else {
    size_t tail = pos;
    while (tail < raw.size() && (raw[tail] == ' ' || raw[tail] == '\t' || raw[tail] == '\r' || raw[tail] == '\n')) tail++;
    if (tail < raw.size()) {
      const char d = raw[tail];
      if (!(d == ',' || d == '}' || d == ']')) return false;
    }
  }

  const std::string tok = raw.substr(start, pos - start);
  const double dv = strtod(tok.c_str(), nullptr);
  if (!isfinite(dv)) return false;
  *outValue = (int)lround(dv);
  return true;
}

static bool h312BleExtractIntAnyField(const std::string &raw, const char *const *keys, size_t keyCount, int *outValue, int minValue,
                                      int maxValue) {
  if (!outValue || !keys || keyCount == 0) return false;
  for (size_t i = 0; i < keyCount; i++) {
    int v = 0;
    if (!h312BleExtractIntField(raw, keys[i], &v)) continue;
    if (minValue <= maxValue) {
      if (v < minValue || v > maxValue) continue;
    }
    *outValue = v;
    return true;
  }
  return false;
}

static bool h312BleParseTelemetry(const std::string &raw, int *outRt, int *outWs, int *outHf, int *outSetTemp, int *outRealFan,
                                  int *outSetFan) {
  if (!outRt) return false;
  // Keep BLE parser aligned with the prebuilt H312 Wi-Fi driver contract:
  // telemetry is JSON with canonical keys realTimeTemp/workStatus/heatFlag/setTemp.
  std::string json;
  if (!h312BleExtractJsonObject(raw, &json)) return false;

  int rt = -1000;
  if (!h312BleExtractIntField(json, "realTimeTemp", &rt)) return false;
  if (rt < -40 || rt > 700) return false;
  *outRt = rt;

  int v = -1;
  if (outWs) {
    *outWs = h312BleExtractIntField(json, "workStatus", &v) ? v : -1;
  }
  if (outHf) {
    *outHf = h312BleExtractIntField(json, "heatFlag", &v) ? v : -1;
  }
  if (outSetTemp) {
    *outSetTemp = h312BleExtractIntField(json, "setTemp", &v) ? v : -1;
  }
  if (outRealFan) {
    *outRealFan = h312BleExtractIntField(json, "realTimeFanSpeed", &v) ? v : -1;
  }
  if (outSetFan) {
    *outSetFan = h312BleExtractIntField(json, "setFanSpeed", &v) ? v : -1;
  }
  return true;
}

static bool h312BleParseBb01Approx(const std::string &raw, int *outRt) {
  if (!outRt) return false;
  if (raw.size() != 16) return false;
  const uint8_t *p = (const uint8_t *)raw.data();
  if (!(p[0] == 0x20 && p[1] == 0x00 && p[2] == 0x10)) return false;
  const float approx = (0.9177f * (float)p[7]) + 13.44f;
  *outRt = (int)lroundf(approx);
  return true;
}

static void h312BleMarkError(uint8_t slot, const char *logReason) {
  uint8_t s = h312NormSlot(slot);
  uint8_t idx = (uint8_t)(s - 1);
  if (!gH312UiErrActive[idx]) {
    extractorCmdLogPushf("[h312] slot=%u %s", (unsigned int)s, logReason ? logReason : "error");
    if (logReason && strcmp(logReason, "timeout") == 0) {
      h312BleLogEvent("H312_BLE_TIMEOUT", s);
    }
  }
  gH312UiErrActive[idx] = true;
}

static void h312BleMarkRecovered(uint8_t slot, int tempC) {
  uint8_t s = h312NormSlot(slot);
  uint8_t idx = (uint8_t)(s - 1);
  if (gH312UiErrActive[idx]) {
    extractorCmdLogPushf("[h312] slot=%u recovered temp=%.1fC", (unsigned int)s, (double)tempC);
  }
  gH312UiErrActive[idx] = false;
}

static void h312BleAcceptTelemetry(uint32_t nowMs, int rawLen, const char *src, int rt, int ws, int hf, int setTemp, int realFan, int setFan) {
  gH312BleFailStreak = 0;
  gH312BleLastConnectAttemptMs = 0;
  gH312SeenTelemetry = true;
  gH312LastJsonMs = nowMs;
  gH312RealTimeTemp = rt;
  gH312WorkStatus = ws;
  gH312HeatFlag = hf;
  gH312SetTemp = setTemp;
  gH312RealFan = realFan;
  gH312SetFan = setFan;
  gH312LastError = "";
  gH312BleJsonWaitStreak = 0;
  if (kH312BleDiagLogs) {
    const bool firstOk = (gH312BleDiagLastMs == 0);
    const bool period = (nowMs >= gH312BleDiagLastMs) && ((nowMs - gH312BleDiagLastMs) >= kH312BleDiagSampleMs);
    const bool delta = (gH312BleDiagLastTemp < -100) || (abs(rt - gH312BleDiagLastTemp) >= 10);
    if (firstOk || period || delta) {
      extractorCmdLogPushf("[h312] ble ok slot=%u temp=%d ws=%d hf=%d rfan=%d sfan=%d src=%s len=%u",
                           (unsigned int)h312NormSlot(h312Slot),
                           rt,
                           ws,
                           hf,
                           realFan,
                           setFan,
                           (src && src[0]) ? src : "na",
                           (unsigned int)((rawLen < 0) ? 0 : rawLen));
      gH312BleDiagLastMs = nowMs;
      gH312BleDiagLastTemp = rt;
    }
  }
  h312BleMarkRecovered(h312Slot, rt);
  uint8_t idx = (uint8_t)(h312NormSlot(h312Slot) - 1);
  bool tempChanged = (gH312UiLastTempLogC[idx] <= -200) || (abs(rt - gH312UiLastTempLogC[idx]) >= 1);
  bool gapOk = (gH312UiLastTempLogMs[idx] == 0) || ((nowMs - gH312UiLastTempLogMs[idx]) >= kH312TempLogMinGapMs);
  if (tempChanged || gapOk) {
    h312BleLogEvent("H312_TEMP_UPDATE", h312Slot, rt);
    gH312UiLastTempLogMs[idx] = nowMs;
    gH312UiLastTempLogC[idx] = (int16_t)rt;
  }
}

static void h312BleTick(uint32_t nowMs) {
  if (nowMs < gH312BleNextPollMs) return;
  gH312BleNextPollMs = nowMs + kH312BlePollMs;

  int rt = -1000;
  int ws = -1;
  int hf = -1;
  int setTemp = -1;
  int realFan = -1;
  int setFan = -1;

  const bool h312Connected = (gH312BleClient && gH312BleClient->isConnected() && (gH312BleCharBb02 || gH312BleCharFf01));
  if (gH312BleLinkWasUp && !h312Connected) {
    if (gH312BleLastReconnectLogMs == 0 || (nowMs - gH312BleLastReconnectLogMs) >= kH312BleReconnectLogMinMs) {
      h312BleLogEvent("H312_BLE_RECONNECT", h312Slot, -1000, "link_lost");
      gH312BleLastReconnectLogMs = nowMs;
    }
    gH312BleLinkWasUp = false;
  }
  const bool recentHttp = (gWebReqLastMs > 0 && nowMs >= gWebReqLastMs && (nowMs - gWebReqLastMs) < kH312BleYieldAfterHttpMs);
  const bool tes02RecentCmd = (gBleLastSetMs > 0 && nowMs >= gBleLastSetMs &&
                               (nowMs - gBleLastSetMs) < kH312BleYieldAfterTes02Ms);
  const bool tes02SessionBusy = (gExtractorSessionActive && gTes02Driver.ready());
  if (recentHttp || tes02SessionBusy || tes02RecentCmd) {
    if (kH312BleDiagLogs && (gH312BleSkipDiagMs == 0 || (nowMs - gH312BleSkipDiagMs) >= 5000U)) {
      extractorCmdLogPushf("[h312] skip http=%u sess=%u tes02=%u dt_set=%lu",
                           recentHttp ? 1U : 0U,
                           tes02SessionBusy ? 1U : 0U,
                           tes02RecentCmd ? 1U : 0U,
                           (unsigned long)((gBleLastSetMs > 0 && nowMs >= gBleLastSetMs) ? (nowMs - gBleLastSetMs) : 0U));
      gH312BleSkipDiagMs = nowMs;
    }
    gH312BleNextPollMs = nowMs + kH312BlePollMs;
    return;
  }

  if (!h312TargetConfigured()) {
    h312BleReleaseClient();
    gH312RealTimeTemp = -1000;
    gH312LastError = "h312 ble not configured";
    return;
  }

  const bool needBleSeed = gH312BleRuntimeIp.isEmpty();
  if (!needBleSeed && gH312BleClient && gH312BleClient->isConnected()) {
    // Mirror python seed flow: BLE is used to discover ff01 IP, then released.
    h312BleDisconnectKeepSeed();
  }

  if (needBleSeed && (!gH312BleClient || !gH312BleClient->isConnected() || (!gH312BleCharBb02 && !gH312BleCharFf01))) {
    if (gH312SeenTelemetry || gH312LastJsonMs > 0) {
      if (gH312BleLastReconnectLogMs == 0 || (nowMs - gH312BleLastReconnectLogMs) >= kH312BleReconnectLogMinMs) {
        h312BleLogEvent("H312_BLE_RECONNECT", h312Slot, -1000, "attempt");
        gH312BleLastReconnectLogMs = nowMs;
      }
    }
    if (gH312BleLastConnectAttemptMs != 0 && (nowMs - gH312BleLastConnectAttemptMs) < kH312BleReconnectAttemptMinMs) {
      gH312BleNextPollMs = gH312BleLastConnectAttemptMs + kH312BleReconnectAttemptMinMs;
      return;
    }
    gH312BleLastConnectAttemptMs = nowMs;
    if (gH312BleClient || gH312BleCharBb02 || gH312BleCharFf01) {
      h312BleReleaseClient();
    }
    String targetAddr;
    int8_t targetAddrType = -1;
    if (h312BleAddrLooksValid(h312BleAddr)) {
      targetAddr = h312BleAddr;
      targetAddrType = gH312BleRuntimeAddrType;
    } else {
      if (!h312BleResolveTarget(kH312BleScanMs, &targetAddr, nullptr, nullptr, &targetAddrType)) {
        gH312LastError = "h312 ble scan failed";
        gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
        h312BleMarkError(h312Slot, "timeout");
        gH312BleConnectPending = false;
        gH312BleNextPollMs = nowMs + kH312BleRetryFailMs;
        return;
      }
    }
    if (kH312BleDiagLogs) {
      extractorCmdLogPushf("[h312] ble target slot=%u addr=%s atype=%s name=%s",
                           (unsigned int)h312NormSlot(h312Slot),
                           targetAddr.c_str(),
                           h312BleAddrTypeText((int)targetAddrType),
                           h312BleName.c_str());
    }
    NimBLEClient *client = h312BleAcquireClient();
    if (!client) {
      gH312LastError = "h312 ble client alloc failed";
      if (kH312BleDiagLogs) {
        extractorCmdLogPushf("[h312] ble client alloc failed slot=%u", (unsigned int)h312NormSlot(h312Slot));
      }
      h312BleMarkError(h312Slot, "timeout");
      gH312BleConnectPending = false;
      gH312BleNextPollMs = nowMs + kH312BleRetryFailMs;
      return;
    }
    if (client->isConnected()) client->disconnect();
    uint8_t connectedAddrType = 0xFF;
    bool connectedOk = h312BleConnectWithTypeFallback(client, targetAddr, targetAddrType, &connectedAddrType);
    if (!connectedOk) {
      gH312LastError = "h312 ble connect failed";
      gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
      gH312BleLastFailedAddr = targetAddr;
      gH312BleLastFailedMs = nowMs;
      gH312BleRuntimeAddr = "";
      gH312BleRuntimeAddrType = -1;
      if (gH312BleFailStreak >= 20) {
        h312BleDropClientHard();
      }
      if (kH312BleDiagLogs) {
        extractorCmdLogPushf("[h312] ble connect failed slot=%u addr=%s atype=%s", (unsigned int)h312NormSlot(h312Slot),
                             targetAddr.c_str(), h312BleAddrTypeText((int)targetAddrType));
      }
      h312BleMarkError(h312Slot, "timeout");
      gH312BleConnectPending = false;
      gH312BleNextPollMs = nowMs + kH312BleRetryFailMs;
      return;
    }

    NimBLERemoteCharacteristic *rxChar = nullptr;
    NimBLERemoteCharacteristic *ff01Char = nullptr;
    uint8_t rxKind = 2;
    String foundSvc;
    String foundChr;
    String ff01Svc;
    String ff01Chr;

    NimBLERemoteService *svc = client->getService("0000bb00-0000-1000-8000-00805f9b34fb");
    if (svc) {
      rxChar = svc->getCharacteristic("0000bb02-0000-1000-8000-00805f9b34fb");
      if (rxChar) {
        foundSvc = String(svc->getUUID().toString().c_str());
        foundChr = String(rxChar->getUUID().toString().c_str());
        rxKind = 2;
      }
      ff01Char = svc->getCharacteristic("0000ff01-0000-1000-8000-00805f9b34fb");
      if (ff01Char) {
        ff01Svc = String(svc->getUUID().toString().c_str());
        ff01Chr = String(ff01Char->getUUID().toString().c_str());
      }
    }
    if (!rxChar) {
      rxChar = h312BleFindCharacteristicAnywhere(client, "BB02", &foundSvc, &foundChr, true);
      rxKind = rxChar ? 2 : rxKind;
    }
    if (!ff01Char) {
      ff01Char = h312BleFindCharacteristicAnywhere(client, "FF01", &ff01Svc, &ff01Chr, true);
    }
    if (!ff01Char) {
      if (kH312BleDiagLogs) {
        extractorCmdLogPushf("[h312] ble ff01 missing slot=%u addr=%s", (unsigned int)h312NormSlot(h312Slot), targetAddr.c_str());
        h312BleLogGattMap(client, h312NormSlot(h312Slot));
      }
      client->disconnect();
      gH312LastError = "h312 ble ff01 missing";
      gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
      h312BleMarkError(h312Slot, "bad_response");
      gH312BleConnectPending = false;
      gH312BleNextPollMs = nowMs + kH312BleRetryFailMs;
      return;
    }

    gH312BleClient = client;
    gH312BleCharBb02 = rxChar;
    gH312BleCharFf01 = ff01Char;
    gH312BleCharKind = rxKind;
    gH312BleRuntimeAddr = targetAddr;
    gH312BleRuntimeAddrType = (connectedAddrType <= 3) ? (int8_t)connectedAddrType : targetAddrType;
    gH312BleLastFailedAddr = "";
    gH312BleLastFailedMs = 0;
    gH312BleLastIpReadMs = 0;
    gH312BleRuntimeIp = "";
    gH312BleJsonWaitStreak = 0;
    gH312BleJsonBuf.clear();
    portENTER_CRITICAL(&gH312BleNotifyMux);
    gH312BleNotifyBuf.clear();
    portEXIT_CRITICAL(&gH312BleNotifyMux);
    gH312BleNotifySubscribed = false;
    h312BleMaybeRefreshRuntimeIp(nowMs);
    h312BleLogEvent("H312_BLE_CONNECT", h312Slot, -1000, targetAddr.c_str());
    gH312BleLinkWasUp = true;
    gH312BleConnectPending = false;
    if (kH312BleDiagLogs) {
      extractorCmdLogPushf("[h312] ble connected slot=%u addr=%s atype=%s svc=%s chr=%s kind=%s ff01=%s/%s",
                           (unsigned int)h312NormSlot(h312Slot), targetAddr.c_str(), h312BleAddrTypeText((int)gH312BleRuntimeAddrType),
                           foundSvc.c_str(), foundChr.c_str(), (rxKind == 2) ? "bb02" : "bb01",
                           ff01Svc.c_str(), ff01Chr.c_str());
    }
    if (!gH312BleRuntimeIp.isEmpty()) {
      h312BleDisconnectKeepSeed();
      gH312BleNextPollMs = nowMs + 120;
      return;
    }
  }

  h312BleMaybeRefreshRuntimeIp(nowMs);

  if (gH312BleRuntimeIp.isEmpty()) {
    gH312LastError = "h312 ble ff01 ip missing";
    gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
    if (gH312BleFailStreak >= 8) h312BleMarkError(h312Slot, "timeout");
    if (gH312BleFailStreak >= 24) h312BleReleaseClient();
    gH312BleNextPollMs = nowMs + kH312BleTcpFailBackoffMs;
    return;
  }

  IPAddress runtimeIp;
  if (!runtimeIp.fromString(gH312BleRuntimeIp)) {
    gH312LastError = "h312 ble ff01 ip invalid";
    gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
    if (gH312BleFailStreak >= 8) h312BleMarkError(h312Slot, "bad_response");
    if (gH312BleFailStreak >= 24) h312BleReleaseClient();
    gH312BleNextPollMs = nowMs + kH312BleTcpFailBackoffMs;
    return;
  }

  h312::H312ProbeOptions opt;
  opt.connectTimeoutMs = kH312BleTcpConnectMs;
  opt.readMs = kH312BleTcpReadMs;
  opt.startupDelayMs = kH312BleTcpStartupDelayMs;
  opt.keepaliveAfterMs = kH312BleTcpKeepaliveAfterMs;
  opt.requireRealTimeTemp = true;
  h312::H312ProbeResult pr;
  String reason;
  bool ok = h312::probeHost(runtimeIp, h312Port, opt, &pr, &reason);
  if (!ok || pr.rt <= -200) {
    gH312LastError = String("h312 ble tcp no temp: ") + (reason.length() ? reason : "timeout");
    gH312BleFailStreak = (gH312BleFailStreak < 120) ? (int8_t)(gH312BleFailStreak + 1) : gH312BleFailStreak;
    if (gH312BleFailStreak >= 8) h312BleMarkError(h312Slot, "timeout");
    if (gH312BleFailStreak >= 30) h312BleReleaseClient();
    gH312BleNextPollMs = nowMs + kH312BleTcpFailBackoffMs;
    return;
  }

  rt = pr.rt;
  ws = pr.ws;
  hf = pr.hf;
  setTemp = -1;
  realFan = -1;
  setFan = -1;
  gH312BlePrevRt = (int16_t)rt;
  gH312BlePrevRtMs = nowMs;
  gH312BleSecLikeStreak = 0;
  h312BleAcceptTelemetry(nowMs, 0, "tcp", rt, ws, hf, setTemp, realFan, setFan);
}

static const char *h312ErrText(h312::H312Error e) {
  switch (e) {
    case h312::H312Error::OK:
      return "";
    case h312::H312Error::TIMEOUT:
      return "h312 timeout";
    case h312::H312Error::BAD_RESPONSE:
      return "h312 bad response";
    case h312::H312Error::NO_ROUTE:
      return "h312 connect failed";
    case h312::H312Error::SLOT_DISABLED:
      return "disabled";
    case h312::H312Error::NO_IP:
      return "h312 no ip";
    default:
      return "h312 error";
  }
}

static void h312DriverLogCb(void *ctx, h312::H312LogEventType ev, uint8_t slot, float tempC) {
  (void)ctx;
  // BLE mode is handled by the NimBLE runtime path; suppress Wi-Fi driver slot
  // events to avoid misleading fallback/timeout logs.
  if (h312Mode == H312_MODE_BLE) return;
  uint8_t s = h312NormSlot(slot);
  uint8_t i = (uint8_t)(s - 1);
  switch (ev) {
    case h312::H312LogEventType::H312_SLOT_OK:
      if (gH312UiErrActive[i]) {
        extractorCmdLogPushf("[h312] slot=%u recovered temp=%.1fC", (unsigned int)s, (double)tempC);
        gH312UiErrActive[i] = false;
      }
      break;
    case h312::H312LogEventType::H312_SLOT_TIMEOUT:
      if (!gH312UiErrActive[i]) {
        extractorCmdLogPushf("[h312] slot=%u timeout", (unsigned int)s);
      }
      gH312UiErrActive[i] = true;
      break;
    case h312::H312LogEventType::H312_SLOT_BADRESP:
      if (!gH312UiErrActive[i]) {
        extractorCmdLogPushf("[h312] slot=%u bad_response", (unsigned int)s);
      }
      gH312UiErrActive[i] = true;
      break;
    case h312::H312LogEventType::H312_ACTIVE_ON:
    case h312::H312LogEventType::H312_ACTIVE_OFF:
      break;
  }
}

static void h312ApplyDriverConfig() {
  h312::H312Config cfg;
  cfg.reqTimeoutMs = 3200;
  for (uint8_t i = 0; i < 3; i++) {
    cfg.slots[i].enabled = false;
    cfg.slots[i].ip = IPAddress();
    cfg.slots[i].port = 6666;
    cfg.slots[i].pollMs = kH312PollIdleMs;
    cfg.slots[i].thresholdC = 0.0f;
    cfg.slots[i].holdMs = 0;
  }

  const uint8_t slot = h312NormSlot(h312Slot);
  const uint8_t idx = (uint8_t)(slot - 1);
  cfg.slots[idx].port = h312Port;
  cfg.slots[idx].pollMs = kH312PollIdleMs;
  cfg.slots[idx].thresholdC = (float)h312TempOn;
  cfg.slots[idx].holdMs = h312MinOnMs;

  if (h312Mode == H312_MODE_WIFI && h312Enabled && !h312Ip.isEmpty()) {
    IPAddress ip;
    if (ip.fromString(h312Ip)) {
      cfg.slots[idx].enabled = true;
      cfg.slots[idx].ip = ip;
    }
  }

  h312::H312Error err = h312::H312Error::OK;
  gH312Driver.applyConfig(cfg, &err);
  if (h312Mode != H312_MODE_BLE) {
    h312BleReleaseClient();
    gH312BleRuntimeAddr = "";
    gH312BleRuntimeAddrType = -1;
  }
}

static uint32_t h312StaleWindowMs() {
  const bool bleMode = (h312Mode == H312_MODE_BLE);
  const uint32_t basePollMs = bleMode ? kH312BlePollMs : kH312PollIdleMs;
  uint32_t s = basePollMs * 3U;
  // BLE requires a wider window because connect/read cycles are naturally longer
  // than Wi-Fi polling and may briefly overlap with other BLE control traffic.
  const uint32_t minWindowMs = bleMode ? 12000U : 3000U;
  if (s < minWindowMs) s = minWindowMs;
  return s;
}

static bool h312SlotConnectedNow(uint8_t slot) {
  if (h312NormSlot(slot) != h312NormSlot(h312Slot)) return false;
  if (gH312LastJsonMs == 0) return false;
  const uint32_t now = (uint32_t)millis();
  if (now < gH312LastJsonMs) return false;
  return ((now - gH312LastJsonMs) <= h312StaleWindowMs());
}

static String jsonEscape(const String &s) {
  String o;
  o.reserve(s.length() + 8);
  for (size_t i = 0; i < s.length(); i++) {
    unsigned char c = (unsigned char)s[i];
    if (c == '\\') o += F("\\\\");
    else if (c == '"') o += F("\\\"");
    else if (c == '\n') o += F("\\n");
    else if (c == '\r') o += F("\\r");
    else if (c == '\t') o += F("\\t");
    else if (c < 0x20) {
      char esc[7];
      snprintf(esc, sizeof(esc), "\\u%04X", (unsigned)c);
      o += esc;
    } else {
      o += (char)c;
    }
  }
  return o;
}

static String safePartitionLabel(const esp_partition_t *p) {
  if (!p) return String("");
  char raw[17];
  memset(raw, 0, sizeof(raw));
  memcpy(raw, p->label, 16);
  raw[16] = '\0';

  String out;
  out.reserve(16);
  for (size_t i = 0; i < 16; i++) {
    unsigned char c = (unsigned char)raw[i];
    if (c == 0) break;
    if (c < 0x20 || c == 0x7F) out += '_';
    else out += (char)c;
  }
  return out;
}

static inline uint8_t clampSpeedInt(int v) {
  if (v < 0) return 0;
  if (v > 100) return 100;
  return (uint8_t)v;
}

static inline uint32_t clampDelayMs(uint32_t v) {
  if (v > 600000UL) return 600000UL;
  return v;
}

static inline int16_t clampTzOffsetMin(int v) {
  if (v < -840) return -840; // UTC-14
  if (v > 840) return 840;   // UTC+14
  return (int16_t)v;
}

static String normalizeTzName(const String &raw) {
  String s = raw;
  s.trim();
  if (s.isEmpty()) s = "UTC";
  if (s.length() > 63) s = s.substring(0, 63);
  return s;
}

static void clockSetFromEpochMs(uint64_t epochMs, const char *source) {
  if (epochMs < 1500000000000ULL) return; // reject clearly invalid timestamps
  clockBaseEpochMs = epochMs;
  clockBaseUptimeMs = (uint32_t)millis();
  clockValid = true;
  clockLastSetMs = clockBaseUptimeMs;
  clockLastSyncSource = source ? String(source) : String("set");
}

static uint64_t clockNowEpochMs() {
  if (!clockValid) return 0;
  uint32_t now = (uint32_t)millis();
  uint32_t dt = (now >= clockBaseUptimeMs) ? (now - clockBaseUptimeMs) : 0;
  return clockBaseEpochMs + (uint64_t)dt;
}

static bool clockGetLocalTm(struct tm *outTm) {
  if (!outTm) return false;
  uint64_t utcMs = clockNowEpochMs();
  if (utcMs == 0) return false;
  int64_t localMs = (int64_t)utcMs + (int64_t)tzOffsetMin * 60000LL;
  time_t sec = (time_t)(localMs / 1000LL);
  gmtime_r(&sec, outTm);
  return true;
}

static String clockLocalText() {
  struct tm tmv;
  if (!clockGetLocalTm(&tmv)) return String("-");
  char buf[24];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d", tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday, tmv.tm_hour,
           tmv.tm_min, tmv.tm_sec);
  return String(buf);
}

static uint16_t clockLocalMinuteOfDay() {
  struct tm tmv;
  if (!clockGetLocalTm(&tmv)) return 0;
  return (uint16_t)(tmv.tm_hour * 60 + tmv.tm_min);
}

static uint32_t clockLocalYmd() {
  struct tm tmv;
  if (!clockGetLocalTm(&tmv)) return 0;
  return (uint32_t)((tmv.tm_year + 1900) * 10000 + (tmv.tm_mon + 1) * 100 + tmv.tm_mday);
}

static bool clockTryNtpSync(bool force = false) {
  if (WiFi.status() != WL_CONNECTED) return false;
  uint32_t now = (uint32_t)millis();
  if (!force) {
    if (clockNtpLastTryMs != 0 && (now - clockNtpLastTryMs) < kClockNtpRetryMs) return false;
    if (clockValid && clockLastNtpSyncMs != 0 && (now - clockLastNtpSyncMs) < kClockNtpResyncMs) return false;
  }
  clockNtpLastTryMs = now;

  configTime(0, 0, "pool.ntp.org", "time.google.com", "time.windows.com");
  time_t t = 0;
  for (uint8_t i = 0; i < 20; i++) {
    t = time(nullptr);
    if (t > 1700000000) break;
    delay(100);
  }
  if (t <= 1700000000) return false;

  clockSetFromEpochMs((uint64_t)t * 1000ULL, "ntp");
  clockLastNtpSyncMs = (uint32_t)millis();
  return true;
}

static void saveTimeConfig(int16_t offsetMin, const String &name) {
  offsetMin = clampTzOffsetMin((int)offsetMin);
  String normalized = normalizeTzName(name);
  prefs.begin(kPrefsNs, false);
  prefs.putShort("tz2_off_min", offsetMin);
  prefs.putString("tz2_name", normalized);
  prefs.putBool("tz2_set", true);
  prefs.end();
  tzOffsetMin = offsetMin;
  tzName = normalized;
  tzConfigured = true;
}

static void saveAutoStopConfig(bool enabled, uint16_t minuteOfDay) {
  if (minuteOfDay > 1439) minuteOfDay = 1439;
  prefs.begin(kPrefsNs, false);
  prefs.putBool("auto_stop_en", enabled);
  prefs.putUShort("auto_stop_min", minuteOfDay);
  prefs.end();
  autoStopEnabled = enabled;
  autoStopMinute = minuteOfDay;
  if (!enabled) autoStopLastYmd = 0;
}

static const char *extractorSourceText(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "h1";
    case SRC_H2: return "h2";
    case SRC_HA1: return "ha1";
    case SRC_HA2: return "ha2";
    case SRC_HA3: return "ha3";
    default: return "h1";
  }
}

static const char *extractorSourceNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "sp_h1";
    case SRC_H2: return "sp_h2";
    case SRC_HA1: return "sp_ha1";
    case SRC_HA2: return "sp_ha2";
    case SRC_HA3: return "sp_ha3";
    default: return "sp_h1";
  }
}

static const char *extractorSourceLightNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "lt_h1";
    case SRC_H2: return "lt_h2";
    case SRC_HA1: return "lt_ha1";
    case SRC_HA2: return "lt_ha2";
    case SRC_HA3: return "lt_ha3";
    default: return "lt_h1";
  }
}

static const char *extractorSourceBrightnessModeNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "brm_h1";
    case SRC_H2: return "brm_h2";
    case SRC_HA1: return "brm_ha1";
    case SRC_HA2: return "brm_ha2";
    case SRC_HA3: return "brm_ha3";
    default: return "brm_h1";
  }
}

static const char *extractorSourceBrightnessManualNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "brv_h1";
    case SRC_H2: return "brv_h2";
    case SRC_HA1: return "brv_ha1";
    case SRC_HA2: return "brv_ha2";
    case SRC_HA3: return "brv_ha3";
    default: return "brv_h1";
  }
}

static const char *extractorSourceBrightnessLastNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "brl_h1";
    case SRC_H2: return "brl_h2";
    case SRC_HA1: return "brl_ha1";
    case SRC_HA2: return "brl_ha2";
    case SRC_HA3: return "brl_ha3";
    default: return "brl_h1";
  }
}

static const char *extractorSourceLastModeNvsKey(ExtractorSource src) {
  switch (src) {
    case SRC_H1: return "ltm_h1";
    case SRC_H2: return "ltm_h2";
    case SRC_HA1: return "ltm_ha1";
    case SRC_HA2: return "ltm_ha2";
    case SRC_HA3: return "ltm_ha3";
    default: return "ltm_h1";
  }
}

static const char *lightModeText(uint8_t mode) {
  if (mode == LIGHT_WHITE) return "white";
  if (mode == LIGHT_GREEN) return "green";
  if (mode == LIGHT_LAST) return "last_enabled";
  return "off";
}

static uint8_t normalizeLastEnabledLightMode(uint8_t mode) {
  if (mode == LIGHT_GREEN) return LIGHT_GREEN;
  return LIGHT_WHITE;
}

static uint8_t clampBrightnessInt(int v) {
  if (v < 0) return 0;
  if (v > 100) return 100;
  return (uint8_t)v;
}

static const char *brightnessModeText(uint8_t mode) {
  return (mode == BRI_MANUAL) ? "manual" : "last";
}

static uint8_t selectorToLightMode(uint8_t selector) {
  // TES02 runtime selector map observed in testing:
  // white -> b10=0, green -> b10=1, off -> b10=2.
  if (selector == 0) return LIGHT_WHITE;
  if (selector == 1) return LIGHT_GREEN;
  return LIGHT_OFF;
}

static ExtractorSource sourceFromHotAirSlot(uint8_t slot) {
  if (slot <= 1) return SRC_HA1;
  if (slot == 2) return SRC_HA2;
  return SRC_HA3;
}

static bool parseExtractorSource(const String &rawIn, ExtractorSource *out) {
  String raw = rawIn;
  raw.trim();
  raw.toLowerCase();
  if (raw.isEmpty()) return false;
  if (raw == "h1" || raw == "handle1" || raw == "iron1" || raw == "t420d1") {
    if (out) *out = SRC_H1;
    return true;
  }
  if (raw == "h2" || raw == "handle2" || raw == "iron2" || raw == "t420d2") {
    if (out) *out = SRC_H2;
    return true;
  }
  if (raw == "ha" || raw == "hotair" || raw == "hot_air" || raw == "h312" || raw == "ha1" || raw == "hotair1" ||
      raw == "hot_air_1") {
    if (out) *out = SRC_HA1;
    return true;
  }
  if (raw == "ha2" || raw == "hotair2" || raw == "hot_air_2") {
    if (out) *out = SRC_HA2;
    return true;
  }
  if (raw == "ha3" || raw == "hotair3" || raw == "hot_air_3") {
    if (out) *out = SRC_HA3;
    return true;
  }
  return false;
}

static uint8_t sourceSpeed(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  return gSourceLastSpeed[idx];
}

static uint8_t sourceLightMode(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  uint8_t mode = gSourceLightMode[idx];
  if (mode > LIGHT_LAST) mode = LIGHT_OFF;
  return mode;
}

static uint8_t sourceBrightnessMode(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  uint8_t mode = gSourceBrightnessMode[idx];
  if (mode > BRI_MANUAL) mode = BRI_LAST;
  return mode;
}

static uint8_t sourceLastEnabledLightMode(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  return normalizeLastEnabledLightMode(gSourceLastEnabledLightMode[idx]);
}

static uint8_t sourceManualBrightness(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  return clampBrightnessInt((int)gSourceManualBrightness[idx]);
}

static uint8_t sourceLastBrightness(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) idx = (uint8_t)SRC_H1;
  return clampBrightnessInt((int)gSourceLastBrightness[idx]);
}

static uint8_t sourceTargetBrightness(ExtractorSource src) {
  return (sourceBrightnessMode(src) == BRI_MANUAL) ? sourceManualBrightness(src) : sourceLastBrightness(src);
}

static void appendJsonLightProfileFields(String &body) {
  body += F(",\"light_h1\":");
  body += String((int)sourceLightMode(SRC_H1));
  body += F(",\"light_h2\":");
  body += String((int)sourceLightMode(SRC_H2));
  body += F(",\"light_ha1\":");
  body += String((int)sourceLightMode(SRC_HA1));
  body += F(",\"light_ha2\":");
  body += String((int)sourceLightMode(SRC_HA2));
  body += F(",\"light_ha3\":");
  body += String((int)sourceLightMode(SRC_HA3));
  body += F(",\"light_bri_mode_h1\":");
  body += String((int)sourceBrightnessMode(SRC_H1));
  body += F(",\"light_bri_mode_h2\":");
  body += String((int)sourceBrightnessMode(SRC_H2));
  body += F(",\"light_bri_mode_ha1\":");
  body += String((int)sourceBrightnessMode(SRC_HA1));
  body += F(",\"light_bri_mode_ha2\":");
  body += String((int)sourceBrightnessMode(SRC_HA2));
  body += F(",\"light_bri_mode_ha3\":");
  body += String((int)sourceBrightnessMode(SRC_HA3));
  body += F(",\"light_bri_manual_h1\":");
  body += String((int)sourceManualBrightness(SRC_H1));
  body += F(",\"light_bri_manual_h2\":");
  body += String((int)sourceManualBrightness(SRC_H2));
  body += F(",\"light_bri_manual_ha1\":");
  body += String((int)sourceManualBrightness(SRC_HA1));
  body += F(",\"light_bri_manual_ha2\":");
  body += String((int)sourceManualBrightness(SRC_HA2));
  body += F(",\"light_bri_manual_ha3\":");
  body += String((int)sourceManualBrightness(SRC_HA3));
  body += F(",\"light_bri_last_h1\":");
  body += String((int)sourceLastBrightness(SRC_H1));
  body += F(",\"light_bri_last_h2\":");
  body += String((int)sourceLastBrightness(SRC_H2));
  body += F(",\"light_bri_last_ha1\":");
  body += String((int)sourceLastBrightness(SRC_HA1));
  body += F(",\"light_bri_last_ha2\":");
  body += String((int)sourceLastBrightness(SRC_HA2));
  body += F(",\"light_bri_last_ha3\":");
  body += String((int)sourceLastBrightness(SRC_HA3));
}

static void appendJsonH312StaticConfigFields(String &body) {
  body += F(",\"h312_mode\":\"");
  body += h312ModeText(h312Mode);
  body += F("\"");
  body += F(",\"h312_ble_addr\":\"");
  body += jsonEscape(h312BleAddr);
  body += F("\"");
  body += F(",\"h312_ble_name\":\"");
  body += jsonEscape(h312BleName);
  body += F("\"");
  body += F(",\"h312_t_on\":");
  body += String((int)h312TempOn);
  body += F(",\"h312_t_off\":");
  body += String((int)h312TempOff);
  body += F(",\"h312_debounce_ms\":");
  body += String((uint32_t)h312DebounceMs);
  body += F(",\"h312_min_on_ms\":");
  body += String((uint32_t)h312MinOnMs);
  body += F(",\"h312_slot\":");
  body += String((uint32_t)h312Slot);
  body += F(",\"t420d_h1_gpio\":");
  body += String((int)t420dH1Gpio);
  body += F(",\"t420d_h2_gpio\":");
  body += String((int)t420dH2Gpio);
  body += F(",\"t420d_h1_raised_on_high\":");
  body += (t420dH1RaisedOnHigh ? "true" : "false");
  body += F(",\"t420d_h2_raised_on_high\":");
  body += (t420dH2RaisedOnHigh ? "true" : "false");
  body += F(",\"t420d_logic_enabled\":");
  body += (t420dLogicEnabled ? "true" : "false");
  body += F(",\"t420d_h1_on_ms\":");
  body += String((uint32_t)t420dH1OnDelayMs);
  body += F(",\"t420d_h2_on_ms\":");
  body += String((uint32_t)t420dH2OnDelayMs);
  body += F(",\"t420d_h1_off_ms\":");
  body += String((uint32_t)t420dH1OffDelayMs);
  body += F(",\"t420d_h2_off_ms\":");
  body += String((uint32_t)t420dH2OffDelayMs);
}

static void setActiveSource(ExtractorSource src) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) src = SRC_H1;
  gActiveSource = src;
  lastOnSpeed = sourceSpeed(gActiveSource);
}

static void bleSyncRuntimeFromDriver();
static void bleApplyDriverConfig();
static bool bleEnsureConnectedCallback(void *ctx, uint32_t scanMs);
static void bleDriverLogCb(void *ctx, const char *line);
static void bleSetConnectSource(const String &src);
static bool bleReady();
static bool bleEnsureConnected(uint32_t scanMs);
static void blePrimeFreshSession(uint16_t minAgeMs);

static const char *wifiModeText(wifi_mode_t m) {
  if (m == WIFI_AP) return "ap";
  if (m == WIFI_STA) return "sta";
  if (m == WIFI_AP_STA) return "apsta";
  return "off";
}

static void uartDiagSnapshot(const char *tag) {
  if (!kUartDiagEnabled) return;
  uint32_t now = (uint32_t)millis();
  wifi_mode_t m = WiFi.getMode();
  String ip;
  if (m == WIFI_AP) ip = WiFi.softAPIP().toString();
  else if (m == WIFI_AP_STA) ip = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
  else ip = WiFi.localIP().toString();

  uint32_t h312AgeMs = 0;
  bool h312AgeValid = false;
  if (gH312LastJsonMs > 0 && now >= gH312LastJsonMs) {
    h312AgeMs = now - gH312LastJsonMs;
    h312AgeValid = true;
  }
  uint32_t bleIdleMs = 0;
  if (bleReady() && now >= gBleLastActivityMs) bleIdleMs = now - gBleLastActivityMs;
  String h312AgeTxt = h312AgeValid ? String(h312AgeMs) : String("na");

  LB_SERIAL_PRINTF(
      "[diag] %s up=%lu heap=%u min=%u wifi=%s sta=%d ip=%s bleBound=%d bleReady=%d src=%s addr=%s rssi=%d idle=%lu "
      "h312Conn=%d rt=%d set=%d fan=%d/%d ws=%d hf=%d age=%s logic=%s pend=%d/%d exOn=%d sess=%d act=%s sp=%u err=%s "
      "cpu=%u core=%u h312p=%u sessp=%u blep=%u web=%u lmax_us=%lu loops=%lu\n",
      tag ? tag : "-", (unsigned long)now, (unsigned int)ESP.getFreeHeap(), (unsigned int)esp_get_minimum_free_heap_size(),
      wifiModeText(m), (WiFi.status() == WL_CONNECTED) ? 1 : 0, ip.c_str(), bleBound ? 1 : 0, bleReady() ? 1 : 0,
      gBleConnectSource.c_str(), gBleLastAddr.c_str(), gBleLastRssi, (unsigned long)bleIdleMs,
      h312SlotConnectedNow(h312Slot) ? 1 : 0,
      gH312RealTimeTemp, gH312SetTemp, gH312RealFan, gH312SetFan, gH312WorkStatus, gH312HeatFlag, h312AgeTxt.c_str(),
      gH312LastLogic.c_str(), gH312PendingValid ? 1 : 0, gH312PendingOn ? 1 : 0, gH312ExtractorOn ? 1 : 0,
      gExtractorSessionActive ? 1 : 0, extractorSourceText(gActiveSource), (unsigned int)sourceSpeed(gActiveSource),
      gH312LastError.c_str(), (unsigned int)gPerfCpuPct, (unsigned int)gPerfCorePct, (unsigned int)gPerfH312Pct,
      (unsigned int)gPerfSessPct, (unsigned int)gPerfBlePct, (unsigned int)gPerfWebPct, (unsigned long)gPerfLoopMaxUs,
      (unsigned long)gPerfLoops);
}

static void uartDiagTransitions() {
  if (!kUartDiagEnabled) return;
  bool bleOk = bleReady();
  if (bleOk != gDiagPrevBleReady) {
    LB_SERIAL_PRINTF("[diag] ble ready=%d src=%s addr=%s rssi=%d\n", bleOk ? 1 : 0, gBleConnectSource.c_str(),
                  gBleLastAddr.c_str(), gBleLastRssi);
    gDiagPrevBleReady = bleOk;
  }
  bool h312Conn = h312SlotConnectedNow(h312Slot);
  if (h312Conn != gDiagPrevH312Connected) {
    LB_SERIAL_PRINTF("[diag] h312 connected=%d ip=%s:%u\n", h312Conn ? 1 : 0, h312Ip.c_str(), (unsigned int)h312Port);
    gDiagPrevH312Connected = h312Conn;
  }
  if (gH312ExtractorOn != gDiagPrevExtractorOn) {
    LB_SERIAL_PRINTF("[diag] extractor on=%d source=%s saved=%u\n", gH312ExtractorOn ? 1 : 0, extractorSourceText(gActiveSource),
                  (unsigned int)sourceSpeed(gActiveSource));
    gDiagPrevExtractorOn = gH312ExtractorOn;
  }
  if (gH312LastLogic != gDiagPrevLogic) {
    LB_SERIAL_PRINTF("[diag] logic -> %s\n", gH312LastLogic.c_str());
    gDiagPrevLogic = gH312LastLogic;
  }
  if (gH312LastError != gDiagPrevH312Error) {
    if (gH312LastError.length() > 0) LB_SERIAL_PRINTF("[diag] h312 error: %s\n", gH312LastError.c_str());
    gDiagPrevH312Error = gH312LastError;
  }
}

static void uartDiagTick() {
  if (!kUartDiagEnabled) return;
  uartDiagTransitions();
  uint32_t now = (uint32_t)millis();
  if (gUartDiagLastMs != 0 && (now - gUartDiagLastMs) < kUartDiagTickMs) return;
  gUartDiagLastMs = now;
  uartDiagSnapshot("tick");
}

static uint32_t ipToBeU32(const IPAddress &ip) {
  return ((uint32_t)ip[0] << 24) | ((uint32_t)ip[1] << 16) | ((uint32_t)ip[2] << 8) | (uint32_t)ip[3];
}

static IPAddress beU32ToIp(uint32_t be) {
  return IPAddress((uint8_t)((be >> 24) & 0xFF), (uint8_t)((be >> 16) & 0xFF), (uint8_t)((be >> 8) & 0xFF),
                   (uint8_t)(be & 0xFF));
}

static uint32_t h312HostOrderToOffset(uint32_t orderIdx, uint32_t hostCount) {
  if (hostCount >= 100) {
    uint32_t firstStart = 100;
    uint32_t firstEnd = (hostCount < 199) ? hostCount : 199;
    uint32_t firstLen = (hostCount >= firstStart) ? (firstEnd - firstStart + 1) : 0;
    uint32_t secondLen = (hostCount < 99) ? hostCount : 99; // 1..99
    if (orderIdx < firstLen) return firstStart + orderIdx;
    if (orderIdx < (firstLen + secondLen)) return 1 + (orderIdx - firstLen);
    return 200 + (orderIdx - firstLen - secondLen);
  }
  return orderIdx + 1;
}

struct H312ScanFoundDevice {
  String ip;
  String name;
  String model;
  String sn;
  int ws = -1;
  int rt = -1000;
  int hf = -1;
  bool weak = false;
  size_t weakRxBytes = 0;
};

struct H312ScanShared {
  uint32_t ipU = 0;
  uint32_t netU = 0;
  uint32_t hostCount = 0;
  uint16_t port = 6666;
  uint16_t connectTimeoutMs = 900;
  uint16_t readMs = 1250;
  uint16_t probeConnectTimeoutMs = 1200;
  uint16_t probeReadMs = 1500;
  uint16_t startupDelayMs = 20;
  uint16_t keepaliveAfterMs = 700;
  uint8_t retries = 1;
  bool includeWeak = false;
  bool stopOnFirst = true;
  volatile bool stopTriggered = false;
  volatile uint32_t nextOrderIdx = 0;
  volatile uint32_t scanned = 0;
  volatile uint32_t skippedSelf = 0;
  volatile uint32_t foundProbe = 0;
  volatile uint32_t foundWeak = 0;
  volatile uint32_t failCount = 0;
  volatile uint32_t workersDone = 0;
  static const uint8_t kMaxDevices = 8;
  uint8_t deviceCount = 0;
  H312ScanFoundDevice devices[kMaxDevices];
  SemaphoreHandle_t lock = nullptr;
};

static bool h312ProbeHost(const IPAddress &target, uint16_t port, uint16_t connectTimeoutMs, uint16_t readMs, String *outName,
                          String *outModel, String *outSn, int *outWs, int *outTemp, bool requireRealTimeTemp,
                          uint16_t startupDelayMs, uint16_t keepaliveAfterMs, int *outHeat, String *outReason);
static bool h312SocketLooksAlive(const IPAddress &target, uint16_t port, uint16_t connectTimeoutMs, uint16_t readMs,
                                 size_t *outRxBytes);

static void h312ScanWorkerTask(void *arg) {
  H312ScanShared *s = (H312ScanShared *)arg;
  for (;;) {
    if (s->stopOnFirst && s->stopTriggered) break;

    uint32_t orderIdx = 0;
    xSemaphoreTake(s->lock, portMAX_DELAY);
    if (s->nextOrderIdx >= s->hostCount) {
      xSemaphoreGive(s->lock);
      break;
    }
    orderIdx = s->nextOrderIdx++;
    xSemaphoreGive(s->lock);

    uint32_t i = h312HostOrderToOffset(orderIdx, s->hostCount);
    uint32_t hostU = s->netU + i;
    if (hostU == s->ipU) {
      xSemaphoreTake(s->lock, portMAX_DELAY);
      s->skippedSelf++;
      xSemaphoreGive(s->lock);
      continue;
    }

    IPAddress target = beU32ToIp(hostU);
    String name;
    String model;
    String sn;
    int ws = -1;
    int rt = -1000;
    int hf = -1;
    bool ok = false;
    bool weak = false;
    size_t weakRxBytes = 0;

    for (uint8_t attempt = 0; attempt < s->retries; attempt++) {
      String probeReason;
      if (h312ProbeHost(target, s->port, s->probeConnectTimeoutMs, s->probeReadMs, &name, &model, &sn, &ws, &rt, true,
                        s->startupDelayMs, s->keepaliveAfterMs, &hf, &probeReason)) {
        ok = true;
        break;
      }
      delay(2);
    }

    if (!ok && s->includeWeak &&
        h312SocketLooksAlive(target, s->port, (uint16_t)(s->connectTimeoutMs + 220), (uint16_t)max((int)s->readMs * 3, 900),
                             &weakRxBytes)) {
      ok = true;
      weak = true;
      if (name.isEmpty()) name = "H31X";
      if (model.isEmpty()) model = "H31X";
      if (sn.isEmpty()) sn = "";
      if (ws < 0) ws = -1;
      if (rt <= -200) rt = -1000;
    }

    xSemaphoreTake(s->lock, portMAX_DELAY);
    if (ok) {
      if (weak) s->foundWeak++;
      else s->foundProbe++;

      if (s->deviceCount < H312ScanShared::kMaxDevices) {
        H312ScanFoundDevice &d = s->devices[s->deviceCount++];
        d.ip = target.toString();
        d.name = name;
        d.model = model;
        d.sn = sn;
        d.ws = ws;
        d.rt = rt;
        d.hf = hf;
        d.weak = weak;
        d.weakRxBytes = weakRxBytes;
      }
      if (s->stopOnFirst) s->stopTriggered = true;
    } else {
      s->failCount++;
    }
    s->scanned++;
    xSemaphoreGive(s->lock);
    delay(1);
  }

  xSemaphoreTake(s->lock, portMAX_DELAY);
  s->workersDone++;
  xSemaphoreGive(s->lock);
  vTaskDelete(nullptr);
}

static String macSuffixFromBytes(const uint8_t mac[6], bool lower) {
  // Use exactly the last 5 hex chars of MAC (e.g. ...:CB:B9:E4 -> BB9E4).
  char full[13];
  snprintf(full, sizeof(full), lower ? "%02x%02x%02x%02x%02x%02x" : "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2],
           mac[3], mac[4], mac[5]);
  String s(full);
  if (s.length() <= 5) return s;
  return s.substring(s.length() - 5);
}

static String macTextFromBytes(const uint8_t mac[6]) {
  char text[18];
  snprintf(text, sizeof(text), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(text);
}

static String buildDeviceSecret() {
  // Deterministic per-device secret (non-default, no random dependency in early boot).
  // Example: lbbb9e4!2026
  String s = String("lb") + gMacSuffixLower + "!2026";
  if (s.length() < 8) s += "secure";
  return s;
}

static void initDeviceIdentity() {
  uint8_t mac[6] = {0};
  esp_err_t err = esp_read_mac(mac, ESP_MAC_WIFI_STA);
  if (err != ESP_OK) {
    LB_SERIAL_PRINTF("esp_read_mac failed: %d\n", (int)err);
  }

  gMacText = macTextFromBytes(mac);
  gMacSuffixLower = macSuffixFromBytes(mac, true);
  gMacSuffixUpper = macSuffixFromBytes(mac, false);
  gStaHostname = String("lb-bridge-") + gMacSuffixLower;
  gApSsid = String(kApSsidPrefix) + gMacSuffixUpper;
  snprintf(gMdnsIdTxt, sizeof(gMdnsIdTxt), "LB-%s", gMacSuffixUpper.c_str());

  LB_SERIAL_PRINTF("MAC: %s\n", gMacText.c_str());
  LB_SERIAL_PRINTF("hostname: %s\n", gStaHostname.c_str());
  LB_SERIAL_PRINTF("http://%s.local\n", gStaHostname.c_str());
}

static bool ipIsLocalLan(const IPAddress &ip) {
  if ((ip[0] == 10) || (ip[0] == 127)) return true;
  if (ip[0] == 192 && ip[1] == 168) return true;
  if (ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31) return true;
  if (ip[0] == 169 && ip[1] == 254) return true;
  return false;
}

static bool isValidWebUser(const String &user) {
  if (user.length() < 3 || user.length() > 24) return false;
  for (size_t i = 0; i < user.length(); i++) {
    char c = user.charAt(i);
    bool ok = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-' || c == '.';
    if (!ok) return false;
  }
  return true;
}

static String hexFromBytes(const uint8_t *data, size_t len) {
  static const char kHex[] = "0123456789abcdef";
  String out;
  out.reserve(len * 2);
  for (size_t i = 0; i < len; i++) {
    uint8_t b = data[i];
    out += kHex[(b >> 4) & 0x0F];
    out += kHex[b & 0x0F];
  }
  return out;
}

static String sha256Hex(const String &input) {
  uint8_t hash[32];
  mbedtls_sha256_context ctx;
  mbedtls_sha256_init(&ctx);
  mbedtls_sha256_starts_ret(&ctx, 0);
  mbedtls_sha256_update_ret(&ctx, (const unsigned char *)input.c_str(), input.length());
  mbedtls_sha256_finish_ret(&ctx, hash);
  mbedtls_sha256_free(&ctx);
  return hexFromBytes(hash, sizeof(hash));
}

static bool isHexToken(const String &token) {
  if (token.length() != 64) return false;
  for (size_t i = 0; i < token.length(); i++) {
    char c = token.charAt(i);
    bool ok = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    if (!ok) return false;
  }
  return true;
}

static String normalizeToken(String token) {
  token.trim();
  token.toLowerCase();
  if (!isHexToken(token)) return "";
  return token;
}

static String randomTokenHex() {
  uint8_t raw[32];
  esp_fill_random(raw, sizeof(raw));
  return hexFromBytes(raw, sizeof(raw));
}

static String rememberHashKey(uint8_t idx) {
  char k[8];
  snprintf(k, sizeof(k), "rt_h%u", (unsigned)idx);
  return String(k);
}

static String rememberExpKey(uint8_t idx) {
  char k[8];
  snprintf(k, sizeof(k), "rt_e%u", (unsigned)idx);
  return String(k);
}

static String rememberLabelKey(uint8_t idx) {
  char k[8];
  snprintf(k, sizeof(k), "rt_l%u", (unsigned)idx);
  return String(k);
}

static uint32_t unixNowSec() {
  time_t now = time(nullptr);
  if (now <= 0) return 0;
  return (uint32_t)now;
}

static void rememberSlotClear(Preferences &p, uint8_t idx) {
  p.remove(rememberHashKey(idx).c_str());
  p.remove(rememberExpKey(idx).c_str());
  p.remove(rememberLabelKey(idx).c_str());
}

static String sanitizeDeviceLabel(String label) {
  label.trim();
  String out;
  out.reserve(48);
  for (size_t i = 0; i < label.length() && out.length() < 48; i++) {
    char c = label.charAt(i);
    if (c >= 32 && c <= 126) out += c;
  }
  return out;
}

static void rememberCleanupExpired(Preferences &p, uint32_t nowSec) {
  if (nowSec == 0) return;
  for (uint8_t i = 0; i < kRememberTokenSlots; i++) {
    String h = p.getString(rememberHashKey(i).c_str(), "");
    if (h.isEmpty()) continue;
    uint32_t exp = (uint32_t)p.getULong(rememberExpKey(i).c_str(), 0);
    if (exp != 0 && exp <= nowSec) rememberSlotClear(p, i);
  }
}

static bool rememberValidateAndTouchHash(const String &tokenHash, String *outLabel = nullptr) {
  if (tokenHash.isEmpty()) return false;
  if (!prefs.begin(kPrefsNs, false)) return false;
  uint32_t nowSec = unixNowSec();
  rememberCleanupExpired(prefs, nowSec);
  bool ok = false;
  for (uint8_t i = 0; i < kRememberTokenSlots; i++) {
    String h = prefs.getString(rememberHashKey(i).c_str(), "");
    if (h.isEmpty() || h != tokenHash) continue;
    uint32_t exp = (uint32_t)prefs.getULong(rememberExpKey(i).c_str(), 0);
    if (nowSec != 0 && exp != 0 && exp <= nowSec) {
      rememberSlotClear(prefs, i);
      break;
    }
    if (outLabel) *outLabel = prefs.getString(rememberLabelKey(i).c_str(), "");
    if (nowSec != 0) {
      uint32_t nextExp = nowSec + kRememberTokenMaxAgeSec;
      if (exp == 0 || exp < (nowSec + (kRememberTokenMaxAgeSec / 2UL))) {
        prefs.putULong(rememberExpKey(i).c_str(), nextExp);
      }
    }
    ok = true;
    break;
  }
  prefs.end();
  return ok;
}

static bool rememberStoreToken(const String &token, const String &labelIn) {
  String norm = normalizeToken(token);
  if (norm.isEmpty()) return false;
  String hash = sha256Hex(norm);
  String label = sanitizeDeviceLabel(labelIn);

  if (!prefs.begin(kPrefsNs, false)) return false;
  uint32_t nowSec = unixNowSec();
  rememberCleanupExpired(prefs, nowSec);
  uint32_t exp = (nowSec == 0) ? 0 : (nowSec + kRememberTokenMaxAgeSec);

  int freeIdx = -1;
  int sameIdx = -1;
  int oldestIdx = -1;
  uint32_t oldestExp = 0xFFFFFFFFUL;
  for (uint8_t i = 0; i < kRememberTokenSlots; i++) {
    String h = prefs.getString(rememberHashKey(i).c_str(), "");
    if (h.isEmpty()) {
      if (freeIdx < 0) freeIdx = (int)i;
      continue;
    }
    if (h == hash) sameIdx = (int)i;
    uint32_t e = (uint32_t)prefs.getULong(rememberExpKey(i).c_str(), 0);
    uint32_t rank = (e == 0) ? 0xFFFFFFFEUL : e;
    if (rank < oldestExp) {
      oldestExp = rank;
      oldestIdx = (int)i;
    }
  }

  int idx = sameIdx;
  if (idx < 0) idx = (freeIdx >= 0) ? freeIdx : oldestIdx;
  if (idx < 0) {
    prefs.end();
    return false;
  }

  prefs.putString(rememberHashKey((uint8_t)idx).c_str(), hash);
  prefs.putULong(rememberExpKey((uint8_t)idx).c_str(), exp);
  prefs.putString(rememberLabelKey((uint8_t)idx).c_str(), label);
  prefs.end();
  return true;
}

static bool rememberRevokeHash(const String &tokenHash) {
  if (tokenHash.isEmpty()) return false;
  bool removed = false;
  if (!prefs.begin(kPrefsNs, false)) return false;
  for (uint8_t i = 0; i < kRememberTokenSlots; i++) {
    String h = prefs.getString(rememberHashKey(i).c_str(), "");
    if (h.isEmpty() || h != tokenHash) continue;
    rememberSlotClear(prefs, i);
    removed = true;
  }
  prefs.end();
  return removed;
}

static void rememberRevokeAll() {
  if (!prefs.begin(kPrefsNs, false)) return;
  for (uint8_t i = 0; i < kRememberTokenSlots; i++) rememberSlotClear(prefs, i);
  prefs.end();
}

static String cookieValue(const String &cookieHeader, const String &name) {
  if (cookieHeader.isEmpty() || name.isEmpty()) return "";
  int pos = 0;
  while (pos < (int)cookieHeader.length()) {
    while (pos < (int)cookieHeader.length()) {
      char c = cookieHeader.charAt(pos);
      if (c != ' ' && c != ';') break;
      pos++;
    }
    if (pos >= (int)cookieHeader.length()) break;
    int eq = cookieHeader.indexOf('=', pos);
    if (eq < 0) break;
    int end = cookieHeader.indexOf(';', eq + 1);
    if (end < 0) end = (int)cookieHeader.length();
    String key = cookieHeader.substring(pos, eq);
    key.trim();
    if (key == name) {
      String val = cookieHeader.substring(eq + 1, end);
      val.trim();
      return val;
    }
    pos = end + 1;
  }
  return "";
}

static String rememberCookieFromRequest() {
  String raw = server.header("Cookie");
  if (raw.isEmpty()) return "";
  return normalizeToken(cookieValue(raw, kRememberCookieName));
}

static String rememberCookieHeader(const String &token, bool clearCookie) {
  if (clearCookie) {
    return String(kRememberCookieName) + "=; Max-Age=0; Path=/; HttpOnly; SameSite=Strict";
  }
  return String(kRememberCookieName) + "=" + token + "; Max-Age=" + String((uint32_t)kRememberTokenMaxAgeSec) +
         "; Path=/; HttpOnly; SameSite=Strict";
}

static String requestDeviceLabel(const IPAddress &rip) {
  String ua = sanitizeDeviceLabel(server.header("User-Agent"));
  if (ua.length() > 32) ua = ua.substring(0, 32);
  String out = String(rip.toString()) + "|" + ua;
  out.trim();
  return sanitizeDeviceLabel(out);
}

static int hexNibble(char c) {
  if (c >= '0' && c <= '9') return (int)(c - '0');
  if (c >= 'a' && c <= 'f') return (int)(c - 'a' + 10);
  if (c >= 'A' && c <= 'F') return (int)(c - 'A' + 10);
  return -1;
}

static String percentDecodeLoose(const String &in) {
  String out;
  out.reserve(in.length());
  for (size_t i = 0; i < in.length(); i++) {
    char c = in.charAt(i);
    if (c == '%' && (i + 2) < in.length()) {
      int hi = hexNibble(in.charAt(i + 1));
      int lo = hexNibble(in.charAt(i + 2));
      if (hi >= 0 && lo >= 0) {
        out += (char)((hi << 4) | lo);
        i += 2;
        continue;
      }
    }
    out += c;
  }
  return out;
}

static bool requireCriticalPassword(const JsonDocument *doc = nullptr, const char *action = "critical") {
  String pass = server.header(kCriticalPassHeader);
  if (pass.isEmpty()) {
    String enc = server.header(kCriticalPassHeaderEnc);
    if (!enc.isEmpty()) pass = percentDecodeLoose(enc);
  }
  if (pass.isEmpty() && doc && !(*doc)["confirm_pass"].isNull()) {
    pass = String((const char *)(*doc)["confirm_pass"]);
  }
  if (pass != gWebAuthPass) {
    extractorCmdLogPushf("[security] critical auth fail action=%s ip=%s",
                         action ? action : "critical",
                         server.client().remoteIP().toString().c_str());
    server.sendHeader("Cache-Control", "no-store");
    server.send(401, "application/json", "{\"ok\":false,\"error\":\"confirm password required\"}");
    return false;
  }
  return true;
}

static String extractHostFromUrl(String url) {
  url.trim();
  int schemePos = url.indexOf("://");
  if (schemePos < 0) return "";
  int hostStart = schemePos + 3;
  int hostEnd = url.indexOf('/', hostStart);
  if (hostEnd < 0) hostEnd = (int)url.length();
  String hostPort = url.substring(hostStart, hostEnd);
  int atPos = hostPort.lastIndexOf('@');
  if (atPos >= 0) hostPort = hostPort.substring(atPos + 1);
  int colonPos = hostPort.lastIndexOf(':');
  if (colonPos > 0) hostPort = hostPort.substring(0, colonPos);
  hostPort.trim();
  return hostPort;
}

static void applyStaHostname() {
  esp_netif_t *sta = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
  if (!sta) {
    LB_SERIAL_PRINTLN("STA netif not ready for hostname");
    return;
  }
  esp_err_t err = esp_netif_set_hostname(sta, gStaHostname.c_str());
  if (err != ESP_OK) {
    LB_SERIAL_PRINTF("esp_netif_set_hostname failed: %d\n", (int)err);
  }
}

static void startMdnsForSta() {
  if (WiFi.status() != WL_CONNECTED) return;

  applyStaHostname();
  if (gMdnsStarted) {
    LB_SERIAL_PRINTF("mDNS URL: http://%s.local\n", gStaHostname.c_str());
    return;
  }

  esp_err_t err = mdns_init();
  if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
    LB_SERIAL_PRINTF("mDNS init failed: %d\n", (int)err);
    return;
  }

  err = mdns_hostname_set(gStaHostname.c_str());
  if (err != ESP_OK) {
    LB_SERIAL_PRINTF("mDNS hostname set failed: %d\n", (int)err);
    return;
  }

  String instance = String("LB Legacy Bridge ") + gMacSuffixUpper;
  err = mdns_instance_name_set(instance.c_str());
  if (err != ESP_OK) {
    LB_SERIAL_PRINTF("mDNS instance set failed: %d\n", (int)err);
  }

  mdns_txt_item_t txt[] = {
    {"id", gMdnsIdTxt},
    {"fw", kFwVersion},
    {"profile", kFwProfile},
    {"model", "legacy-bridge"},
  };
  err = mdns_service_add(NULL, "_http", "_tcp", 80, txt, sizeof(txt) / sizeof(txt[0]));
  if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
    LB_SERIAL_PRINTF("mDNS service add failed: %d\n", (int)err);
    return;
  }

  gMdnsStarted = true;
  LB_SERIAL_PRINTF("mDNS URL: http://%s.local\n", gStaHostname.c_str());
}

static void loadConfig() {
  prefs.begin(kPrefsNs, true);
  wifiSsid = prefs.getString("wifi_ssid", "");
  wifiPass = prefs.getString("wifi_pass", "");
  bleNamePrefix = prefs.getString("ble_pref", "TES02_");
  bleLockedAddr = prefs.getString("ble_addr", "");
  bleLockedName = prefs.getString("ble_name", "");
  bleBound = prefs.getBool("ble_bound", false);
  uint8_t legacySpeed = clampSpeedInt((int)prefs.getUChar("last_speed", 30));
  gSourceLastSpeed[SRC_H1] = clampSpeedInt((int)prefs.getUChar(extractorSourceNvsKey(SRC_H1), legacySpeed));
  gSourceLastSpeed[SRC_H2] = clampSpeedInt((int)prefs.getUChar(extractorSourceNvsKey(SRC_H2), legacySpeed));
  gSourceLastSpeed[SRC_HA1] = clampSpeedInt((int)prefs.getUChar(extractorSourceNvsKey(SRC_HA1), legacySpeed));
  gSourceLastSpeed[SRC_HA2] = clampSpeedInt((int)prefs.getUChar(extractorSourceNvsKey(SRC_HA2), legacySpeed));
  gSourceLastSpeed[SRC_HA3] = clampSpeedInt((int)prefs.getUChar(extractorSourceNvsKey(SRC_HA3), legacySpeed));
  gSourceLightMode[SRC_H1] = (uint8_t)prefs.getUChar(extractorSourceLightNvsKey(SRC_H1), LIGHT_OFF);
  gSourceLightMode[SRC_H2] = (uint8_t)prefs.getUChar(extractorSourceLightNvsKey(SRC_H2), LIGHT_OFF);
  gSourceLightMode[SRC_HA1] = (uint8_t)prefs.getUChar(extractorSourceLightNvsKey(SRC_HA1), LIGHT_OFF);
  gSourceLightMode[SRC_HA2] = (uint8_t)prefs.getUChar(extractorSourceLightNvsKey(SRC_HA2), LIGHT_OFF);
  gSourceLightMode[SRC_HA3] = (uint8_t)prefs.getUChar(extractorSourceLightNvsKey(SRC_HA3), LIGHT_OFF);
  uint8_t legacyLastMode = normalizeLastEnabledLightMode((uint8_t)prefs.getUChar("lt_last", LIGHT_WHITE));
  gSourceLastEnabledLightMode[SRC_H1] =
      normalizeLastEnabledLightMode((uint8_t)prefs.getUChar(extractorSourceLastModeNvsKey(SRC_H1), legacyLastMode));
  gSourceLastEnabledLightMode[SRC_H2] =
      normalizeLastEnabledLightMode((uint8_t)prefs.getUChar(extractorSourceLastModeNvsKey(SRC_H2), legacyLastMode));
  gSourceLastEnabledLightMode[SRC_HA1] =
      normalizeLastEnabledLightMode((uint8_t)prefs.getUChar(extractorSourceLastModeNvsKey(SRC_HA1), legacyLastMode));
  gSourceLastEnabledLightMode[SRC_HA2] =
      normalizeLastEnabledLightMode((uint8_t)prefs.getUChar(extractorSourceLastModeNvsKey(SRC_HA2), legacyLastMode));
  gSourceLastEnabledLightMode[SRC_HA3] =
      normalizeLastEnabledLightMode((uint8_t)prefs.getUChar(extractorSourceLastModeNvsKey(SRC_HA3), legacyLastMode));
  gSourceBrightnessMode[SRC_H1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessModeNvsKey(SRC_H1), BRI_LAST);
  gSourceBrightnessMode[SRC_H2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessModeNvsKey(SRC_H2), BRI_LAST);
  gSourceBrightnessMode[SRC_HA1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessModeNvsKey(SRC_HA1), BRI_LAST);
  gSourceBrightnessMode[SRC_HA2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessModeNvsKey(SRC_HA2), BRI_LAST);
  gSourceBrightnessMode[SRC_HA3] = (uint8_t)prefs.getUChar(extractorSourceBrightnessModeNvsKey(SRC_HA3), BRI_LAST);
  gSourceManualBrightness[SRC_H1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessManualNvsKey(SRC_H1), 60);
  gSourceManualBrightness[SRC_H2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessManualNvsKey(SRC_H2), 60);
  gSourceManualBrightness[SRC_HA1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessManualNvsKey(SRC_HA1), 60);
  gSourceManualBrightness[SRC_HA2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessManualNvsKey(SRC_HA2), 60);
  gSourceManualBrightness[SRC_HA3] = (uint8_t)prefs.getUChar(extractorSourceBrightnessManualNvsKey(SRC_HA3), 60);
  gSourceLastBrightness[SRC_H1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessLastNvsKey(SRC_H1), 60);
  gSourceLastBrightness[SRC_H2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessLastNvsKey(SRC_H2), 60);
  gSourceLastBrightness[SRC_HA1] = (uint8_t)prefs.getUChar(extractorSourceBrightnessLastNvsKey(SRC_HA1), 60);
  gSourceLastBrightness[SRC_HA2] = (uint8_t)prefs.getUChar(extractorSourceBrightnessLastNvsKey(SRC_HA2), 60);
  gSourceLastBrightness[SRC_HA3] = (uint8_t)prefs.getUChar(extractorSourceBrightnessLastNvsKey(SRC_HA3), 60);
  wifiAutoOffMin = (uint16_t)prefs.getUShort("wifi_autooff", 0);
  h312Enabled = prefs.getBool("h312_en", false);
  h312Mode = (H312TransportMode)prefs.getUChar("h312_mode", (uint8_t)H312_MODE_WIFI);
  h312Ip = prefs.getString("h312_ip", "");
  h312BleAddr = prefs.getString("h312_baddr", "");
  h312BleName = prefs.getString("h312_bname", "");
  h312Port = (uint16_t)prefs.getUShort("h312_port", 6666);
  h312TempOn = (int16_t)prefs.getShort("h312_ton", 140);
  h312TempOff = (int16_t)prefs.getShort("h312_toff", 90);
  h312DebounceMs = (uint16_t)prefs.getUShort("h312_dbms", 1200);
  h312MinOnMs = (uint32_t)prefs.getULong("h312_minon", 5000);
  h312Slot = (uint8_t)prefs.getUChar("h312_slot", 1);
  (void)prefs.getChar("t4_h1gp", -1);
  (void)prefs.getChar("t4_h2gp", -1);
  t420dH1RaisedOnHigh = prefs.getBool("t4_h1_rhi", true);
  t420dH2RaisedOnHigh = prefs.getBool("t4_h2_rhi", true);
  t420dLogicEnabled = prefs.getBool("t4_logic_en", true);
  t420dH1OnDelayMs = (uint32_t)prefs.getULong("t4_h1on_ms", 1000);
  t420dH2OnDelayMs = (uint32_t)prefs.getULong("t4_h2on_ms", 1000);
  t420dH1OffDelayMs = (uint32_t)prefs.getULong("t4_h1off_ms", 20000);
  t420dH2OffDelayMs = (uint32_t)prefs.getULong("t4_h2off_ms", 20000);
  t420dH1Gpio = kT420dHandle1GpioFixed;
  t420dH2Gpio = kT420dHandle2GpioFixed;
  tzConfigured = prefs.getBool("tz2_set", false);
  tzOffsetMin = clampTzOffsetMin((int)prefs.getShort("tz2_off_min", 0));
  if (prefs.isKey("tz2_name")) tzName = normalizeTzName(prefs.getString("tz2_name", "UTC"));
  else tzName = "UTC";
  autoStopEnabled = prefs.getBool("auto_stop_en", false);
  autoStopMinute = (uint16_t)prefs.getUShort("auto_stop_min", 0);
  gWebAuthUser = prefs.getString("web_user", kWebAuthUserDefault);
  gWebAuthUser.trim();
  if (!isValidWebUser(gWebAuthUser)) gWebAuthUser = kWebAuthUserDefault;
  gWebAuthPass = prefs.getString("web_pass", "");
  prefs.end();

  bool authChanged = false;
  if (gWebAuthPass.length() < 8 || gWebAuthPass == "admin" || gWebAuthPass == "12345678") {
    gWebAuthPass = buildDeviceSecret();
    authChanged = true;
    extractorCmdLogPushf("[security] web auth secret initialized");
  }
  if (!prefs.begin(kPrefsNs, false)) {
  } else {
    if (authChanged) prefs.putString("web_pass", gWebAuthPass);
    if (!prefs.isKey("web_user")) {
      prefs.putString("web_user", gWebAuthUser);
      authChanged = true;
    }
    prefs.end();
  }
  gApPass = gWebAuthPass;

  // Migration for old builds that did not have ble_bound key.
  if (!bleBound && (!bleLockedAddr.isEmpty() || !bleLockedName.isEmpty())) {
    bleBound = true;
  }

  if (h312Port == 0) h312Port = 6666;
  if (h312TempOn < -40 || h312TempOn > 600) h312TempOn = 140;
  if (h312TempOff < -40 || h312TempOff > 600) h312TempOff = 90;
  if (h312TempOn <= h312TempOff) h312TempOff = h312TempOn - 10;
  if (h312DebounceMs > 60000) h312DebounceMs = 60000;
  if (h312MinOnMs > 300000UL) h312MinOnMs = 300000UL;
  if (h312Slot < 1 || h312Slot > 3) h312Slot = 1;
  if (h312Mode != H312_MODE_BLE) h312Mode = H312_MODE_WIFI;
  t420dH1OnDelayMs = clampDelayMs(t420dH1OnDelayMs);
  t420dH2OnDelayMs = clampDelayMs(t420dH2OnDelayMs);
  t420dH1OffDelayMs = clampDelayMs(t420dH1OffDelayMs);
  t420dH2OffDelayMs = clampDelayMs(t420dH2OffDelayMs);
  h312BleAddr.trim();
  h312BleName.trim();
  if (!h312BleAddrLooksValid(h312BleAddr)) h312BleAddr = "";
  if (t420dH1Gpio != kT420dHandle1GpioFixed || t420dH2Gpio != kT420dHandle2GpioFixed) {
    t420dH1Gpio = kT420dHandle1GpioFixed;
    t420dH2Gpio = kT420dHandle2GpioFixed;
  }
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) {
    if (gSourceLightMode[i] > LIGHT_LAST) gSourceLightMode[i] = LIGHT_OFF;
    if (gSourceBrightnessMode[i] > BRI_MANUAL) gSourceBrightnessMode[i] = BRI_LAST;
    gSourceManualBrightness[i] = clampBrightnessInt((int)gSourceManualBrightness[i]);
    gSourceLastBrightness[i] = clampBrightnessInt((int)gSourceLastBrightness[i]);
  }
  if (autoStopMinute > 1439) autoStopMinute = 0;

  setActiveSource(SRC_H1);
}

static void bootHealthCheckInit() {
  prefs.begin(kPrefsNs, false);

  // Boot-loop detection: previous boot must mark itself OK; otherwise count as failed.
  bool prevOk = prefs.getBool("boot_ok", true);
  uint32_t bootCnt = prefs.getULong("boot_cnt", 0);
  uint8_t bootFail = (uint8_t)prefs.getUChar("boot_fail", 0);
  bool forceRecovery = prefs.getBool("force_recovery", false);

  bootCnt++;
  if (!prevOk && bootFail < 250) bootFail++;

  prefs.putULong("boot_cnt", bootCnt);
  prefs.putUChar("boot_fail", bootFail);
  prefs.putBool("boot_ok", false);

  if (forceRecovery) {
    gSafeMode = true;
    gSafeCause = "Recovery requested";
  } else if (bootFail >= 3) {
    gSafeMode = true;
    gSafeCause = "Boot loop detected";
  }

  prefs.end();
}

static void bootHealthMarkOk() {
  prefs.begin(kPrefsNs, false);
  prefs.putBool("boot_ok", true);
  prefs.putUChar("boot_fail", 0);
  prefs.end();
}

static void saveWifi(const String &ssid, const String &pass) {
  prefs.begin(kPrefsNs, false);
  prefs.putString("wifi_ssid", ssid);
  prefs.putString("wifi_pass", pass);
  prefs.end();
  wifiSsid = ssid;
  wifiPass = pass;
}

static void saveBle(const String &namePrefix, const String &lockedAddr, const String &lockedName = "", bool bound = true) {
  prefs.begin(kPrefsNs, false);
  prefs.putString("ble_pref", namePrefix);
  prefs.putString("ble_addr", lockedAddr);
  prefs.putString("ble_name", lockedName);
  prefs.putBool("ble_bound", bound);
  prefs.end();
  bleNamePrefix = namePrefix;
  bleLockedAddr = lockedAddr;
  bleLockedName = lockedName;
  bleBound = bound;
  gLockedBackoffUntilMs = 0;
  bleApplyDriverConfig();
}

static void bleSyncRuntimeFromDriver() {
  const tes02::RuntimeState &st = gTes02Driver.state();
  gBleLastAddr = st.lastAddr;
  gBleLastName = st.lastName;
  gBleLastRssi = st.lastRssi;
  gBleConnectSource = st.connectSource;
  gBleLastTx = st.lastTx;
  gBleLastSetBefore = st.lastSetBefore;
  gBleLastSetAfter = st.lastSetAfter;
  gBleLastSetTarget = st.lastSetTarget;
  gBleLastSetMs = st.lastSetMs;
  gBleConnectedAtMs = st.connectedAtMs;
  gBleLastActivityMs = st.lastActivityMs;
}

static void bleApplyDriverConfig() {
  tes02::Config cfg;
  cfg.prefix = bleNamePrefix;
  cfg.lockedAddr = bleLockedAddr;
  cfg.lockedName = bleLockedName;
  cfg.bound = bleBound;
  gTes02Driver.setConfig(cfg);
  gTes02Driver.setSafeMode(gSafeMode);
}

static void bleSetConnectSource(const String &src) {
  gTes02Driver.setConnectSource(src);
  gBleConnectSource = src;
}

static bool bleEnsureConnectedCallback(void *ctx, uint32_t scanMs) {
  (void)ctx;
  return bleEnsureConnected(scanMs);
}

static void bleDriverLogCb(void *ctx, const char *line) {
  (void)ctx;
  if (!line || !line[0]) return;
  extractorCmdLogPushf("%s", line);
}

static void saveWifiAutoOffMin(uint16_t v) {
  prefs.begin(kPrefsNs, false);
  prefs.putUShort("wifi_autooff", v);
  prefs.end();
  wifiAutoOffMin = v;
}

static void saveSourceSpeed(ExtractorSource src, uint8_t spd) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  spd = clampSpeedInt((int)spd);
  if (gSourceLastSpeed[idx] == spd) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceNvsKey(src), spd);
  // Keep legacy key for backward diagnostics/compatibility.
  prefs.putUChar("last_speed", spd);
  prefs.end();
  gSourceLastSpeed[idx] = spd;
  if (gActiveSource == src) lastOnSpeed = spd;
}

static void saveSourceLightMode(ExtractorSource src, uint8_t mode) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  if (mode > LIGHT_LAST) mode = LIGHT_OFF;
  if (gSourceLightMode[idx] == mode) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceLightNvsKey(src), mode);
  prefs.end();
  gSourceLightMode[idx] = mode;
}

static void saveSourceLastEnabledLightMode(ExtractorSource src, uint8_t mode) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  mode = normalizeLastEnabledLightMode(mode);
  if (gSourceLastEnabledLightMode[idx] == mode) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceLastModeNvsKey(src), mode);
  // Keep legacy key for backward compatibility with old builds.
  prefs.putUChar("lt_last", mode);
  prefs.end();
  gSourceLastEnabledLightMode[idx] = mode;
}

static void saveSourceBrightnessMode(ExtractorSource src, uint8_t mode) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  if (mode > BRI_MANUAL) mode = BRI_LAST;
  if (gSourceBrightnessMode[idx] == mode) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceBrightnessModeNvsKey(src), mode);
  prefs.end();
  gSourceBrightnessMode[idx] = mode;
}

static void saveSourceManualBrightness(ExtractorSource src, uint8_t val) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  val = clampBrightnessInt((int)val);
  if (gSourceManualBrightness[idx] == val) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceBrightnessManualNvsKey(src), val);
  prefs.end();
  gSourceManualBrightness[idx] = val;
}

static void saveSourceLastBrightness(ExtractorSource src, uint8_t val) {
  uint8_t idx = (uint8_t)src;
  if (idx >= (uint8_t)SRC_COUNT) return;
  val = clampBrightnessInt((int)val);
  if (gSourceLastBrightness[idx] == val) return;
  prefs.begin(kPrefsNs, false);
  prefs.putUChar(extractorSourceBrightnessLastNvsKey(src), val);
  prefs.end();
  gSourceLastBrightness[idx] = val;
}

static void saveH312Config(bool enabled, H312TransportMode mode, const String &ip, const String &bleAddr, const String &bleName,
                           uint16_t port, int16_t tOn, int16_t tOff, uint16_t debounceMs, uint32_t minOnMs, uint8_t slot) {
  prefs.begin(kPrefsNs, false);
  prefs.putBool("h312_en", enabled);
  prefs.putUChar("h312_mode", (uint8_t)mode);
  prefs.putString("h312_ip", ip);
  prefs.putString("h312_baddr", bleAddr);
  prefs.putString("h312_bname", bleName);
  prefs.putUShort("h312_port", port);
  prefs.putShort("h312_ton", tOn);
  prefs.putShort("h312_toff", tOff);
  prefs.putUShort("h312_dbms", debounceMs);
  prefs.putULong("h312_minon", minOnMs);
  prefs.putUChar("h312_slot", slot);
  prefs.end();

  h312Enabled = enabled;
  h312Mode = mode;
  h312Ip = ip;
  h312BleAddr = bleAddr;
  h312BleName = bleName;
  h312Port = port;
  h312TempOn = tOn;
  h312TempOff = tOff;
  h312DebounceMs = debounceMs;
  h312MinOnMs = minOnMs;
  h312Slot = (slot < 1 || slot > 3) ? 1 : slot;
  gH312BleRuntimeAddr = "";
  gH312BleRuntimeAddrType = -1;
  gH312BleNextPollMs = 0;
  gH312BleFailStreak = 0;
  h312ApplyDriverConfig();
}

static void saveT420dGpioConfig(int8_t h1, int8_t h2) {
  (void)h1;
  (void)h2;
  h1 = kT420dHandle1GpioFixed;
  h2 = kT420dHandle2GpioFixed;
  prefs.begin(kPrefsNs, false);
  prefs.putChar("t4_h1gp", h1);
  prefs.putChar("t4_h2gp", h2);
  prefs.end();
  t420dH1Gpio = h1;
  t420dH2Gpio = h2;
}

static void saveT420dSensePolarity(bool h1RaisedHigh, bool h2RaisedHigh) {
  prefs.begin(kPrefsNs, false);
  prefs.putBool("t4_h1_rhi", h1RaisedHigh);
  prefs.putBool("t4_h2_rhi", h2RaisedHigh);
  prefs.end();
  t420dH1RaisedOnHigh = h1RaisedHigh;
  t420dH2RaisedOnHigh = h2RaisedHigh;
}

static void saveT420dLogicConfig(bool enabled, uint32_t h1OnMs, uint32_t h2OnMs, uint32_t h1OffMs, uint32_t h2OffMs) {
  h1OnMs = clampDelayMs(h1OnMs);
  h2OnMs = clampDelayMs(h2OnMs);
  h1OffMs = clampDelayMs(h1OffMs);
  h2OffMs = clampDelayMs(h2OffMs);
  prefs.begin(kPrefsNs, false);
  prefs.putBool("t4_logic_en", enabled);
  prefs.putULong("t4_h1on_ms", h1OnMs);
  prefs.putULong("t4_h2on_ms", h2OnMs);
  prefs.putULong("t4_h1off_ms", h1OffMs);
  prefs.putULong("t4_h2off_ms", h2OffMs);
  prefs.end();
  t420dLogicEnabled = enabled;
  t420dH1OnDelayMs = h1OnMs;
  t420dH2OnDelayMs = h2OnMs;
  t420dH1OffDelayMs = h1OffMs;
  t420dH2OffDelayMs = h2OffMs;
  gT420dPendingValid = false;
}

static int readStableGpioLevel(int8_t gpio, uint16_t sampleMs, uint16_t stepMs) {
  if (gpio < 0) return -1;
  pinMode((uint8_t)gpio, INPUT);
  uint16_t hi = 0;
  uint16_t lo = 0;
  uint16_t leftMs = sampleMs;
  while (leftMs > 0) {
    int v = digitalRead((uint8_t)gpio);
    if (v == HIGH) hi++;
    else lo++;
    uint16_t d = (leftMs < stepMs) ? leftMs : stepMs;
    delay(d);
    leftMs -= d;
  }
  if (hi == 0 && lo == 0) return -1;
  return (hi >= lo) ? HIGH : LOW;
}

static int readHandleGpioLevelNow(int8_t gpio) {
  if (gpio < 0) return -1;
  pinMode((uint8_t)gpio, INPUT);
  return digitalRead((uint8_t)gpio);
}

static int readHandleGpioAdcRawNow(int8_t gpio) {
  if (gpio < 0) return -1;
  return analogRead((uint8_t)gpio);
}

static bool isHandleRaisedFromLevel(int8_t gpio, int level);

static int readHandleGpioMilliVoltsNow(int8_t gpio) {
  if (gpio < 0) return -1;
  int mv = analogReadMilliVolts((uint8_t)gpio);
  if (mv < 0) return -1;
  return mv;
}

static bool readHandleRaisedHysteresis(int8_t gpio, bool prevRaised) {
  static uint32_t lastChangeH1 = 0;
  static uint32_t lastChangeH2 = 0;
  static bool stableRaisedH1 = false;
  static bool stableRaisedH2 = false;
  
  if (gpio < 0) return prevRaised;

  pinMode((uint8_t)gpio, INPUT);
  int rawLevel = digitalRead((uint8_t)gpio);
  bool rawRaised = isHandleRaisedFromLevel(gpio, rawLevel);

  // Keep independent debounce state per logical handle, not hardcoded GPIO number.
  uint32_t *lastChange = nullptr;
  bool *stableRaised = nullptr;
  if (gpio == t420dH1Gpio) {
    lastChange = &lastChangeH1;
    stableRaised = &stableRaisedH1;
  } else if (gpio == t420dH2Gpio) {
    lastChange = &lastChangeH2;
    stableRaised = &stableRaisedH2;
  } else {
    return prevRaised;
  }
  uint32_t now = millis();

  if (rawRaised != *stableRaised && (uint32_t)(now - *lastChange) > 50) {
      *stableRaised = rawRaised;
      *lastChange = now;
  }

  return *stableRaised;
}

static void handleGpioEdgeDiagLog(const char *name, int8_t gpio, int level, uint32_t nowMs, uint32_t *ioLastEdgeMs,
                                  uint32_t *ioBurstWinMs, uint8_t *ioBurst) {
  if (!name || gpio < 0 || !ioLastEdgeMs || !ioBurstWinMs || !ioBurst) return;
  if (*ioBurstWinMs == 0 || (uint32_t)(nowMs - *ioBurstWinMs) > kHandleBounceWindowMs) {
    *ioBurstWinMs = nowMs;
    *ioBurst = 0;
  }
  if (*ioBurst < 250) (*ioBurst)++;
  uint32_t dt = (*ioLastEdgeMs == 0) ? 0 : (uint32_t)(nowMs - *ioLastEdgeMs);
  *ioLastEdgeMs = nowMs;
  int mv = readHandleGpioMilliVoltsNow(gpio);
  int raw = readHandleGpioAdcRawNow(gpio);
  extractorCmdLogPushf("[gpio] %s edge gpio=%d lvl=%d mv=%d raw=%d dt=%lums burst=%u", name, (int)gpio, level, mv, raw,
                       (unsigned long)dt, (unsigned int)(*ioBurst));
  if (*ioBurst >= kHandleBounceBurstWarn) {
    extractorCmdLogPushf("[gpio] %s bounce? gpio=%d burst=%u/%lums", name, (int)gpio, (unsigned int)(*ioBurst),
                         (unsigned long)kHandleBounceWindowMs);
  }
}

static void handleGpioDiagLoop(uint32_t nowMs) {
  if ((uint32_t)(nowMs - gHandleDiagLastMs) < kHandleDiagSampleMs) return;
  gHandleDiagLastMs = nowMs;

  int h1 = readHandleGpioLevelNow(t420dH1Gpio);
  int h2 = readHandleGpioLevelNow(t420dH2Gpio);

  if (gHandleDiagPrevH1 == -2) gHandleDiagPrevH1 = h1;
  else if ((h1 == HIGH || h1 == LOW) && h1 != gHandleDiagPrevH1) {
    handleGpioEdgeDiagLog("h1", t420dH1Gpio, h1, nowMs, &gHandleDiagLastEdgeH1Ms, &gHandleDiagBurstWinH1Ms, &gHandleDiagBurstH1);
    gHandleDiagPrevH1 = h1;
  }

  if (gHandleDiagPrevH2 == -2) gHandleDiagPrevH2 = h2;
  else if ((h2 == HIGH || h2 == LOW) && h2 != gHandleDiagPrevH2) {
    handleGpioEdgeDiagLog("h2", t420dH2Gpio, h2, nowMs, &gHandleDiagLastEdgeH2Ms, &gHandleDiagBurstWinH2Ms, &gHandleDiagBurstH2);
    gHandleDiagPrevH2 = h2;
  }
}

static bool isHandleRaisedFromLevel(int8_t gpio, int level) {
  if (level != HIGH && level != LOW) return false;
  bool raisedOnHigh = kHandleGpioActiveHigh;
  if (gpio == t420dH1Gpio) raisedOnHigh = t420dH1RaisedOnHigh;
  else if (gpio == t420dH2Gpio) raisedOnHigh = t420dH2RaisedOnHigh;
  return raisedOnHigh ? (level == HIGH) : (level == LOW);
}

static bool bootRecoveryByDualHandleGpio() {
  if (t420dH1Gpio < 0 || t420dH2Gpio < 0) return false;
  if (t420dH1Gpio == t420dH2Gpio) return false;

  int h1 = readStableGpioLevel(t420dH1Gpio, kRecoveryHandleSampleMs, kRecoveryHandleSampleStepMs);
  int h2 = readStableGpioLevel(t420dH2Gpio, kRecoveryHandleSampleMs, kRecoveryHandleSampleStepMs);
  int activeLevel = kHandleGpioActiveHigh ? HIGH : LOW;
  bool raised = (h1 == activeLevel && h2 == activeLevel);
  if (raised) {
    extractorCmdLogPushf("[recovery] dual-handle trigger h1=gpio%d h2=gpio%d", (int)t420dH1Gpio, (int)t420dH2Gpio);
  }
  return raised;
}

static bool isRecoveryUnlockUri(const String &uri) {
  return uri == "/recovery-min" || uri == "/recovery-min/" || uri == "/recovery" || uri == "/recovery/" ||
         uri == "/api/fw/update_manual" || uri == "/api/fw/layout" || uri == "/api/fw/verify" || uri == "/api/reboot";
}

static bool isRecoveryUnlockActive(uint32_t nowMs) {
  if (!gRecoveryUnlockByHandles || gRecoveryUnlockUntilMs == 0) return false;
  return (int32_t)(gRecoveryUnlockUntilMs - nowMs) > 0;
}

static bool canServeRecoveryUi() {
  if (gSafeMode) return true;
  return isRecoveryUnlockActive((uint32_t)millis());
}

static bool isRecoveryUiAssetPath(const String &uriIn) {
  String uri = uriIn;
  if (uri.endsWith("/")) uri += "index.html";
  if (!uri.startsWith("/")) uri = "/" + uri;
  if (uri == "/recovery.html" || uri == "/recovery.js" || uri == "/recovery/index.html") return true;
  if (uri.startsWith("/recovery/")) return true;
  return false;
}

static bool wifiConnectSta(uint32_t timeoutMs = 15000, bool keepAp = false) {
  if (wifiSsid.isEmpty()) return false;

  WiFi.mode(keepAp ? WIFI_AP_STA : WIFI_STA);
  applyStaHostname();
  WiFi.begin(wifiSsid.c_str(), wifiPass.c_str());
  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < timeoutMs) {
    delay(200);
  }
  bool ok = WiFi.status() == WL_CONNECTED;
  if (ok) startMdnsForSta();
  return ok;
}

static void wifiStartAp() {
  // Keep STA enabled while running AP so Wi-Fi scanning works during onboarding.
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(gApSsid.c_str(), gApPass.c_str());
}

static bool wifiApIsUp() {
  wifi_mode_t m = WiFi.getMode();
  if (m != WIFI_AP && m != WIFI_AP_STA) return false;
  return ((uint32_t)WiFi.softAPIP()) != 0;
}

static void wifiEnsureFailsafeAp(const char *reason, bool pin = true) {
  bool wasUp = wifiApIsUp();
  if (!wasUp) wifiStartAp();
  if (pin) gFailsafeApPinned = true;
  if (!wasUp || pin) {
    extractorCmdLogPushf("[wifi] failsafe AP on reason=%s ssid=%s", reason ? reason : "unknown", gApSsid.c_str());
  }
}

static void wifiEnforceStaOnlyIfConnected() {
  // Recovery/safe mode must keep AP available even when STA is connected.
  if (gSafeMode) return;
  if (gFailsafeApPinned) return;
  if (WiFi.status() != WL_CONNECTED) return;
  wifi_mode_t m = WiFi.getMode();
  if (m != WIFI_AP_STA) return;
  // STA is up: shut down AP side and keep station-only mode.
  WiFi.softAPdisconnect(true);
  delay(20);
  WiFi.mode(WIFI_STA);
  applyStaHostname();
  startMdnsForSta();
}

static void wifiFailsafeLoop(uint32_t nowMs) {
  if (gFailsafeApPinned && gFailsafeApPinUntilMs != 0) {
    int32_t left = (int32_t)(gFailsafeApPinUntilMs - nowMs);
    if (left <= 0) {
      gFailsafeApPinned = false;
      gFailsafeApPinUntilMs = 0;
      extractorCmdLogPushf("[wifi] failsafe AP pin released");
    }
  }

  if (gSafeMode || wifiSsid.isEmpty()) {
    gStaLostSinceMs = 0;
    return;
  }

  wifi_mode_t m = WiFi.getMode();
  bool staConnected = (WiFi.status() == WL_CONNECTED);
  if (m == WIFI_STA && !staConnected) {
    if (gStaLostSinceMs == 0) {
      gStaLostSinceMs = nowMs;
    } else if ((uint32_t)(nowMs - gStaLostSinceMs) >= kFailsafeStaLostToApMs) {
      wifiEnsureFailsafeAp("sta_lost", true);
      gFailsafeApPinUntilMs = 0;
    }
  } else {
    gStaLostSinceMs = 0;
  }

  const bool h312BleActive = (h312Enabled && h312Mode == H312_MODE_BLE);
  if (!gFailsafeApPinned && !h312BleActive && gPerfLoopMaxUs >= kFailsafeServiceStallUs) {
    wifiEnsureFailsafeAp("service_stall", true);
    gFailsafeApPinUntilMs = 0;
  }
}

static void bleResetHandles() {
  gTes02Driver.resetHandles();
  bleSyncRuntimeFromDriver();
}

static bool bleFindBestByPrefix(const String &prefix, uint32_t scanMs, NimBLEAddress *outAddr, String *outName,
                                int *outRssi) {
  bool ok = gTes02Driver.findBestByPrefix(prefix, scanMs, outAddr, outName, outRssi);
  bleSyncRuntimeFromDriver();
  return ok;
}

static bool bleConnectTo(const NimBLEAddress &addr) {
  bool ok = gTes02Driver.connectTo(addr);
  bleSyncRuntimeFromDriver();
  return ok;
}

static bool bleReady() {
  return gTes02Driver.ready();
}

static int bleReadSpeedDirect() {
  int v = gTes02Driver.readSpeedDirect();
  bleSyncRuntimeFromDriver();
  return v;
}

static int bleReadSpeedStable(uint8_t reads = 2, uint16_t gapMs = 120) {
  int v = gTes02Driver.readSpeedStable(reads, gapMs);
  bleSyncRuntimeFromDriver();
  return v;
}

static int bleReadLightSelDirect() {
  int v = gTes02Driver.readLightSelDirect();
  bleSyncRuntimeFromDriver();
  return v;
}

static int bleReadLightSelStable(uint8_t reads = 2, uint16_t gapMs = 120) {
  int v = gTes02Driver.readLightSelStable(reads, gapMs);
  bleSyncRuntimeFromDriver();
  return v;
}

static int bleReadLightBrightnessDirect() {
  int v = gTes02Driver.readLightBrightnessDirect();
  bleSyncRuntimeFromDriver();
  return v;
}

static int bleReadLightBrightnessStable(uint8_t reads = 2, uint16_t gapMs = 120) {
  int v = gTes02Driver.readLightBrightnessStable(reads, gapMs);
  bleSyncRuntimeFromDriver();
  return v;
}

static uint8_t lightModeToSelector(uint8_t mode) {
  if (mode == LIGHT_WHITE) return 0;
  if (mode == LIGHT_GREEN) return 1;
  return 2;
}

static uint8_t effectiveLightModeForActivation(uint8_t mode, ExtractorSource src) {
  if (mode == LIGHT_LAST) return sourceLastEnabledLightMode(src);
  if (mode == LIGHT_WHITE || mode == LIGHT_GREEN) return mode;
  return LIGHT_OFF;
}

static bool bleSetLightMode(uint8_t mode) {
  if (mode > LIGHT_LAST) mode = LIGHT_OFF;
  uint8_t appliedMode = effectiveLightModeForActivation(mode, gActiveSource);
  uint8_t targetSel = lightModeToSelector(appliedMode);
  bool ok = gTes02Driver.setLightSelector(targetSel, lightModeText(mode), lightModeText(appliedMode), extractorSourceText(gActiveSource));
  bleSyncRuntimeFromDriver();
  if (ok && (appliedMode == LIGHT_WHITE || appliedMode == LIGHT_GREEN)) saveSourceLastEnabledLightMode(gActiveSource, appliedMode);
  return ok;
}

static bool bleSetLightBrightness(uint8_t target, ExtractorSource src) {
  target = clampBrightnessInt((int)target);
  bool ok = gTes02Driver.setBrightness(target, brightnessModeText(sourceBrightnessMode(src)), extractorSourceText(src));
  bleSyncRuntimeFromDriver();
  if (ok) saveSourceLastBrightness(src, target);
  return ok;
}

static bool readCurrentLightFromDevice(uint8_t *outSel, uint8_t *outBri) {
  if (!bleReady() && !bleEnsureConnected(1800)) return false;
  uint8_t sel = 0;
  uint8_t bri = 0;
  if (!gTes02Driver.readLightState(&sel, &bri)) return false;
  bleSyncRuntimeFromDriver();
  if (outSel) *outSel = sel;
  if (outBri) *outBri = clampBrightnessInt((int)bri);
  return true;
}

static void captureLastEnabledLightFromDevice(ExtractorSource src) {
  uint8_t sel = 0;
  uint8_t bri = 0;
  if (!readCurrentLightFromDevice(&sel, &bri)) return;
  uint8_t m = selectorToLightMode(sel);
  if (m == LIGHT_WHITE || m == LIGHT_GREEN) saveSourceLastEnabledLightMode(src, m);
  saveSourceLastBrightness(src, bri);
  extractorCmdLogPushf("capture light sel=%u bri=%u src=%s", (unsigned int)sel, (unsigned int)bri, extractorSourceText(src));
}

static void blePrimeFreshSession(uint16_t minAgeMs = 1400) {
  gTes02Driver.primeFreshSession(minAgeMs);
  bleSyncRuntimeFromDriver();
}

static bool bleEnsureConnected(uint32_t scanMs = 4000) {
  bleApplyDriverConfig();
  if (gSafeMode) return false;
  if (!bleBound) {
    bleSetConnectSource("not_bound");
    if (kUartDiagEnabled) LB_SERIAL_PRINTLN("[ble] ensure: not bound");
    return false;
  }
  if (bleReady()) {
    bleSetConnectSource("cached");
    gBleLastActivityMs = (uint32_t)millis();
    return true;
  }

  // Try locked address first, but do not hard-fail on stale lock.
  uint32_t nowMs = (uint32_t)millis();
  if (!bleLockedAddr.isEmpty() && nowMs >= gLockedBackoffUntilMs) {
    NimBLEAddress addr(bleLockedAddr.c_str());
    if (bleConnectTo(addr)) {
      bleSetConnectSource("locked");
      if (!bleLockedName.isEmpty()) gBleLastName = bleLockedName;
      return true;
    }
    // Locked address is stale. Drop it to avoid repeated slow attempts (often address-type mismatch).
    saveBle(bleNamePrefix, "", "");
    bleSetConnectSource("locked_invalid");
    // Also keep a short runtime backoff.
    gLockedBackoffUntilMs = nowMs + 120000UL; // 2 minutes backoff
  }

  NimBLEAddress bestAddr;
  String bestName;
  int bestRssi = -999;
  if (!bleFindBestByPrefix(bleNamePrefix, scanMs, &bestAddr, &bestName, &bestRssi)) return false;
  if (bleConnectTo(bestAddr)) {
    bleSetConnectSource(bleLockedAddr.isEmpty() ? "scan" : "locked_fallback_scan");
    gBleLastName = bestName;
    gBleLastRssi = bestRssi;
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[ble] connected via %s addr=%s name=%s rssi=%d\n", gBleConnectSource.c_str(), gBleLastAddr.c_str(),
                    gBleLastName.c_str(), gBleLastRssi);
    }
    return true;
  }
  if (kUartDiagEnabled) LB_SERIAL_PRINTLN("[ble] ensure: connect failed");
  return false;
}

static int bleReadSpeed() {
  if (!bleEnsureConnected()) return -1;
  return bleReadSpeedDirect();
}

static void beginExtractorSession(ExtractorSource src, uint8_t appliedSpeed, bool hadPreLightOn = false) {
  setActiveSource(src);
  gExtractorSessionActive = true;
  gExtractorSessionSource = src;
  gExtractorSessionHadPreLightOn = hadPreLightOn;
  if (appliedSpeed > 0) saveSourceSpeed(src, appliedSpeed);
}

static bool finalizeExtractorSession(int speedBeforeOff = -1, uint8_t *outSavedSpeed = nullptr) {
  if (!gExtractorSessionActive) return false;
  const ExtractorSource src = gExtractorSessionSource;
  gExtractorSessionActive = false;
  gExtractorSessionHadPreLightOn = false;
  int captured = speedBeforeOff;
  if (captured < 0 && bleReady()) captured = bleReadSpeedStable(2, 120);
  if (captured <= 0 || captured > 100) return false;
  uint8_t newSpeed = clampSpeedInt(captured);
  if (newSpeed == 0) return false;
  if (newSpeed == sourceSpeed(src)) return false;
  saveSourceSpeed(src, newSpeed);
  if (outSavedSpeed) *outSavedSpeed = newSpeed;
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[ble] session saved src=%s speed=%u\n", extractorSourceText(src), (unsigned int)newSpeed);
  }
  return true;
}

static void extractorSessionPollLoop() {
  // Manual speed capture from TES02 is intentionally disabled.
}

static bool bleSetSpeed(uint8_t spd) {
  bool ok = gTes02Driver.setSpeed(spd, extractorSourceText(gActiveSource));
  bleSyncRuntimeFromDriver();
  return ok;
}

static void autoStopLoop() {
  if (!autoStopEnabled) return;
  if (!clockValid) return;

  uint32_t ymd = clockLocalYmd();
  if (ymd == 0) return;
  uint16_t mod = clockLocalMinuteOfDay();
  if (mod != autoStopMinute) return;
  if (autoStopLastYmd == ymd) return;
  autoStopLastYmd = ymd;

  if (!gH312ExtractorOn && !gExtractorSessionActive) {
    extractorCmdLogPushf("auto-stop no-op hhmm=%02u:%02u", (unsigned)(autoStopMinute / 60), (unsigned)(autoStopMinute % 60));
    return;
  }

  int speedBeforeOff = -1;
  ExtractorSource offSrc = gExtractorSessionActive ? gExtractorSessionSource : sourceFromHotAirSlot(h312Slot);
  captureLastEnabledLightFromDevice(offSrc);
  if (gExtractorSessionActive && bleReady()) speedBeforeOff = bleReadSpeedStable(2, 120);
  bool ok = bleSetSpeed(0);
  if (ok) {
    finalizeExtractorSession(speedBeforeOff);
    gH312ExtractorOn = false;
    gH312PendingValid = false;
    gH312LastError = "";
    gH312LastLogic = "auto_stop_off";
    (void)bleSetLightMode(LIGHT_OFF);
    extractorCmdLogPushf("auto-stop off hhmm=%02u:%02u", (unsigned)(autoStopMinute / 60), (unsigned)(autoStopMinute % 60));
  } else {
    gH312LastError = "auto-stop off failed";
    gH312LastLogic = "auto_stop_retry";
    extractorCmdLogPushf("auto-stop fail hhmm=%02u:%02u", (unsigned)(autoStopMinute / 60), (unsigned)(autoStopMinute % 60));
  }
}

static bool h312ProbeHost(const IPAddress &target, uint16_t port, uint16_t connectTimeoutMs, uint16_t readMs, String *outName,
                          String *outModel, String *outSn, int *outWs, int *outTemp, bool requireRealTimeTemp = false,
                          uint16_t startupDelayMs = 20, uint16_t keepaliveAfterMs = 700, int *outHeat = nullptr,
                          String *outReason = nullptr) {
  h312::H312ProbeOptions opt;
  opt.connectTimeoutMs = connectTimeoutMs;
  opt.readMs = readMs;
  opt.startupDelayMs = startupDelayMs;
  opt.keepaliveAfterMs = keepaliveAfterMs;
  opt.requireRealTimeTemp = requireRealTimeTemp;
  h312::H312ProbeResult pr;
  String reason;
  bool ok = h312::probeHost(target, port, opt, &pr, &reason);
  if (!ok) {
    if (outReason) *outReason = reason;
    return false;
  }
  if (outName) *outName = pr.name;
  if (outModel) *outModel = pr.model;
  if (outSn) *outSn = pr.sn;
  if (outWs) *outWs = pr.ws;
  if (outTemp) *outTemp = pr.rt;
  if (outHeat) *outHeat = pr.hf;
  if (outReason) *outReason = "ok";
  return true;
}

static bool h312SocketLooksAlive(const IPAddress &target, uint16_t port, uint16_t connectTimeoutMs, uint16_t readMs,
                                 size_t *outRxBytes = nullptr) {
  return h312::socketLooksAlive(target, port, connectTimeoutMs, readMs, outRxBytes);
}

static bool h312VerifyTarget(const String &ipText, uint16_t port, String *outName = nullptr, String *outModel = nullptr,
                             String *outSn = nullptr, int *outWs = nullptr, int *outTemp = nullptr,
                             String *outReason = nullptr, uint8_t attempts = 3, uint16_t connectTimeoutMs = 2500,
                             uint16_t readMs = 2600, uint16_t startupDelayMs = 20, uint16_t keepaliveAfterMs = 700) {
  h312::H312ProbeOptions opt;
  opt.connectTimeoutMs = connectTimeoutMs;
  opt.readMs = readMs;
  opt.startupDelayMs = startupDelayMs;
  opt.keepaliveAfterMs = keepaliveAfterMs;
  opt.requireRealTimeTemp = true;
  h312::H312ProbeResult pr;
  String reason;
  bool ok = h312::verifyTarget(ipText, port, attempts, opt, &pr, &reason);
  if (!ok) {
    if (outReason) *outReason = reason;
    return false;
  }
  if (outName) *outName = pr.name;
  if (outModel) *outModel = pr.model;
  if (outSn) *outSn = pr.sn;
  if (outWs) *outWs = pr.ws;
  if (outTemp) *outTemp = pr.rt;
  if (outReason) *outReason = "ok";
  return true;
}

static uint32_t t420dOnDelayFor(bool h1Raised, bool h2Raised) {
  uint32_t d = 0;
  bool has = false;
  if (h1Raised) {
    d = t420dH1OnDelayMs;
    has = true;
  }
  if (h2Raised) {
    d = has ? min(d, t420dH2OnDelayMs) : t420dH2OnDelayMs;
    has = true;
  }
  return has ? d : 0;
}

static uint32_t t420dOffDelayFor() {
  uint32_t cfg = max(t420dH1OffDelayMs, t420dH2OffDelayMs);
  // UI-configured stop delay should represent real user-visible delay.
  // We already have a global OFF confirmation guard, so compensate it here.
  if (cfg <= kGlobalOffDemandConfirmMs) return 0;
  return cfg - kGlobalOffDemandConfirmMs;
}

static ExtractorSource t420dPickSource(bool h1Raised, bool h2Raised) {
  if (h1Raised) return SRC_H1;
  if (h2Raised) return SRC_H2;
  return gActiveSource == SRC_H2 ? SRC_H2 : SRC_H1;
}

static bool t420dWantsOnNow() {
  return gT420dExtractorOn || (gT420dPendingValid && gT420dPendingOn);
}

static bool h312WantsOnNow() {
  return gH312ExtractorOn || (gH312PendingValid && gH312PendingOn);
}

static bool globalOffDemandConfirmed(uint32_t nowMs) {
  const bool anyDemandOn = gH312DemandOn || gT420dDemandOn;
  if (anyDemandOn) {
    gGlobalAllDemandOffSinceMs = 0;
    return false;
  }
  if (gGlobalAllDemandOffSinceMs == 0) {
    gGlobalAllDemandOffSinceMs = nowMs;
    return false;
  }
  return (uint32_t)(nowMs - gGlobalAllDemandOffSinceMs) >= kGlobalOffDemandConfirmMs;
}

static void t420dApplyLogic() {
  const uint32_t now = (uint32_t)millis();
  if ((uint32_t)(now - gT420dLastEvalMs) < kT420dLogicTickMs) return;
  gT420dLastEvalMs = now;
  if (gSafeMode) {
    gT420dDemandOn = false;
    return;
  }
  if (!t420dLogicEnabled) {
    gT420dDemandOn = false;
    gT420dPendingValid = false;
    gT420dExtractorOn = false;
    gT420dLastLogic = "disabled";
    return;
  }
  if (t420dH1Gpio < 0 && t420dH2Gpio < 0) {
    gT420dDemandOn = false;
    gT420dPendingValid = false;
    gT420dExtractorOn = false;
    gT420dLastLogic = "gpio_disabled";
    return;
  }

  // Non-blocking GPIO read in main loop: avoid delay()-based sampling here,
  // otherwise BLE telemetry polling (H312) can starve and timeout.
  const bool h1RawRaised = readHandleRaisedHysteresis(t420dH1Gpio, gT420dDebounceCandH1Raised);
  const bool h2RawRaised = readHandleRaisedHysteresis(t420dH2Gpio, gT420dDebounceCandH2Raised);

  if (h1RawRaised != gT420dDebounceCandH1Raised) {
    gT420dDebounceCandH1Raised = h1RawRaised;
    gT420dDebounceH1SinceMs = now;
  }
  if (h2RawRaised != gT420dDebounceCandH2Raised) {
    gT420dDebounceCandH2Raised = h2RawRaised;
    gT420dDebounceH2SinceMs = now;
  }
  if (gT420dDebouncedH1Raised != gT420dDebounceCandH1Raised && gT420dDebounceH1SinceMs != 0 &&
      (uint32_t)(now - gT420dDebounceH1SinceMs) >= kT420dGpioDebounceMs) {
    gT420dDebouncedH1Raised = gT420dDebounceCandH1Raised;
  }
  if (gT420dDebouncedH2Raised != gT420dDebounceCandH2Raised && gT420dDebounceH2SinceMs != 0 &&
      (uint32_t)(now - gT420dDebounceH2SinceMs) >= kT420dGpioDebounceMs) {
    gT420dDebouncedH2Raised = gT420dDebounceCandH2Raised;
  }
  const bool h1Raised = gT420dDebouncedH1Raised;
  const bool h2Raised = gT420dDebouncedH2Raised;
  const bool desired = (h1Raised || h2Raised);
  gT420dDemandOn = desired;
  const bool h312WantsOn = h312WantsOnNow();

  if (h1Raised != gT420dLastH1Raised || h2Raised != gT420dLastH2Raised) {
    extractorCmdLogPushf("[t420d] state h1=%d h2=%d desired=%d", h1Raised ? 1 : 0, h2Raised ? 1 : 0, desired ? 1 : 0);
    gT420dLastH1Raised = h1Raised;
    gT420dLastH2Raised = h2Raised;
  }

  // Do not force OFF while hot-air logic still requires extractor ON.
  if (!desired && h312WantsOn) {
    gT420dPendingValid = false;
    gT420dExtractorOn = false;
    gT420dLastLogic = "hold_by_h312";
    return;
  }
  if (!desired && gT420dExtractorOn && !globalOffDemandConfirmed(now)) {
    gT420dPendingValid = false;
    gT420dLastLogic = "hold_global_off_confirm";
    return;
  }

  if (desired == gT420dExtractorOn) {
    gT420dPendingValid = false;
    gT420dLastLogic = desired ? "hold_on" : "hold_off";
    return;
  }

  if (!gT420dPendingValid || gT420dPendingOn != desired) {
    gT420dPendingValid = true;
    gT420dPendingOn = desired;
    gT420dPendingSinceMs = now;
    gT420dPendingSource = t420dPickSource(h1Raised, h2Raised);
    gT420dPendingDelayMs = desired ? t420dOnDelayFor(h1Raised, h2Raised) : t420dOffDelayFor();
    gT420dLastLogic = desired ? "pending_on" : "pending_off";
    extractorCmdLogPushf("[t420d] %s delay=%lums src=%s", gT420dLastLogic.c_str(), (unsigned long)gT420dPendingDelayMs,
                         extractorSourceText(gT420dPendingSource));
    return;
  }

  uint32_t waited = (now >= gT420dPendingSinceMs) ? (now - gT420dPendingSinceMs) : 0;
  if (waited < gT420dPendingDelayMs) return;
  if (!desired && (gH312ExtractorOn || (gH312PendingValid && gH312PendingOn))) {
    gT420dPendingValid = false;
    gT420dExtractorOn = false;
    gT420dLastLogic = "off_canceled_by_h312";
    return;
  }

  ExtractorSource src = gT420dPendingSource;
  if (src != SRC_H1 && src != SRC_H2) src = SRC_H1;
  setActiveSource(src);
  uint8_t target = 0;
  int speedBeforeOff = -1;

  if (desired) {
    target = sourceSpeed(src);
    if (target == 0) target = kAutoOnSpeedCap;
  } else {
    if (gH312ExtractorOn || (gH312PendingValid && gH312PendingOn)) {
      gT420dPendingValid = false;
      gT420dExtractorOn = false;
      gT420dLastLogic = "off_blocked_by_h312";
      return;
    }
    if (!bleReady() && !bleEnsureConnected(2800)) {
      gT420dLastError = "ble reconnect failed before off";
      gT420dLastLogic = "off_reconnect_retry";
      gT420dPendingSinceMs = now;
      return;
    }
    captureLastEnabledLightFromDevice(src);
    if (gExtractorSessionActive && gExtractorSessionSource == src) speedBeforeOff = bleReadSpeedStable(2, 120);
  }

  bool ok = bleSetSpeed(target);
  if (!ok) {
    gT420dLastError = "ble command failed";
    gT420dLastLogic = desired ? "on_retry" : "off_retry";
    gT420dPendingSinceMs = now;
    extractorCmdLogPushf("[t420d] %s src=%s target=%u", gT420dLastLogic.c_str(), extractorSourceText(src), (unsigned int)target);
    return;
  }

  gT420dPendingValid = false;
  gT420dLastError = "";
  gT420dExtractorOn = desired;
  if (desired) {
    beginExtractorSession(src, target, false);
    uint8_t lm = sourceLightMode(src);
    (void)bleSetLightMode(lm);
    if (lm != LIGHT_OFF) (void)bleSetLightBrightness(sourceTargetBrightness(src), src);
    if (kBleReleaseAfterOn) {
      bleResetHandles();
      bleSetConnectSource("session_release_on_t420d");
    }
    gT420dLastLogic = "on_ok";
  } else {
    bool saved = finalizeExtractorSession(speedBeforeOff, nullptr);
    (void)bleSetLightMode(LIGHT_OFF);
    bleResetHandles();
    bleSetConnectSource("session_release_off_t420d");
    gT420dLastLogic = saved ? "off_ok_saved" : "off_ok";
  }
  extractorCmdLogPushf("[t420d] %s src=%s target=%u", gT420dLastLogic.c_str(), extractorSourceText(src), (unsigned int)target);
}

static void h312ApplyLogic() {
  if (!h312Enabled) return;
  if (gSafeMode) return;
  if (gH312RealTimeTemp < -100) return;
  uint32_t now = (uint32_t)millis();
  gH312DemandOn = gH312ExtractorOn;
  if (gBleLastSetMs > 0 && now >= gBleLastSetMs && (now - gBleLastSetMs) < kH312PostSwitchGuardMs) {
    gH312LastLogic = "post_switch_guard";
    return;
  }

  // H312 control is temperature-driven, but with telemetry guard:
  // ON uses tempOn threshold, OFF uses tempOff threshold (hysteresis).
  // workStatus/heatFlag gate only ON transition to avoid ON/OFF chatter.
  bool heatStateKnown = (gH312WorkStatus >= 0) || (gH312HeatFlag >= 0);
  bool heatStateOn = (gH312WorkStatus > 0) || (gH312HeatFlag > 0);
  bool desired;
  if (gH312ExtractorOn) {
    // While H312 still reports active heating, keep extractor ON even on short temp dips.
    // This prevents ON/OFF chatter when telemetry briefly glitches.
    if (heatStateKnown && heatStateOn)
      desired = true;
    else
      desired = (gH312RealTimeTemp >= h312TempOff);
  } else {
    // Start immediately when H312 reports active heating, even if temp is still low
    // (or telemetry briefly lags at standby-like values such as 38C).
    desired = (gH312RealTimeTemp >= h312TempOn) && (!heatStateKnown || heatStateOn);
    if (gH312ExtractorOffSinceMs > 0 && now >= gH312ExtractorOffSinceMs &&
        (now - gH312ExtractorOffSinceMs) < kH312MinOffMs) {
      desired = false;
    }
  }
  gH312DemandOn = desired;
  if (desired == gH312ExtractorOn) {
    gH312PendingValid = false;
    gH312LastLogic = desired ? "temp_hold_on" : "temp_hold_off";
    return;
  }

  if (!desired && gH312ExtractorOn) {
    if (!globalOffDemandConfirmed(now)) {
      gH312PendingValid = false;
      gH312LastLogic = "hold_global_off_confirm";
      return;
    }
    uint32_t onFor = (now >= gH312ExtractorOnSinceMs) ? (now - gH312ExtractorOnSinceMs) : 0;
    if (onFor < h312MinOnMs) {
      gH312LastLogic = "hold_min_on";
      return;
    }
    if (t420dWantsOnNow()) {
      gH312PendingValid = false;
      gH312LastLogic = "off_blocked_by_t420d";
      return;
    }
  }

  if (!gH312PendingValid || gH312PendingOn != desired) {
    gH312PendingValid = true;
    gH312PendingOn = desired;
    gH312PendingSinceMs = now;
    gH312LastLogic = desired ? "pending_on" : "pending_off";
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[logic] %s rt=%d on=%d off=%d debounce=%u\n", gH312LastLogic.c_str(), gH312RealTimeTemp, h312TempOn,
                    h312TempOff, (unsigned int)h312DebounceMs);
    }
    return;
  }

  uint32_t waited = (now >= gH312PendingSinceMs) ? (now - gH312PendingSinceMs) : 0;
  if (waited < h312DebounceMs) return;

  ExtractorSource src = sourceFromHotAirSlot(h312Slot);
  setActiveSource(src);
  uint8_t target = 0;
  int speedBeforeOff = -1;
  if (desired) {
    target = sourceSpeed(src);
    if (target == 0) target = kAutoOnSpeedCap;
  } else {
    if (t420dWantsOnNow()) {
      gH312PendingValid = false;
      gH312LastLogic = "off_blocked_by_t420d";
      return;
    }
    // Capture light state before motor-off, because some firmware revisions
    // report brightness=0 after off command even when user brightness was non-zero.
    captureLastEnabledLightFromDevice(src);
    if (gExtractorSessionActive && gExtractorSessionSource == src) {
      speedBeforeOff = bleReadSpeedStable(2, 120);
    }
  }
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[logic] apply desired=%d target=%u src=%s rt=%d slot=%u\n", desired ? 1 : 0, (unsigned int)target,
                  extractorSourceText(src), gH312RealTimeTemp, (unsigned int)h312Slot);
  }
  bool ok = bleSetSpeed(target);
  if (ok) {
    bool saved = false;
    uint8_t savedSpeed = 0;
    gH312ExtractorOn = desired;
    if (desired) {
      gH312ExtractorOnSinceMs = now;
      gH312ExtractorOffSinceMs = 0;
    }
    if (desired) gH312LastFailSafeTryMs = 0;
    if (desired) {
      beginExtractorSession(src, target, false);
      uint8_t lm = sourceLightMode(src);
      (void)bleSetLightMode(lm);
      if (lm != LIGHT_OFF) {
        (void)bleSetLightBrightness(sourceTargetBrightness(src), src);
      }
      if (kBleReleaseAfterOn) {
        bleResetHandles();
        bleSetConnectSource("session_release_on");
      }
    } else {
      if (speedBeforeOff < 0 && gBleLastSetTarget == 0 && gBleLastSetBefore > 0) {
        uint32_t nowSet = (uint32_t)millis();
        if (nowSet >= gBleLastSetMs && (nowSet - gBleLastSetMs) <= 4000U) speedBeforeOff = gBleLastSetBefore;
      }
      saved = finalizeExtractorSession(speedBeforeOff, &savedSpeed);
      (void)bleSetLightMode(LIGHT_OFF);
      gH312ExtractorOffSinceMs = now;
      if (saved && kUartDiagEnabled) {
        LB_SERIAL_PRINTF("[logic] off saved src=%s speed=%u\n", extractorSourceText(src), (unsigned int)savedSpeed);
      }
    }
    gH312PendingValid = false;
    gH312LastError = "";
    gH312LastLogic = desired ? "on_ok" : (saved ? "off_ok_saved" : "off_ok");
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[logic] %s src=%s target=%u\n", gH312LastLogic.c_str(), extractorSourceText(src), (unsigned int)target);
    }
    return;
  }

  gH312LastError = "ble command failed";
  gH312PendingSinceMs = now;
  gH312LastLogic = desired ? "on_retry" : "off_retry";
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[logic] %s src=%s target=%u\n", gH312LastLogic.c_str(), extractorSourceText(src), (unsigned int)target);
  }
}

static void h312FailSafeOffIfTelemetryLost(uint32_t now) {
  if (!gH312ExtractorOn) return;
  if (t420dWantsOnNow()) {
    gH312LastLogic = "failsafe_hold_by_t420d";
    return;
  }
  uint32_t baseMs = gH312LastJsonMs;
  if (baseMs == 0) baseMs = gH312ExtractorOnSinceMs;
  if (baseMs == 0 || now < baseMs) return;
  uint32_t lossMs = now - baseMs;
  if (lossMs < kH312TelemetryLossForceOffMs) return;
  if (gH312LastFailSafeTryMs != 0 && (now - gH312LastFailSafeTryMs) < kH312FailSafeRetryMs) return;
  gH312LastFailSafeTryMs = now;

  captureLastEnabledLightFromDevice(sourceFromHotAirSlot(h312Slot));
  bool ok = bleSetSpeed(0);
  if (ok) {
    finalizeExtractorSession();
    gH312ExtractorOn = false;
    gH312ExtractorOffSinceMs = now;
    gH312PendingValid = false;
    gH312LastError = "";
    gH312LastLogic = "failsafe_off";
    (void)bleSetLightMode(LIGHT_OFF);
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[logic] %s reason=telemetry_lost loss_ms=%lu\n", gH312LastLogic.c_str(), (unsigned long)lossMs);
    }
  } else {
    gH312LastError = "failsafe off failed";
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[logic] failsafe_off_retry reason=telemetry_lost loss_ms=%lu\n", (unsigned long)lossMs);
    }
  }
}

static void h312Loop() {
  const uint32_t now = (uint32_t)millis();
  if (!h312Enabled || gSafeMode || !h312TargetConfigured()) {
    gH312DemandOn = false;
    gH312PendingValid = false;
    gH312SeenTelemetry = false;
    gH312LastLogic = "idle";
    gH312LastError = "";
    gH312ExtractorOffSinceMs = 0;
    gH312WorkStatus = -1;
    gH312HeatFlag = -1;
    if (!h312Enabled || !h312TargetConfigured()) gH312RealTimeTemp = -1000;
    if (!h312Enabled || h312Mode != H312_MODE_BLE) {
      h312BleReleaseClient();
      gH312BleRuntimeAddr = "";
      gH312BleRuntimeAddrType = -1;
    }
    return;
  }

  bool connected = false;
  if (h312Mode == H312_MODE_BLE) {
    h312BleTick(now);
    connected = h312SlotConnectedNow(h312Slot);
  } else {
    if (WiFi.status() != WL_CONNECTED) {
      gH312LastError = "wifi disconnected";
      h312FailSafeOffIfTelemetryLost(now);
      return;
    }

    gH312Driver.tick(now);

    const uint8_t slot = h312NormSlot(h312Slot);
    const h312::H312Health h = gH312Driver.health(slot);
    const float rt = gH312Driver.lastTempC(slot);
    gH312WorkStatus = gH312Driver.lastWorkStatus(slot);
    gH312HeatFlag = gH312Driver.lastHeatFlag(slot);

    if (h.lastOkMs > 0) gH312LastJsonMs = h.lastOkMs;

    if (isfinite(rt)) {
      gH312SeenTelemetry = true;
      gH312RealTimeTemp = (int)lroundf(rt);
    } else {
      if (gH312LastJsonMs == 0 || (now - gH312LastJsonMs) > h312StaleWindowMs()) {
        gH312RealTimeTemp = -1000;
        gH312PendingValid = false;
      }
    }

    if (h.error == h312::H312Error::OK || h.error == h312::H312Error::SLOT_DISABLED)
      gH312LastError = "";
    else
      gH312LastError = h312ErrText(h.error);
    connected = h.connected;
  }

  if (!connected) {
    if (gH312LastJsonMs == 0 || now < gH312LastJsonMs || (now - gH312LastJsonMs) > h312StaleWindowMs()) {
      gH312RealTimeTemp = -1000;
    }
  }

  if (connected && gH312RealTimeTemp > -100) h312ApplyLogic();
  h312FailSafeOffIfTelemetryLost(now);
}

static void bleBootPulse() {
  if (!kBleBootPulseEnabled || gSafeMode) return;
  if (!bleBound) return;
  LB_SERIAL_PRINTLN("[ble] boot pulse: start");
  extractorCmdLogPushf("[boot-pulse] start speed=%u scan_ms=%lu", (unsigned int)kBleBootPulseSpeed,
                       (unsigned long)kBleBootPulseScanMs);
  if (!bleEnsureConnected(kBleBootPulseScanMs)) {
    LB_SERIAL_PRINTLN("[ble] boot pulse: extractor not found");
    extractorCmdLogPushf("[boot-pulse] extractor not found");
    return;
  }

  bool onOk = bleSetSpeed(kBleBootPulseSpeed);
  delay(kBleBootPulseOnMs);
  bool offOk = bleSetSpeed(0);

  LB_SERIAL_PRINTF("[ble] boot pulse: on=%d off=%d src=%s addr=%s\n", onOk ? 1 : 0, offOk ? 1 : 0,
                gBleConnectSource.c_str(), gBleLastAddr.c_str());
  extractorCmdLogPushf("[boot-pulse] on=%d off=%d src=%s addr=%s", onOk ? 1 : 0, offOk ? 1 : 0, gBleConnectSource.c_str(),
                       gBleLastAddr.c_str());

  bleResetHandles();
  bleSetConnectSource("boot_pulse_release");
}

static String contentTypeFor(const String &path) {
  String p = path;
  if (p.endsWith(".gz")) p = p.substring(0, p.length() - 3);
  if (p.endsWith(".html")) return "text/html";
  if (p.endsWith(".css")) return "text/css";
  if (p.endsWith(".js")) return "application/javascript";
  if (p.endsWith(".webmanifest")) return "application/manifest+json";
  if (p.endsWith(".png")) return "image/png";
  if (p.endsWith(".ico")) return "image/x-icon";
  if (p.endsWith(".svg")) return "image/svg+xml";
  return "application/octet-stream";
}

static bool tryServeFile(String path) {
  gWebReqLastMs = (uint32_t)millis();
  if (path.endsWith("/")) path += "index.html";
  if (!path.startsWith("/")) path = "/" + path;
  String servePath = path;
  if (!LittleFS.exists(servePath)) {
    String gzPath = path + ".gz";
    if (LittleFS.exists(gzPath)) {
      servePath = gzPath;
    } else {
      return false;
    }
  }

  File f = LittleFS.open(servePath, "r");
  if (!f) return false;

  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  if (servePath.endsWith(".gz")) {
    server.sendHeader("Content-Encoding", "gzip");
    server.sendHeader("Vary", "Accept-Encoding");
  }
  server.streamFile(f, contentTypeFor(servePath));
  f.close();
  return true;
}

static void handleWifiPost() {
  String ssid = server.arg("ssid");
  String pass = server.arg("pass");
  saveWifi(ssid, pass);
  server.send(200, "text/plain", "Saved. Rebooting...");
  delay(600);
  ESP.restart();
}

static void sendJson(int code, const String &body) {
  gWebReqLastMs = (uint32_t)millis();
  server.sendHeader("Cache-Control", "no-store");
  server.send(code, "application/json", body);
}

static void sendJsonError(int code, const String &msg) {
  String b;
  b.reserve(msg.length() + 40);
  b += F("{\"ok\":false,\"error\":\"");
  b += jsonEscape(msg);
  b += F("\"}");
  sendJson(code, b);
}

static bool isApiPath(const String &uri) {
  return uri.startsWith("/api/") || uri == "/wifi";
}

static bool webAuthGuard() {
  gReqAuthedByToken = false;
  gReqAuthedByBasic = false;
  gReqTokenHash = "";
  const String uri = server.uri();
  const bool api = isApiPath(uri);
  IPAddress rip = server.client().remoteIP();
  uint32_t nowMs = (uint32_t)millis();

  if (!ipIsLocalLan(rip)) {
    extractorCmdLogPushf("[security] deny non-local ip=%s uri=%s", rip.toString().c_str(), uri.c_str());
    if (api) sendJsonError(403, "local network only");
    else server.send(403, "text/plain", "local network only");
    return false;
  }

  if (isRecoveryUnlockActive(nowMs) && isRecoveryUnlockUri(uri)) {
    return true;
  }

  if (gWebAuthBlockUntilMs != 0 && rip == gWebAuthFailIp && (int32_t)(gWebAuthBlockUntilMs - nowMs) > 0) {
    extractorCmdLogPushf("[security] blocked ip=%s uri=%s", rip.toString().c_str(), uri.c_str());
    if (api) sendJsonError(429, "too many auth attempts; try later");
    else server.send(429, "text/plain", "too many auth attempts; try later");
    return false;
  }

  String token = rememberCookieFromRequest();
  if (!token.isEmpty()) {
    String tokenHash = sha256Hex(token);
    if (rememberValidateAndTouchHash(tokenHash)) {
      gReqAuthedByToken = true;
      gReqTokenHash = tokenHash;
      if (rip == gWebAuthFailIp && (gWebAuthFailCount != 0 || gWebAuthBlockUntilMs != 0)) {
        gWebAuthFailCount = 0;
        gWebAuthBlockUntilMs = 0;
      }
      return true;
    }
    server.sendHeader("Set-Cookie", rememberCookieHeader("", true));
  }

  if (!server.authenticate(gWebAuthUser.c_str(), gWebAuthPass.c_str())) {
    if (rip != gWebAuthFailIp) {
      gWebAuthFailIp = rip;
      gWebAuthFailCount = 0;
      gWebAuthBlockUntilMs = 0;
    }
    if (gWebAuthFailCount < 250) gWebAuthFailCount++;
    extractorCmdLogPushf("[security] auth fail ip=%s uri=%s count=%u",
                         rip.toString().c_str(), uri.c_str(), (unsigned)gWebAuthFailCount);
    if (gWebAuthFailCount >= kWebAuthFailMax) {
      gWebAuthBlockUntilMs = nowMs + kWebAuthBlockMs;
      extractorCmdLogPushf("[security] auth blocked ip=%s for %lus",
                           rip.toString().c_str(), (unsigned long)(kWebAuthBlockMs / 1000UL));
    }
    server.requestAuthentication();
    return false;
  }

  if (rip == gWebAuthFailIp && (gWebAuthFailCount != 0 || gWebAuthBlockUntilMs != 0)) {
    extractorCmdLogPushf("[security] auth ok ip=%s", rip.toString().c_str());
    gWebAuthFailCount = 0;
    gWebAuthBlockUntilMs = 0;
  }
  gReqAuthedByBasic = true;
  return true;
}

static void sendRecoveryMinPage(const char *hint = nullptr) {
  String html;
  html.reserve(1800);
  html += F(
      "<!doctype html><html><head><meta charset='utf-8'>"
      "<meta name='viewport' content='width=device-width,initial-scale=1'>"
      "<title>LB Recovery</title>"
      "<style>body{font-family:Arial,sans-serif;background:#111827;color:#e5e7eb;padding:16px}"
      ".c{max-width:680px;margin:0 auto;background:#1f2937;padding:16px;border-radius:10px}"
      "button{padding:10px 14px;border:0;border-radius:8px;background:#0ea5e9;color:#001;font-weight:700;cursor:pointer}"
      "input{margin:8px 0 12px 0}pre{background:#0b1220;padding:10px;border-radius:8px;white-space:pre-wrap}</style>"
      "</head><body><div class='c'><h2>LB Fail-safe Recovery</h2>"
      "<p>AP active. Upload <code>firmware.bin</code> to recover the device.</p>");
  if (hint && *hint) {
    html += F("<p><b>Hint:</b> ");
    html += String(hint);
    html += F("</p>");
  }
  html += F(
      "<form id='f'><input id='bin' type='file' accept='.bin,application/octet-stream' required><br>"
      "<button type='submit'>Flash Firmware</button></form>"
      "<p><a href='/recovery' style='color:#67e8f9'>Open full recovery page</a></p>"
      "<pre id='log'>ready</pre>"
      "<script>"
      "const log=document.getElementById('log');"
      "document.getElementById('f').onsubmit=async(e)=>{e.preventDefault();"
      "const fi=document.getElementById('bin').files[0]; if(!fi){log.textContent='file required';return;}"
      "const fd=new FormData(); fd.append('file',fi);"
      "log.textContent='uploading...';"
      "try{const r=await fetch('/api/fw/update_manual',{method:'POST',body:fd});"
      "const t=await r.text(); log.textContent='response: '+t+'\\nIf success, device will reboot.';}"
      "catch(err){log.textContent='error: '+err;}};"
      "</script></div></body></html>");
  gWebReqLastMs = (uint32_t)millis();
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/html", html);
}

static bool parseJsonBody(JsonDocument &doc) {
  if (!server.hasArg("plain")) return false;
  DeserializationError err = deserializeJson(doc, server.arg("plain"));
  return !err;
}

static void handleApiStatus() {
  bool bleConnected = bleReady();
  int cur = -1;
  bool staConnected = (WiFi.status() == WL_CONNECTED);
  int32_t staRssi = staConnected ? WiFi.RSSI() : -127;
  const int h1LevelRaw = readHandleGpioLevelNow(t420dH1Gpio);
  const int h2LevelRaw = readHandleGpioLevelNow(t420dH2Gpio);
  const int h1Level = readStableGpioLevel(t420dH1Gpio, kHandleStatusSampleMs, kHandleStatusSampleStepMs);
  const int h2Level = readStableGpioLevel(t420dH2Gpio, kHandleStatusSampleMs, kHandleStatusSampleStepMs);
  const bool h1Raised = isHandleRaisedFromLevel(t420dH1Gpio, h1Level);
  const bool h2Raised = isHandleRaisedFromLevel(t420dH2Gpio, h2Level);
  const int h1Mv = readHandleGpioMilliVoltsNow(t420dH1Gpio);
  const int h2Mv = readHandleGpioMilliVoltsNow(t420dH2Gpio);
  if (bleConnected) cur = bleReadSpeedDirect();
  bool bleOk = bleConnected && cur >= 0;

  wifi_mode_t m = WiFi.getMode();
  String ip;
  if (m == WIFI_AP) ip = WiFi.softAPIP().toString();
  else if (m == WIFI_AP_STA) ip = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : WiFi.softAPIP().toString();
  else ip = WiFi.localIP().toString();

  String body;
  body.reserve(620);
  body += F("{\"ok\":true");
  body += F(",\"fw\":\"");
  body += kFwVersion;
  body += F("\",\"fw_profile\":\"");
  body += kFwProfile;
  body += F("\",\"fw_hardened\":");
  body += (kFwHardenedBuild ? "true" : "false");
  body += F(",\"uptime_ms\":");
  body += String((uint32_t)millis());
  body += F(",\"heap_free\":");
  body += String((uint32_t)ESP.getFreeHeap());
  body += F(",\"heap_min\":");
  body += String((uint32_t)esp_get_minimum_free_heap_size());
  body += F(",\"heap_largest\":");
  body += String((uint32_t)heap_caps_get_largest_free_block(MALLOC_CAP_8BIT));
  body += F(",\"reset_reason\":");
  body += String((int)esp_reset_reason());
  body += F(",\"wifi_mode\":\"");
  if (m == WIFI_AP) body += "ap";
  else if (m == WIFI_STA) body += "sta";
  else if (m == WIFI_AP_STA) body += "apsta";
  else body += "off";
  body += F("\",\"ip\":\"");
  body += jsonEscape(ip);
  body += F("\",\"ap_ssid\":\"");
  body += jsonEscape(gApSsid);
  body += F("\",\"sta_ssid\":\"");
  body += jsonEscape(wifiSsid);
  body += F("\",\"sta_connected\":");
  body += (staConnected ? "true" : "false");
  body += F(",\"sta_rssi\":");
  if (staConnected) body += String((int32_t)staRssi);
  else body += F("null");
  body += F(",\"wifi_autooff_min\":");
  body += String((uint32_t)wifiAutoOffMin);
  body += F(",\"clock_valid\":");
  body += (clockValid ? "true" : "false");
  body += F(",\"tz_offset_min\":");
  body += String((int)tzOffsetMin);
  body += F(",\"tz_name\":\"");
  body += jsonEscape(tzName);
  body += F("\",\"clock_local\":\"");
  body += jsonEscape(clockLocalText());
  body += F("\"");
  body += F(",\"auto_stop_enabled\":");
  body += (autoStopEnabled ? "true" : "false");
  body += F(",\"auto_stop_minute\":");
  body += String((uint32_t)autoStopMinute);

  body += F(",\"ble_connected\":");
  body += (bleConnected ? "true" : "false");
  body += F(",\"ble_ok\":");
  body += (bleOk ? "true" : "false");
  body += F(",\"ble_prefix\":\"");
  body += jsonEscape(bleNamePrefix);
  body += F("\",\"ble_locked_addr\":\"");
  body += jsonEscape(bleLockedAddr);
  body += F("\",\"ble_locked_name\":\"");
  body += jsonEscape(bleLockedName);
  body += F("\",\"speed\":");
  if (bleOk) body += String(cur);
  else body += F("null");
  body += F(",\"saved\":");
  body += String((int)sourceSpeed(gActiveSource));
  body += F(",\"saved_source\":\"");
  body += extractorSourceText(gActiveSource);
  body += F("\"");
  body += F(",\"saved_h1\":");
  body += String((int)sourceSpeed(SRC_H1));
  body += F(",\"saved_h2\":");
  body += String((int)sourceSpeed(SRC_H2));
  body += F(",\"h1_raised\":");
  body += (h1Raised ? "true" : "false");
  body += F(",\"h2_raised\":");
  body += (h2Raised ? "true" : "false");
  body += F(",\"h1_level\":");
  if (h1Level == HIGH || h1Level == LOW) body += String(h1Level);
  else body += F("null");
  body += F(",\"h2_level\":");
  if (h2Level == HIGH || h2Level == LOW) body += String(h2Level);
  else body += F("null");
  body += F(",\"h1_level_raw\":");
  if (h1LevelRaw == HIGH || h1LevelRaw == LOW) body += String(h1LevelRaw);
  else body += F("null");
  body += F(",\"h2_level_raw\":");
  if (h2LevelRaw == HIGH || h2LevelRaw == LOW) body += String(h2LevelRaw);
  else body += F("null");
  body += F(",\"h1_mv\":");
  if (h1Mv >= 0) body += String(h1Mv);
  else body += F("null");
  body += F(",\"h2_mv\":");
  if (h2Mv >= 0) body += String(h2Mv);
  else body += F("null");
  body += F(",\"t420d_logic_enabled\":");
  body += (t420dLogicEnabled ? "true" : "false");
  body += F(",\"t420d_on\":");
  body += (gT420dExtractorOn ? "true" : "false");
  body += F(",\"t420d_pending\":");
  body += (gT420dPendingValid ? "true" : "false");
  body += F(",\"t420d_pending_on\":");
  body += (gT420dPendingOn ? "true" : "false");
  body += F(",\"t420d_logic\":\"");
  body += jsonEscape(gT420dLastLogic);
  body += F("\",\"t420d_error\":\"");
  body += jsonEscape(gT420dLastError);
  body += F("\"");
  body += F(",\"saved_ha1\":");
  body += String((int)sourceSpeed(SRC_HA1));
  body += F(",\"saved_ha2\":");
  body += String((int)sourceSpeed(SRC_HA2));
  body += F(",\"saved_ha3\":");
  body += String((int)sourceSpeed(SRC_HA3));
  appendJsonLightProfileFields(body);
  body += F(",\"h312_enabled\":");
  body += (h312Enabled ? "true" : "false");
  body += F(",\"h312_connected\":");
  body += (h312SlotConnectedNow(h312Slot) ? "true" : "false");
  body += F(",\"h312_ip\":\"");
  body += jsonEscape(h312Ip);
  body += F("\",\"h312_port\":");
  body += String((uint32_t)h312Port);
  body += F(",\"h312_work_status\":");
  if (gH312WorkStatus >= 0) body += String(gH312WorkStatus);
  else body += F("null");
  body += F(",\"h312_heat_flag\":");
  if (gH312HeatFlag >= 0) body += String(gH312HeatFlag);
  else body += F("null");
  body += F(",\"h312_real_temp\":");
  if (gH312RealTimeTemp > -100) body += String(gH312RealTimeTemp);
  else body += F("null");
  body += F(",\"h312_set_temp\":");
  if (gH312SetTemp >= 0) body += String(gH312SetTemp);
  else body += F("null");
  body += F(",\"h312_real_fan\":");
  if (gH312RealFan >= 0) body += String(gH312RealFan);
  else body += F("null");
  body += F(",\"h312_set_fan\":");
  if (gH312SetFan >= 0) body += String(gH312SetFan);
  else body += F("null");
  body += F(",\"h312_last_json_ms\":");
  body += String((uint32_t)gH312LastJsonMs);
  appendJsonH312StaticConfigFields(body);
  body += F(",\"h312_pending\":");
  body += (gH312PendingValid ? "true" : "false");
  body += F(",\"h312_pending_on\":");
  body += (gH312PendingOn ? "true" : "false");
  body += F(",\"h312_logic\":\"");
  body += jsonEscape(gH312LastLogic);
  body += F("\",\"h312_error\":\"");
  body += jsonEscape(gH312LastError);
  body += F("\"");
  body += F("}");

  sendJson(200, body);
}

static const char *wifiAuthStr(wifi_auth_mode_t a) {
  switch (a) {
    case WIFI_AUTH_OPEN: return "OPEN";
    case WIFI_AUTH_WEP: return "WEP";
    case WIFI_AUTH_WPA_PSK: return "WPA";
    case WIFI_AUTH_WPA2_PSK: return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
#ifdef WIFI_AUTH_WPA2_ENTERPRISE
    case WIFI_AUTH_WPA2_ENTERPRISE: return "WPA2-ENT";
#endif
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK: return "WPA3";
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK: return "WPA2/WPA3";
#endif
    default: return "UNK";
  }
}

static void handleApiWifiScan() {
  // Safe: does not write flash, does not change stored credentials.
  uint32_t t0 = millis();

  // scanNetworks requires STA enabled; in AP onboarding mode we run AP+STA.
  wifi_mode_t oldMode = WiFi.getMode();
  if (oldMode == WIFI_AP) {
    WiFi.mode(WIFI_AP_STA);
    delay(30);
  } else if (oldMode == WIFI_OFF) {
    WiFi.mode(WIFI_STA);
    delay(30);
  }

  int n = WiFi.scanNetworks(false /*async*/, true /*show_hidden*/);
  if (n < 0) {
    sendJsonError(500, String("scanNetworks failed: ") + String(n));
    return;
  }

  String body;
  body.reserve(120 + (size_t)n * 90);
  body += F("{\"ok\":true,\"count\":");
  body += String(n);
  body += F(",\"ms\":");
  body += String((uint32_t)(millis() - t0));
  body += F(",\"nets\":[");
  for (int i = 0; i < n; i++) {
    if (i) body += ",";
    String ssid = WiFi.SSID(i);
    int32_t rssi = WiFi.RSSI(i);
    int32_t chan = WiFi.channel(i);
    wifi_auth_mode_t auth = WiFi.encryptionType(i);

    body += F("{\"ssid\":\"");
    body += jsonEscape(ssid);
    body += F("\",\"rssi\":");
    body += String((int32_t)rssi);
    body += F(",\"chan\":");
    body += String((int32_t)chan);
    body += F(",\"auth\":\"");
    body += wifiAuthStr(auth);
    body += F("\"");
    if (!wifiSsid.isEmpty() && ssid == wifiSsid) body += F(",\"known\":true");
    body += F("}");
  }
  body += F("]}");
  sendJson(200, body);

  WiFi.scanDelete();
}

static void handleApiWifiConnect() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }

  String ssid = doc["ssid"] | "";
  String pass = doc["pass"] | "";
  ssid.trim();
  if (ssid.isEmpty()) {
    sendJsonError(400, "ssid required");
    return;
  }

  // Keep AP available during connect attempts (important for AP onboarding flow).
  WiFi.mode(WIFI_AP_STA);
  applyStaHostname();
  WiFi.disconnect();
  delay(60);
  WiFi.begin(ssid.c_str(), pass.c_str());

  uint32_t t0 = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t0 < 15000) {
    delay(200);
  }
  if (WiFi.status() != WL_CONNECTED) {
    sendJsonError(400, "connect failed");
    return;
  }

  saveWifi(ssid, pass);
  startMdnsForSta();
  // Explicit user connect action should restore normal STA-only behavior.
  if (!gSafeMode) {
    gFailsafeApPinned = false;
    gFailsafeApPinUntilMs = 0;
  }
  // Give HTTP response time to leave over AP, then force STA-only mode in loop().
  gApDisableAtMs = (uint32_t)millis() + 1200;

  String body;
  body.reserve(280);
  body += F("{\"ok\":true,\"ip\":\"");
  body += jsonEscape(WiFi.localIP().toString());
  body += F("\",\"sta_ssid\":\"");
  body += jsonEscape(ssid);
  body += F("\",\"hostname\":\"");
  body += jsonEscape(gStaHostname);
  body += F("\",\"local_url\":\"http://");
  body += jsonEscape(gStaHostname);
  body += F(".local\"");
  body += F("}");
  sendJson(200, body);
}

static void handleApiH312BleScan() {
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }

  JsonDocument doc;
  uint32_t scanMs = 5000;
  if (server.hasArg("plain")) {
    if (parseJsonBody(doc)) {
      scanMs = (uint32_t)(doc["scan_ms"] | scanMs);
    }
  }
  if (scanMs < 800) scanMs = 800;
  if (scanMs > 12000) scanMs = 12000;

  NimBLEScan *scan = NimBLEDevice::getScan();
  if (!scan) {
    sendJsonError(500, "ble scan unavailable");
    return;
  }
  scan->setActiveScan(true);
  scan->setInterval(45);
  scan->setWindow(15);
  scan->setDuplicateFilter(true);
  scan->setMaxResults(40);

  uint32_t durSec = (scanMs + 999) / 1000;
  if (durSec < 1) durSec = 1;
  if (durSec > 12) durSec = 12;

  NimBLEScanResults res = scan->start(durSec, false);

  String body;
  body.reserve(1400);
  body += F("{\"ok\":true,\"devices\":[");
  bool first = true;
  for (int i = 0; i < res.getCount(); i++) {
    NimBLEAdvertisedDevice dev = res.getDevice((uint32_t)i);
    std::string rawName = dev.getName();
    if (rawName.empty()) continue;
    String name(rawName.c_str());
    if (!h312BleLooksLikeH312Name(name)) continue;

    if (!first) body += ",";
    first = false;
    body += F("{\"name\":\"");
    body += jsonEscape(name);
    body += F("\",\"addr\":\"");
    body += jsonEscape(String(dev.getAddress().toString().c_str()));
    body += F("\",\"rssi\":");
    body += String(dev.getRSSI());
    body += F("}");
  }
  body += F("]}");

  scan->clearResults();
  sendJson(200, body);
}

static void handleApiH312Scan() {
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  if (WiFi.status() != WL_CONNECTED) {
    sendJsonError(400, "wifi sta not connected");
    return;
  }

  JsonDocument doc;
  if (server.hasArg("plain")) (void)parseJsonBody(doc);
  uint16_t port = (uint16_t)(doc["port"] | (int)h312Port);
  uint16_t connectTimeoutMs = (uint16_t)(doc["timeout_ms"] | 900);
  uint16_t readMs = (uint16_t)(doc["read_ms"] | 1250);
  uint16_t probeConnectTimeoutMs = (uint16_t)(doc["probe_timeout_ms"] | 450);
  uint16_t probeReadMs = (uint16_t)(doc["probe_read_ms"] | 1250);
  uint16_t startupDelayMs = (uint16_t)(doc["startup_delay_ms"] | 20);
  uint16_t keepaliveAfterMs = (uint16_t)(doc["keepalive_after_ms"] | 700);
  uint16_t maxHosts = (uint16_t)(doc["max_hosts"] | 254);
  uint8_t retries = (uint8_t)(doc["retries"] | 2);
  uint8_t workers = (uint8_t)(doc["workers"] | 1);
  bool stopOnFirst = doc["stop_on_first"].isNull() ? false : (bool)(doc["stop_on_first"] | false);
  bool includeWeak = doc["include_weak"].isNull() ? false : (bool)(doc["include_weak"] | false);
  bool debug = doc["debug"].isNull() ? false : (bool)(doc["debug"] | false);
  uint8_t maxDebugHosts = (uint8_t)(doc["max_debug_hosts"] | 16);

  if (port == 0) port = 6666;
  if (connectTimeoutMs < 30) connectTimeoutMs = 30;
  if (connectTimeoutMs > 4000) connectTimeoutMs = 4000;
  if (readMs < 40) readMs = 40;
  if (readMs > 5000) readMs = 5000;
  if (probeConnectTimeoutMs < 120) probeConnectTimeoutMs = 120;
  if (probeConnectTimeoutMs > 6000) probeConnectTimeoutMs = 6000;
  if (probeReadMs < 200) probeReadMs = 200;
  if (probeReadMs > 7000) probeReadMs = 7000;
  if (startupDelayMs > 500) startupDelayMs = 500;
  if (keepaliveAfterMs > 10000) keepaliveAfterMs = 10000;
  if (maxHosts < 1) maxHosts = 1;
  if (maxHosts > 512) maxHosts = 512;
  if (retries < 1) retries = 1;
  if (retries > 3) retries = 3;
  if (workers < 1) workers = 1;
  // ESP32-C3 guard: cap parallel probes to 10.
  if (workers > 10) workers = 10;
  if (maxDebugHosts < 1) maxDebugHosts = 1;
  if (maxDebugHosts > 64) maxDebugHosts = 64;

  IPAddress ip = WiFi.localIP();
  IPAddress mask = WiFi.subnetMask();
  if ((uint32_t)ip == 0 || (uint32_t)mask == 0) {
    sendJsonError(500, "failed to read sta ip/mask");
    return;
  }

  // IMPORTANT: operate on big-endian integer form to avoid host-order
  // ambiguities in IPAddress(uint32_t) conversion.
  uint32_t ipU = ipToBeU32(ip);
  uint32_t maskU = ipToBeU32(mask);
  uint32_t netU = ipU & maskU;
  uint32_t bcU = netU | (~maskU);
  if (bcU <= netU + 1U) {
    sendJsonError(400, "subnet too small");
    return;
  }

  uint32_t hostCount = bcU - netU - 1;
  if (hostCount > maxHosts) hostCount = maxHosts;

  if (workers > 1) {
    uint32_t t0 = (uint32_t)millis();
    H312ScanShared sh;
    sh.ipU = ipU;
    sh.netU = netU;
    sh.hostCount = hostCount;
    sh.port = port;
    sh.connectTimeoutMs = connectTimeoutMs;
    sh.readMs = readMs;
    sh.probeConnectTimeoutMs = probeConnectTimeoutMs;
    sh.probeReadMs = probeReadMs;
    sh.startupDelayMs = startupDelayMs;
    sh.keepaliveAfterMs = keepaliveAfterMs;
    sh.retries = retries;
    sh.includeWeak = includeWeak;
    sh.stopOnFirst = stopOnFirst;
    sh.lock = xSemaphoreCreateMutex();
    if (!sh.lock) {
      sendJsonError(500, "scan worker lock failed");
      return;
    }

    uint8_t workerCount = workers;
    if ((uint32_t)workerCount > hostCount) workerCount = (uint8_t)hostCount;
    if (workerCount < 1) workerCount = 1;

    uint8_t created = 0;
    for (uint8_t w = 0; w < workerCount; w++) {
      TaskHandle_t th = nullptr;
      BaseType_t rc = xTaskCreate(h312ScanWorkerTask, "h312scan", 6144, &sh, 1, &th);
      if (rc == pdPASS) created++;
    }
    if (created == 0) {
      vSemaphoreDelete(sh.lock);
      sendJsonError(500, "scan workers start failed");
      return;
    }

    while (true) {
      uint32_t done = 0;
      xSemaphoreTake(sh.lock, portMAX_DELAY);
      done = sh.workersDone;
      xSemaphoreGive(sh.lock);
      if (done >= created) break;
      delay(2);
    }

    String body;
    body.reserve(1700);
    body += F("{\"ok\":true,\"devices\":[");
    for (uint8_t idx = 0; idx < sh.deviceCount; idx++) {
      const H312ScanFoundDevice &d = sh.devices[idx];
      if (idx) body += ",";
      body += F("{\"ip\":\"");
      body += jsonEscape(d.ip);
      body += F("\",\"name\":\"");
      body += jsonEscape(d.name);
      body += F("\",\"model\":\"");
      body += jsonEscape(d.model);
      body += F("\",\"port\":");
      body += String((uint32_t)port);
      if (!d.sn.isEmpty()) {
        body += F(",\"sn\":\"");
        body += jsonEscape(d.sn);
        body += F("\"");
      }
      if (d.ws >= 0) {
        body += F(",\"workStatus\":");
        body += String(d.ws);
      }
      if (d.hf >= 0) {
        body += F(",\"heatFlag\":");
        body += String(d.hf);
      }
      if (d.rt > -200) {
        body += F(",\"realTimeTemp\":");
        body += String(d.rt);
      }
      if (d.weak) {
        body += F(",\"weak\":true");
        body += F(",\"rxBytes\":");
        body += String((uint32_t)d.weakRxBytes);
      }
      body += F("}");
    }
    body += F("],\"scanned\":");
    body += String(sh.scanned);
    uint32_t orderProcessed = sh.scanned + sh.skippedSelf;
    bool orderComplete = (!sh.stopTriggered) && (orderProcessed >= hostCount);
    body += F(",\"order_processed\":");
    body += String(orderProcessed);
    body += F(",\"order_target\":");
    body += String(hostCount);
    body += F(",\"order_complete\":");
    body += (orderComplete ? "true" : "false");
    body += F(",\"host_total\":");
    body += String((uint32_t)(bcU - netU - 1));
    body += F(",\"skipped_self\":");
    body += String(sh.skippedSelf);
    body += F(",\"found_probe\":");
    body += String(sh.foundProbe);
    body += F(",\"found_weak\":");
    body += String(sh.foundWeak);
    body += F(",\"failed\":");
    body += String(sh.failCount);
    body += F(",\"include_weak\":");
    body += (includeWeak ? "true" : "false");
    body += F(",\"stop_on_first\":");
    body += (stopOnFirst ? "true" : "false");
    body += F(",\"stopped\":");
    body += (sh.stopTriggered ? "true" : "false");
    body += F(",\"retries\":");
    body += String((uint32_t)retries);
    body += F(",\"workers\":");
    body += String((uint32_t)created);
    body += F(",\"parallel\":true");
    body += F(",\"timeout_ms\":");
    body += String((uint32_t)connectTimeoutMs);
    body += F(",\"read_ms\":");
    body += String((uint32_t)readMs);
    body += F(",\"startup_delay_ms\":");
    body += String((uint32_t)startupDelayMs);
    body += F(",\"keepalive_after_ms\":");
    body += String((uint32_t)keepaliveAfterMs);
    body += F(",\"probe_timeout_ms\":");
    body += String((uint32_t)probeConnectTimeoutMs);
    body += F(",\"probe_read_ms\":");
    body += String((uint32_t)probeReadMs);
    if (debug) {
      body += F(",\"debug_note\":\"disabled_in_parallel\"");
    }
    body += F(",\"ms\":");
    body += String((uint32_t)((uint32_t)millis() - t0));
    body += F("}");

    vSemaphoreDelete(sh.lock);
    sendJson(200, body);
    return;
  }

  String body;
  body.reserve(3600);
  body += F("{\"ok\":true,\"devices\":[");
  bool first = true;
  uint32_t scanned = 0;
  uint32_t skippedSelf = 0;
  bool stopTriggered = false;
  uint32_t foundProbe = 0;
  uint32_t foundWeak = 0;
  uint32_t failCount = 0;
  uint32_t t0 = (uint32_t)millis();
  String dbg;
  bool dbgFirst = true;
  uint8_t dbgCount = 0;
  if (debug) dbg = "[";

  for (uint32_t orderIdx = 0; orderIdx < hostCount; orderIdx++) {
    uint32_t i = h312HostOrderToOffset(orderIdx, hostCount);

    uint32_t hostU = netU + i;
    if (hostU == ipU) {
      skippedSelf++;
      if (debug && dbgCount < maxDebugHosts) {
        if (!dbgFirst) dbg += ",";
        dbgFirst = false;
        dbg += F("{\"ip\":\"");
        dbg += jsonEscape(beU32ToIp(hostU).toString());
        dbg += F("\",\"reason\":\"skipped_self\",\"rxBytes\":0}");
        dbgCount++;
      }
      continue;
    }

    IPAddress target = beU32ToIp(hostU);
    String name;
    String model;
    String sn;
    int ws = -1;
    int rt = -1000;
    int hf = -1;
    String failReason = "unknown";
    bool ok = false;
    bool weak = false;
    size_t weakRxBytes = 0;
    // Probe directly with handshake parity to avoid false negatives from pre-check only.
    for (uint8_t attempt = 0; attempt < retries; attempt++) {
      String probeReason;
      if (h312ProbeHost(target, port, probeConnectTimeoutMs, probeReadMs, &name, &model, &sn, &ws, &rt, true, startupDelayMs,
                        keepaliveAfterMs, &hf, &probeReason)) {
        ok = true;
        failReason = probeReason;
        break;
      }
      if (!probeReason.isEmpty()) failReason = probeReason;
      delay(2);
    }

    // Optional single weak-check after all probe retries.
    // Keeps request count predictable: retries + at most 1.
    if (!ok && includeWeak &&
        h312SocketLooksAlive(target, port, (uint16_t)(connectTimeoutMs + 220), (uint16_t)max((int)readMs * 3, 900), &weakRxBytes)) {
      ok = true;
      weak = true;
      if (name.isEmpty()) name = "H31X";
      if (model.isEmpty()) model = "H31X";
      if (sn.isEmpty()) sn = "";
      if (ws < 0) ws = -1;
      if (rt <= -200) rt = -1000;
    }

    if (ok) {
      if (weak) foundWeak++;
      else foundProbe++;
      if (!first) body += ",";
      first = false;
      body += F("{\"ip\":\"");
      body += jsonEscape(target.toString());
      body += F("\",\"name\":\"");
      body += jsonEscape(name);
      body += F("\",\"model\":\"");
      body += jsonEscape(model);
      body += F("\",\"port\":");
      body += String((uint32_t)port);
      if (!sn.isEmpty()) {
        body += F(",\"sn\":\"");
        body += jsonEscape(sn);
        body += F("\"");
      }
      if (ws >= 0) {
        body += F(",\"workStatus\":");
        body += String(ws);
      }
      if (hf >= 0) {
        body += F(",\"heatFlag\":");
        body += String(hf);
      }
      if (rt > -200) {
        body += F(",\"realTimeTemp\":");
        body += String(rt);
      }
      if (weak) {
        body += F(",\"weak\":true");
        body += F(",\"rxBytes\":");
        body += String((uint32_t)weakRxBytes);
        if (debug && dbgCount < maxDebugHosts) {
          if (!dbgFirst) dbg += ",";
          dbgFirst = false;
          dbg += F("{\"ip\":\"");
          dbg += jsonEscape(target.toString());
          dbg += F("\",\"reason\":\"weak_stream\",\"rxBytes\":");
          dbg += String((uint32_t)weakRxBytes);
          dbg += F("}");
          dbgCount++;
        }
      }
      body += F("}");

      if (stopOnFirst) {
        scanned++;
        stopTriggered = true;
        break;
      }
    } else {
      failCount++;
      if (debug && dbgCount < maxDebugHosts) {
        if (!dbgFirst) dbg += ",";
        dbgFirst = false;
        dbg += F("{\"ip\":\"");
        dbg += jsonEscape(target.toString());
        dbg += F("\",\"reason\":\"");
        dbg += jsonEscape(failReason);
        dbg += F("\",\"rxBytes\":");
        dbg += String((uint32_t)0);
        dbg += F("}");
        dbgCount++;
      }
    }
    scanned++;
    delay(1);
  }
  if (debug) dbg += "]";

  body += F("],\"scanned\":");
  body += String(scanned);
  uint32_t orderProcessed = scanned + skippedSelf;
  bool orderComplete = (!stopTriggered) && (orderProcessed >= hostCount);
  body += F(",\"order_processed\":");
  body += String(orderProcessed);
  body += F(",\"order_target\":");
  body += String(hostCount);
  body += F(",\"order_complete\":");
  body += (orderComplete ? "true" : "false");
  body += F(",\"host_total\":");
  body += String((uint32_t)(bcU - netU - 1));
  body += F(",\"skipped_self\":");
  body += String(skippedSelf);
  body += F(",\"found_probe\":");
  body += String(foundProbe);
  body += F(",\"found_weak\":");
  body += String(foundWeak);
  body += F(",\"failed\":");
  body += String(failCount);
  body += F(",\"include_weak\":");
  body += (includeWeak ? "true" : "false");
  body += F(",\"stop_on_first\":");
  body += (stopOnFirst ? "true" : "false");
  body += F(",\"stopped\":");
  body += (stopTriggered ? "true" : "false");
  body += F(",\"retries\":");
  body += String((uint32_t)retries);
  body += F(",\"workers\":1");
  body += F(",\"parallel\":false");
  body += F(",\"timeout_ms\":");
  body += String((uint32_t)connectTimeoutMs);
  body += F(",\"read_ms\":");
  body += String((uint32_t)readMs);
  body += F(",\"startup_delay_ms\":");
  body += String((uint32_t)startupDelayMs);
  body += F(",\"keepalive_after_ms\":");
  body += String((uint32_t)keepaliveAfterMs);
  body += F(",\"probe_timeout_ms\":");
  body += String((uint32_t)probeConnectTimeoutMs);
  body += F(",\"probe_read_ms\":");
  body += String((uint32_t)probeReadMs);
  if (debug) {
    body += F(",\"debug\":");
    body += dbg;
  }
  body += F(",\"ms\":");
  body += String((uint32_t)((uint32_t)millis() - t0));
  body += F("}");

  sendJson(200, body);
}

static void handleApiH312Probe() {
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  if (WiFi.status() != WL_CONNECTED) {
    sendJsonError(400, "wifi sta not connected");
    return;
  }

  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }

  String ipText = doc["ip"] | "";
  ipText.trim();
  uint16_t port = (uint16_t)(doc["port"] | (int)h312Port);
  uint8_t attempts = (uint8_t)(doc["attempts"] | 3);
  uint16_t connectTimeoutMs = (uint16_t)(doc["connect_timeout_ms"] | 2500);
  uint16_t readMs = (uint16_t)(doc["read_ms"] | 2600);
  uint16_t startupDelayMs = (uint16_t)(doc["startup_delay_ms"] | 20);
  uint16_t keepaliveAfterMs = (uint16_t)(doc["keepalive_after_ms"] | 700);
  if (port == 0) port = 6666;
  if (ipText.isEmpty()) {
    sendJsonError(400, "ip required");
    return;
  }

  String name;
  String model;
  String sn;
  String reason;
  int ws = -1;
  int rt = -1000;
  bool ok = h312VerifyTarget(ipText, port, &name, &model, &sn, &ws, &rt, &reason, attempts, connectTimeoutMs, readMs, startupDelayMs,
                             keepaliveAfterMs);
  if (!ok) {
    String body;
    body.reserve(160);
    body += F("{\"ok\":false,\"error\":\"target is not H310/H312 or no telemetry\",\"reason\":\"");
    body += jsonEscape(reason);
    body += F("\"}");
    sendJson(400, body);
    return;
  }

  String body;
  body.reserve(220);
  body += F("{\"ok\":true,\"ip\":\"");
  body += jsonEscape(ipText);
  body += F("\",\"port\":");
  body += String((uint32_t)port);
  body += F(",\"name\":\"");
  body += jsonEscape(name);
  body += F("\",\"model\":\"");
  body += jsonEscape(model);
  if (!sn.isEmpty()) {
    body += F("\",\"sn\":\"");
    body += jsonEscape(sn);
  }
  body += F("\"");
  if (ws >= 0) {
    body += F(",\"workStatus\":");
    body += String(ws);
  }
  if (rt > -200) {
    body += F(",\"realTimeTemp\":");
    body += String(rt);
  }
  body += F("}");
  sendJson(200, body);
}

static void handleApiExtractorPower() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  bool on = doc["on"] | false;
  ExtractorSource reqSource = gActiveSource;
  if (!doc["source"].isNull()) {
    String rawSource = String((const char *)(doc["source"] | ""));
    ExtractorSource parsed;
    if (!parseExtractorSource(rawSource, &parsed)) {
      sendJsonError(400, "bad source");
      return;
    }
    reqSource = parsed;
  } else if (!on && gExtractorSessionActive) {
    reqSource = gExtractorSessionSource;
  }
  setActiveSource(reqSource);
  uint32_t t0 = millis();
  bool wasReady = bleReady();
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[api] extractor/power req on=%d src=%s was_ready=%d\n", on ? 1 : 0, extractorSourceText(reqSource),
                  wasReady ? 1 : 0);
  }

  uint8_t target = 0;
  int speedBeforeOff = -1;
  bool savedOnOff = false;
  uint8_t savedSpeedOnOff = 0;
  if (on) {
    if (gExtractorSessionActive && gExtractorSessionSource != reqSource) {
      ExtractorSource prevSource = gExtractorSessionSource;
      int prevSpeed = bleReadSpeedStable(2, 120);
      uint8_t prevSaved = 0;
      bool prevSavedOk = finalizeExtractorSession(prevSpeed, &prevSaved);
      if (prevSavedOk && kUartDiagEnabled) {
        LB_SERIAL_PRINTF("[api] extractor/power switch-save src=%s speed=%u\n", extractorSourceText(prevSource),
                      (unsigned int)prevSaved);
      }
    }
    target = sourceSpeed(reqSource);
    if (target == 0) target = kAutoOnSpeedCap;
  } else if (gExtractorSessionActive && gExtractorSessionSource == reqSource) {
    speedBeforeOff = bleReadSpeedStable(2, 120);
  }
  bool ok = bleSetSpeed(target);
  uint32_t dt = (uint32_t)(millis() - t0);
  if (ok) {
    if (on) {
      beginExtractorSession(reqSource, target, false);
      uint8_t lm = sourceLightMode(reqSource);
      (void)bleSetLightMode(lm);
      if (lm != LIGHT_OFF) {
        (void)bleSetLightBrightness(sourceTargetBrightness(reqSource), reqSource);
      }
      if (kBleReleaseAfterOn) {
        bleResetHandles();
        bleSetConnectSource("session_release_on");
      }
    } else {
      if (speedBeforeOff < 0 && gBleLastSetTarget == 0 && gBleLastSetBefore > 0) {
        uint32_t nowSet = (uint32_t)millis();
        if (nowSet >= gBleLastSetMs && (nowSet - gBleLastSetMs) <= 4000U) speedBeforeOff = gBleLastSetBefore;
      }
      savedOnOff = finalizeExtractorSession(speedBeforeOff, &savedSpeedOnOff);
      captureLastEnabledLightFromDevice(reqSource);
      (void)bleSetLightMode(LIGHT_OFF);
    }
  }
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[api] extractor/power res ok=%d on=%d src=%s target=%u dt=%lu ready_after=%d tx=%s\n", ok ? 1 : 0,
                  on ? 1 : 0, extractorSourceText(reqSource), (unsigned int)target, (unsigned long)dt,
                  bleReady() ? 1 : 0, gBleLastTx.c_str());
  }

  String body;
  body.reserve(520);
  body += F("{\"ok\":");
  body += (ok ? "true" : "false");
  body += F(",\"requested_on\":");
  body += (on ? "true" : "false");
  body += F(",\"source\":\"");
  body += extractorSourceText(reqSource);
  body += F("\"");
  body += F(",\"target\":");
  body += String((int)target);
  body += F(",\"last_on_speed\":");
  body += String((int)sourceSpeed(reqSource));
  body += F(",\"ready_before\":");
  body += (wasReady ? "true" : "false");
  body += F(",\"ready_after\":");
  body += (bleReady() ? "true" : "false");
  body += F(",\"connect_source\":\"");
  body += jsonEscape(gBleConnectSource);
  body += F("\",\"addr\":\"");
  body += jsonEscape(gBleLastAddr);
  body += F("\",\"name\":\"");
  body += jsonEscape(gBleLastName);
  body += F("\",\"rssi\":");
  body += String(gBleLastRssi);
  body += F(",\"tx\":\"");
  body += jsonEscape(gBleLastTx);
  body += F("\"");
  body += F(",\"duration_ms\":");
  body += String(dt);
  body += F(",\"saved_updated\":");
  body += (savedOnOff ? "true" : "false");
  body += F(",\"saved_speed\":");
  body += String((int)(savedOnOff ? savedSpeedOnOff : sourceSpeed(reqSource)));
  if (!ok) {
    body += F(",\"error\":\"ble command failed\"");
  }
  body += F("}");

  if (!ok) {
    sendJson(500, body);
    return;
  }
  sendJson(200, body);
}

static void handleApiExtractorSpeed() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  int v = doc["speed"] | 0;
  if (v < 0) v = 0;
  if (v > 100) v = 100;
  ExtractorSource reqSource = gActiveSource;
  if (!doc["source"].isNull()) {
    String rawSource = String((const char *)(doc["source"] | ""));
    ExtractorSource parsed;
    if (!parseExtractorSource(rawSource, &parsed)) {
      sendJsonError(400, "bad source");
      return;
    }
    reqSource = parsed;
  }
  setActiveSource(reqSource);
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[api] extractor/speed req speed=%d src=%s ready_before=%d\n", v, extractorSourceText(reqSource),
                  bleReady() ? 1 : 0);
  }
  bool ok = bleSetSpeed((uint8_t)v);
  if (!ok) {
    if (kUartDiagEnabled) {
      LB_SERIAL_PRINTF("[api] extractor/speed res ok=0 speed=%d src=%s tx=%s\n", v, extractorSourceText(reqSource),
                    gBleLastTx.c_str());
    }
    sendJsonError(500, "ble command failed");
    return;
  }
  if (v > 0) saveSourceSpeed(reqSource, (uint8_t)v);
  if (kUartDiagEnabled) {
    LB_SERIAL_PRINTF("[api] extractor/speed res ok=1 speed=%d src=%s ready_after=%d tx=%s\n", v, extractorSourceText(reqSource),
                  bleReady() ? 1 : 0, gBleLastTx.c_str());
  }
  sendJson(200, "{\"ok\":true}");
}

static void handleApiExtractorStatus() {
  bool bleOk = bleReady();
  int cur = -1;

  // Do not auto-connect by default: this endpoint is polled by UI and should not lock BLE.
  bool doConnect = server.hasArg("connect") && server.arg("connect") == "1";
  const bool h312BlePriority = (h312Enabled && h312Mode == H312_MODE_BLE);
  if (h312BlePriority && doConnect) doConnect = false;
  if (!bleOk && doConnect) {
    bleOk = bleEnsureConnected(1800);
  }
  if (bleOk) {
    cur = bleReadSpeedDirect();
    if (cur < 0) bleOk = false;
  }
  if (cur < 0) cur = 0;

  String body;
  body.reserve(420);
  body += F("{\"ok\":true,\"ble_ok\":");
  body += (bleOk ? "true" : "false");
  body += F(",\"ble_bound\":");
  body += (bleBound ? "true" : "false");
  body += F(",\"safe_mode\":");
  body += (gSafeMode ? "true" : "false");
  body += F(",\"h312_ble_priority\":");
  body += (h312BlePriority ? "true" : "false");
  body += F(",\"speed\":");
  body += String(cur);
  body += F(",\"saved\":");
  body += String((int)sourceSpeed(gActiveSource));
  body += F(",\"saved_source\":\"");
  body += extractorSourceText(gActiveSource);
  body += F("\"");
  body += F(",\"ble_prefix\":\"");
  body += jsonEscape(bleNamePrefix);
  body += F("\",\"ble_locked_addr\":\"");
  body += jsonEscape(bleLockedAddr);
  body += F("\",\"ble_locked_name\":\"");
  body += jsonEscape(bleLockedName);
  body += F("\",\"connect_source\":\"");
  body += jsonEscape(gBleConnectSource);
  body += F("\",\"addr\":\"");
  body += jsonEscape(gBleLastAddr);
  body += F("\",\"name\":\"");
  body += jsonEscape(gBleLastName);
  body += F("\",\"rssi\":");
  body += String(gBleLastRssi);
  body += F(",\"tx\":\"");
  body += jsonEscape(gBleLastTx);
  body += F("\"}");
  sendJson(200, body);
}

static void handleApiExtractorCmdLog() {
  uint32_t since = 0;
  if (server.hasArg("since")) {
    String s = server.arg("since");
    s.trim();
    if (s.length() > 0) since = (uint32_t)strtoul(s.c_str(), nullptr, 10);
  }

  String body;
  body.reserve(4096);
  body += F("{\"ok\":true,\"next_seq\":");
  body += String(gExtractorCmdLogSeq);
  body += F(",\"items\":[");

  bool first = true;
  for (uint16_t i = 0; i < gExtractorCmdLogCount; i++) {
    uint16_t idx = (uint16_t)((gExtractorCmdLogHead + i) % kExtractorCmdLogCap);
    const ExtractorCmdLogEntry &e = gExtractorCmdLog[idx];
    if (e.seq <= since) continue;
    if (!first) body += ",";
    first = false;
    body += F("{\"seq\":");
    body += String(e.seq);
    body += F(",\"ms\":");
    body += String(e.ms);
    body += F(",\"line\":\"");
    body += jsonEscape(String(e.line));
    body += F("\"}");
  }

  body += F("]}");
  sendJson(200, body);
}

static void handleApiExtractorScan() {
  JsonDocument doc;
  uint32_t scanMs = 4000;
  if (server.hasArg("plain")) {
    if (parseJsonBody(doc)) {
      scanMs = (uint32_t)(doc["scan_ms"] | scanMs);
    }
  }
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  if (scanMs < 800) scanMs = 800;
  if (scanMs > 12000) scanMs = 12000;

  NimBLEScan *scan = NimBLEDevice::getScan();
  scan->setActiveScan(true);
  scan->setInterval(45);
  scan->setWindow(15);
  scan->setDuplicateFilter(true);
  scan->setMaxResults(30);

  uint32_t durSec = (scanMs + 999) / 1000;
  if (durSec < 1) durSec = 1;
  if (durSec > 12) durSec = 12;

  NimBLEScanResults res = scan->start(durSec, false);

  String body;
  body.reserve(900);
  body += F("{\"ok\":true,\"devices\":[");
  bool first = true;
  for (int i = 0; i < res.getCount(); i++) {
    NimBLEAdvertisedDevice dev = res.getDevice((uint32_t)i);
    std::string name = dev.getName();
    if (name.empty()) continue;
    String s(name.c_str());
    if (!bleNamePrefix.isEmpty() && !s.startsWith(bleNamePrefix)) continue;

    if (!first) body += ',';
    first = false;
    body += F("{\"name\":\"");
    body += jsonEscape(s);
    body += F("\",\"addr\":\"");
    body += jsonEscape(String(dev.getAddress().toString().c_str()));
    body += F("\",\"rssi\":");
    body += String(dev.getRSSI());
    body += F("}");
  }
  body += F("]}");

  scan->clearResults();
  sendJson(200, body);
}

static void handleApiExtractorSelect() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }
  if (gSafeMode) {
    sendJsonError(423, "safe mode");
    return;
  }
  String prefix = doc["prefix"] | bleNamePrefix;
  String addr = doc["addr"] | bleLockedAddr;
  String name = doc["name"] | bleLockedName;
  prefix.trim();
  addr.trim();
  name.trim();
  saveBle(prefix, addr, name, true);
  bleResetHandles(); // force reconnect using new config
  sendJson(200, "{\"ok\":true}");
}

static void handleApiTimeStatus() {
  uint64_t utcMs = clockNowEpochMs();
  String body;
  body.reserve(320);
  body += F("{\"ok\":true,\"valid\":");
  body += (clockValid ? "true" : "false");
  body += F(",\"tz_configured\":");
  body += (tzConfigured ? "true" : "false");
  body += F(",\"tz_offset_min\":");
  body += String((int)tzOffsetMin);
  body += F(",\"tz_name\":\"");
  body += jsonEscape(tzName);
  body += F("\",\"utc_epoch_ms\":");
  char epochBuf[24];
  snprintf(epochBuf, sizeof(epochBuf), "%llu", (unsigned long long)utcMs);
  body += epochBuf;
  body += F(",\"local_time\":\"");
  body += jsonEscape(clockLocalText());
  body += F("\",\"minute_of_day\":");
  body += String((uint32_t)clockLocalMinuteOfDay());
  body += F(",\"last_sync_source\":\"");
  body += jsonEscape(clockLastSyncSource);
  body += F("\",\"last_set_ms\":");
  body += String((uint32_t)clockLastSetMs);
  body += F(",\"last_ntp_sync_ms\":");
  body += String((uint32_t)clockLastNtpSyncMs);
  body += F(",\"auto_stop_enabled\":");
  body += (autoStopEnabled ? "true" : "false");
  body += F(",\"auto_stop_minute\":");
  body += String((uint32_t)autoStopMinute);
  body += F("}");
  sendJson(200, body);
}

static void handleApiTimeSet() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }

  uint64_t epochMs = doc["epoch_ms"] | 0ULL;
  if (epochMs < 1500000000000ULL) {
    sendJsonError(400, "epoch_ms required");
    return;
  }

  bool tzChanged = false;
  int16_t nextOffset = tzOffsetMin;
  String nextName = tzName;
  if (!doc["tz_offset_min"].isNull()) {
    nextOffset = clampTzOffsetMin((int)(doc["tz_offset_min"] | 0));
    tzChanged = true;
  }
  if (!doc["tz_name"].isNull()) {
    nextName = normalizeTzName(String((const char *)doc["tz_name"]));
    tzChanged = true;
  }
  if (tzChanged || !tzConfigured) {
    saveTimeConfig(nextOffset, nextName);
  }

  clockSetFromEpochMs(epochMs, "client");
  sendJson(200, "{\"ok\":true}");
}

static void handleApiTimeSync() {
  bool ok = clockTryNtpSync(true);
  if (!ok) {
    sendJsonError(503, "ntp sync failed");
    return;
  }
  sendJson(200, "{\"ok\":true}");
}

static void handleApiConfigGet() {
  String body;
  body.reserve(1400);
  body += F("{\"ok\":true,\"wifi_autooff_min\":");
  body += String((uint32_t)wifiAutoOffMin);
  body += F(",\"ble_bound\":");
  body += (bleBound ? "true" : "false");
  body += F(",\"ble_prefix\":\"");
  body += jsonEscape(bleNamePrefix);
  body += F("\",\"ble_locked_addr\":\"");
  body += jsonEscape(bleLockedAddr);
  body += F("\",\"ble_locked_name\":\"");
  body += jsonEscape(bleLockedName);
  body += F("\",\"h312_enabled\":");
  body += (h312Enabled ? "true" : "false");
  body += F(",\"h312_ip\":\"");
  body += jsonEscape(h312Ip);
  body += F("\",\"h312_port\":");
  body += String((uint32_t)h312Port);
  appendJsonH312StaticConfigFields(body);
  appendJsonLightProfileFields(body);
  body += F(",\"tz_offset_min\":");
  body += String((int)tzOffsetMin);
  body += F(",\"tz_name\":\"");
  body += jsonEscape(tzName);
  body += F("\",\"tz_configured\":");
  body += (tzConfigured ? "true" : "false");
  body += F(",\"auto_stop_enabled\":");
  body += (autoStopEnabled ? "true" : "false");
  body += F(",\"auto_stop_minute\":");
  body += String((uint32_t)autoStopMinute);
  body += F(",\"web_auth_user\":\"");
  body += jsonEscape(gWebAuthUser);
  body += F("\",\"web_auth_pass_set\":");
  body += (gWebAuthPass.length() >= 8 ? "true" : "false");
  body += F("}");
  sendJson(200, body);
}

static void handleApiConfigSet() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }
  if (!doc["wifi_autooff_min"].isNull()) {
    int v = doc["wifi_autooff_min"] | 0;
    if (v < 0) v = 0;
    if (v > 1440) v = 1440;
    saveWifiAutoOffMin((uint16_t)v);
  }
  String nextWebUser = gWebAuthUser;
  String nextWebPass = gWebAuthPass;
  bool hasWebUser = !doc["web_user"].isNull();
  bool hasWebPass = !doc["web_pass"].isNull();
  if (hasWebUser) {
    nextWebUser = String((const char *)doc["web_user"]);
    nextWebUser.trim();
    if (!isValidWebUser(nextWebUser)) {
      sendJsonError(400, "web_user invalid (3..24, a-zA-Z0-9_.-)");
      return;
    }
  }
  if (hasWebPass) {
    nextWebPass = String((const char *)doc["web_pass"]);
    nextWebPass.trim();
    if (nextWebPass.length() < 8) {
      sendJsonError(400, "web_pass must be >= 8 chars");
      return;
    }
    if (nextWebPass == "admin" || nextWebPass == "12345678") {
      sendJsonError(400, "web_pass too weak");
      return;
    }
  }
  if (hasWebUser || hasWebPass) {
    if (!requireCriticalPassword(&doc, "web_auth_change")) return;
    bool changedUser = (nextWebUser != gWebAuthUser);
    bool changedPass = (nextWebPass != gWebAuthPass);
    if (changedUser || changedPass) {
      prefs.begin(kPrefsNs, false);
      if (changedUser) prefs.putString("web_user", nextWebUser);
      if (changedPass) prefs.putString("web_pass", nextWebPass);
      prefs.end();
      gWebAuthUser = nextWebUser;
      gWebAuthPass = nextWebPass;
      gApPass = nextWebPass;
      rememberRevokeAll();
      server.sendHeader("Set-Cookie", rememberCookieHeader("", true));
      gWebAuthFailCount = 0;
      gWebAuthBlockUntilMs = 0;
      gWebAuthFailIp = IPAddress(0, 0, 0, 0);
      extractorCmdLogPushf("[security] web auth changed user=%s pass=%d",
                           gWebAuthUser.c_str(),
                           changedPass ? 1 : 0);
    }
  }
  if (!doc["ble_prefix"].isNull() || !doc["ble_locked_addr"].isNull() || !doc["ble_locked_name"].isNull()) {
    String prefix = doc["ble_prefix"] | bleNamePrefix;
    String addr = doc["ble_locked_addr"] | bleLockedAddr;
    String name = doc["ble_locked_name"] | bleLockedName;
    prefix.trim();
    addr.trim();
    name.trim();
    bool bound = (!addr.isEmpty() || !name.isEmpty());
    saveBle(prefix, addr, name, bound);
    bleResetHandles();
  }

  if (!doc["h312_enabled"].isNull() || !doc["h312_mode"].isNull() || !doc["h312_ip"].isNull() || !doc["h312_ble_addr"].isNull() ||
      !doc["h312_ble_name"].isNull() || !doc["h312_port"].isNull() || !doc["h312_t_on"].isNull() || !doc["h312_t_off"].isNull() ||
      !doc["h312_debounce_ms"].isNull() || !doc["h312_min_on_ms"].isNull() || !doc["h312_slot"].isNull()) {
    const bool hasEnabledField = !doc["h312_enabled"].isNull();
    const bool touchesBleCfg = !doc["h312_mode"].isNull() || !doc["h312_ble_addr"].isNull() || !doc["h312_ble_name"].isNull();
    bool enabled = hasEnabledField ? (bool)(doc["h312_enabled"] | false) : h312Enabled;
    H312TransportMode mode = doc["h312_mode"].isNull() ? h312Mode : h312ModeFromText(String((const char *)(doc["h312_mode"] | "")));
    String ip = doc["h312_ip"] | h312Ip;
    String bleAddr = doc["h312_ble_addr"] | h312BleAddr;
    String bleName = doc["h312_ble_name"] | h312BleName;
    int port = doc["h312_port"] | (int)h312Port;
    int tOn = doc["h312_t_on"] | (int)h312TempOn;
    int tOff = doc["h312_t_off"] | (int)h312TempOff;
    int debounceMs = doc["h312_debounce_ms"] | (int)h312DebounceMs;
    uint32_t minOnMs = (uint32_t)(doc["h312_min_on_ms"] | (int)h312MinOnMs);
    int slot = doc["h312_slot"] | (int)h312Slot;
    ip.trim();
    bleAddr.trim();
    bleName.trim();

    // UI BLE bind flows may update mode/addr/name without sending h312_enabled.
    // After factory reset this keeps H312 disabled forever. Auto-enable when a BLE
    // target is configured unless caller explicitly provides h312_enabled.
    if (touchesBleCfg && mode == H312_MODE_BLE) {
      const bool hasBleTarget = h312BleAddrLooksValid(bleAddr) || !bleName.isEmpty();
      if (hasBleTarget && !enabled) {
        enabled = true;
        extractorCmdLogPushf("[h312] auto-enable on ble bind addr=%s name=%s", bleAddr.c_str(), bleName.c_str());
      }
    }

    if (port < 1 || port > 65535) {
      sendJsonError(400, "h312_port out of range");
      return;
    }
    if (mode != H312_MODE_WIFI && mode != H312_MODE_BLE) {
      sendJsonError(400, "h312_mode must be wifi|ble");
      return;
    }
    if (!bleAddr.isEmpty() && !h312BleAddrLooksValid(bleAddr)) {
      sendJsonError(400, "h312_ble_addr invalid");
      return;
    }
    if (tOn < -40 || tOn > 600 || tOff < -40 || tOff > 600) {
      sendJsonError(400, "h312 temperature threshold out of range");
      return;
    }
    if (tOn <= tOff) {
      sendJsonError(400, "h312_t_on must be > h312_t_off");
      return;
    }
    if (debounceMs < 0 || debounceMs > 60000) {
      sendJsonError(400, "h312_debounce_ms out of range");
      return;
    }
    if (minOnMs > 300000UL) {
      sendJsonError(400, "h312_min_on_ms out of range");
      return;
    }
    if (slot < 1 || slot > 3) {
      sendJsonError(400, "h312_slot out of range (1..3)");
      return;
    }

    bool changed = (enabled != h312Enabled) || (mode != h312Mode) || (ip != h312Ip) || (bleAddr != h312BleAddr) ||
                   (bleName != h312BleName) || ((uint16_t)port != h312Port) || ((int16_t)tOn != h312TempOn) ||
                   ((int16_t)tOff != h312TempOff) || ((uint16_t)debounceMs != h312DebounceMs) || (minOnMs != h312MinOnMs) ||
                   ((uint8_t)slot != h312Slot);
    if (changed) {
      // Do not hard-block config save on transient telemetry windows.
      // Runtime listener will validate stream by realTimeTemp.
      saveH312Config(enabled, mode, ip, bleAddr, bleName, (uint16_t)port, (int16_t)tOn, (int16_t)tOff, (uint16_t)debounceMs, minOnMs,
                     (uint8_t)slot);
      gH312PendingValid = false;
      gH312LastError = "";
      gH312LastLogic = "config_updated";
      bool hasTarget = (mode == H312_MODE_BLE) ? (h312BleAddrLooksValid(bleAddr) || !bleName.isEmpty()) : !ip.isEmpty();
      if (!enabled || !hasTarget) gH312RealTimeTemp = -1000;
    }
  }
  if (!doc["t420d_h1_gpio"].isNull() || !doc["t420d_h2_gpio"].isNull()) {
    int h1Req = doc["t420d_h1_gpio"] | (int)t420dH1Gpio;
    int h2Req = doc["t420d_h2_gpio"] | (int)t420dH2Gpio;
    if (h1Req != (int)kT420dHandle1GpioFixed || h2Req != (int)kT420dHandle2GpioFixed) {
      extractorCmdLogPushf("[gpio] t420d fixed mapping forced h1=%d->%d h2=%d->%d", h1Req, (int)kT420dHandle1GpioFixed, h2Req,
                           (int)kT420dHandle2GpioFixed);
    }
    if (t420dH1Gpio != kT420dHandle1GpioFixed || t420dH2Gpio != kT420dHandle2GpioFixed) {
      saveT420dGpioConfig(kT420dHandle1GpioFixed, kT420dHandle2GpioFixed);
      extractorCmdLogPushf("[gpio] t420d saved h1=%d h2=%d", (int)kT420dHandle1GpioFixed, (int)kT420dHandle2GpioFixed);
    }
  }
  if (!doc["t420d_h1_raised_on_high"].isNull() || !doc["t420d_h2_raised_on_high"].isNull()) {
    bool h1Rhi = doc["t420d_h1_raised_on_high"].isNull() ? t420dH1RaisedOnHigh : (bool)(doc["t420d_h1_raised_on_high"] | true);
    bool h2Rhi = doc["t420d_h2_raised_on_high"].isNull() ? t420dH2RaisedOnHigh : (bool)(doc["t420d_h2_raised_on_high"] | true);
    if (h1Rhi != t420dH1RaisedOnHigh || h2Rhi != t420dH2RaisedOnHigh) {
      saveT420dSensePolarity(h1Rhi, h2Rhi);
      gT420dPendingValid = false;
      extractorCmdLogPushf("[t420d] calib polarity h1=%s h2=%s", h1Rhi ? "high" : "low", h2Rhi ? "high" : "low");
    }
  }
  if (!doc["t420d_logic_enabled"].isNull() || !doc["t420d_h1_on_ms"].isNull() || !doc["t420d_h2_on_ms"].isNull() ||
      !doc["t420d_h1_off_ms"].isNull() || !doc["t420d_h2_off_ms"].isNull()) {
    bool enabled = doc["t420d_logic_enabled"].isNull() ? t420dLogicEnabled : (bool)(doc["t420d_logic_enabled"] | false);
    uint32_t h1OnMs = doc["t420d_h1_on_ms"].isNull() ? t420dH1OnDelayMs : (uint32_t)(doc["t420d_h1_on_ms"] | t420dH1OnDelayMs);
    uint32_t h2OnMs = doc["t420d_h2_on_ms"].isNull() ? t420dH2OnDelayMs : (uint32_t)(doc["t420d_h2_on_ms"] | t420dH2OnDelayMs);
    uint32_t h1OffMs =
        doc["t420d_h1_off_ms"].isNull() ? t420dH1OffDelayMs : (uint32_t)(doc["t420d_h1_off_ms"] | t420dH1OffDelayMs);
    uint32_t h2OffMs =
        doc["t420d_h2_off_ms"].isNull() ? t420dH2OffDelayMs : (uint32_t)(doc["t420d_h2_off_ms"] | t420dH2OffDelayMs);
    h1OnMs = clampDelayMs(h1OnMs);
    h2OnMs = clampDelayMs(h2OnMs);
    h1OffMs = clampDelayMs(h1OffMs);
    h2OffMs = clampDelayMs(h2OffMs);
    bool changed = (enabled != t420dLogicEnabled) || (h1OnMs != t420dH1OnDelayMs) || (h2OnMs != t420dH2OnDelayMs) ||
                   (h1OffMs != t420dH1OffDelayMs) || (h2OffMs != t420dH2OffDelayMs);
    if (changed) {
      saveT420dLogicConfig(enabled, h1OnMs, h2OnMs, h1OffMs, h2OffMs);
      extractorCmdLogPushf("[t420d] cfg en=%d on_ms=%lu/%lu off_ms=%lu/%lu", enabled ? 1 : 0, (unsigned long)h1OnMs,
                           (unsigned long)h2OnMs, (unsigned long)h1OffMs, (unsigned long)h2OffMs);
    }
  }
  if (!doc["light_h1"].isNull() || !doc["light_h2"].isNull() || !doc["light_ha1"].isNull() || !doc["light_ha2"].isNull() ||
      !doc["light_ha3"].isNull()) {
    int lh1 = doc["light_h1"].isNull() ? (int)sourceLightMode(SRC_H1) : (int)(doc["light_h1"] | 0);
    int lh2 = doc["light_h2"].isNull() ? (int)sourceLightMode(SRC_H2) : (int)(doc["light_h2"] | 0);
    int lha1 = doc["light_ha1"].isNull() ? (int)sourceLightMode(SRC_HA1) : (int)(doc["light_ha1"] | 0);
    int lha2 = doc["light_ha2"].isNull() ? (int)sourceLightMode(SRC_HA2) : (int)(doc["light_ha2"] | 0);
    int lha3 = doc["light_ha3"].isNull() ? (int)sourceLightMode(SRC_HA3) : (int)(doc["light_ha3"] | 0);
    if (lh1 < 0 || lh1 > 3 || lh2 < 0 || lh2 > 3 || lha1 < 0 || lha1 > 3 || lha2 < 0 || lha2 > 3 || lha3 < 0 || lha3 > 3) {
      sendJsonError(400, "light mode out of range (0..3)");
      return;
    }
    saveSourceLightMode(SRC_H1, (uint8_t)lh1);
    saveSourceLightMode(SRC_H2, (uint8_t)lh2);
    saveSourceLightMode(SRC_HA1, (uint8_t)lha1);
    saveSourceLightMode(SRC_HA2, (uint8_t)lha2);
    saveSourceLightMode(SRC_HA3, (uint8_t)lha3);
  }
  if (!doc["light_bri_mode_h1"].isNull() || !doc["light_bri_mode_h2"].isNull() || !doc["light_bri_mode_ha1"].isNull() ||
      !doc["light_bri_mode_ha2"].isNull() || !doc["light_bri_mode_ha3"].isNull() || !doc["light_bri_manual_h1"].isNull() ||
      !doc["light_bri_manual_h2"].isNull() || !doc["light_bri_manual_ha1"].isNull() || !doc["light_bri_manual_ha2"].isNull() ||
      !doc["light_bri_manual_ha3"].isNull() || !doc["light_bri_last_h1"].isNull() || !doc["light_bri_last_h2"].isNull() ||
      !doc["light_bri_last_ha1"].isNull() || !doc["light_bri_last_ha2"].isNull() || !doc["light_bri_last_ha3"].isNull()) {
    int m1 = doc["light_bri_mode_h1"].isNull() ? (int)sourceBrightnessMode(SRC_H1) : (int)(doc["light_bri_mode_h1"] | 0);
    int m2 = doc["light_bri_mode_h2"].isNull() ? (int)sourceBrightnessMode(SRC_H2) : (int)(doc["light_bri_mode_h2"] | 0);
    int m3 = doc["light_bri_mode_ha1"].isNull() ? (int)sourceBrightnessMode(SRC_HA1) : (int)(doc["light_bri_mode_ha1"] | 0);
    int m4 = doc["light_bri_mode_ha2"].isNull() ? (int)sourceBrightnessMode(SRC_HA2) : (int)(doc["light_bri_mode_ha2"] | 0);
    int m5 = doc["light_bri_mode_ha3"].isNull() ? (int)sourceBrightnessMode(SRC_HA3) : (int)(doc["light_bri_mode_ha3"] | 0);
    if (m1 < 0 || m1 > 1 || m2 < 0 || m2 > 1 || m3 < 0 || m3 > 1 || m4 < 0 || m4 > 1 || m5 < 0 || m5 > 1) {
      sendJsonError(400, "brightness mode out of range (0..1)");
      return;
    }
    int v1 = doc["light_bri_manual_h1"].isNull() ? (int)sourceManualBrightness(SRC_H1) : (int)(doc["light_bri_manual_h1"] | 0);
    int v2 = doc["light_bri_manual_h2"].isNull() ? (int)sourceManualBrightness(SRC_H2) : (int)(doc["light_bri_manual_h2"] | 0);
    int v3 = doc["light_bri_manual_ha1"].isNull() ? (int)sourceManualBrightness(SRC_HA1) : (int)(doc["light_bri_manual_ha1"] | 0);
    int v4 = doc["light_bri_manual_ha2"].isNull() ? (int)sourceManualBrightness(SRC_HA2) : (int)(doc["light_bri_manual_ha2"] | 0);
    int v5 = doc["light_bri_manual_ha3"].isNull() ? (int)sourceManualBrightness(SRC_HA3) : (int)(doc["light_bri_manual_ha3"] | 0);
    int l1 = doc["light_bri_last_h1"].isNull() ? (int)sourceLastBrightness(SRC_H1) : (int)(doc["light_bri_last_h1"] | 0);
    int l2 = doc["light_bri_last_h2"].isNull() ? (int)sourceLastBrightness(SRC_H2) : (int)(doc["light_bri_last_h2"] | 0);
    int l3 = doc["light_bri_last_ha1"].isNull() ? (int)sourceLastBrightness(SRC_HA1) : (int)(doc["light_bri_last_ha1"] | 0);
    int l4 = doc["light_bri_last_ha2"].isNull() ? (int)sourceLastBrightness(SRC_HA2) : (int)(doc["light_bri_last_ha2"] | 0);
    int l5 = doc["light_bri_last_ha3"].isNull() ? (int)sourceLastBrightness(SRC_HA3) : (int)(doc["light_bri_last_ha3"] | 0);
    if (v1 < 0 || v1 > 100 || v2 < 0 || v2 > 100 || v3 < 0 || v3 > 100 || v4 < 0 || v4 > 100 || v5 < 0 || v5 > 100 ||
        l1 < 0 || l1 > 100 || l2 < 0 || l2 > 100 || l3 < 0 || l3 > 100 || l4 < 0 || l4 > 100 || l5 < 0 || l5 > 100) {
      sendJsonError(400, "brightness value out of range (0..100)");
      return;
    }
    saveSourceBrightnessMode(SRC_H1, (uint8_t)m1);
    saveSourceBrightnessMode(SRC_H2, (uint8_t)m2);
    saveSourceBrightnessMode(SRC_HA1, (uint8_t)m3);
    saveSourceBrightnessMode(SRC_HA2, (uint8_t)m4);
    saveSourceBrightnessMode(SRC_HA3, (uint8_t)m5);
    saveSourceManualBrightness(SRC_H1, (uint8_t)v1);
    saveSourceManualBrightness(SRC_H2, (uint8_t)v2);
    saveSourceManualBrightness(SRC_HA1, (uint8_t)v3);
    saveSourceManualBrightness(SRC_HA2, (uint8_t)v4);
    saveSourceManualBrightness(SRC_HA3, (uint8_t)v5);
    saveSourceLastBrightness(SRC_H1, (uint8_t)l1);
    saveSourceLastBrightness(SRC_H2, (uint8_t)l2);
    saveSourceLastBrightness(SRC_HA1, (uint8_t)l3);
    saveSourceLastBrightness(SRC_HA2, (uint8_t)l4);
    saveSourceLastBrightness(SRC_HA3, (uint8_t)l5);
  }
  if (!doc["tz_offset_min"].isNull() || !doc["tz_name"].isNull()) {
    int16_t off = tzOffsetMin;
    String name = tzName;
    if (!doc["tz_offset_min"].isNull()) off = clampTzOffsetMin((int)(doc["tz_offset_min"] | 0));
    if (!doc["tz_name"].isNull()) name = normalizeTzName(String((const char *)doc["tz_name"]));
    saveTimeConfig(off, name);
  }
  if (!doc["auto_stop_enabled"].isNull() || !doc["auto_stop_minute"].isNull()) {
    bool en = doc["auto_stop_enabled"].isNull() ? autoStopEnabled : (bool)(doc["auto_stop_enabled"] | false);
    int mod = doc["auto_stop_minute"].isNull() ? (int)autoStopMinute : (int)(doc["auto_stop_minute"] | 0);
    if (mod < 0 || mod > 1439) {
      sendJsonError(400, "auto_stop_minute out of range (0..1439)");
      return;
    }
    saveAutoStopConfig(en, (uint16_t)mod);
  }
  sendJson(200, "{\"ok\":true}");
}

static void handleApiAuthRemember() {
  if (gReqAuthedByToken && !gReqTokenHash.isEmpty()) {
    sendJson(200, "{\"ok\":true,\"remembered\":true}");
    return;
  }

  String label = requestDeviceLabel(server.client().remoteIP());
  String token = randomTokenHex();
  if (!rememberStoreToken(token, label)) {
    sendJsonError(500, "failed to store remember token");
    return;
  }
  server.sendHeader("Set-Cookie", rememberCookieHeader(token, false));
  sendJson(200, "{\"ok\":true,\"remembered\":true}");
}

static void handleApiAuthLogout() {
  if (!gReqTokenHash.isEmpty()) rememberRevokeHash(gReqTokenHash);
  server.sendHeader("Set-Cookie", rememberCookieHeader("", true));
  sendJson(200, "{\"ok\":true}");
}

static void handleApiAuthRevokeAll() {
  rememberRevokeAll();
  server.sendHeader("Set-Cookie", rememberCookieHeader("", true));
  sendJson(200, "{\"ok\":true}");
}

static void handleApiReboot() {
  sendJson(200, "{\"ok\":true}");
  delay(250);
  ESP.restart();
}

static void handleApiFactoryReset() {
  // Drop all runtime sessions immediately so device becomes "clean" before reboot.
  bleResetHandles();
  bleSetConnectSource("factory_reset");
  gBleLastAddr = "";
  gBleLastName = "";
  gBleLastRssi = -999;
  gBleLastTx = "";
  bleBound = false;
  bleNamePrefix = "TES02_";
  bleLockedAddr = "";
  bleLockedName = "";
  bleApplyDriverConfig();
  tzConfigured = false;
  tzOffsetMin = 0;
  tzName = "UTC";
  clockValid = false;
  clockBaseEpochMs = 0;
  clockLastSyncSource = "factory_reset";
  autoStopEnabled = false;
  autoStopMinute = 0;
  autoStopLastYmd = 0;
  h312BleReleaseClient();
  h312Enabled = false;
  h312Mode = H312_MODE_WIFI;
  h312Ip = "";
  h312BleAddr = "";
  h312BleName = "";
  gH312BleRuntimeAddr = "";
  gH312BleRuntimeAddrType = -1;
  gH312BleNextPollMs = 0;
  gH312BleFailStreak = 0;
  h312Slot = 1;
  h312ApplyDriverConfig();
  gH312RealTimeTemp = -1000;
  gExtractorSessionActive = false;
  gExtractorSessionHadPreLightOn = false;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceLastSpeed[i] = 30;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceLightMode[i] = LIGHT_OFF;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceBrightnessMode[i] = BRI_LAST;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceManualBrightness[i] = 60;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceLastBrightness[i] = 60;
  for (uint8_t i = 0; i < (uint8_t)SRC_COUNT; i++) gSourceLastEnabledLightMode[i] = LIGHT_WHITE;
  setActiveSource(SRC_H1);

  prefs.begin(kPrefsNs, false);
  prefs.clear();
  prefs.end();
  sendJson(200, "{\"ok\":true}");
  delay(250);
  ESP.restart();
}

static bool verifyAppPartitionHeader(const esp_partition_t *p, String *outErr = nullptr) {
  if (!p) {
    if (outErr) *outErr = "partition not found";
    return false;
  }

  esp_image_header_t hdr;
  esp_err_t err = esp_partition_read(p, 0, &hdr, sizeof(hdr));
  if (err != ESP_OK) {
    if (outErr) *outErr = String("read failed: ") + String((int)err);
    return false;
  }
  if (hdr.magic != ESP_IMAGE_HEADER_MAGIC) {
    if (outErr) *outErr = "bad image magic";
    return false;
  }
  return true;
}

static bool isSystemWritableAppSlot(const esp_partition_t *p) {
  if (!p) return false;
  if (p->type != ESP_PARTITION_TYPE_APP) return false;
  const int st = (int)p->subtype;
  return st >= (int)ESP_PARTITION_SUBTYPE_APP_OTA_MIN &&
         st <= (int)ESP_PARTITION_SUBTYPE_APP_OTA_MAX;
}

struct OnlineFwResult {
  bool ok = false;
  String err;
  String targetSlot;
  uint32_t bytes = 0;
  uint32_t ms = 0;
};

struct ManualFwUploadState {
  bool active = false;
  bool done = false;
  bool ok = false;
  String err;
  String targetSlot;
  const esp_partition_t *target = nullptr;
  esp_ota_handle_t handle = 0;
  uint32_t written = 0;
};

static ManualFwUploadState gManualFwUpload;

static const esp_partition_t *pickOnlineTargetSlot(String *outSlotName = nullptr) {
  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *otaA = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
  const esp_partition_t *otaB = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);

  const esp_partition_t *target = nullptr;
  if (running == otaA && otaB) {
    target = otaB;
  } else if (running == otaB && otaA) {
    target = otaA;
  } else {
    target = otaA ? otaA : otaB;
  }

  if (outSlotName) {
    if (!target) *outSlotName = "none";
    else if (target == otaA) *outSlotName = "ota_a";
    else if (target == otaB) *outSlotName = "ota_b";
    else *outSlotName = String(target->label);
  }
  return target;
}

static OnlineFwResult performOnlineFwUpdate(const String &urlIn) {
  OnlineFwResult out;
  const uint32_t t0 = millis();
  if (!kFwOnlineUpdateEnabled) {
    out.err = "online update disabled";
    return out;
  }

  String url = urlIn;
  url.trim();
  if (url.isEmpty()) url = String(kFwOnlineDefaultUrl);
  if (url.isEmpty()) {
    out.err = "url missing";
    return out;
  }
  if (!url.startsWith("https://")) {
    out.err = "only https:// is allowed";
    return out;
  }
  String host = extractHostFromUrl(url);
  if (host.isEmpty()) {
    out.err = "invalid url host";
    return out;
  }
  if (strlen(kFwOnlineAllowedHost) > 0 && !host.equalsIgnoreCase(kFwOnlineAllowedHost)) {
    out.err = "host not allowed";
    return out;
  }
  if (strlen(kFwOnlineRootCaPem) == 0) {
    out.err = "root ca not configured";
    return out;
  }
  if (WiFi.status() != WL_CONNECTED) {
    out.err = "wifi not connected";
    return out;
  }

  String slotName;
  const esp_partition_t *target = pickOnlineTargetSlot(&slotName);
  out.targetSlot = slotName;
  if (!target) {
    out.err = "no target ota slot";
    return out;
  }

  HTTPClient http;
  WiFiClientSecure httpsClient;
  httpsClient.setCACert(kFwOnlineRootCaPem);
  bool beginOk = http.begin(httpsClient, url);
  if (!beginOk) {
    out.err = "http begin failed";
    return out;
  }

  http.setConnectTimeout((int)kFwOnlineConnectTimeoutMs);
  http.setTimeout((int)kFwOnlineReadTimeoutMs);
  const int code = http.GET();
  if (code != HTTP_CODE_OK) {
    out.err = String("http get failed: ") + String(code);
    http.end();
    return out;
  }

  const int contentLen = http.getSize();
  if (contentLen == 0) {
    out.err = "empty payload";
    http.end();
    return out;
  }

  esp_ota_handle_t otaHandle = 0;
  esp_err_t e = esp_ota_begin(target, (contentLen > 0) ? (size_t)contentLen : OTA_SIZE_UNKNOWN, &otaHandle);
  if (e != ESP_OK) {
    if (e == ESP_ERR_OTA_PARTITION_CONFLICT) {
      out.err = "ota partition conflict (single-slot layout active)";
    } else {
      out.err = String("esp_ota_begin failed: ") + String((int)e);
    }
    http.end();
    return out;
  }

  WiFiClient *stream = http.getStreamPtr();
  int remain = contentLen;
  uint8_t buf[1024];
  bool writeFailed = false;
  uint32_t lastDataMs = millis();

  while (http.connected() && (remain > 0 || remain < 0)) {
    size_t avail = stream ? stream->available() : 0;
    if (avail == 0) {
      if (remain == 0) break;
      if ((millis() - lastDataMs) > kFwOnlineReadTimeoutMs) {
        out.err = "download timeout";
        writeFailed = true;
        break;
      }
      delay(1);
      continue;
    }

    size_t toRead = avail;
    if (toRead > sizeof(buf)) toRead = sizeof(buf);
    int got = stream->readBytes(buf, toRead);
    if (got <= 0) {
      delay(1);
      continue;
    }
    lastDataMs = millis();
    e = esp_ota_write(otaHandle, buf, (size_t)got);
    if (e != ESP_OK) {
      out.err = String("esp_ota_write failed: ") + String((int)e);
      writeFailed = true;
      break;
    }
    out.bytes += (uint32_t)got;
    if (remain > 0) remain -= got;
  }

  if (!writeFailed && remain > 0) {
    out.err = "download incomplete";
    writeFailed = true;
  }

  if (writeFailed) {
    esp_ota_abort(otaHandle);
    http.end();
    return out;
  }

  e = esp_ota_end(otaHandle);
  if (e != ESP_OK) {
    out.err = String("esp_ota_end failed: ") + String((int)e);
    http.end();
    return out;
  }

  e = esp_ota_set_boot_partition(target);
  if (e != ESP_OK) {
    out.err = String("esp_ota_set_boot_partition failed: ") + String((int)e);
    http.end();
    return out;
  }

  http.end();
  out.ms = millis() - t0;
  out.ok = true;
  return out;
}

static void resetManualFwUploadState() {
  if (gManualFwUpload.active && gManualFwUpload.handle) {
    esp_ota_abort(gManualFwUpload.handle);
  }
  gManualFwUpload = ManualFwUploadState();
}

static void handleApiFwUpdateManualUpload() {
  HTTPUpload &up = server.upload();

  if (up.status == UPLOAD_FILE_START) {
    extractorCmdLogPushf("[security] ota manual start ip=%s file=%s",
                         server.client().remoteIP().toString().c_str(),
                         up.filename.c_str());
    resetManualFwUploadState();
    gManualFwUpload.active = true;
    gManualFwUpload.done = false;
    gManualFwUpload.ok = false;
    gManualFwUpload.err = "";

    String slotName;
    const esp_partition_t *target = pickOnlineTargetSlot(&slotName);
    gManualFwUpload.target = target;
    gManualFwUpload.targetSlot = slotName;
    if (!target) {
      gManualFwUpload.err = "no target system slot";
      extractorCmdLogPushf("[security] ota manual rejected: no target system slot");
      return;
    }
    if (!isSystemWritableAppSlot(target)) {
      gManualFwUpload.err = "target is not system slot";
      extractorCmdLogPushf("[security] ota manual rejected: target not system slot");
      return;
    }

    esp_err_t e = esp_ota_begin(target, OTA_SIZE_UNKNOWN, &gManualFwUpload.handle);
    if (e != ESP_OK) {
      if (e == ESP_ERR_OTA_PARTITION_CONFLICT) {
        gManualFwUpload.err = "ota partition conflict (single-slot layout active)";
      } else {
        gManualFwUpload.err = String("esp_ota_begin failed: ") + String((int)e);
      }
      extractorCmdLogPushf("[security] ota manual begin fail: %s", gManualFwUpload.err.c_str());
      return;
    }
    gManualFwUpload.ok = true;
    return;
  }

  if (up.status == UPLOAD_FILE_WRITE) {
    if (!gManualFwUpload.active || !gManualFwUpload.ok || !gManualFwUpload.handle) return;
    if (up.currentSize == 0) return;
    esp_err_t e = esp_ota_write(gManualFwUpload.handle, up.buf, up.currentSize);
    if (e != ESP_OK) {
      gManualFwUpload.ok = false;
      gManualFwUpload.err = String("esp_ota_write failed: ") + String((int)e);
      return;
    }
    gManualFwUpload.written += up.currentSize;
    return;
  }

  if (up.status == UPLOAD_FILE_END) {
    if (!gManualFwUpload.active) return;
    if (!gManualFwUpload.ok) {
      gManualFwUpload.done = true;
      return;
    }
    if (!gManualFwUpload.handle) {
      gManualFwUpload.ok = false;
      gManualFwUpload.err = "no ota handle";
      gManualFwUpload.done = true;
      return;
    }
    esp_err_t e = esp_ota_end(gManualFwUpload.handle);
    if (e != ESP_OK) {
      gManualFwUpload.ok = false;
      gManualFwUpload.err = String("esp_ota_end failed: ") + String((int)e);
      extractorCmdLogPushf("[security] ota manual end fail: %s", gManualFwUpload.err.c_str());
      gManualFwUpload.done = true;
      return;
    }
    gManualFwUpload.handle = 0;
    e = esp_ota_set_boot_partition(gManualFwUpload.target);
    if (e != ESP_OK) {
      gManualFwUpload.ok = false;
      gManualFwUpload.err = String("esp_ota_set_boot_partition failed: ") + String((int)e);
      extractorCmdLogPushf("[security] ota manual boot-set fail: %s", gManualFwUpload.err.c_str());
      gManualFwUpload.done = true;
      return;
    }
    gManualFwUpload.ok = true;
    gManualFwUpload.done = true;
    extractorCmdLogPushf("[security] ota manual staged slot=%s bytes=%lu",
                         gManualFwUpload.targetSlot.c_str(),
                         (unsigned long)gManualFwUpload.written);
    return;
  }

  if (up.status == UPLOAD_FILE_ABORTED) {
    if (gManualFwUpload.handle) esp_ota_abort(gManualFwUpload.handle);
    gManualFwUpload.handle = 0;
    gManualFwUpload.ok = false;
    gManualFwUpload.err = "upload aborted";
    gManualFwUpload.done = true;
    extractorCmdLogPushf("[security] ota manual aborted");
    return;
  }
}

static void handleApiFwUpdateManual() {
  if (!gManualFwUpload.done) {
    sendJsonError(400, "no upload");
    return;
  }
  if (!gManualFwUpload.ok) {
    String err = gManualFwUpload.err.isEmpty() ? "manual update failed" : gManualFwUpload.err;
    extractorCmdLogPushf("[security] ota manual failed: %s", err.c_str());
    resetManualFwUploadState();
    sendJsonError(500, err);
    return;
  }

  String body;
  body.reserve(180);
  body += F("{\"ok\":true,\"slot\":\"");
  body += jsonEscape(gManualFwUpload.targetSlot);
  body += F("\",\"bytes\":");
  body += String((unsigned long)gManualFwUpload.written);
  body += F("}");
  extractorCmdLogPushf("[security] ota manual commit slot=%s bytes=%lu reboot=1",
                       gManualFwUpload.targetSlot.c_str(),
                       (unsigned long)gManualFwUpload.written);
  sendJson(200, body);
  delay(250);
  ESP.restart();
}

static void handleApiFwUpdateOnline() {
  extractorCmdLogPushf("[security] ota online request ip=%s", server.client().remoteIP().toString().c_str());
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }
  String url = doc["url"] | "";
  url.trim();
  OnlineFwResult r = performOnlineFwUpdate(url);
  if (!r.ok) {
    extractorCmdLogPushf("[security] ota online failed: %s", r.err.c_str());
    sendJsonError(400, r.err);
    return;
  }

  String body;
  body.reserve(180);
  body += F("{\"ok\":true,\"slot\":\"");
  body += jsonEscape(r.targetSlot);
  body += F("\",\"bytes\":");
  body += String((unsigned long)r.bytes);
  body += F(",\"ms\":");
  body += String((unsigned long)r.ms);
  body += F("}");
  extractorCmdLogPushf("[security] ota online commit slot=%s bytes=%lu reboot=1",
                       r.targetSlot.c_str(),
                       (unsigned long)r.bytes);
  sendJson(200, body);
  delay(250);
  ESP.restart();
}

static void handleApiFwLayout() {
  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *boot = esp_ota_get_boot_partition();
  const esp_partition_t *otaA = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
  const esp_partition_t *otaB = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);
  const esp_partition_t *factory = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);

  // Keep this endpoint lightweight by default; full partition dump only on ?verbose=1.
  bool verbose = (server.hasArg("verbose") && server.arg("verbose") == "1");

  String body;
  body.reserve(verbose ? 2200 : 1400);
  body += F("{\"ok\":true");
  body += F(",\"fw\":\"");
  body += kFwVersion;
  body += F("\",\"fw_profile\":\"");
  body += kFwProfile;
  body += F("\",\"fw_hardened\":");
  body += (kFwHardenedBuild ? "true" : "false");
  body += F(",\"safe_mode\":");
  body += (gSafeMode ? "true" : "false");
  body += F(",\"safe_cause\":\"");
  body += jsonEscape(gSafeCause);
  body += F("\"");

  body += F(",\"bootloader\":{\"offset\":\"0x0000\",\"size\":\"0x8000\"}");
  body += F(",\"partition_table\":{\"offset\":\"0x8000\",\"size\":\"0x1000\"}");
  body += F(",\"bootloader_writable_over_wifi\":false");
  body += F(",\"recovery_writable_over_wifi\":false");

  auto partToJson = [&](const esp_partition_t *p) -> String {
    if (!p) return "null";
    String s;
    s.reserve(180);
    s += F("{\"label\":\"");
    s += jsonEscape(safePartitionLabel(p));
    s += F("\",\"type\":");
    s += String((int)p->type);
    s += F(",\"subtype\":");
    s += String((int)p->subtype);
    s += F(",\"addr\":\"0x");
    s += String((uint32_t)p->address, HEX);
    s += F("\",\"size\":\"0x");
    s += String((uint32_t)p->size, HEX);
    s += F("\"}");
    return s;
  };

  body += F(",\"running\":");
  body += partToJson(running);
  body += F(",\"boot\":");
  body += partToJson(boot);

  body += F(",\"slots\":{");
  body += F("\"ota_a\":");
  body += partToJson(otaA);
  body += F(",\"ota_b\":");
  body += partToJson(otaB);
  body += F(",\"recovery\":");
  body += partToJson(factory);
  body += F("}");

  bool first = true;
  if (verbose) {
    // Enumerate full partition table only when explicitly requested.
    body += F(",\"partitions\":[");
    esp_partition_iterator_t it = esp_partition_find(ESP_PARTITION_TYPE_ANY, ESP_PARTITION_SUBTYPE_ANY, NULL);
    esp_partition_iterator_t it0 = it;
    while (it) {
      const esp_partition_t *p = esp_partition_get(it);
      if (p) {
        if (!first) body += ",";
        first = false;
        body += partToJson(p);
      }
      it = esp_partition_next(it);
    }
    esp_partition_iterator_release(it0);
    body += F("]");
  }

  // Human-readable status of key "sections" (some are logical, not partitions).
  body += F(",\"layout\":[");
  auto addRow = [&](const String &name, const String &status, const String &detail) {
    if (!first) body += ",";
    first = false;
    body += F("{\"name\":\"");
    body += jsonEscape(name);
    body += F("\",\"status\":\"");
    body += jsonEscape(status);
    body += F("\",\"detail\":\"");
    body += jsonEscape(detail);
    body += F("\"}");
  };

  first = true;
  addRow("Bootloader", "LOCKED", "0x0000 (not writable over Wi-Fi)");
  if (otaA) addRow("OTA_A", (running == otaA ? "ACTIVE" : "STANDBY"), safePartitionLabel(otaA));
  else addRow("OTA_A", "MISSING", "");
  if (otaB) addRow("OTA_B", (running == otaB ? "ACTIVE" : "STANDBY"), safePartitionLabel(otaB));
  else addRow("OTA_B", "MISSING", "");
  addRow("NVS", "OK", "Preferences");
  addRow("Config", "OK", String("wifi_autooff=") + String((uint32_t)wifiAutoOffMin));
  addRow("Logs", "OK", "LittleFS:/");
  if (factory) addRow("Recovery Image", "LOCKED", safePartitionLabel(factory) + " (not writable over Wi-Fi)");
  else addRow("Recovery Image", "N/A", "no factory partition");

  body += F("]");
  body += F("}");

  sendJson(200, body);
}

static void handleApiFwVerify() {
  const esp_partition_t *otaA = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
  const esp_partition_t *otaB = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);
  const esp_partition_t *factory = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);

  auto verifyOne = [&](const String &name, const esp_partition_t *p) -> String {
    String err;
    bool ok = verifyAppPartitionHeader(p, &err);
    String s;
    s.reserve(160);
    s += F("{\"name\":\"");
    s += jsonEscape(name);
    s += F("\",\"ok\":");
    s += (ok ? "true" : "false");
    s += F(",\"detail\":\"");
    if (ok) s += jsonEscape(p ? String(p->label) : "");
    else s += jsonEscape(err);
    s += F("\"}");
    return s;
  };

  String body;
  body.reserve(520);
  body += F("{\"ok\":true,\"checks\":[");
  body += verifyOne("OTA_A", otaA);
  body += F(",");
  body += verifyOne("OTA_B", otaB);
  body += F(",");
  body += verifyOne("Recovery", factory);
  body += F("]}");

  sendJson(200, body);
}

static void handleApiFwSwitchSlot() {
  JsonDocument doc;
  if (!parseJsonBody(doc)) {
    sendJsonError(400, "bad json");
    return;
  }

  String to = doc["to"] | "";
  to.toLowerCase();
  if (to == "recovery" || to == "factory") {
    sendJsonError(400, "recovery slot is not switchable via this endpoint");
    return;
  }

  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *otaA = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
  const esp_partition_t *otaB = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);

  const esp_partition_t *target = nullptr;
  if (to == "ota_a" || to == "a") {
    if (!otaA) {
      sendJsonError(400, "ota_a slot missing");
      return;
    }
    target = otaA;
  } else if (to == "ota_b" || to == "b") {
    if (!otaB) {
      sendJsonError(400, "ota_b slot missing (single-slot layout)");
      return;
    }
    target = otaB;
  } else {
    // Default: switch to the other slot.
    if (running == otaA) target = otaB;
    else if (running == otaB) target = otaA;
    else target = otaA ? otaA : otaB;

    if (!target) {
      sendJsonError(400, "no OTA slots found");
      return;
    }
    if (target == running && (!otaA || !otaB)) {
      sendJsonError(400, "alternate ota slot missing (single-slot layout)");
      return;
    }
  }

  String err;
  if (!verifyAppPartitionHeader(target, &err)) {
    sendJsonError(400, String("target invalid: ") + err);
    return;
  }
  if (!isSystemWritableAppSlot(target)) {
    sendJsonError(400, "target is not a system slot");
    return;
  }

  esp_err_t e = esp_ota_set_boot_partition(target);
  if (e != ESP_OK) {
    sendJsonError(500, String("esp_ota_set_boot_partition failed: ") + String((int)e));
    return;
  }

  // If we were in recovery/safe mode by request, switching slots is an exit path.
  prefs.begin(kPrefsNs, false);
  prefs.putBool("force_recovery", false);
  prefs.end();

  sendJson(200, "{\"ok\":true}");
  delay(250);
  ESP.restart();
}

static void handleApiFwEnterRecovery() {
  const esp_partition_t *factory = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);

  // Always remember the intent; if no factory partition exists, we'll just boot in SAFE MODE (same image).
  prefs.begin(kPrefsNs, false);
  prefs.putBool("force_recovery", true);
  prefs.end();

  if (factory) {
    String err;
    if (!verifyAppPartitionHeader(factory, &err)) {
      sendJsonError(400, String("recovery image invalid: ") + err);
      return;
    }
    esp_err_t e = esp_ota_set_boot_partition(factory);
    if (e != ESP_OK) {
      sendJsonError(500, String("esp_ota_set_boot_partition failed: ") + String((int)e));
      return;
    }
  }

  sendJson(200, "{\"ok\":true}");
  delay(250);
  ESP.restart();
}

static void setupWeb() {
  const char *kHeaderKeys[] = {"Cookie", "User-Agent", "X-LB-Confirm-Pass", "X-LB-Confirm-Pass-Enc"};
  server.collectHeaders(kHeaderKeys, sizeof(kHeaderKeys) / sizeof(kHeaderKeys[0]));
  auto denyRecoveryUi = []() {
    server.sendHeader("Cache-Control", "no-store");
    server.send(403, "text/plain", "recovery ui is available only after switching to Recovery mode");
  };

  server.on("/", HTTP_GET, []() {
    if (!webAuthGuard()) return;
    if (tryServeFile("/index.html")) return;
    sendRecoveryMinPage("index missing");
  });
  server.on("/recovery-min", HTTP_GET, [denyRecoveryUi]() {
    if (!webAuthGuard()) return;
    if (!canServeRecoveryUi()) {
      denyRecoveryUi();
      return;
    }
    sendRecoveryMinPage();
  });
  server.on("/recovery-min/", HTTP_GET, [denyRecoveryUi]() {
    if (!webAuthGuard()) return;
    if (!canServeRecoveryUi()) {
      denyRecoveryUi();
      return;
    }
    sendRecoveryMinPage();
  });
  server.on("/recovery", HTTP_GET, [denyRecoveryUi]() {
    if (!webAuthGuard()) return;
    if (!canServeRecoveryUi()) {
      denyRecoveryUi();
      return;
    }
    if (tryServeFile("/recovery.html")) return;
    sendRecoveryMinPage("recovery page not found");
  });
  server.on("/recovery/", HTTP_GET, [denyRecoveryUi]() {
    if (!webAuthGuard()) return;
    if (!canServeRecoveryUi()) {
      denyRecoveryUi();
      return;
    }
    if (tryServeFile("/recovery.html")) return;
    sendRecoveryMinPage("recovery page not found");
  });
  server.on("/wifi", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    handleWifiPost();
  });

  server.on("/api/status", HTTP_GET, []() {
    if (!webAuthGuard()) return;
    handleApiStatus();
  });

  // New API used by the dashboard UI.
  server.on("/api/config", HTTP_GET, []() {
    if (!webAuthGuard()) return;
    handleApiConfigGet();
  });
  server.on("/api/config", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    handleApiConfigSet();
  });
  server.on("/api/auth/remember", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    handleApiAuthRemember();
  });
  server.on("/api/auth/logout", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    handleApiAuthLogout();
  });
  server.on("/api/auth/revoke_all", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    if (!requireCriticalPassword(nullptr, "auth_revoke_all")) return;
    handleApiAuthRevokeAll();
  });

  // Firmware service mode.
  server.on("/api/fw/layout", HTTP_GET, []() { if (!webAuthGuard()) return; handleApiFwLayout(); });
  server.on("/api/fw/verify", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiFwVerify(); });
  server.on("/api/fw/switch_slot", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    if (!requireCriticalPassword(nullptr, "fw_switch_slot")) return;
    handleApiFwSwitchSlot();
  });
  server.on("/api/fw/update_manual", HTTP_POST,
            []() {
              if (!webAuthGuard()) return;
              if (!requireCriticalPassword(nullptr, "fw_update_manual")) return;
              handleApiFwUpdateManual();
            },
            []() {
              if (!webAuthGuard()) return;
              if (!requireCriticalPassword(nullptr, "fw_update_manual_upload")) return;
              handleApiFwUpdateManualUpload();
            });
  server.on("/api/fw/enter_recovery", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    if (!requireCriticalPassword(nullptr, "fw_enter_recovery")) return;
    handleApiFwEnterRecovery();
  });

  server.on("/api/wifi/scan", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiWifiScan(); });
  server.on("/api/wifi/connect", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiWifiConnect(); });
  server.on("/api/time/status", HTTP_GET, []() { if (!webAuthGuard()) return; handleApiTimeStatus(); });
  server.on("/api/time/set", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiTimeSet(); });
  server.on("/api/time/sync", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiTimeSync(); });
  server.on("/api/h312/ble_scan", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiH312BleScan(); });
  server.on("/api/h312/scan", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiH312Scan(); });
  server.on("/api/h312/probe", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiH312Probe(); });

  server.on("/api/extractor/scan", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiExtractorScan(); });
  server.on("/api/extractor/select", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiExtractorSelect(); });
  server.on("/api/extractor/status", HTTP_GET, []() { if (!webAuthGuard()) return; handleApiExtractorStatus(); });
  server.on("/api/extractor/cmdlog", HTTP_GET, []() { if (!webAuthGuard()) return; handleApiExtractorCmdLog(); });
  server.on("/api/extractor/power", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiExtractorPower(); });
  server.on("/api/extractor/speed", HTTP_POST, []() { if (!webAuthGuard()) return; handleApiExtractorSpeed(); });

  server.on("/api/reboot", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    if (!requireCriticalPassword(nullptr, "reboot")) return;
    handleApiReboot();
  });
  server.on("/api/factory_reset", HTTP_POST, []() {
    if (!webAuthGuard()) return;
    if (!requireCriticalPassword(nullptr, "factory_reset")) return;
    handleApiFactoryReset();
  });

  server.onNotFound([denyRecoveryUi]() {
    if (!webAuthGuard()) return;
    if (isRecoveryUiAssetPath(server.uri()) && !canServeRecoveryUi()) {
      denyRecoveryUi();
      return;
    }
    if (tryServeFile(server.uri())) return;
    sendJson(404, "{\"ok\":false,\"error\":\"not found\"}");
  });
  server.begin();
}

void setup() {
  LB_SERIAL_BEGIN(115200);
  delay(200);
  initDeviceIdentity();

  bootHealthCheckInit();
  loadConfig();
  if (bootRecoveryByDualHandleGpio()) {
    // Dual-handle trigger should unlock recovery access window without forcing safe mode.
    // Forcing gSafeMode here blocks BLE scanner/H312 runtime in normal firmware.
    gRecoveryUnlockByHandles = true;
    gRecoveryUnlockUntilMs = (uint32_t)millis() + kRecoveryUnlockWindowMs;
    gWebAuthFailIp = IPAddress(0, 0, 0, 0);
    gWebAuthFailCount = 0;
    gWebAuthBlockUntilMs = 0;
    // Physical dual-handle unlock opens AP for recovery-only endpoints.
    gApPass = "";
    extractorCmdLogPushf("[recovery] unlock window %lus over AP", (unsigned long)(kRecoveryUnlockWindowMs / 1000UL));
  }
  gH312Driver.begin();
  gH312Driver.setLogger(h312DriverLogCb, nullptr);
  h312ApplyDriverConfig();

  if (!LittleFS.begin(true)) {
    LB_SERIAL_PRINTLN("LittleFS: mount failed");
  }

  NimBLEDevice::init("");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);
  gTes02Driver.begin();
  gTes02Driver.setLogger(bleDriverLogCb, nullptr);
  gTes02Driver.setEnsureConnected(bleEnsureConnectedCallback, nullptr);
  bleApplyDriverConfig();
  bleSetConnectSource("boot");
  bleSyncRuntimeFromDriver();

  // Early-boot failsafe: AP is always available while STA/services are starting.
  wifiEnsureFailsafeAp("early_boot", true);
  setupWeb();

  bool ok = false;
  if (!wifiSsid.isEmpty()) {
    ok = wifiConnectSta(gSafeMode ? 12000 : 15000, true);
  }
  if (!ok) {
    gFailsafeApPinned = true;
    gFailsafeApPinUntilMs = 0;
    LB_SERIAL_PRINTLN("WiFi AP fallback active");
    LB_SERIAL_PRINT("AP SSID: ");
    LB_SERIAL_PRINTLN(gApSsid);
    LB_SERIAL_PRINT("AP IP: ");
    LB_SERIAL_PRINTLN(WiFi.softAPIP());
    if (gSafeMode) {
      LB_SERIAL_PRINTLN("Recovery mode: AP active, STA pending/offline");
    }
  } else {
    if (isRecoveryUnlockActive((uint32_t)millis())) {
      gFailsafeApPinned = true;
      gFailsafeApPinUntilMs = gRecoveryUnlockUntilMs;
      extractorCmdLogPushf("[wifi] recovery unlock AP pin %lus", (unsigned long)(kRecoveryUnlockWindowMs / 1000UL));
    } else if (!gSafeMode) {
      // Legacy behavior: when STA is up in normal mode, drop AP immediately.
      gFailsafeApPinned = false;
      gFailsafeApPinUntilMs = 0;
      wifiEnforceStaOnlyIfConnected();
      extractorCmdLogPushf("[wifi] sta connected: AP disabled (sta-only)");
    } else {
      gFailsafeApPinned = true;
      gFailsafeApPinUntilMs = 0;
      LB_SERIAL_PRINTLN("Recovery mode: AP+STA active");
    }
    LB_SERIAL_PRINTLN("WiFi STA connected");
    LB_SERIAL_PRINT("IP: ");
    LB_SERIAL_PRINTLN(WiFi.localIP());
    LB_SERIAL_PRINT("URL: http://");
    LB_SERIAL_PRINT(gStaHostname);
    LB_SERIAL_PRINTLN(".local");
  }

  if (WiFi.status() == WL_CONNECTED && tzConfigured) {
    if (clockTryNtpSync(true)) {
      LB_SERIAL_PRINTF("[clock] ntp sync ok local=%s tz=%s(%d)\n", clockLocalText().c_str(), tzName.c_str(), (int)tzOffsetMin);
    } else {
      LB_SERIAL_PRINTF("[clock] ntp sync deferred/failed tz=%s(%d)\n", tzName.c_str(), (int)tzOffsetMin);
    }
  }

  // One-time BLE wake pulse at boot, then release channel.
  bleBootPulse();
  uartDiagSnapshot("boot");
  bootHealthMarkOk();
}

void loop() {
  uint32_t loopStartUs = (uint32_t)micros();
  uint32_t nowMs = (uint32_t)millis();

  uint32_t secUs = (uint32_t)micros();
  (void)clockTryNtpSync(false);
  autoStopLoop();
  wifiFailsafeLoop(nowMs);
  if (gApDisableAtMs != 0) {
    int32_t left = (int32_t)(gApDisableAtMs - nowMs);
    if (left <= 0) {
      gApDisableAtMs = 0;
      wifiEnforceStaOnlyIfConnected();
    }
  } else {
    wifiEnforceStaOnlyIfConnected();
  }
  wifiTxAutoTuneLoop((uint32_t)millis());
  handleGpioDiagLoop(nowMs);
  gPerfAccCoreUs += (uint32_t)micros() - secUs;

  secUs = (uint32_t)micros();
  h312Loop();
  gPerfAccH312Us += (uint32_t)micros() - secUs;

  // Handle-triggered extractor automation (T420D GPIO1/GPIO2).
  t420dApplyLogic();

  secUs = (uint32_t)micros();
  extractorSessionPollLoop();
  gPerfAccSessUs += (uint32_t)micros() - secUs;

  secUs = (uint32_t)micros();
  if (bleReady() && kBleIdleDisconnectMs > 0) {
    uint32_t now = (uint32_t)millis();
    bool holdBle = gExtractorSessionActive || h312WantsOnNow() || t420dWantsOnNow();
    if (holdBle) {
      // Keep BLE session alive while extractor logic/session is active.
      gBleLastActivityMs = now;
    }
    uint32_t idleMs = (now >= gBleLastActivityMs) ? (now - gBleLastActivityMs) : 0;
    if (!holdBle && idleMs >= kBleIdleDisconnectMs) {
      bleResetHandles();
      bleSetConnectSource("idle_release");
    }
  }
  gPerfAccBleUs += (uint32_t)micros() - secUs;

  secUs = (uint32_t)micros();
  uartDiagTick();
  gPerfAccDiagUs += (uint32_t)micros() - secUs;

  secUs = (uint32_t)micros();
  server.handleClient();
  gPerfAccWebUs += (uint32_t)micros() - secUs;

  uint32_t loopBusyUs = (uint32_t)micros() - loopStartUs;
  gPerfAccBusyUs += loopBusyUs;
  gPerfAccLoops++;
  if (loopBusyUs > gPerfAccLoopMaxUs) gPerfAccLoopMaxUs = loopBusyUs;
  nowMs = (uint32_t)millis();
  perfMaybeSnap(nowMs);
  uiPerfMaybeLog(nowMs);
  delay(2);
}
