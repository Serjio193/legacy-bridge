#include <Arduino.h>
#if defined(ARDUINO_USB_MODE) && defined(ARDUINO_USB_CDC_ON_BOOT)
#if ARDUINO_USB_MODE && ARDUINO_USB_CDC_ON_BOOT
#include <HWCDC.h>
#endif
#endif
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <memory>

extern "C" {
#include "esp_app_format.h"
#include "esp_mac.h"
#include "esp_ota_ops.h"
#include "esp_partition.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
}

static const char *kPrefsNs = "tes02ctrl";
static const char *kRecoverySsid = "LB_RECOVERY";
#ifndef LB_RECOVERY_VERSION
#define LB_RECOVERY_VERSION "v0.2.1-r1"
#endif
static const char *kRecoveryVersion = LB_RECOVERY_VERSION;
static const char *kWifiSsidKey = "wifi_ssid";
static const char *kWifiPassKey = "wifi_pass";
static const uint32_t kRecoveryStaConnectTimeoutMs = 8000;
static const bool kRecoveryRequireSignature = true;
static const char *kRecoverySignerPubKeyPem =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAzLPQ1NydJzwPo3uETTNT\n"
    "K8R5VI0tkc0MyPYQk6dle8ssxvX0l5K8Mig3Wj5h7rs5d8xHorgWDW4+YkSacAwi\n"
    "3GedvZJtx1rUmxk0Eh5OsfmVYqV5B3faV4yf4qwBSlc0HXwvl6Db8pOz2SrYB3r1\n"
    "FyVYC2I9s9sKlnvjoIqIUu6aUPCevZJ8LuSlkqtVEnjES0f/2lJjMseo/hmVgcOi\n"
    "VejCarc4sFAIc8pyO7+RBThsgiZ3Toiga+s3fTKWhw9knrOmHJJ2H3uWJiTxjZUq\n"
    "bNHA+nIVbmiMrV6OtsTy24bzxClzs2q4eZjUXg/fOr8asBiRGNwC9jxcjeMMDYFY\n"
    "BwIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

static Preferences prefs;
static WebServer server(80);

static bool gRequested = false;
static bool gAutoExitAttempted = false;
static String gAutoExitErr;
static String gAutoExitTarget;
static bool gRebootScheduled = false;
static uint32_t gRebootAtMs = 0;
static String gRebootReason;

struct OtaCtx {
  bool active = false;
  bool ok = false;
  String err;
  esp_ota_handle_t handle = 0;
  const esp_partition_t *target = nullptr;
  size_t written = 0;
  bool fwDone = false;
  bool sigDone = false;
  bool sigPartActive = false;
  bool sigOverflow = false;
  uint8_t sigBuf[512];
  size_t sigLen = 0;
  mbedtls_sha256_context sha;
  bool shaInit = false;
  uint8_t digest[32];
};

static OtaCtx gOta;

struct DataFlashCtx {
  bool active = false;
  bool ok = false;
  String err;
  const esp_partition_t *target = nullptr;
  size_t written = 0;
  bool dataDone = false;
  bool sigDone = false;
  bool sigPartActive = false;
  bool sigOverflow = false;
  uint8_t sigBuf[512];
  size_t sigLen = 0;
  mbedtls_sha256_context sha;
  bool shaInit = false;
  uint8_t digest[32];
};

static DataFlashCtx gDataFlash;
static uint8_t gFlashDlBuf[1024];

static const uint8_t kPackMagic[8] = {'L', 'B', 'P', 'A', 'C', 'K', '1', 0};
static const uint32_t kPackVersion = 1;
static const size_t kPackHeaderSize = 28;

enum class PackStage : uint8_t {
  HDR = 0,
  FW_SIG = 1,
  FS_SIG = 2,
  FW_DATA = 3,
  FS_DATA = 4,
  DONE = 5
};

static const char *packStageName(PackStage s) {
  switch (s) {
    case PackStage::HDR: return "hdr";
    case PackStage::FW_SIG: return "fw_sig";
    case PackStage::FS_SIG: return "fs_sig";
    case PackStage::FW_DATA: return "fw_data";
    case PackStage::FS_DATA: return "fs_data";
    case PackStage::DONE: return "done";
  }
  return "unknown";
}

struct PackFlashCtx {
  bool active = false;
  bool ok = false;
  String err;
  const esp_partition_t *otaTarget = nullptr;
  const esp_partition_t *fsTarget = nullptr;

  size_t totalExpected = 0;
  size_t received = 0;

  PackStage stage = PackStage::HDR;
  uint8_t hdr[kPackHeaderSize];
  size_t hdrLen = 0;

  uint32_t fwSize = 0;
  uint32_t fsSize = 0;
  uint16_t fwSigLen = 0;
  uint16_t fsSigLen = 0;

  uint8_t fwSig[512];
  uint8_t fsSig[512];
  size_t fwSigRead = 0;
  size_t fsSigRead = 0;

  esp_ota_handle_t otaHandle = 0;
  size_t fwWritten = 0;
  size_t fsWritten = 0;

  bool fwDone = false;
  bool fsDone = false;

  mbedtls_sha256_context fwSha;
  mbedtls_sha256_context fsSha;
  bool fwShaInit = false;
  bool fsShaInit = false;
  uint8_t fwDigest[32];
  uint8_t fsDigest[32];
};

static PackFlashCtx gPackFlash;

static String mbedtlsErrToString(int rc) {
  char buf[128];
  buf[0] = 0;
  mbedtls_strerror(rc, buf, sizeof(buf));
  if (buf[0]) return String(buf);
  return String(rc);
}

static bool verifyDigestSignature(const uint8_t digest[32], const uint8_t *sig, size_t sigLen, String *outErr = nullptr) {
  if (!kRecoveryRequireSignature) return true;
  if (!sig || sigLen == 0) {
    if (outErr) *outErr = "signature missing";
    return false;
  }
  if (strlen(kRecoverySignerPubKeyPem) < 16) {
    if (outErr) *outErr = "public key not configured";
    return false;
  }

  mbedtls_pk_context pk;
  mbedtls_pk_init(&pk);
  int rc = mbedtls_pk_parse_public_key(&pk, (const unsigned char *)kRecoverySignerPubKeyPem,
                                       strlen(kRecoverySignerPubKeyPem) + 1);
  if (rc != 0) {
    if (outErr) *outErr = String("parse pubkey failed: ") + mbedtlsErrToString(rc);
    mbedtls_pk_free(&pk);
    return false;
  }
  rc = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, digest, 0, sig, sigLen);
  mbedtls_pk_free(&pk);
  if (rc != 0) {
    if (outErr) *outErr = String("signature verify failed: ") + mbedtlsErrToString(rc);
    return false;
  }
  return true;
}

static bool loadSavedWifiCreds(String *outSsid, String *outPass) {
  if (!outSsid || !outPass) return false;
  prefs.begin(kPrefsNs, true);
  *outSsid = prefs.getString(kWifiSsidKey, "");
  *outPass = prefs.getString(kWifiPassKey, "");
  prefs.end();
  return !outSsid->isEmpty();
}

static String currentRecoveryIp() {
  if (WiFi.status() == WL_CONNECTED) {
    IPAddress ip = WiFi.localIP();
    if (ip[0] || ip[1] || ip[2] || ip[3]) return ip.toString();
  }
  return WiFi.softAPIP().toString();
}

static void startRecoveryNetwork() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(kRecoverySsid);  // keep AP always available as fallback

  String ssid, pass;
  if (!loadSavedWifiCreds(&ssid, &pass)) {
    Serial.println("Recovery STA skipped: no saved Wi-Fi credentials");
    return;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());
  const uint32_t t0 = millis();
  while ((millis() - t0) < kRecoveryStaConnectTimeoutMs) {
    if (WiFi.status() == WL_CONNECTED) break;
    delay(120);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.printf("Recovery STA connected: ssid=%s ip=%s\n", ssid.c_str(), WiFi.localIP().toString().c_str());
  } else {
    Serial.printf("Recovery STA timeout: ssid=%s status=%d\n", ssid.c_str(), (int)WiFi.status());
  }
}

static String jsonEscape(const String &s) {
  String o;
  o.reserve(s.length() + 8);
  for (size_t i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '\\') o += F("\\\\");
    else if (c == '"') o += F("\\\"");
    else if (c == '\n') o += F("\\n");
    else if (c == '\r') o += F("\\r");
    else if (c == '\t') o += F("\\t");
    else o += c;
  }
  return o;
}

static String mainApSsidHint() {
  uint8_t mac[6] = {0};
  if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) return String("LB-SETUP");
  char full[13];
  snprintf(full, sizeof(full), "%02X%02X%02X%02X%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  String s(full);
  if (s.length() > 5) s = s.substring(s.length() - 5);
  return String("LB-SETUP-") + s;
}

static String mainMdnsHostName() {
  uint8_t mac[6] = {0};
  if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) return String("lb-bridge");
  char full[13];
  snprintf(full, sizeof(full), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  String s(full);
  if (s.length() > 5) s = s.substring(s.length() - 5);
  return String("lb-bridge-") + s;
}

static String mainMdnsHostHint() {
  return mainMdnsHostName() + ".local";
}

static String mainMdnsUrlHint() {
  return String("http://") + mainMdnsHostHint() + "/";
}

