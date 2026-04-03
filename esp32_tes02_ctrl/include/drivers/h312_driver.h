#pragma once

#include <Arduino.h>
#include <WiFi.h>

namespace h312 {

enum class H312Error : uint8_t {
  OK = 0,
  TIMEOUT,
  BAD_RESPONSE,
  NO_ROUTE,
  SLOT_DISABLED,
  NO_IP
};

struct H312Health {
  bool connected = false;
  uint32_t lastOkMs = 0;
  H312Error error = H312Error::SLOT_DISABLED;
};

struct H312SlotConfig {
  bool enabled = false;
  IPAddress ip = IPAddress();
  uint16_t port = 6666;
  uint32_t pollMs = 5000;
  float thresholdC = 0.0f; // kept for compatibility; driver does not apply trigger logic
  uint32_t holdMs = 0;     // kept for compatibility; driver does not apply trigger logic
};

struct H312Config {
  H312SlotConfig slots[3];
  uint16_t reqTimeoutMs = 900;
};

enum class H312LogEventType : uint8_t {
  H312_SLOT_OK = 0,
  H312_SLOT_TIMEOUT,
  H312_SLOT_BADRESP,
  H312_ACTIVE_ON,
  H312_ACTIVE_OFF
};

using H312LogFn = void (*)(void *ctx, H312LogEventType ev, uint8_t slot, float tempC);

struct H312ProbeOptions {
  uint16_t connectTimeoutMs = 2500;
  uint16_t readMs = 2600;
  uint16_t startupDelayMs = 20;
  uint16_t keepaliveAfterMs = 700;
  bool requireRealTimeTemp = true;
};

struct H312ProbeResult {
  String name;
  String model;
  String sn;
  int ws = -1;
  int rt = -1000;
  int hf = -1;
};

bool probeHost(const IPAddress &target, uint16_t port, const H312ProbeOptions &opt, H312ProbeResult *out,
               String *outReason = nullptr);
bool socketLooksAlive(const IPAddress &target, uint16_t port, uint16_t connectTimeoutMs, uint16_t readMs,
                      size_t *outRxBytes = nullptr);
bool verifyTarget(const String &ipText, uint16_t port, uint8_t attempts, const H312ProbeOptions &opt,
                  H312ProbeResult *out, String *outReason = nullptr);

class H312Driver {
public:
  bool begin();
  bool applyConfig(const H312Config &cfg, H312Error *err = nullptr);
  void tick(uint32_t nowMs);

  bool isActive(uint8_t slot) const;
  float lastTempC(uint8_t slot) const;
  int lastWorkStatus(uint8_t slot) const;
  int lastHeatFlag(uint8_t slot) const;
  H312Health health(uint8_t slot) const;

  bool anyActive() const;
  uint8_t activeCount() const;

  void setLogger(H312LogFn fn, void *ctx);

private:
  struct SlotState {
    float lastTempC = NAN;
    int lastWorkStatus = -1;
    int lastHeatFlag = -1;
    uint32_t lastOkMs = 0;
    uint32_t nextDueMs = 0;
    H312Error error = H312Error::SLOT_DISABLED;
    bool active = false;
    bool everOk = false;
    uint8_t failStreak = 0;
  };

  static bool isJsonAsciiByte(uint8_t b);
  static bool extractJsonFromBuffer(String &src, String &out);
  static bool parseTelemetry(const String &json, float *outTempC, int *outWorkStatus, int *outHeatFlag);
  static bool ipValid(const IPAddress &ip);
  static uint32_t staleMsForPoll(uint32_t pollMs);
  static uint32_t failBackoffMs(uint32_t pollMs, uint8_t failStreak);

  void closeInFlight(H312Error err, bool scheduleNext, bool keepSocket = false);
  void markStateFromHealth(uint8_t idx, uint32_t nowMs);
  void emit(H312LogEventType ev, uint8_t slot, float tempC);
  int8_t pickNextDueSlot(uint32_t nowMs);
  bool startRequestForSlot(uint8_t idx, uint32_t nowMs);
  void handleInFlight(uint32_t nowMs);

  H312Config cfg_;
  SlotState st_[3];

  WiFiClient client_;
  String rx_;
  bool inFlight_ = false;
  bool inFlightGotAnyJson_ = false;
  uint8_t inFlightSlot_ = 0; // 0..2
  uint32_t inFlightStartMs_ = 0;
  uint32_t inFlightLastKickMs_ = 0;
  uint8_t inFlightWakeTries_ = 0;
  uint8_t activeSlotCursor_ = 0; // 0..2
  int8_t connectedSlot_ = -1;

  H312LogFn logFn_ = nullptr;
  void *logCtx_ = nullptr;
};

} // namespace h312
