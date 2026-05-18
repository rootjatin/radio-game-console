/*
 * SAFE LIVE DEMO CANDIDATE.
 *
 * Bluetooth Scan/Sniff passively lists visible Classic Bluetooth and BLE
 * advertisements. It does not connect, pair, authenticate, or modify any target
 * device.
 *
 * Classroom demo steps:
 *   1. Turn on the ESP32 device.
 *   2. Open the Bluetooth/PAN menu and select Sniff/Scan.
 *   3. Turn on Bluetooth on your own phone or smartwatch.
 *   4. For easier discovery, open Bluetooth settings or a BLE scanner app.
 *   5. Wait until scan completes and show the listed names/RSSI/address data.
 *
 * Expected outcome:
 *   - OLED result list shows visible devices plus RESCAN/BACK.
 *   - Serial Monitor prints discovered Classic/BLE devices and metadata.
 *
 * TODO / limitation:
 *   - BLE random/private addresses may change, so the same phone may appear
 *     differently across scans.
 */

#include "BluetoothScan.h"
#include "BluetoothReportUI.h"
#include "BluetoothState.h"

#include <Arduino.h>
#include <BluetoothSerial.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// ================= ESP32 BLUETOOTH CHECK =================

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled.
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Bluetooth Classic SPP is only available on regular ESP32, not ESP32-S3/C3.
#endif

// ================= DEVICE LIST HELPERS =================

int findDeviceByAddress(const char* address) {
  if (address == NULL || strlen(address) == 0) {
    return -1;
  }

  for (int i = 0; i < btDeviceCount; i++) {
    if (stringsEqualIgnoreCase(btDeviceAddresses[i], address)) {
      return i;
    }
  }

  return -1;
}

void addDeviceToList(
  const char* name,
  const char* address,
  const char* typeName,
  int rssi,
  bool hasRssi,
  int bleServiceUUIDCount = 0,
  int bleServiceDataCount = 0,
  int bleManufacturerDataLen = 0,
  int blePayloadLen = 0,
  int bleAddressType = -1,
  bool bleHasTxPower = false,
  int bleTxPower = 0,
  bool bleHasAppearance = false,
  int bleAppearance = 0
) {
  if (address == NULL || strlen(address) == 0) {
    return;
  }

  int existingIndex = findDeviceByAddress(address);

  if (existingIndex >= 0) {
    if (name != NULL && strlen(name) > 0) {
      copyCleanText(btDeviceLabels[existingIndex], name, BT_LABEL_LEN);
    }

    copyCleanText(btDeviceTypes[existingIndex], typeName, BT_TYPE_LEN);

    btDeviceRSSI[existingIndex] = rssi;
    btDeviceHasRSSI[existingIndex] = hasRssi;

    btBleServiceUUIDCount[existingIndex] = bleServiceUUIDCount;
    btBleServiceDataCount[existingIndex] = bleServiceDataCount;
    btBleManufacturerDataLen[existingIndex] = bleManufacturerDataLen;
    btBlePayloadLen[existingIndex] = blePayloadLen;
    btBleAddressType[existingIndex] = bleAddressType;

    btBleHasTxPower[existingIndex] = bleHasTxPower;
    btBleTxPower[existingIndex] = bleTxPower;

    btBleHasAppearance[existingIndex] = bleHasAppearance;
    btBleAppearance[existingIndex] = bleAppearance;

    return;
  }

  if (btDeviceCount >= MAX_BT_DEVICES) {
    return;
  }

  if (name != NULL && strlen(name) > 0) {
    copyCleanText(btDeviceLabels[btDeviceCount], name, BT_LABEL_LEN);
  } else {
    copyCleanText(btDeviceLabels[btDeviceCount], address, BT_LABEL_LEN);
  }

  copyCleanText(btDeviceAddresses[btDeviceCount], address, BT_ADDR_LEN);
  copyCleanText(btDeviceTypes[btDeviceCount], typeName, BT_TYPE_LEN);

  btDeviceRSSI[btDeviceCount] = rssi;
  btDeviceHasRSSI[btDeviceCount] = hasRssi;

  btBleServiceUUIDCount[btDeviceCount] = bleServiceUUIDCount;
  btBleServiceDataCount[btDeviceCount] = bleServiceDataCount;
  btBleManufacturerDataLen[btDeviceCount] = bleManufacturerDataLen;
  btBlePayloadLen[btDeviceCount] = blePayloadLen;
  btBleAddressType[btDeviceCount] = bleAddressType;

  btBleHasTxPower[btDeviceCount] = bleHasTxPower;
  btBleTxPower[btDeviceCount] = bleTxPower;

  btBleHasAppearance[btDeviceCount] = bleHasAppearance;
  btBleAppearance[btDeviceCount] = bleAppearance;

  btDeviceCount++;
}