static String contentTypeFor(const String &path) {
  if (path.endsWith(".html")) return "text/html";
  if (path.endsWith(".css")) return "text/css";
  if (path.endsWith(".js")) return "application/javascript";
  if (path.endsWith(".webmanifest")) return "application/manifest+json";
  if (path.endsWith(".png")) return "image/png";
  if (path.endsWith(".ico")) return "image/x-icon";
  if (path.endsWith(".svg")) return "image/svg+xml";
  return "application/octet-stream";
}

static bool tryServeFile(String path) {
  if (path.endsWith("/")) path += "recovery.html";
  if (!path.startsWith("/")) path = "/" + path;
  if (!LittleFS.exists(path)) return false;

  File f = LittleFS.open(path, "r");
  if (!f) return false;

  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.streamFile(f, contentTypeFor(path));
  f.close();
  return true;
}

static void sendJson(int code, const String &body) {
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

static void scheduleReboot(const char *reason, uint32_t delayMs = 450) {
  gRebootScheduled = true;
  gRebootAtMs = millis() + delayMs;
  gRebootReason = reason ? String(reason) : String();
}

// Forward declarations used by package updater helpers.
static bool isSystemWritableAppSlot(const esp_partition_t *p);
static const esp_partition_t *littlefsPartition();
static const esp_partition_t *otaPartitionA();

static uint16_t readLe16(const uint8_t *p) {
  return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t readLe32(const uint8_t *p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void packCleanup(bool resetState) {
  if (gPackFlash.fwShaInit) {
    mbedtls_sha256_free(&gPackFlash.fwSha);
    gPackFlash.fwShaInit = false;
  }
  if (gPackFlash.fsShaInit) {
    mbedtls_sha256_free(&gPackFlash.fsSha);
    gPackFlash.fsShaInit = false;
  }
  if (gPackFlash.otaHandle) {
    esp_ota_abort(gPackFlash.otaHandle);
    gPackFlash.otaHandle = 0;
  }
  if (resetState) gPackFlash = PackFlashCtx{};
}

static bool packPrepareFromHeader() {
  if (!gPackFlash.ok) return false;
  if (memcmp(gPackFlash.hdr + 0, kPackMagic, sizeof(kPackMagic)) != 0) {
    gPackFlash.ok = false;
    gPackFlash.err = "bad package magic";
    return false;
  }
  const uint32_t ver = readLe32(gPackFlash.hdr + 8);
  gPackFlash.fwSize = readLe32(gPackFlash.hdr + 12);
  gPackFlash.fsSize = readLe32(gPackFlash.hdr + 16);
  gPackFlash.fwSigLen = readLe16(gPackFlash.hdr + 20);
  gPackFlash.fsSigLen = readLe16(gPackFlash.hdr + 22);
  (void)readLe32(gPackFlash.hdr + 24);  // reserved

  if (ver != kPackVersion) {
    gPackFlash.ok = false;
    gPackFlash.err = "unsupported package version";
    return false;
  }
  if (gPackFlash.fwSize == 0 || gPackFlash.fsSize == 0) {
    gPackFlash.ok = false;
    gPackFlash.err = "empty package sections";
    return false;
  }
  if (gPackFlash.fwSigLen == 0 || gPackFlash.fsSigLen == 0) {
    gPackFlash.ok = false;
    gPackFlash.err = "signature missing";
    return false;
  }
  if (gPackFlash.fwSigLen > sizeof(gPackFlash.fwSig) || gPackFlash.fsSigLen > sizeof(gPackFlash.fsSig)) {
    gPackFlash.ok = false;
    gPackFlash.err = "signature too large";
    return false;
  }
  if (!gPackFlash.otaTarget || !isSystemWritableAppSlot(gPackFlash.otaTarget)) {
    gPackFlash.ok = false;
    gPackFlash.err = "invalid system target";
    return false;
  }
  if (!gPackFlash.fsTarget || gPackFlash.fsTarget->type != ESP_PARTITION_TYPE_DATA ||
      gPackFlash.fsTarget->subtype != ESP_PARTITION_SUBTYPE_DATA_SPIFFS) {
    gPackFlash.ok = false;
    gPackFlash.err = "invalid littlefs target";
    return false;
  }
  if (gPackFlash.fwSize > gPackFlash.otaTarget->size) {
    gPackFlash.ok = false;
    gPackFlash.err = "firmware too large for target";
    return false;
  }
  if (gPackFlash.fsSize > gPackFlash.fsTarget->size) {
    gPackFlash.ok = false;
    gPackFlash.err = "littlefs too large for target";
    return false;
  }

  esp_err_t e = esp_ota_begin(gPackFlash.otaTarget, OTA_SIZE_UNKNOWN, &gPackFlash.otaHandle);
  if (e != ESP_OK) {
    gPackFlash.ok = false;
    gPackFlash.err = String("esp_ota_begin failed: ") + String((int)e);
    return false;
  }

  const size_t eraseSize = (gPackFlash.fsSize + 0xFFF) & ~((size_t)0xFFF);
  e = esp_partition_erase_range(gPackFlash.fsTarget, 0, eraseSize);
  if (e != ESP_OK) {
    gPackFlash.ok = false;
    gPackFlash.err = String("littlefs erase failed: ") + String((int)e);
    return false;
  }

  mbedtls_sha256_init(&gPackFlash.fwSha);
  if (mbedtls_sha256_starts_ret(&gPackFlash.fwSha, 0) != 0) {
    gPackFlash.ok = false;
    gPackFlash.err = "fw sha256 start failed";
    return false;
  }
  gPackFlash.fwShaInit = true;

  mbedtls_sha256_init(&gPackFlash.fsSha);
  if (mbedtls_sha256_starts_ret(&gPackFlash.fsSha, 0) != 0) {
    gPackFlash.ok = false;
    gPackFlash.err = "fs sha256 start failed";
    return false;
  }
  gPackFlash.fsShaInit = true;

  gPackFlash.stage = PackStage::FW_SIG;
  return true;
}

static void packConsume(const uint8_t *buf, size_t len) {
  if (!gPackFlash.active || !gPackFlash.ok) return;
  size_t off = 0;
  while (off < len && gPackFlash.ok) {
    if (gPackFlash.stage == PackStage::HDR) {
      const size_t need = kPackHeaderSize - gPackFlash.hdrLen;
      const size_t take = (len - off < need) ? (len - off) : need;
      memcpy(gPackFlash.hdr + gPackFlash.hdrLen, buf + off, take);
      gPackFlash.hdrLen += take;
      off += take;
      if (gPackFlash.hdrLen == kPackHeaderSize) {
        if (!packPrepareFromHeader()) return;
      }
      continue;
    }

    if (gPackFlash.stage == PackStage::FW_SIG) {
      const size_t need = gPackFlash.fwSigLen - gPackFlash.fwSigRead;
      const size_t take = (len - off < need) ? (len - off) : need;
      memcpy(gPackFlash.fwSig + gPackFlash.fwSigRead, buf + off, take);
      gPackFlash.fwSigRead += take;
      off += take;
      if (gPackFlash.fwSigRead == gPackFlash.fwSigLen) gPackFlash.stage = PackStage::FS_SIG;
      continue;
    }

    if (gPackFlash.stage == PackStage::FS_SIG) {
      const size_t need = gPackFlash.fsSigLen - gPackFlash.fsSigRead;
      const size_t take = (len - off < need) ? (len - off) : need;
      memcpy(gPackFlash.fsSig + gPackFlash.fsSigRead, buf + off, take);
      gPackFlash.fsSigRead += take;
      off += take;
      if (gPackFlash.fsSigRead == gPackFlash.fsSigLen) gPackFlash.stage = PackStage::FW_DATA;
      continue;
    }

    if (gPackFlash.stage == PackStage::FW_DATA) {
      const size_t need = (size_t)gPackFlash.fwSize - gPackFlash.fwWritten;
      const size_t take = (len - off < need) ? (len - off) : need;
      esp_err_t e = esp_ota_write(gPackFlash.otaHandle, buf + off, take);
      if (e != ESP_OK) {
        gPackFlash.ok = false;
        gPackFlash.err = String("esp_ota_write failed: ") + String((int)e);
        return;
      }
      if (mbedtls_sha256_update_ret(&gPackFlash.fwSha, buf + off, take) != 0) {
        gPackFlash.ok = false;
        gPackFlash.err = "fw sha256 update failed";
        return;
      }
      gPackFlash.fwWritten += take;
      off += take;
      if (gPackFlash.fwWritten == gPackFlash.fwSize) {
        e = esp_ota_end(gPackFlash.otaHandle);
        gPackFlash.otaHandle = 0;
        if (e != ESP_OK) {
          gPackFlash.ok = false;
          gPackFlash.err = String("esp_ota_end failed: ") + String((int)e);
          return;
        }
        if (mbedtls_sha256_finish_ret(&gPackFlash.fwSha, gPackFlash.fwDigest) != 0) {
          gPackFlash.ok = false;
          gPackFlash.err = "fw sha256 finish failed";
          return;
        }
        mbedtls_sha256_free(&gPackFlash.fwSha);
        gPackFlash.fwShaInit = false;
        gPackFlash.fwDone = true;
        gPackFlash.stage = PackStage::FS_DATA;
      }
      continue;
    }

    if (gPackFlash.stage == PackStage::FS_DATA) {
      const size_t need = (size_t)gPackFlash.fsSize - gPackFlash.fsWritten;
      const size_t take = (len - off < need) ? (len - off) : need;
      esp_err_t e = esp_partition_write(gPackFlash.fsTarget, gPackFlash.fsWritten, buf + off, take);
      if (e != ESP_OK) {
        gPackFlash.ok = false;
        gPackFlash.err = String("littlefs write failed: ") + String((int)e);
        return;
      }
      if (mbedtls_sha256_update_ret(&gPackFlash.fsSha, buf + off, take) != 0) {
        gPackFlash.ok = false;
        gPackFlash.err = "fs sha256 update failed";
        return;
      }
      gPackFlash.fsWritten += take;
      off += take;
      if (gPackFlash.fsWritten == gPackFlash.fsSize) {
        if (mbedtls_sha256_finish_ret(&gPackFlash.fsSha, gPackFlash.fsDigest) != 0) {
          gPackFlash.ok = false;
          gPackFlash.err = "fs sha256 finish failed";
          return;
        }
        mbedtls_sha256_free(&gPackFlash.fsSha);
        gPackFlash.fsShaInit = false;
        gPackFlash.fsDone = true;
        gPackFlash.stage = PackStage::DONE;
      }
      continue;
    }

    if (gPackFlash.stage == PackStage::DONE) {
      gPackFlash.ok = false;
      gPackFlash.err = "pack has trailing data";
      return;
    }
  }
}

static void beginPackFlash(size_t totalSize) {
  packCleanup(true);
  gPackFlash.active = true;
  gPackFlash.ok = true;
  gPackFlash.totalExpected = totalSize;
  gPackFlash.stage = PackStage::HDR;
  gPackFlash.otaTarget = otaPartitionA();
  gPackFlash.fsTarget = littlefsPartition();
}

static void uploadHandlerPack() {
  HTTPUpload &up = server.upload();
  String field = up.name;
  field.toLowerCase();
  if (field != "pack") return;

  if (up.status == UPLOAD_FILE_START) {
    beginPackFlash((size_t)up.totalSize);
    Serial.printf("Pack upload start: %s (%u bytes)\n", up.filename.c_str(), (unsigned)up.totalSize);
    return;
  }
  if (up.status == UPLOAD_FILE_WRITE) {
    if (!gPackFlash.active || !gPackFlash.ok) return;
    gPackFlash.received += up.currentSize;
    packConsume(up.buf, up.currentSize);
    return;
  }
  if (up.status == UPLOAD_FILE_END) {
    if (!gPackFlash.active) return;
    if (gPackFlash.ok && gPackFlash.stage != PackStage::DONE) {
      gPackFlash.ok = false;
      gPackFlash.err = "incomplete package";
    }
    Serial.printf("Pack upload end: recv=%u ok=%d err=%s\n", (unsigned)gPackFlash.received, gPackFlash.ok ? 1 : 0,
                  gPackFlash.err.c_str());
    return;
  }
  if (up.status == UPLOAD_FILE_ABORTED) {
    gPackFlash.ok = false;
    gPackFlash.err = "upload aborted";
    Serial.println("Pack upload aborted");
  }
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

static const esp_partition_t *littlefsPartition() {
  const esp_partition_t *p = esp_partition_find_first(
      ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_SPIFFS, NULL);
  return p;
}

static const esp_partition_t *otaPartitionA() {
  return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, NULL);
}
static const esp_partition_t *otaPartitionB() {
  return esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, NULL);
}

static void setForceRecovery(bool on) {
  prefs.begin(kPrefsNs, false);
  prefs.putBool("force_recovery", on);
  prefs.end();
}

static bool getForceRecovery() {
  prefs.begin(kPrefsNs, true);
  bool v = prefs.getBool("force_recovery", false);
  prefs.end();
  return v;
}

static void clearMainSettings() {
  prefs.begin(kPrefsNs, false);
  prefs.clear();  // clears ALL keys used by the main firmware (wifi/ble/config/boot flags)
  prefs.end();
}

static const esp_partition_t *pickBootableMainSlot(String *outWhich = nullptr) {
  const esp_partition_t *a = otaPartitionA();
  const esp_partition_t *b = otaPartitionB();
  String err;
  if (verifyAppPartitionHeader(a, &err)) {
    if (outWhich) *outWhich = "ota_a";
    return a;
  }
  if (verifyAppPartitionHeader(b, &err)) {
    if (outWhich) *outWhich = "ota_b";
    return b;
  }
  return nullptr;
}

static const char *slotName(const esp_partition_t *p) {
  if (!p) return "unknown";
  if (p->type == ESP_PARTITION_TYPE_APP && p->subtype == ESP_PARTITION_SUBTYPE_APP_OTA_0) return "ota_a";
  if (p->type == ESP_PARTITION_TYPE_APP && p->subtype == ESP_PARTITION_SUBTYPE_APP_OTA_1) return "ota_b";
  if (p->type == ESP_PARTITION_TYPE_APP && p->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY) return "recovery";
  return "unknown";
}

static void handleStatus() {
  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *boot = esp_ota_get_boot_partition();
  const esp_partition_t *otaA = otaPartitionA();
  const esp_partition_t *otaB = otaPartitionB();
  bool otaAValid = verifyAppPartitionHeader(otaA);
  bool otaBValid = verifyAppPartitionHeader(otaB);
  String bootableWhich;
  (void)pickBootableMainSlot(&bootableWhich);
  String ip = currentRecoveryIp();
  String apIp = WiFi.softAPIP().toString();
  bool staConnected = (WiFi.status() == WL_CONNECTED);
  bool forceRecovery = getForceRecovery();

  String body;
  body.reserve(840);
  body += F("{\"ok\":true");
  body += F(",\"mode\":\"recovery\"");
  body += F(",\"recovery_version\":\"");
  body += jsonEscape(kRecoveryVersion);
  body += F("\",\"pack_version\":");
  body += String((unsigned long)kPackVersion);
  body += F(",\"signature_required\":");
  body += (kRecoveryRequireSignature ? "true" : "false");
  body += F(",\"ap_ssid\":\"");
  body += jsonEscape(kRecoverySsid);
  body += F("\",\"ip\":\"");
  body += jsonEscape(ip);
  body += F("\",\"ap_ip\":\"");
  body += jsonEscape(apIp);
  body += F("\",\"sta_connected\":");
  body += (staConnected ? "true" : "false");
  body += F(",\"bootloader_writable_over_wifi\":false");
  body += F(",\"recovery_writable_over_wifi\":false");
  body += F(",\"force_recovery\":");
  body += (forceRecovery ? "true" : "false");
  body += F(",\"requested\":");
  body += (gRequested ? "true" : "false");
  body += F(",\"auto_exit_attempted\":");
  body += (gAutoExitAttempted ? "true" : "false");
  body += F(",\"auto_exit_target\":\"");
  body += jsonEscape(gAutoExitTarget);
  body += F("\",\"auto_exit_err\":\"");
  body += jsonEscape(gAutoExitErr);
  body += F("\"");

  body += F(",\"running_slot\":\"");
  body += slotName(running);
  body += F("\",\"running_label\":\"");
  body += jsonEscape(running ? String(running->label) : "");
  body += F("\"");

  body += F(",\"boot_slot\":\"");
  body += slotName(boot);
  body += F("\",\"boot_label\":\"");
  body += jsonEscape(boot ? String(boot->label) : "");
  body += F("\"");

  body += F(",\"ota_a_valid\":");
  body += (otaAValid ? "true" : "false");
  body += F(",\"ota_b_valid\":");
  body += (otaBValid ? "true" : "false");
  body += F(",\"ota_a_present\":");
  body += (otaA ? "true" : "false");
  body += F(",\"ota_b_present\":");
  body += (otaB ? "true" : "false");

  body += F(",\"bootable_main\":\"");
  body += jsonEscape(bootableWhich);
  body += F("\"");
  body += F(",\"main_ap_ssid_hint\":\"");
  body += jsonEscape(mainApSsidHint());
  body += F("\"");
  body += F(",\"pack_active\":");
  body += (gPackFlash.active ? "true" : "false");
  body += F(",\"pack_ok\":");
  body += (gPackFlash.ok ? "true" : "false");
  body += F(",\"pack_stage\":\"");
  body += packStageName(gPackFlash.stage);
  body += F("\",\"pack_received\":");
  body += String((unsigned long)gPackFlash.received);
  body += F(",\"pack_total\":");
  body += String((unsigned long)gPackFlash.totalExpected);
  body += F(",\"pack_fw_written\":");
  body += String((unsigned long)gPackFlash.fwWritten);
  body += F(",\"pack_fw_total\":");
  body += String((unsigned long)gPackFlash.fwSize);
  body += F(",\"pack_fs_written\":");
  body += String((unsigned long)gPackFlash.fsWritten);
  body += F(",\"pack_fs_total\":");
  body += String((unsigned long)gPackFlash.fsSize);
  body += F(",\"pack_err\":\"");
  body += jsonEscape(gPackFlash.err);
  body += F("\"");

  body += F("}");
  sendJson(200, body);
}

static void handleResetSettings() {
  clearMainSettings();
  sendJson(200, "{\"ok\":true}");
}

static void handleBootMain() {
  String which;
  const esp_partition_t *t = pickBootableMainSlot(&which);
  if (!t) {
    sendJsonError(400, "no valid OTA slot found");
    return;
  }

  esp_err_t e = esp_ota_set_boot_partition(t);
  if (e != ESP_OK) {
    sendJsonError(500, String("esp_ota_set_boot_partition failed: ") + String((int)e));
    return;
  }

  setForceRecovery(false);
  String body;
  body.reserve(220);
  body += F("{\"ok\":true,\"rebooting\":true,\"slot\":\"");
  body += jsonEscape(which);
  body += F("\",\"next_url\":\"");
  body += jsonEscape(mainMdnsUrlHint());
  body += F("\",\"main_ap_ssid_hint\":\"");
  body += jsonEscape(mainApSsidHint());
  body += F("\",\"main_host_hint\":\"");
  body += jsonEscape(mainMdnsHostHint());
  body += F("\"}");
  sendJson(200, body);
  scheduleReboot("boot_main");
}

static void handleRebootRecovery() {
  const esp_partition_t *factory =
      esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_FACTORY, NULL);
  if (!factory) {
    sendJsonError(500, "recovery slot not found");
    return;
  }

  esp_err_t e = esp_ota_set_boot_partition(factory);
  if (e != ESP_OK) {
    sendJsonError(500, String("esp_ota_set_boot_partition(factory) failed: ") + String((int)e));
    return;
  }

  setForceRecovery(true);
  sendJson(200, "{\"ok\":true,\"rebooting\":true}");
  scheduleReboot("reboot_recovery");
}

static void beginOtaTo(const esp_partition_t *target) {
  if (gOta.shaInit) {
    mbedtls_sha256_free(&gOta.sha);
    gOta.shaInit = false;
  }
  gOta = OtaCtx{};
  gOta.active = true;
  gOta.target = target;

  if (!target) {
    gOta.ok = false;
    gOta.err = "target partition missing";
    return;
  }

  // Safety rule: Recovery over Wi-Fi may only flash app OTA slots (System), never bootloader/recovery.
  if (!isSystemWritableAppSlot(target)) {
    gOta.ok = false;
    gOta.err = "invalid target (only System slot is writable in recovery)";
    return;
  }
  if (target->address < 0x10000) {
    gOta.ok = false;
    gOta.err = "invalid target address";
    return;
  }

  esp_err_t e = esp_ota_begin(target, OTA_SIZE_UNKNOWN, &gOta.handle);
  if (e != ESP_OK) {
    gOta.ok = false;
    gOta.err = String("esp_ota_begin failed: ") + String((int)e);
    return;
  }

  mbedtls_sha256_init(&gOta.sha);
  if (mbedtls_sha256_starts_ret(&gOta.sha, 0) != 0) {
    gOta.ok = false;
    gOta.err = "sha256 start failed";
    return;
  }
  gOta.shaInit = true;
  gOta.ok = true;
}

static void writeOtaChunk(const uint8_t *buf, size_t len) {
  if (!gOta.active || gOta.ok == false) return;
  if (!gOta.handle) return;
  esp_err_t e = esp_ota_write(gOta.handle, buf, len);
  if (e != ESP_OK) {
    gOta.ok = false;
    gOta.err = String("esp_ota_write failed: ") + String((int)e);
    return;
  }
  if (gOta.shaInit) {
    if (mbedtls_sha256_update_ret(&gOta.sha, buf, len) != 0) {
      gOta.ok = false;
      gOta.err = "sha256 update failed";
      return;
    }
  }
  gOta.written += len;
}

static void endOtaImage() {
  if (!gOta.active) return;
  if (!gOta.handle) {
    gOta.ok = false;
    if (gOta.err.isEmpty()) gOta.err = "no OTA handle";
    return;
  }

  esp_err_t e = esp_ota_end(gOta.handle);
  if (e != ESP_OK) {
    gOta.ok = false;
    gOta.err = String("esp_ota_end failed: ") + String((int)e);
    return;
  }
  gOta.handle = 0;
  if (gOta.shaInit) {
    if (mbedtls_sha256_finish_ret(&gOta.sha, gOta.digest) != 0) {
      gOta.ok = false;
      gOta.err = "sha256 finish failed";
      return;
    }
    mbedtls_sha256_free(&gOta.sha);
    gOta.shaInit = false;
  }
  gOta.fwDone = true;
  gOta.ok = true;
}

static void uploadHandlerTo(const esp_partition_t *target) {
  HTTPUpload &up = server.upload();
  String field = up.name;
  field.toLowerCase();

  if (up.status == UPLOAD_FILE_START) {
    if (field == "fw") {
      beginOtaTo(target);
      if (!gOta.ok) {
        Serial.printf("OTA upload rejected: %s\n", gOta.err.c_str());
      }
      Serial.printf("OTA upload start: %s (%u bytes?)\n", up.filename.c_str(), (unsigned)up.totalSize);
      return;
    }
    if (field == "sig") {
      if (!gOta.active) {
        gOta = OtaCtx{};
        gOta.active = true;
      }
      gOta.sigPartActive = true;
      gOta.sigDone = false;
      gOta.sigOverflow = false;
      gOta.sigLen = 0;
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_WRITE) {
    if (!gOta.active) return;
    if (field == "fw") {
      if (!gOta.ok) return;
      writeOtaChunk(up.buf, up.currentSize);
      return;
    }
    if (field == "sig") {
      if (!gOta.sigPartActive || gOta.sigOverflow) return;
      if ((gOta.sigLen + up.currentSize) > sizeof(gOta.sigBuf)) {
        gOta.sigOverflow = true;
        gOta.ok = false;
        gOta.err = "signature too large";
        return;
      }
      memcpy(gOta.sigBuf + gOta.sigLen, up.buf, up.currentSize);
      gOta.sigLen += up.currentSize;
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_END) {
    if (!gOta.active) return;
    if (field == "fw") {
      if (gOta.ok) endOtaImage();
      Serial.printf("OTA upload end: written=%u ok=%d err=%s\n", (unsigned)gOta.written, gOta.ok ? 1 : 0,
                    gOta.err.c_str());
      return;
    }
    if (field == "sig") {
      gOta.sigPartActive = false;
      if (!gOta.sigOverflow && gOta.sigLen > 0) {
        gOta.sigDone = true;
      } else if (gOta.err.isEmpty()) {
        gOta.ok = false;
        gOta.err = "signature missing";
      }
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_ABORTED) {
    gOta.ok = false;
    gOta.err = "upload aborted";
    Serial.println("OTA upload aborted");
  }
}

static void handleFlashSystemPost() {
  if (!gOta.active) {
    sendJsonError(400, "no upload");
    return;
  }
  if (!gOta.fwDone) {
    sendJsonError(400, "firmware part missing");
    return;
  }
  String sigErr;
  if (!verifyDigestSignature(gOta.digest, gOta.sigBuf, gOta.sigLen, &sigErr)) {
    gOta.ok = false;
    gOta.err = sigErr;
    sendJsonError(401, sigErr);
    return;
  }
  if (!gOta.ok) {
    sendJsonError(500, gOta.err.isEmpty() ? "flash failed" : gOta.err);
    return;
  }
  esp_err_t e = esp_ota_set_boot_partition(gOta.target);
  if (e != ESP_OK) {
    sendJsonError(500, String("esp_ota_set_boot_partition failed: ") + String((int)e));
    return;
  }
  setForceRecovery(false);
  String body;
  body.reserve(220);
  body += F("{\"ok\":true,\"rebooting\":true,\"next_url\":\"");
  body += jsonEscape(mainMdnsUrlHint());
  body += F("\",\"main_ap_ssid_hint\":\"");
  body += jsonEscape(mainApSsidHint());
  body += F("\",\"main_host_hint\":\"");
  body += jsonEscape(mainMdnsHostHint());
  body += F("\",\"note\":\"reconnect to main network and open mDNS host\"}");
  sendJson(200, body);
  scheduleReboot("flash_system_done", 900);
}

static void beginDataFlashTo(const esp_partition_t *target, size_t incomingSize) {
  if (gDataFlash.shaInit) {
    mbedtls_sha256_free(&gDataFlash.sha);
    gDataFlash.shaInit = false;
  }
  gDataFlash = DataFlashCtx{};
  gDataFlash.active = true;
  gDataFlash.target = target;

  if (!target) {
    gDataFlash.ok = false;
    gDataFlash.err = "littlefs partition missing";
    return;
  }
  if (target->type != ESP_PARTITION_TYPE_DATA || target->subtype != ESP_PARTITION_SUBTYPE_DATA_SPIFFS) {
    gDataFlash.ok = false;
    gDataFlash.err = "invalid littlefs target";
    return;
  }
  if (incomingSize == 0) {
    gDataFlash.ok = false;
    gDataFlash.err = "empty upload";
    return;
  }
  if (incomingSize > target->size) {
    gDataFlash.ok = false;
    gDataFlash.err = "littlefs image too large";
    return;
  }

  const size_t eraseSize = (incomingSize + 0xFFF) & ~((size_t)0xFFF);
  esp_err_t e = esp_partition_erase_range(target, 0, eraseSize);
  if (e != ESP_OK) {
    gDataFlash.ok = false;
    gDataFlash.err = String("erase failed: ") + String((int)e);
    return;
  }
  mbedtls_sha256_init(&gDataFlash.sha);
  if (mbedtls_sha256_starts_ret(&gDataFlash.sha, 0) != 0) {
    gDataFlash.ok = false;
    gDataFlash.err = "sha256 start failed";
    return;
  }
  gDataFlash.shaInit = true;
  gDataFlash.ok = true;
}

static void writeDataFlashChunk(const uint8_t *buf, size_t len) {
  if (!gDataFlash.active || !gDataFlash.ok || !gDataFlash.target) return;
  if (len == 0) return;
  if ((gDataFlash.written + len) > gDataFlash.target->size) {
    gDataFlash.ok = false;
    gDataFlash.err = "littlefs write overflow";
    return;
  }
  esp_err_t e = esp_partition_write(gDataFlash.target, gDataFlash.written, buf, len);
  if (e != ESP_OK) {
    gDataFlash.ok = false;
    gDataFlash.err = String("write failed: ") + String((int)e);
    return;
  }
  if (gDataFlash.shaInit) {
    if (mbedtls_sha256_update_ret(&gDataFlash.sha, buf, len) != 0) {
      gDataFlash.ok = false;
      gDataFlash.err = "sha256 update failed";
      return;
    }
  }
  gDataFlash.written += len;
}

static void uploadHandlerLittlefs() {
  HTTPUpload &up = server.upload();
  String field = up.name;
  field.toLowerCase();

  if (up.status == UPLOAD_FILE_START) {
    if (field == "fw") {
      beginDataFlashTo(littlefsPartition(), (size_t)up.totalSize);
      if (!gDataFlash.ok) {
        Serial.printf("LittleFS upload rejected: %s\n", gDataFlash.err.c_str());
      }
      Serial.printf("LittleFS upload start: %s (%u bytes)\n", up.filename.c_str(), (unsigned)up.totalSize);
      return;
    }
    if (field == "sig") {
      if (!gDataFlash.active) {
        gDataFlash = DataFlashCtx{};
        gDataFlash.active = true;
      }
      gDataFlash.sigPartActive = true;
      gDataFlash.sigDone = false;
      gDataFlash.sigOverflow = false;
      gDataFlash.sigLen = 0;
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_WRITE) {
    if (!gDataFlash.active) return;
    if (field == "fw") {
      if (!gDataFlash.ok) return;
      writeDataFlashChunk(up.buf, up.currentSize);
      return;
    }
    if (field == "sig") {
      if (!gDataFlash.sigPartActive || gDataFlash.sigOverflow) return;
      if ((gDataFlash.sigLen + up.currentSize) > sizeof(gDataFlash.sigBuf)) {
        gDataFlash.sigOverflow = true;
        gDataFlash.ok = false;
        gDataFlash.err = "signature too large";
        return;
      }
      memcpy(gDataFlash.sigBuf + gDataFlash.sigLen, up.buf, up.currentSize);
      gDataFlash.sigLen += up.currentSize;
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_END) {
    if (!gDataFlash.active) return;
    if (field == "fw") {
      if (gDataFlash.shaInit) {
        if (mbedtls_sha256_finish_ret(&gDataFlash.sha, gDataFlash.digest) != 0) {
          gDataFlash.ok = false;
          gDataFlash.err = "sha256 finish failed";
        }
        mbedtls_sha256_free(&gDataFlash.sha);
        gDataFlash.shaInit = false;
      }
      gDataFlash.dataDone = true;
      Serial.printf("LittleFS upload end: written=%u ok=%d err=%s\n",
                    (unsigned)gDataFlash.written, gDataFlash.ok ? 1 : 0, gDataFlash.err.c_str());
      return;
    }
    if (field == "sig") {
      gDataFlash.sigPartActive = false;
      if (!gDataFlash.sigOverflow && gDataFlash.sigLen > 0) {
        gDataFlash.sigDone = true;
      } else if (gDataFlash.err.isEmpty()) {
        gDataFlash.ok = false;
        gDataFlash.err = "signature missing";
      }
      return;
    }
    return;
  }

  if (up.status == UPLOAD_FILE_ABORTED) {
    gDataFlash.ok = false;
    gDataFlash.err = "upload aborted";
    Serial.println("LittleFS upload aborted");
  }
}

static void handleFlashLittlefsPost() {
  if (!gDataFlash.active) {
    sendJsonError(400, "no upload");
    return;
  }
  if (!gDataFlash.dataDone) {
    sendJsonError(400, "littlefs part missing");
    return;
  }
  String sigErr;
  if (!verifyDigestSignature(gDataFlash.digest, gDataFlash.sigBuf, gDataFlash.sigLen, &sigErr)) {
    gDataFlash.ok = false;
    gDataFlash.err = sigErr;
    sendJsonError(401, sigErr);
    return;
  }
  if (!gDataFlash.ok) {
    sendJsonError(500, gDataFlash.err.isEmpty() ? "littlefs flash failed" : gDataFlash.err);
    return;
  }
  String body;
  body.reserve(96);
  body += F("{\"ok\":true,\"part\":\"littlefs\",\"bytes\":");
  body += String((unsigned long)gDataFlash.written);
  body += F("}");
  sendJson(200, body);
}

static void handleFlashAllPackPost() {
  if (!gPackFlash.active) {
    sendJsonError(400, "no upload");
    return;
  }
  if (!gPackFlash.ok) {
    sendJsonError(500, gPackFlash.err.isEmpty() ? "pack flash failed" : gPackFlash.err);
    packCleanup(true);
    return;
  }
  if (gPackFlash.stage != PackStage::DONE || !gPackFlash.fwDone || !gPackFlash.fsDone) {
    sendJsonError(400, "incomplete package");
    packCleanup(true);
    return;
  }

  String sigErr;
  if (!verifyDigestSignature(gPackFlash.fwDigest, gPackFlash.fwSig, gPackFlash.fwSigLen, &sigErr)) {
    sendJsonError(401, String("system ") + sigErr);
    packCleanup(true);
    return;
  }
  if (!verifyDigestSignature(gPackFlash.fsDigest, gPackFlash.fsSig, gPackFlash.fsSigLen, &sigErr)) {
    sendJsonError(401, String("littlefs ") + sigErr);
    packCleanup(true);
    return;
  }

  esp_err_t e = esp_ota_set_boot_partition(gPackFlash.otaTarget);
  if (e != ESP_OK) {
    sendJsonError(500, String("esp_ota_set_boot_partition failed: ") + String((int)e));
    packCleanup(true);
    return;
  }

  setForceRecovery(false);
  String body;
  body.reserve(260);
  body += F("{\"ok\":true,\"rebooting\":true,\"mode\":\"pack\",\"system_bytes\":");
  body += String((unsigned long)gPackFlash.fwWritten);
  body += F(",\"littlefs_bytes\":");
  body += String((unsigned long)gPackFlash.fsWritten);
  body += F(",\"next_url\":\"");
  body += jsonEscape(mainMdnsUrlHint());
  body += F("\",\"main_ap_ssid_hint\":\"");
  body += jsonEscape(mainApSsidHint());
  body += F("\",\"main_host_hint\":\"");
  body += jsonEscape(mainMdnsHostHint());
  body += F("\"}");
  sendJson(200, body);
  packCleanup(true);
  scheduleReboot("flash_pack_done", 900);
}

static bool packFinalizeAndStageBoot(String *outErr, String *outBody) {
  if (!outErr || !outBody) return false;
  *outErr = "";
  outBody->remove(0);

  if (!gPackFlash.active) {
    *outErr = "no upload";
    return false;
  }
  if (!gPackFlash.ok) {
    *outErr = gPackFlash.err.isEmpty() ? "pack flash failed" : gPackFlash.err;
    return false;
  }
  if (gPackFlash.stage != PackStage::DONE || !gPackFlash.fwDone || !gPackFlash.fsDone) {
    *outErr = "incomplete package";
    return false;
  }

  String sigErr;
  if (!verifyDigestSignature(gPackFlash.fwDigest, gPackFlash.fwSig, gPackFlash.fwSigLen, &sigErr)) {
    *outErr = String("system ") + sigErr;
    return false;
  }
  if (!verifyDigestSignature(gPackFlash.fsDigest, gPackFlash.fsSig, gPackFlash.fsSigLen, &sigErr)) {
    *outErr = String("littlefs ") + sigErr;
    return false;
  }

  esp_err_t e = esp_ota_set_boot_partition(gPackFlash.otaTarget);
  if (e != ESP_OK) {
    *outErr = String("esp_ota_set_boot_partition failed: ") + String((int)e);
    return false;
  }

  setForceRecovery(false);
  outBody->reserve(300);
  *outBody += F("{\"ok\":true,\"rebooting\":true,\"mode\":\"pack\",\"system_bytes\":");
  *outBody += String((unsigned long)gPackFlash.fwWritten);
  *outBody += F(",\"littlefs_bytes\":");
  *outBody += String((unsigned long)gPackFlash.fsWritten);
  *outBody += F(",\"next_url\":\"");
  *outBody += jsonEscape(mainMdnsUrlHint());
  *outBody += F("\",\"main_ap_ssid_hint\":\"");
  *outBody += jsonEscape(mainApSsidHint());
  *outBody += F("\",\"main_host_hint\":\"");
  *outBody += jsonEscape(mainMdnsHostHint());
  *outBody += F("\"}");
  return true;
}

static void uploadHandlerDiscardRawFlash() {
  HTTPUpload &up = server.upload();
  if (up.status == UPLOAD_FILE_START) {
    Serial.printf("Raw recovery flash blocked: %s (%u bytes)\n", up.filename.c_str(), (unsigned)up.totalSize);
  }
}

static void handleRawFlashDisabled() {
  sendJsonError(403, "raw recovery flash disabled; use signed update.lbpack only");
}

static String mirrorPackToGithubFallback(const String &url) {
  const String pfx = "https://serjio193.github.io/legacy-bridge/releases/";
  const String sfx = "/update.lbpack";
  if (!url.startsWith(pfx) || !url.endsWith(sfx)) return String();
  int tagStart = (int)pfx.length();
  int tagEnd = (int)url.length() - (int)sfx.length();
  if (tagEnd <= tagStart) return String();
  String tag = url.substring(tagStart, tagEnd);
  tag.trim();
  if (tag.isEmpty() || tag.indexOf('/') >= 0) return String();
  return String("https://github.com/Serjio193/legacy-bridge/releases/download/") + tag + "/update.lbpack";
}

static String extractHostFromHttpsUrl(const String &url) {
  if (!url.startsWith("https://")) return String();
  const int start = 8;  // len("https://")
  int slash = url.indexOf('/', start);
  int qmark = url.indexOf('?', start);
  int hash = url.indexOf('#', start);
  int end = (int)url.length();
  if (slash >= 0 && slash < end) end = slash;
  if (qmark >= 0 && qmark < end) end = qmark;
  if (hash >= 0 && hash < end) end = hash;
  if (end <= start) return String();
  return url.substring(start, end);
}

static String extractPathFromUrl(const String &url) {
  int scheme = url.indexOf("://");
  if (scheme < 0) return String("/");
  int start = scheme + 3;
  int slash = url.indexOf('/', start);
  if (slash < 0) return String("/");
  int qmark = url.indexOf('?', slash);
  int hash = url.indexOf('#', slash);
  int end = (int)url.length();
  if (qmark >= 0 && qmark < end) end = qmark;
  if (hash >= 0 && hash < end) end = hash;
  if (end <= slash) return String("/");
  return url.substring(slash, end);
}

static bool isTrustedPackUrl(const String &url, String *outErr = nullptr) {
  if (!url.startsWith("https://")) {
    if (outErr) *outErr = "only https:// is allowed";
    return false;
  }

  String host = extractHostFromHttpsUrl(url);
  host.toLowerCase();
  if (host.isEmpty()) {
    if (outErr) *outErr = "invalid url host";
    return false;
  }

  String path = extractPathFromUrl(url);
  if (!path.endsWith("/update.lbpack")) {
    if (outErr) *outErr = "url must end with /update.lbpack";
    return false;
  }

  if (host == "serjio193.github.io") {
    if (!path.startsWith("/legacy-bridge/releases/")) {
      if (outErr) *outErr = "mirror path not allowed";
      return false;
    }
    return true;
  }

  if (host == "github.com") {
    if (!path.startsWith("/Serjio193/legacy-bridge/releases/download/")) {
      if (outErr) *outErr = "github path not allowed";
      return false;
    }
    return true;
  }

  if (outErr) *outErr = String("host not allowed: ") + host;
  return false;
}

static void handleFlashPackByUrl() {
  String body = server.arg("plain");
  body.trim();
  if (body.isEmpty()) {
    sendJsonError(400, "json body required");
    return;
  }

  JsonDocument doc;
  DeserializationError je = deserializeJson(doc, body);
  if (je) {
    sendJsonError(400, String("bad json: ") + je.c_str());
    return;
  }

  String url = "";
  if (!doc["url"].isNull()) url = String((const char *)doc["url"]);
  url.trim();
  String urlErr;
  if (!isTrustedPackUrl(url, &urlErr)) {
    sendJsonError(400, urlErr);
    return;
  }

  beginPackFlash(0);
  if (!gPackFlash.ok) {
    sendJsonError(500, gPackFlash.err.isEmpty() ? "pack init failed" : gPackFlash.err);
    packCleanup(true);
    return;
  }

  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.setConnectTimeout(12000);
  http.setTimeout(20000);

  std::unique_ptr<WiFiClientSecure> tls;
  bool beginOk = false;
  tls.reset(new WiFiClientSecure());
  tls->setInsecure();  // package signature is verified by recovery key
  beginOk = http.begin(*tls, url);
  if (!beginOk) {
    sendJsonError(500, "http begin failed");
    packCleanup(true);
    return;
  }

  int code = http.GET();
  if (code == HTTP_CODE_NOT_FOUND) {
    String fallbackUrl = mirrorPackToGithubFallback(url);
    if (!fallbackUrl.isEmpty() && fallbackUrl != url) {
      String fallbackErr;
      if (!isTrustedPackUrl(fallbackUrl, &fallbackErr)) {
        sendJsonError(500, String("fallback blocked: ") + fallbackErr);
        http.end();
        packCleanup(true);
        return;
      }
      http.end();
      url = fallbackUrl;
      beginOk = false;
      tls.reset(new WiFiClientSecure());
      tls->setInsecure();
      beginOk = http.begin(*tls, url);
      if (!beginOk) {
        sendJsonError(500, "http begin failed (fallback)");
        packCleanup(true);
        return;
      }
      code = http.GET();
    }
  }
  if (code != HTTP_CODE_OK) {
    String msg = String("download failed: ") + String(code);
    sendJsonError(500, msg);
    http.end();
    packCleanup(true);
    return;
  }

  WiFiClient *stream = http.getStreamPtr();
  if (!stream) {
    sendJsonError(500, "download stream unavailable");
    http.end();
    packCleanup(true);
    return;
  }

  int remaining = http.getSize();
  uint32_t lastDataMs = millis();
  const uint32_t startedMs = millis();
  const uint32_t kTotalDownloadTimeoutMs = 180000UL;

  while (http.connected() && (remaining > 0 || remaining == -1)) {
    if ((millis() - startedMs) > kTotalDownloadTimeoutMs) {
      gPackFlash.ok = false;
      gPackFlash.err = "download timeout";
      break;
    }
    size_t avail = stream->available();
    if (avail > 0) {
      size_t take = avail < sizeof(gFlashDlBuf) ? avail : sizeof(gFlashDlBuf);
      int n = stream->readBytes((char *)gFlashDlBuf, take);
      if (n > 0) {
        gPackFlash.received += (size_t)n;
        packConsume(gFlashDlBuf, (size_t)n);
        if (!gPackFlash.ok) break;
        if (remaining > 0) remaining -= n;
        lastDataMs = millis();
      }
    } else {
      if ((millis() - lastDataMs) > 20000UL) {
        gPackFlash.ok = false;
        gPackFlash.err = "download timeout";
        break;
      }
      delay(2);
    }
  }
  http.end();

  if (gPackFlash.ok && gPackFlash.stage != PackStage::DONE) {
    gPackFlash.ok = false;
    gPackFlash.err = "incomplete package";
  }

  String err;
  String resp;
  if (!packFinalizeAndStageBoot(&err, &resp)) {
    sendJsonError(500, err.isEmpty() ? "pack flash failed" : err);
    packCleanup(true);
    return;
  }

  sendJson(200, resp);
  packCleanup(true);
  scheduleReboot("flash_pack_url_done", 900);
}

static void handleRoot() {
  String page;
  page.reserve(22000);
  page += F("<!doctype html><html><head><meta charset='utf-8'>");
  page += F("<meta name='viewport' content='width=device-width,initial-scale=1'>");
  page += F("<title>LB RECOVERY</title>");
  page += F("<style>"
            ":root{--bg1:#9499ad;--bg2:#6f7489;--card:#2f3443;--line:#4a5164;--soft:#9fb0c9;--fg:#e8f3ff;--cyan:#15d5ff;--dangerLine:#b8506b}"
            "body{margin:0;background:linear-gradient(160deg,var(--bg1),var(--bg2));color:var(--fg);font-family:Segoe UI,Arial,sans-serif;min-height:100vh}"
            ".w{max-width:980px;margin:18px auto;padding:0 12px;position:relative}"
            ".w:before{content:'';position:fixed;inset:0;pointer-events:none;background:radial-gradient(1200px 420px at 75% 18%,rgba(255,255,255,.18),transparent 60%),radial-gradient(900px 360px at 22% 64%,rgba(255,255,255,.11),transparent 62%)}"
            ".c{background:linear-gradient(180deg,rgba(38,45,60,.94),rgba(47,53,69,.92));border:1px solid var(--line);border-radius:14px;padding:14px;box-shadow:0 20px 40px rgba(8,12,20,.35)}"
            ".h{display:flex;justify-content:space-between;gap:8px;flex-wrap:wrap;align-items:center}"
            ".t{font-size:24px;font-weight:700}"
            ".m{color:var(--soft);font-size:13px}"
            ".r{display:flex;gap:8px;flex-wrap:wrap;align-items:center;margin-top:10px}"
            "button{background:linear-gradient(180deg,#101929,#0c1320);border:1px solid #3b557b;color:#e7f1ff;border-radius:10px;padding:9px 12px;cursor:pointer}"
            "button.primary{background:linear-gradient(180deg,#1b6fb0,#134b7b);border-color:#28b9ff;box-shadow:0 0 0 1px rgba(21,213,255,.22) inset}"
            "button.danger{background:#4b2230;border-color:#b8506b}"
            "button:disabled{opacity:.55;cursor:default}"
            ".k{margin-top:12px;padding:10px;border:1px solid #3f4b63;border-radius:10px;background:linear-gradient(180deg,#101a2a,#0f1724)}"
            ".k b{color:var(--cyan)}"
            ".prog{height:10px;border:1px solid #3b4d67;background:#0c1320;border-radius:999px;overflow:hidden}"
            ".prog>i{display:block;height:100%;width:0;background:linear-gradient(90deg,var(--cyan),#49ff9f)}"
            ".log{margin-top:10px;background:#0a1220;border:1px solid #334863;border-radius:10px;padding:10px;white-space:pre-wrap;max-height:220px;overflow:auto;font:12px/1.4 Consolas,monospace}"
            ".in{width:100%;box-sizing:border-box;background:#0c1320;border:1px solid #324969;color:#e7f1ff;border-radius:10px;padding:9px}"
            ".menu{position:fixed;inset:0;background:rgba(0,0,0,.55);display:none;align-items:center;justify-content:center;padding:14px}"
            ".menu.show{display:flex}"
            ".box{width:min(520px,100%);background:linear-gradient(180deg,#1a2233,#121b2a);border:1px solid #35507b;border-radius:14px;padding:14px}"
            ".s{font-size:12px;color:#a7bad5}"
            "</style></head><body>");
  page += F("<div class='w'><section class='c'>");
  page += F("<div class='h'><div><div class='t'>RECOVERY MODE</div><div class='m'>");
  page += F("Recovery: <code>");
  page += jsonEscape(kRecoveryVersion);
  page += F("</code> | LBPACK v");
  page += String((unsigned long)kPackVersion);
  page += F("</div></div><div class='m'>AP: ");
  page += kRecoverySsid;
  page += F(" | IP: <span id='ip'>-</span></div></div>");
  page += F("<div class='r'><button id='btnFw' class='primary'>Firmware Update</button><button id='btnBoot'>Boot Main Firmware</button><button id='btnGit'>Visit GitHub</button><button id='btnReset' class='danger'>Reset Main Settings</button></div>");
  page += F("<div class='k'><b>Status:</b> <span id='phase'>Ожидание</span><div class='prog' style='margin-top:8px'><i id='bar'></i></div><div id='detail' class='s' style='margin-top:7px'>0%</div></div>");
  page += F("<div class='k'><b>Flash package вручную</b><div class='s'>Файл <code>update.lbpack</code> (System + LittleFS + signatures)</div>");
  page += F("<input id='file' class='in' type='file' accept='.lbpack,application/octet-stream' style='margin-top:8px'/>");
  page += F("<div class='r'><button id='btnPack' class='primary'>Flash Package</button></div></div>");
  page += F("<div id='log' class='log'>[recovery] embedded ui ready</div>");
  page += F("</section></div>");
  page += F("<div id='menu' class='menu'><div class='box'><div style='font-size:18px;font-weight:700'>Firmware Update</div><div class='m'>Выберите способ обновления</div>"
            "<div class='r' style='margin-top:12px;justify-content:center'><button id='btnLatest' class='primary'>Online (Latest)</button><button id='btnChoose'>Choose Version</button><button id='btnCancel'>Cancel</button></div></div></div>");

  page += F("<script>"
            "const q=(id)=>document.getElementById(id);"
            "const el={ip:q('ip'),phase:q('phase'),detail:q('detail'),bar:q('bar'),log:q('log'),menu:q('menu'),btnFw:q('btnFw'),btnLatest:q('btnLatest'),btnChoose:q('btnChoose'),btnCancel:q('btnCancel'),btnBoot:q('btnBoot'),btnGit:q('btnGit'),btnReset:q('btnReset'),btnPack:q('btnPack'),file:q('file')};"
            "let busy=false;"
            "let autoPack='';"
            "const repo='Serjio193/legacy-bridge';"
            "const mirror='https://serjio193.github.io/legacy-bridge';"
            "function ts(){const d=new Date();const p=(n)=>String(n).padStart(2,'0');return `${d.getFullYear()}-${p(d.getMonth()+1)}-${p(d.getDate())} ${p(d.getHours())}:${p(d.getMinutes())}:${p(d.getSeconds())}`;}"
            "function log(s){el.log.textContent+=`\\n[ui] ${ts()} ${s}`;el.log.scrollTop=el.log.scrollHeight;}"
            "function setPhase(name,pct,detail){el.phase.textContent=name||'';el.bar.style.width=`${Math.max(0,Math.min(100,Number(pct||0))).toFixed(1)}%`;el.detail.textContent=detail||`${Math.round(Number(pct||0))}%`;}"
            "function setBusy(v){busy=!!v;[el.btnFw,el.btnLatest,el.btnChoose,el.btnCancel,el.btnBoot,el.btnGit,el.btnReset,el.btnPack].forEach(b=>{if(b)b.disabled=busy;});}"
            "async function j(url,opts){const o=Object.assign({},opts||{});o.headers=Object.assign({'Content-Type':'application/json'},o.headers||{});try{const r=await fetch(url,o);const t=await r.text();try{return JSON.parse(t||'{}')}catch(_){return {ok:false,error:t||'bad json'}}}catch(e){return {ok:false,error:String(e&&e.message?e.message:e)}}}"
            "function parseAuto(){try{const u=new URL(location.href);const p=String(u.searchParams.get('autopack')||'').trim();u.searchParams.delete('autopack');u.searchParams.delete('v');history.replaceState(null,'',u.pathname+(u.search||''));return p;}catch(_){return '';}}"
            "function toMirrorUrl(u){const s=String(u||'').trim();const m=s.match(/^https?:\\/\\/github\\.com\\/Serjio193\\/legacy-bridge\\/releases\\/download\\/([^\\/]+)\\/update\\.lbpack$/i);if(m&&m[1])return `${mirror}/releases/${encodeURIComponent(m[1])}/update.lbpack`;return s;}"
            "function pickLbpackAsset(rel){const a=Array.isArray(rel&&rel.assets)?rel.assets:[];return a.find(x=>String((x&&x.name)||'').toLowerCase()==='update.lbpack')||a.find(x=>String((x&&x.name)||'').toLowerCase().endsWith('.lbpack'))||null;}"
            "async function latestUrl(){try{const r=await fetch(`https://api.github.com/repos/${repo}/releases/latest`,{cache:'no-store'});if(r&&r.ok){const rel=await r.json();const a=pickLbpackAsset(rel);const u=String(a&&a.browser_download_url||'').trim();if(u)return toMirrorUrl(u);}}catch(_){} return toMirrorUrl(`https://github.com/${repo}/releases/latest/download/update.lbpack`);}"
            "async function listVersions(){const out=[];const seen=new Set();const add=(t,u,pre)=>{const tag=String(t||'').trim();const url=String(u||'').trim();if(!tag||!url||seen.has(tag))return;seen.add(tag);out.push({tag,url,pre:!!pre});}; const tagNum=(t)=>{const m=String(t||'').match(/v?(\\d+)/i);return m?parseInt(m[1],10):0;}; try{for(let page=1;page<=3;page++){const r=await fetch(`https://api.github.com/repos/${repo}/releases?per_page=100&page=${page}`,{cache:'no-store'});if(!r||!r.ok)break;const arr=await r.json();if(!Array.isArray(arr)||!arr.length)break;arr.forEach(rel=>{if(!rel||rel.draft)return;const t=String(rel.tag_name||'').trim();const a=pickLbpackAsset(rel);const u=String(a&&a.browser_download_url||'').trim();if(t&&u)add(t,toMirrorUrl(u),!!rel.prerelease);});}}catch(_){} try{const m=await fetch(`${mirror}/releases/index.json`,{cache:'no-store'});if(m&&m.ok){const arr=await m.json();(Array.isArray(arr)?arr:[]).forEach(rel=>{const t=String(rel&&rel.tag_name||'').trim();const a=rel&&rel.assets&&typeof rel.assets==='object'?rel.assets:{};const u=String(a['update.lbpack']||'').trim();if(t&&u)add(t,new URL(u,`${mirror}/`).href,!!(rel&&rel.prerelease));});}}catch(_){} out.sort((a,b)=>tagNum(b.tag)-tagNum(a.tag)); if(!out.length)throw new Error('github list failed'); return out;}"
            "function openMenu(){if(el.menu)el.menu.classList.add('show');}"
            "function closeMenu(){if(el.menu)el.menu.classList.remove('show');}"
            "function nextMainUrl(r){return String((r&&r.next_url)||'http://lb-bridge.local/');}"
            "async function flashUrl(url,label){const raw=String(url||'').trim();if(!raw){log(`${label}: empty url`);return;}const u=toMirrorUrl(raw);setBusy(true);setPhase('Загрузка',4,'инициализация');if(u!==raw)log(`${label}: using mirror ${u}`);else log(`${label}: ${u}`);const r=await j('/api/recovery/flash/url',{method:'POST',body:JSON.stringify({url:u})});if(r&&r.ok){setPhase('Перезагрузка',98,'переход в систему');log(`${label}: ok, rebooting`);log(`open main: ${nextMainUrl(r)}`);setTimeout(()=>{setPhase('Вход в систему',100,'открытие главной страницы');location.href=nextMainUrl(r);},8000);setTimeout(()=>{setBusy(false);},20000);return;}setBusy(false);setPhase('Ошибка',0,String((r&&r.error)||'unknown'));log(`${label} FAIL: ${(r&&r.error)||'unknown'}`);}"
            "async function refresh(){const s=await j('/api/status',{method:'GET'});if(!s||!s.ok)return;el.ip.textContent=String(s.ip||'-');const st=String(s.pack_stage||'');const pa=!!s.pack_active;const rec=Number(s.pack_received||0),tot=Number(s.pack_total||0),fwW=Number(s.pack_fw_written||0),fwT=Number(s.pack_fw_total||0),fsW=Number(s.pack_fs_written||0),fsT=Number(s.pack_fs_total||0);"
            "if(pa){if(st==='hdr'||st==='fw_sig'||st==='fs_sig'){const p=tot>0?(3+(rec/tot)*42):8;setPhase('Загрузка',Math.max(3,Math.min(45,p)),tot>0?`${Math.round((rec/tot)*100)}%`:`${rec} bytes`);}else if(st==='fw_data'||st==='fs_data'){const allT=Math.max(1,fwT+fsT);const p=45+((fwW+fsW)/allT)*50;setPhase('Обновление',Math.max(45,Math.min(95,p)),`fw ${fwW}/${fwT} | fs ${fsW}/${fsT}`);}else if(st==='done'){setPhase('Перезагрузка',98,'подготовка');}}"
            "}"
            "async function flashFile(){const f=el.file&&el.file.files&&el.file.files[0]?el.file.files[0]:null;if(!f){log('manual FAIL: choose update.lbpack');return;}setBusy(true);setPhase('Загрузка',2,'отправка файла');const x=new XMLHttpRequest();const fd=new FormData();fd.append('pack',f,f.name||'update.lbpack');x.upload.onprogress=(e)=>{if(e.lengthComputable){const p=Math.max(2,Math.min(40,(e.loaded/e.total)*40));setPhase('Загрузка',p,`${Math.round((e.loaded/e.total)*100)}%`);}};x.onload=()=>{let r={ok:false,error:'bad json'};try{r=JSON.parse(x.responseText||'{}')}catch(_){} if(r&&r.ok){setPhase('Перезагрузка',98,'переход в систему');log('manual OK: rebooting');log(`open main: ${nextMainUrl(r)}`);setTimeout(()=>{setPhase('Вход в систему',100,'открытие главной страницы');location.href=nextMainUrl(r);},8000);setTimeout(()=>setBusy(false),20000);}else{setBusy(false);setPhase('Ошибка',0,String((r&&r.error)||'upload failed'));log(`manual FAIL: ${(r&&r.error)||'unknown'}`);}};x.onerror=()=>{setBusy(false);setPhase('Ошибка',0,'network error');log('manual FAIL: network error');};x.open('POST','/api/recovery/flash/allpack',true);x.send(fd);}"
            "el.btnFw.onclick=()=>openMenu(); el.btnCancel.onclick=()=>closeMenu(); el.menu.onclick=(e)=>{if(e.target===el.menu)closeMenu();};"
            "el.btnLatest.onclick=async()=>{closeMenu();const u=autoPack||await latestUrl();autoPack='';await flashUrl(u,'ONLINE UPDATE');};"
            "el.btnChoose.onclick=async()=>{closeMenu();try{const list=await listVersions();if(!list.length){log('choose FAIL: no versions');return;}const lines=list.map((x,i)=>`${i+1}) ${x.tag}${x.pre?' [pre]':''}`);const raw=String(prompt(`Select version:\\n${lines.join('\\n')}\\n\\nEnter number:`,'1')||'').trim();if(!raw)return;const idx=(parseInt(raw,10)||0)-1;if(idx<0||idx>=list.length){log('choose canceled: bad index');return;}await flashUrl(list[idx].url,`ONLINE UPDATE ${list[idx].tag}`);}catch(e){log(`choose FAIL: ${String(e&&e.message?e.message:e)}`);}};"
            "el.btnBoot.onclick=async()=>{setBusy(true);setPhase('Перезагрузка',96,'запрос boot main');const r=await j('/api/recovery/boot_main',{method:'POST',body:'{}'});if(r&&r.ok){log(`open main: ${nextMainUrl(r)}`);setTimeout(()=>{setPhase('Вход в систему',100,'открытие главной страницы');location.href=nextMainUrl(r);},7000);setTimeout(()=>setBusy(false),18000);return;}setBusy(false);setPhase('Ошибка',0,String((r&&r.error)||'boot main failed'));};"
            "el.btnGit.onclick=()=>{const u=`https://github.com/${repo}/`;try{window.open(u,'_blank','noopener,noreferrer');}catch(_){location.href=u;}log(`open github: ${u}`);};"
            "el.btnReset.onclick=async()=>{if(!confirm('Reset main settings?'))return;const r=await j('/api/recovery/reset_settings',{method:'POST',body:'{}'});log(r&&r.ok?'reset OK':`reset FAIL: ${(r&&r.error)||'unknown'}`);};"
            "el.btnPack.onclick=()=>flashFile();"
            "autoPack=parseAuto(); if(autoPack){(async()=>{let u=String(autoPack||'').trim();if(u.toLowerCase()==='latest'){log('auto update: latest package requested');u=await latestUrl();}log('auto update: package url detected');await flashUrl(u,'AUTO UPDATE');autoPack='';})();}"
            "refresh(); setInterval(refresh,1200);"
            "</script>");
  page += F("</body></html>");
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.send(200, "text/html", page);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  // First boot behavior:
  // If the bootloader fell back to the factory(recovery) app due to empty/invalid OTA-data,
  // but a valid main image exists in OTA_A/OTA_B, switch boot to that slot and reboot.
  // We do NOT auto-exit if recovery was requested explicitly from the main firmware.
  const esp_partition_t *running = esp_ota_get_running_partition();
  const esp_partition_t *boot = esp_ota_get_boot_partition();
  bool forceRecovery = getForceRecovery();
  bool requested = forceRecovery && boot && boot->type == ESP_PARTITION_TYPE_APP &&
                   boot->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY;
  gRequested = requested;
  // Make the flag one-shot so a stale NVS value can't permanently lock the device into recovery.
  if (forceRecovery) setForceRecovery(false);
  if (running && running->type == ESP_PARTITION_TYPE_APP &&
      running->subtype == ESP_PARTITION_SUBTYPE_APP_FACTORY &&
      !requested) {
    String which;
    const esp_partition_t *t = pickBootableMainSlot(&which);
    if (t) {
      gAutoExitAttempted = true;
      gAutoExitTarget = which;
      esp_err_t e = esp_ota_set_boot_partition(t);
      if (e == ESP_OK) {
        setForceRecovery(false);
        Serial.print("Auto-exit recovery: boot -> ");
        Serial.println(which);
        delay(150);
        ESP.restart();
      } else {
        gAutoExitErr = String("esp_ota_set_boot_partition=") + String((int)e);
        Serial.printf("Auto-exit recovery failed: esp_ota_set_boot_partition=%d\n", (int)e);
      }
    } else {
      gAutoExitAttempted = true;
      gAutoExitErr = "no valid OTA slot found";
      Serial.println("Auto-exit recovery skipped: no valid OTA slot found");
    }
  }

  startRecoveryNetwork();

  if (!LittleFS.begin(true)) {
    Serial.println("LittleFS: mount failed (recovery)");
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/recovery", HTTP_GET, handleRoot);
  server.on("/recovery/", HTTP_GET, handleRoot);
  server.on("/recovery.html", HTTP_GET, handleRoot);
  server.on("/api/status", HTTP_GET, handleStatus);

  server.on("/api/recovery/reset_settings", HTTP_POST, handleResetSettings);
  server.on("/api/recovery/reboot", HTTP_POST, handleRebootRecovery);
  server.on("/api/recovery/boot_main", HTTP_POST, handleBootMain);

  server.on(
      "/api/recovery/flash/a", HTTP_POST, handleRawFlashDisabled,
      []() { uploadHandlerDiscardRawFlash(); });
  server.on(
      "/api/recovery/flash/fs", HTTP_POST, handleRawFlashDisabled,
      []() { uploadHandlerDiscardRawFlash(); });
  server.on(
      "/api/recovery/flash/allpack", HTTP_POST, handleFlashAllPackPost,
      []() { uploadHandlerPack(); });
  server.on("/api/recovery/flash/url", HTTP_POST, handleFlashPackByUrl);

  server.onNotFound([]() {
    if (server.uri().startsWith("/api/")) {
      sendJsonError(404, "not found");
      return;
    }
    handleRoot();
  });
  server.begin();

  Serial.print("Recovery active IP: ");
  Serial.println(currentRecoveryIp());
  Serial.print("Recovery AP IP: ");
  Serial.println(WiFi.softAPIP());
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("Recovery STA IP: ");
    Serial.println(WiFi.localIP());
  }
}

void loop() {
  server.handleClient();
  if (gRebootScheduled && (int32_t)(millis() - gRebootAtMs) >= 0) {
    Serial.printf("Recovery scheduled reboot: %s\n", gRebootReason.c_str());
    delay(30);
    ESP.restart();
  }
  delay(2);
}

