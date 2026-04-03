#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

namespace tes02 {

using LogFn = void (*)(void *ctx, const char *line);
using EnsureConnectedFn = bool (*)(void *ctx, uint32_t scanMs);

struct Config {
  String prefix;
  String lockedAddr;
  String lockedName;
  bool bound = false;
};

struct RuntimeState {
  String lastAddr;
  String lastName;
  int lastRssi = -999;
  String connectSource = "none";
  String lastTx;
  int lastSetBefore = -1;
  int lastSetAfter = -1;
  int lastSetTarget = -1;
  uint32_t lastSetMs = 0;
  uint32_t connectedAtMs = 0;
  uint32_t lastActivityMs = 0;
};

class Tes02Driver {
public:
  void begin();
  void setLogger(LogFn fn, void *ctx);
  void setEnsureConnected(EnsureConnectedFn fn, void *ctx);
  void setConfig(const Config &cfg);
  const Config &config() const;
  void setSafeMode(bool v);
  void setConnectSource(const String &src);
  const RuntimeState &state() const;

  void resetHandles();
  bool ready() const;
  bool findBestByPrefix(const String &prefix, uint32_t scanMs, NimBLEAddress *outAddr, String *outName, int *outRssi);
  bool connectTo(const NimBLEAddress &addr);

  int readSpeedDirect();
  int readSpeedStable(uint8_t reads = 2, uint16_t gapMs = 120);
  int readLightSelDirect();
  int readLightSelStable(uint8_t reads = 2, uint16_t gapMs = 120);
  int readLightBrightnessDirect();
  int readLightBrightnessStable(uint8_t reads = 2, uint16_t gapMs = 120);
  bool readLightState(uint8_t *outSel, uint8_t *outBri);
  bool readRawFrame(std::string *out);

  bool primeControlChannel();
  void primeFreshSession(uint16_t minAgeMs = 1400);

  bool setSpeed(uint8_t spd, const char *logicSrc);
  bool setLightSelector(uint8_t targetSel, const char *modeTxt, const char *appliedModeTxt, const char *logicSrc);
  bool setBrightness(uint8_t target, const char *briModeTxt, const char *logicSrc);

private:
  static const char *kSvcUuid;
  static const char *kReadUuid;
  static const char *kWriteUuid;

  NimBLEClient *client_ = nullptr;
  NimBLERemoteCharacteristic *charRead_ = nullptr;
  NimBLERemoteCharacteristic *charWrite_ = nullptr;

  Config cfg_;
  RuntimeState st_;
  bool safeMode_ = false;

  LogFn logFn_ = nullptr;
  void *logCtx_ = nullptr;
  EnsureConnectedFn ensureFn_ = nullptr;
  void *ensureCtx_ = nullptr;

  void logf(const char *fmt, ...);
  static void hexEncode(const uint8_t *data, size_t len, char *out, size_t outSize);
  bool writeBestEffort(const uint8_t *data, size_t len);
};

} // namespace tes02
