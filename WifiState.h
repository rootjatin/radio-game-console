/*
 * FILE REPORT: Shared Wi-Fi state declarations
 * EXAM CATEGORY: CORE HELPER FILE — KEEP
 *
 * What this file does:
 *   - Defines buffer sizes and exposes scan result arrays to menu/display code.
 *   - Declares text-cleaning and encryption-name helper functions.
 *
 * Expected outcome when run:
 *   - No standalone output; required by scan/audit/status pages.
 *
 * Viva explanation:
 *   - If OLED layout changes, update WIFI_LABEL_LEN/WIFI_INFO_LEN carefully.
 *   - Large buffers can reduce free heap on ESP32, so keep limits practical.
 *
 * TODO / improvement notes:
 *   - This is the shared data contract for Wi-Fi results shown in the UI.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#ifndef WIFI_STATE_H
#define WIFI_STATE_H

#include <Arduino.h>
#include <WiFi.h>

/**
 * Shared Wi-Fi module state and text helpers.
 *
 * This file intentionally keeps only small fixed-size buffers that are safe for
 * the OLED menu. Every page should copy untrusted/user/device text through
 * wifiCopyCleanText() before rendering it.
 */

// OLED/menu display limits. Keep these sizes aligned with the 128x64 UI layout.
#define MAX_WIFI_NETWORKS   18
#define WIFI_LABEL_LEN      32
#define WIFI_BSSID_LEN      18
#define WIFI_INFO_LEN       32

extern char wifiLabels[MAX_WIFI_NETWORKS][WIFI_LABEL_LEN];
extern char wifiBssids[MAX_WIFI_NETWORKS][WIFI_BSSID_LEN];
extern char wifiInfo[MAX_WIFI_NETWORKS][WIFI_INFO_LEN];
extern int wifiNetworkCount;

bool wifiAllowedOLEDChar(char c);
void wifiCopyCleanText(char* dest, const char* src, int maxLen);
const char* wifiEncryptionName(wifi_auth_mode_t encType);

#endif