// ================= INIT =================

void startClassicIfNeeded() {
  if (classicStarted) {
    return;
  }

  classicStarted = SerialBT.begin(BT_DEVICE_NAME, false, false);

  if (classicStarted) {
    Serial.println("Classic Bluetooth started");
  } else {
    Serial.println("Classic Bluetooth failed");
  }
}

void startBLEIfNeeded() {
  if (bleStarted) {
    return;
  }

  BLEDevice::init(BT_DEVICE_NAME);
  bleStarted = true;

  Serial.println("BLE started");
}

// ==================================================
// ACTIVITY 1: SNIFF
// ==================================================

void drawSniffPhaseScreen(const char* line1, const char* line2, const char* line3) {
  renderBluetoothOperationStage(
    "SNIFF",
    line1,
    line2,
    line3
  );
}

// ================= SNIFF ACTION PART: CLASSIC CALLBACK =================

void classicDeviceFound(BTAdvertisedDevice* pDevice) {
  String name = pDevice->getName().c_str();
  String address = pDevice->getAddress().toString().c_str();

  Serial.print("CLASSIC FOUND: ");
  Serial.print(name);
  Serial.print(" | ");
  Serial.print(address);
  Serial.print(" | RSSI: ");
  Serial.println(pDevice->getRSSI());

  addDeviceToList(
    name.c_str(),
    address.c_str(),
    "BT",
    pDevice->getRSSI(),
    true
  );
}

// ================= SNIFF ACTION PART: CLASSIC SEARCH =================

void scanClassicBluetooth() {
  startClassicIfNeeded();

  if (!classicStarted) {
    Serial.println("Classic scan skipped");
    return;
  }

  drawSniffPhaseScreen("SCANNING", "CLASSIC BT", "DISCOVERY");

  Serial.println("Scanning Classic Bluetooth async...");

  SerialBT.discoverClear();

  bool scanStarted = SerialBT.discoverAsync(classicDeviceFound);

  if (scanStarted) {
    delay(CLASSIC_SCAN_TIME_MS);

    Serial.println("Stopping Classic async scan...");
    SerialBT.discoverAsyncStop();

    delay(500);
  } else {
    Serial.println("Classic async failed, trying sync scan...");

    BTScanResults* results = SerialBT.discover(CLASSIC_SCAN_TIME_MS);

    if (results != NULL) {
      int foundCount = results->getCount();

      Serial.print("Classic sync devices found: ");
      Serial.println(foundCount);

      for (int i = 0; i < foundCount && btDeviceCount < MAX_BT_DEVICES; i++) {
        BTAdvertisedDevice* device = results->getDevice(i);

        String name = device->getName().c_str();
        String address = device->getAddress().toString().c_str();

        Serial.print("CLASSIC SYNC: ");
        Serial.print(name);
        Serial.print(" | ");
        Serial.print(address);
        Serial.print(" | RSSI: ");
        Serial.println(device->getRSSI());

        addDeviceToList(
          name.c_str(),
          address.c_str(),
          "BT",
          device->getRSSI(),
          true
        );
      }
    } else {
      Serial.println("Classic sync scan failed");
    }
  }
}

