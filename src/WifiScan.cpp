/*
 * FILE REPORT: Wi-Fi scan page
 * EXAM CATEGORY: PASSIVE / SAFE LIVE DEMO CANDIDATE
 *
 * What this file does:
 *   - Places ESP32 in station mode and scans nearby Wi-Fi networks.
 *   - Stores SSID/BSSID/RSSI/channel/security details in fixed-size arrays.
 *   - Renders a scrollable OLED list with RESCAN and BACK options.
 *
 * Expected outcome when run:
 *   - OLED first shows SCANNING, then a list of detected networks.
 *   - Selecting a network shows SSID, RSSI/channel/security, and BSSID.
 *   - Serial Monitor prints each detected network line.
 *
 * Viva explanation:
 *   - Hidden SSIDs or weak networks may appear differently depending on environment.
 *   - Future improvement: sort results by RSSI or channel for easier explanation.
 *   - Future improvement: show total network count on result screen.
 *
 * TODO / improvement notes:
 *   - This is a passive discovery test: it listens for Wi-Fi beacons and does not connect to or attack networks.
 *   - Good first demo because nearby networks appear clearly and quickly.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#include "WifiScan.h"
#include "WifiState.h"

#include "../../AppState.h"
#include "../../Menu3_Wifi.h"
#include "WifiDisplayCompat.h"

#include <Arduino.h>
#include <WiFi.h>
#include <stdio.h>

namespace {
const int kVisibleRows = 3;
const int kRowHeight = 12;

void resetWifiScanStorage() {
  wifiNetworkCount = 0;
  for (int i = 0; i < MAX_WIFI_NETWORKS; ++i) {
    wifiLabels[i][0] = '\0';
    wifiBssids[i][0] = '\0';
    wifiInfo[i][0] = '\0';
  }
}
}

void drawWifiScanningScreen() {
  clearBuffer();
  drawTopBar("WIFI SCAN");
  drawText(6, 20, "SCANNING...", 1, true);
  drawText(6, 34, "PLEASE WAIT", 1, true);
  drawText(6, 48, "SSID SEARCH", 1, true);
  sendBuffer();
}

void wifiScanAction() {
  resetWifiScanStorage();
  drawWifiScanningScreen();

  Serial.println();
  Serial.println("===== WIFI SCAN START =====");

  // Station mode is required for passive scanning; this does not join a network.
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(false);
  delay(150);

  // false = synchronous scan; true = include hidden networks where ESP32 can see them.
  int foundCount = WiFi.scanNetworks(false, true);

  Serial.print("WiFi networks found: ");
  Serial.println(foundCount);

  if (foundCount < 0) {
    wifiCopyCleanText(wifiLabels[0], "SCAN FAILED", WIFI_LABEL_LEN);
    wifiCopyCleanText(wifiInfo[0], "TRY RESCAN", WIFI_INFO_LEN);
    wifiCopyCleanText(wifiBssids[0], "", WIFI_BSSID_LEN);
    wifiNetworkCount = 1;
    WiFi.scanDelete();
    return;
  }

  for (int i = 0; i < foundCount && wifiNetworkCount < MAX_WIFI_NETWORKS; ++i) {
    String ssid = WiFi.SSID(i);
    String bssid = WiFi.BSSIDstr(i);
    int rssi = WiFi.RSSI(i);
    int channel = WiFi.channel(i);
    wifi_auth_mode_t encType = WiFi.encryptionType(i);

    if (ssid.length() > 0) {
      wifiCopyCleanText(wifiLabels[wifiNetworkCount], ssid.c_str(), WIFI_LABEL_LEN);
    } else if (bssid.length() > 0) {
      wifiCopyCleanText(wifiLabels[wifiNetworkCount], bssid.c_str(), WIFI_LABEL_LEN);
    } else {
      wifiCopyCleanText(wifiLabels[wifiNetworkCount], "HIDDEN NETWORK", WIFI_LABEL_LEN);
    }

    wifiCopyCleanText(wifiBssids[wifiNetworkCount], bssid.c_str(), WIFI_BSSID_LEN);

    char infoLine[WIFI_INFO_LEN];
    snprintf(infoLine, sizeof(infoLine), "%ddBm CH%d %s", rssi, channel, wifiEncryptionName(encType));
    wifiCopyCleanText(wifiInfo[wifiNetworkCount], infoLine, WIFI_INFO_LEN);

    Serial.print("WiFi: ");
    Serial.print(wifiLabels[wifiNetworkCount]);
    Serial.print(" | ");
    Serial.print(wifiBssids[wifiNetworkCount]);
    Serial.print(" | ");
    Serial.println(wifiInfo[wifiNetworkCount]);

    ++wifiNetworkCount;
  }

  WiFi.scanDelete();
}

void wifiScanResult() {
  currentScreen = SCREEN_WIFI_SCAN;
  selectedIndex = 0;
  drawCurrentScreen();
}

void scanWifiNetworks() {
  runWifiActivity(wifiScanAction, wifiScanResult);
}

int wifiScanResultCount() {
  return wifiNetworkCount + 2;
}

const char* wifiScanResultItemText(int index) {
  if (index >= 0 && index < wifiNetworkCount) {
    return wifiLabels[index];
  }
  if (index == wifiNetworkCount) {
    return "RESCAN";
  }
  return "BACK";
}

void wifiScanRenderResult() {
  drawTopBar("WIFI SCAN");

  if (wifiNetworkCount == 0) {
    drawText(6, 16, "NO NETWORKS", 1, true);
  }

  const int totalItems = wifiScanResultCount();
  if (selectedIndex < 0) selectedIndex = 0;
  if (selectedIndex >= totalItems) selectedIndex = totalItems - 1;

  int startIndex = 0;
  if (selectedIndex >= kVisibleRows) {
    startIndex = selectedIndex - kVisibleRows + 1;
  }

  int startY = (wifiNetworkCount == 0) ? 30 : 20;

  for (int row = 0; row < kVisibleRows; ++row) {
    int itemIndex = startIndex + row;
    if (itemIndex >= totalItems) break;

    int y = startY + row * kRowHeight;
    const char* itemText = wifiScanResultItemText(itemIndex);

    if (itemIndex == selectedIndex) {
      fillRect(0, y - 2, 128, 11, true);
      drawText(6, y, itemText, 1, false);
    } else {
      drawText(6, y, itemText, 1, true);
    }
  }

  drawText(0, 56, "UP/DN FIRE OK", 1, true);
}

void wifiScanSelectResult() {
  if (selectedIndex >= 0 && selectedIndex < wifiNetworkCount) {
    showMessage(
      "WIFI FOUND",
      wifiLabels[selectedIndex],
      wifiInfo[selectedIndex],
      wifiBssids[selectedIndex]
    );
    return;
  }

  if (selectedIndex == wifiNetworkCount) {
    runWifiActivity(wifiScanAction, wifiScanResult);
    return;
  }

  currentScreen = SCREEN_WIFI;
  selectedIndex = 0;
  drawCurrentScreen();
}

int getWifiScanMenuCount() {
  return wifiScanResultCount();
}

void drawWifiScanMenu() {
  wifiScanRenderResult();
}

void selectWifiScanMenuItem() {
  wifiScanSelectResult();
}
