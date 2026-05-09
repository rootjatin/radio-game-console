/*
 * SAFE LIVE DEMO CANDIDATE WITH CONSENT.
 *
 * This page advertises one BLE beacon from your own ESP32. It is suitable for a
 * classroom demo when you use your own phone/BLE scanner and keep the duration
 * short.
 *
 * Classroom demo steps:
 *   1. Install/open a BLE scanner app on your own phone.
 *   2. Turn on phone Bluetooth and allow nearby-device permission.
 *   3. On the ESP32 menu, select BLE Beacon.
 *   4. Watch for the beacon/name/UUID in the scanner app.
 *   5. Wait for the ESP32 to stop advertising automatically.
 *
 * Expected outcome:
 *   - OLED shows advertising countdown.
 *   - Phone BLE scanner shows the ESP32 advertisement/beacon data.
 *
 * TODO / limitation:
 *   - Add a screen that prints the exact UUID/major/minor before starting so it
 *     is easier to verify during viva.
 */

// BluetoothPage_BleBeacon.cpp

#include "BluetoothPage_BleBeacon.h"
#include "BluetoothReportUI.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>
#include <BLEBeacon.h>

static BLEAdvertising* pAdvertising = nullptr;
static bool bleRunning = false;

// ==================================================
// Configurable Beacon Settings
// ==================================================
#define BEACON_DURATION_MS     15000     // 15 seconds (change as needed)
#define BEACON_NAME            "Pentest Beacon"
#define BEACON_UUID            "e2c56db5-dfeb-48d2-b060-d0f5a71096e0"  // Sample UUID

// ==================================================
void initBLEBeacon() {
  if (bleRunning) return;

  Serial.println("Initializing BLE Beacon...");

  BLEDevice::init(BEACON_NAME);
  BLEServer* pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();

  // Set advertising parameters
  pAdvertising->setMinInterval(100);   // 100ms
  pAdvertising->setMaxInterval(200);   // 200ms

  // Create iBeacon style payload (very common and visible)
  BLEBeacon oBeacon;
  oBeacon.setManufacturerId(0x4C00);        // Apple Company ID
  oBeacon.setProximityUUID(BLEUUID(BEACON_UUID));
  oBeacon.setMajor(0x1234);
  oBeacon.setMinor(0x5678);
  oBeacon.setSignalPower(0xC5);             // -59 dBm

  BLEAdvertisementData advertisementData;
  advertisementData.setFlags(0x04);         // BR/EDR Not Supported
  advertisementData.setManufacturerData(oBeacon.getData());

  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->setScanResponseData(advertisementData);

  bleRunning = true;
}

// ==================================================
void stopBLEBeacon() {
  if (pAdvertising) {
    pAdvertising->stop();
  }
  // BLEDevice::deinit();  // Optional - can cause issues if called too often
  bleRunning = false;
  Serial.println("BLE Beacon stopped.");
}

// ==================================================
void bluetoothBleBeaconAction() {
  renderBluetoothOperationStage("BLE BEACON", "INITIALIZING", "BLE STACK", "STARTING");
  delay(600);

  initBLEBeacon();

  renderBluetoothOperationStage("BLE BEACON", "ADVERTISING", "ACTIVE", "SPAMMING");
  delay(800);

  // Start advertising only if initialization succeeded.
  if (pAdvertising == nullptr) {
    renderBluetoothOperationStage("BLE BEACON", "ERROR", "NO ADVERTISER", "CHECK BLE");
    delay(1200);
    return;
  }

  pAdvertising->start();

  // Show live status during the test
  unsigned long startTime = millis();
  while (millis() - startTime < BEACON_DURATION_MS) {
    int remaining = (BEACON_DURATION_MS - (millis() - startTime)) / 1000;
    
    renderBluetoothOperationStage("BLE BEACON", 
                                  "BROADCASTING", 
                                  "ON AIR", 
                                  String(remaining) + "s remaining");

    delay(450);
  }

  renderBluetoothOperationStage("BLE BEACON", "STOPPING", "ADVERTISEMENT", "CLEANUP");
  stopBLEBeacon();
  delay(800);
}

// ==================================================
void bluetoothBleBeaconResult() {
  renderBluetoothOperationResult("BLE BEACON");
  Serial.println("=== BLE Beacon Test Completed ===");
}