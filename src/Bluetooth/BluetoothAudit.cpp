/*
 * SAFE LIVE DEMO CANDIDATE.
 *
 * Pair Audit is a passive visibility audit. It scans for nearby advertised
 * Bluetooth/BLE devices and summarizes what is publicly visible. It does not
 * prove real pairing status and does not attempt pairing.
 *
 * Classroom demo steps:
 *   1. Turn on Bluetooth on your own phone.
 *   2. Open Bluetooth settings or a BLE scanner app to make visibility easier.
 *   3. Select Pair Audit from the ESP32 Bluetooth/PAN menu.
 *   4. Open a detected item to show RSSI/type/metadata.
 *
 * Expected outcome:
 *   - OLED shows visible devices and simple metadata.
 *   - Serial Monitor prints the full audit list.
 *
 * TODO / limitation:
 *   - Rename to Visibility Audit in future versions to avoid implying true
 *     paired-device verification.
 */

#include "BluetoothAudit.h"
#include "BluetoothReportUI.h"
#include "BluetoothScan.h"
#include "BluetoothState.h"

#include "../../AppState.h"

#include <Arduino.h>

// ==================================================
// ACTIVITY 2: PAIR AUDIT
// ==================================================
//
// SAFE BEHAVIOR:
//   Passive available-device audit only.
//   It scans visible Classic/BLE devices and shows public discovery data:
//   type, RSSI, name if advertised, address, and some BLE advertisement metadata.
//
// IMPORTANT LIMITATION:
//   A normal scan cannot prove whether a nearby device is paired with
//   another device, pairable, vulnerable, or authenticated.
//
//   This audit does not initiate pairing, connect to targets, bypass auth,
//   spam pairing prompts, or interfere with other devices.

void drawPairAuditPhaseScreen(const char* line1, const char* line2, const char* line3) {
  renderBluetoothOperationStage(
    "PAIR AUDIT",
    line1,
    line2,
    line3
  );
}

void bluetoothPairAuditResult() {
  bluetoothReportMode = BT_REPORT_AUDIT;

  copyCleanText(bluetoothReportTitle, "PAIR AUDIT", REPORT_TITLE_LEN);

  currentScreen = SCREEN_BT_SNIFF;
  selectedIndex = 0;
  drawCurrentScreen();
}

void bluetoothPairAuditAfterInput(const char* inputText) {
  // Kept for compatibility with your header / older keyboard flow.
  // This safe version ignores input and audits every visible device.
  (void)inputText;

  Serial.println();
  Serial.println("===== PAIR AUDIT PASSIVE SCAN START =====");
  Serial.println("Mode: all visible devices, no connect, no pair");

  drawPairAuditPhaseScreen("PASSIVE SCAN", "ALL VISIBLE", "NO CONNECT");
  delay(600);

  btDeviceCount = 0;

  scanClassicBluetooth();
  scanBLEBluetooth();

  Serial.print("Visible devices: ");
  Serial.println(btDeviceCount);

  for (int i = 0; i < btDeviceCount; i++) {
    Serial.print(i + 1);
    Serial.print(". ");
    Serial.print(btDeviceTypes[i]);
    Serial.print(" | ");
    Serial.print(btDeviceLabels[i]);
    Serial.print(" | ");
    Serial.print(btDeviceAddresses[i]);

    if (btDeviceHasRSSI[i]) {
      Serial.print(" | RSSI: ");
      Serial.print(btDeviceRSSI[i]);
    }

    if (stringsEqualIgnoreCase(btDeviceTypes[i], "BLE")) {
      Serial.print(" | SVC UUIDS: ");
      Serial.print(btBleServiceUUIDCount[i]);
      Serial.print(" | SVC DATA: ");
      Serial.print(btBleServiceDataCount[i]);
      Serial.print(" | MFR LEN: ");
      Serial.print(btBleManufacturerDataLen[i]);
      Serial.print(" | PAYLOAD LEN: ");
      Serial.print(btBlePayloadLen[i]);
      Serial.print(" | ADDR TYPE: ");
      Serial.print(btBleAddressType[i]);

      if (btBleHasTxPower[i]) {
        Serial.print(" | TX: ");
        Serial.print(btBleTxPower[i]);
      }

      if (btBleHasAppearance[i]) {
        Serial.print(" | APP: ");
        Serial.print(btBleAppearance[i]);
      }
    }

    Serial.println();
  }

  Serial.println("===== PAIR AUDIT PASSIVE SCAN END =====");

  bluetoothPairAuditResult();
}

void bluetoothPairAuditAction() {
  // No input prompt. The audit immediately scans all visible devices.
  bluetoothPairAuditAfterInput("");
}