// ================= SNIFF ACTION PART: BLE SEARCH =================

void scanBLEBluetooth() {
  startBLEIfNeeded();

  drawSniffPhaseScreen("SCANNING", "BLE DEVICES", "ACTIVE SCAN");

  Serial.println("Scanning BLE...");

  BLEScan* pBLEScan = BLEDevice::getScan();

  pBLEScan->setActiveScan(true);
  pBLEScan->setInterval(80);
  pBLEScan->setWindow(79);

  BLEScanResults* foundDevices = pBLEScan->start(BLE_SCAN_TIME_SEC, false);

  if (foundDevices == NULL) {
    Serial.println("BLE scan failed");
    return;
  }

  int foundCount = foundDevices->getCount();

  Serial.print("BLE devices found: ");
  Serial.println(foundCount);

  for (int i = 0; i < foundCount && btDeviceCount < MAX_BT_DEVICES; i++) {
    BLEAdvertisedDevice device = foundDevices->getDevice(i);

    String name = "";

    if (device.haveName()) {
      name = device.getName();
    }

    String address = device.getAddress().toString();

    int serviceUUIDCount = device.getServiceUUIDCount();
    int serviceDataCount = device.getServiceDataCount();
    int manufacturerDataLen = 0;
    int payloadLen = device.getPayloadLength();
    int addressType = device.getAddressType();

    bool hasTxPower = device.haveTXPower();
    int txPower = 0;

    bool hasAppearance = device.haveAppearance();
    int appearance = 0;

    if (device.haveManufacturerData()) {
      manufacturerDataLen = device.getManufacturerData().length();
    }

    if (hasTxPower) {
      txPower = device.getTXPower();
    }

    if (hasAppearance) {
      appearance = device.getAppearance();
    }

    Serial.print("BLE FOUND: ");
    Serial.print(name);
    Serial.print(" | ");
    Serial.print(address);
    Serial.print(" | RSSI: ");
    Serial.print(device.getRSSI());
    Serial.print(" | SVC UUIDS: ");
    Serial.print(serviceUUIDCount);
    Serial.print(" | SVC DATA: ");
    Serial.print(serviceDataCount);
    Serial.print(" | MFR LEN: ");
    Serial.print(manufacturerDataLen);
    Serial.print(" | PAYLOAD LEN: ");
    Serial.print(payloadLen);
    Serial.print(" | ADDR TYPE: ");
    Serial.print(addressType);

    if (hasTxPower) {
      Serial.print(" | TX: ");
      Serial.print(txPower);
    }

    if (hasAppearance) {
      Serial.print(" | APP: ");
      Serial.print(appearance);
    }

    Serial.println();

    addDeviceToList(
      name.c_str(),
      address.c_str(),
      "BLE",
      device.getRSSI(),
      true,
      serviceUUIDCount,
      serviceDataCount,
      manufacturerDataLen,
      payloadLen,
      addressType,
      hasTxPower,
      txPower,
      hasAppearance,
      appearance
    );
  }

  pBLEScan->clearResults();
}

// ================= SNIFF ACTION PART: FULL SEARCH =================

void bluetoothSniffAction() {
  btDeviceCount = 0;

  Serial.println();
  Serial.println("===== BLUETOOTH SNIFF START =====");

  scanClassicBluetooth();
  scanBLEBluetooth();

  Serial.print("Total devices listed: ");
  Serial.println(btDeviceCount);
}

// ================= SNIFF RESULT PART =================

void bluetoothSniffResult() {
  bluetoothReportMode = BT_REPORT_SNIFF;

  copyCleanText(bluetoothReportTitle, "SNIFF", REPORT_TITLE_LEN);

  showBluetoothReportScreen();
}

// Legacy wrapper used by RESCAN also.
void scanBluetoothDevices() {
  bluetoothSniffAction();
  bluetoothSniffResult();
}
