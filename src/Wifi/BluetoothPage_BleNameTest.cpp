/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 *
 * This page rapidly changes BLE advertising names. It is lower-power than the
 * nRF24 RF stress pages, but it can clutter other people's BLE scanner apps and
 * may look unprofessional in a classroom. Keep it for code explanation, or run
 * only with permission on your own phone/scanner.
 *
 * Recommended viva use:
 *   1. Use BLE Beacon for the clean live demo.
 *   2. Explain this file as a UI/advertisement-rotation experiment.
 *   3. Do not use offensive/scary names during a viva.
 *
 * Expected outcome if run only with permission:
 *   - A BLE scanner app may show changing advertised names from the ESP32.
 *   - OLED shows current name and remaining time.
 *
 * TODO / limitation:
 *   - Replace the public name pool with neutral lab names.
 *   - Add a user-confirmation screen before starting rapid advertising.
 */

// BluetoothPage_BleNameTest.cpp

// BluetoothPage_BleNameTest.cpp

// BluetoothPage_BleNameTest.cpp

#include "BluetoothPage_BleNameTest.h"
#include "BluetoothReportUI.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>

static BLEServer* pServer = nullptr;
static BLEAdvertising* pAdvertising = nullptr;
static bool isRunning = false;

#define NAME_SPAM_DURATION_MS   35000     // 35 seconds
#define ROTATE_EVERY_MS         450       // Change name every 450ms

const char* namePool[] = {
  "Free WiFi", "AirPods Pro", "Galaxy Watch", "iPhone 16", "Printer-Setup",
  "Tesla Model Y", "Smart Bulb", "LEAKED DATA", "Pentest Tool", "Hidden Cam",
  "Android TV", "ESP32_Pwn", "HackMePlz", "Surveillance", "COVID-TRACER",
  "Bluetooth Bomb", "NSA Listener", "FBI Van #42", "Ghost Device"
};

#define NAME_COUNT (sizeof(namePool) / sizeof(namePool[0]))

int currentName = 0;
unsigned long lastRotate = 0;

// ==================================================
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("✅ Device Connected!");
  }
  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device Disconnected. Restarting advertising...");
    pAdvertising->start();
  }
};

// ==================================================
// ==================================================
void initBLENameSpam() {
  if (isRunning) return;

  Serial.println("Initializing Connectable BLE Name Spam...");

  BLEDevice::init("");   
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create dummy service
  BLEService *pService = pServer->createService(BLEUUID((uint16_t)0x180A));

  pAdvertising = BLEDevice::getAdvertising();
  
  // FIXED LINE BELOW
  pAdvertising->addServiceUUID(BLEUUID((uint16_t)0x180A));

  pAdvertising->setScanResponse(true);

  isRunning = true;
}

// ==================================================
void advertiseCurrentName() {
  if (pAdvertising == nullptr) return;

  pAdvertising->stop();

  String name = namePool[currentName];
  
  BLEAdvertisementData advData;
  BLEAdvertisementData scanData;

  advData.setFlags(0x06);                    // General discoverable + BLE only
  advData.setName(name.c_str());
  scanData.setName(name.c_str());

  pAdvertising->setAdvertisementData(advData);
  pAdvertising->setScanResponseData(scanData);

  pAdvertising->start();

  Serial.print("Advertising as: ");
  Serial.println(name);
}

// ==================================================
void bluetoothBleNameTestAction() {
  renderBluetoothOperationStage("BLE NAME TEST", "INITIALIZING", "BLE STACK", "READY");
  delay(1000);

  initBLENameSpam();

  renderBluetoothOperationStage("BLE NAME TEST", "SPAMMING", "CONNECTABLE", "ACTIVE");
  delay(1000);

  currentName = 0;
  lastRotate = millis();

  advertiseCurrentName();

  unsigned long startTime = millis();

  while (millis() - startTime < NAME_SPAM_DURATION_MS) {
    if (millis() - lastRotate >= ROTATE_EVERY_MS) {
      currentName = (currentName + 1) % NAME_COUNT;
      advertiseCurrentName();
      lastRotate = millis();
    }

    int remaining = (NAME_SPAM_DURATION_MS - (millis() - startTime)) / 1000;
    renderBluetoothOperationStage("BLE NAME TEST", 
                                  "BROADCASTING", 
                                  namePool[currentName], 
                                  String(remaining) + "s");

    delay(100);
  }

  if (pAdvertising) pAdvertising->stop();
  
  renderBluetoothOperationStage("BLE NAME TEST", "STOPPED", "NAME SPAM", "FINISHED");
  delay(1200);
}

// ==================================================
void bluetoothBleNameTestResult() {
  renderBluetoothOperationResult("BLE NAME TEST");
}