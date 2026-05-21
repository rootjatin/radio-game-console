/*
 * FILE REPORT: Shared Wi-Fi state and text-cleaning helpers
 * EXAM CATEGORY: CORE HELPER FILE — KEEP
 *
 * What this file does:
 *   - Stores scan results in fixed arrays for menu display.
 *   - Filters unsafe/unsupported characters before text is drawn on OLED.
 *   - Converts ESP32 Wi-Fi encryption enum values into readable strings.
 *
 * Expected outcome when run:
 *   - Used indirectly by scan/status/input pages; no standalone screen.
 *   - Serial/OLED text becomes cleaner and less likely to contain unreadable characters.
 *
 * Viva explanation:
 *   - Future improvement: add pagination for more than MAX_WIFI_NETWORKS results.
 *   - Future improvement: include WPA3/enterprise handling only when supported by installed ESP32 core.
 *
 * TODO / improvement notes:
 *   - Fixed-size buffers are used for ESP32 reliability and predictable RAM usage.
 *   - Sanitizing SSID/BSSID text prevents display glitches during viva.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#include "WifiState.h"

#include <Arduino.h>
#include <string.h>

/**
 * Storage for scan results shown by WifiScan.cpp.
 *
 * The arrays are fixed-size by design: the OLED menu shows a short list and the
 * ESP32 keeps memory more predictable when we avoid heap-heavy UI state.
 */
char wifiLabels[MAX_WIFI_NETWORKS][WIFI_LABEL_LEN];
char wifiBssids[MAX_WIFI_NETWORKS][WIFI_BSSID_LEN];
char wifiInfo[MAX_WIFI_NETWORKS][WIFI_INFO_LEN];
int wifiNetworkCount = 0;

bool wifiAllowedOLEDChar(char c) {
  if ((c >= 'A' && c <= 'Z') ||
      (c >= 'a' && c <= 'z') ||
      (c >= '0' && c <= '9')) {
    return true;
  }

  switch (c) {
    case ' ':
    case '-':
    case ':':
    case '.':
    case '/':
    case '_':
    case '(':
    case ')':
    case '[':
    case ']':
    case '#':
    case '+':
      return true;
    default:
      return false;
  }
}

void wifiCopyCleanText(char* dest, const char* src, int maxLen) {
  if (dest == nullptr || maxLen <= 0) {
    return;
  }

  dest[0] = '\0';
  if (src == nullptr) {
    return;
  }

  int out = 0;
  for (int in = 0; src[in] != '\0' && out < maxLen - 1; ++in) {
    char c = src[in];
    dest[out++] = wifiAllowedOLEDChar(c) ? c : ' ';
  }
  dest[out] = '\0';
}

const char* wifiEncryptionName(wifi_auth_mode_t encType) {
  switch (encType) {
    case WIFI_AUTH_OPEN:         return "OPEN";
    case WIFI_AUTH_WEP:          return "WEP";
    case WIFI_AUTH_WPA_PSK:      return "WPA";
    case WIFI_AUTH_WPA2_PSK:     return "WPA2";
    case WIFI_AUTH_WPA_WPA2_PSK: return "WPA/WPA2";
#ifdef WIFI_AUTH_WPA3_PSK
    case WIFI_AUTH_WPA3_PSK:     return "WPA3";
#endif
#ifdef WIFI_AUTH_WPA2_WPA3_PSK
    case WIFI_AUTH_WPA2_WPA3_PSK:return "WPA2/WPA3";
#endif
    case WIFI_AUTH_WPA2_ENTERPRISE:
      return "ENTERPRISE";
#ifdef WIFI_AUTH_WPA3_ENTERPRISE
    case WIFI_AUTH_WPA3_ENTERPRISE:
      return "ENTERPRISE";
#endif
#ifdef WIFI_AUTH_WAPI_PSK
    case WIFI_AUTH_WAPI_PSK:
      return "WAPI";
#endif
    default:
      return "SECURED";
  }
}
