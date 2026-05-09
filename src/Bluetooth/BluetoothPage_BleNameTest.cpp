// BluetoothPage_BleNameTest.cpp

#include "BluetoothPage_BleNameTest.h"
#include "BluetoothReportUI.h"
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEAdvertising.h>

static BLEAdvertising* pAdvertising = nullptr;
static bool bleRunning = false;

// ==================================================
// Configuration
// ==================================================
#define NAME_SPAM_DURATION_MS   20000     // 20 seconds total
#define NAME_CHANGE_INTERVAL    280       // Change name every ~280ms (very noticeable)

// Pool of funny / testing names
const char* namePool[] = {
  "Free WiFi", "AirPods Pro", "Galaxy Watch", "iPhone 16", "Printer-Setup",
  "Tesla Model Y", "Smart Bulb", "LEAKED DATA", "Pentest Tool", "Hidden Cam",
  "Android TV", "ESP32_Pwn", "HackMePlz", "Surveillance", "COVID-TRACER",
  "Bluetooth Bomb", "NSA Listener", "FBI Van #42", "_EMPTY_", "Ghost Device"
};

#define NAME_COUNT (sizeof(namePool) / sizeof(namePool[0]))

// ==================================================
void initBLENameSpam() {
  if (bleRunning) return;

  Serial.println("Initializing BLE Name Spam...");

  BLEDevice::init("");  // Start with empty name
  BLEServer* pServer = BLEDevice::createServer();
  pAdvertising = pServer->getAdvertising();

  pAdvertising->setMinInterval(80);
  pAdvertising->setMaxInterval(120);
  pAdvertising->setScanResponse(true);

  bleRunning = true;
}

// ==================================================
void stopBLENameSpam() {
  if (pAdvertising) {
    pAdvertising->stop();
  }
  bleRunning = false;
  Serial.println("BLE Name Spam stopped.");
}

// ==================================================
void bluetoothBleNameTestAction() {
  renderBluetoothOperationStage("BLE NAME TEST", "INITIALIZING", "BLE STACK", "READY");
  delay(600);

  initBLENameSpam();

  renderBluetoothOperationStage("BLE NAME TEST", "SPAMMING", "NAMES", "ACTIVE");
  delay(800);

  pAdvertising->start();

  unsigned long startTime = millis();
  int nameIndex = 0;

  while (millis() - startTime < NAME_SPAM_DURATION_MS) {
    // Change device name rapidly
    String currentName = namePool[nameIndex];
    
    BLEAdvertisementData advertisementData;
    advertisementData.setName(currentName.c_str());
    
    pAdvertising->setAdvertisementData(advertisementData);
    pAdvertising->start();   // Restart advertising with new name

    // Update UI
    int remaining = (NAME_SPAM_DURATION_MS - (millis() - startTime)) / 1000;
    
    renderBluetoothOperationStage("BLE NAME TEST", 
                                  "BROADCASTING", 
                                  currentName, 
                                  String(remaining) + "s left");

    nameIndex = (nameIndex + 1) % NAME_COUNT;
    
    delay(NAME_CHANGE_INTERVAL);
  }

  renderBluetoothOperationStage("BLE NAME TEST", "STOPPING", "ADVERTISEMENT", "CLEANUP");
  stopBLENameSpam();
  delay(800);
}

// ==================================================
void bluetoothBleNameTestResult() {
  renderBluetoothOperationResult("BLE NAME TEST");
  Serial.println("=== BLE Name Spam Test Completed ===");
}

// Compatibility wrappers
void bluetoothBleNameSpamAction() {
  bluetoothBleNameTestAction();
}

void bluetoothBleNameSpamResult() {
  bluetoothBleNameTestResult();
}