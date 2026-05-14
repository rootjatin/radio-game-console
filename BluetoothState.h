/*
 * Shared Bluetooth/PAN state declarations and small text helper functions.
 */

#ifndef BLUETOOTH_STATE_H
#define BLUETOOTH_STATE_H

#include <Arduino.h>
#include <BluetoothSerial.h>

// ================= SETTINGS =================

#define BT_DEVICE_NAME          "ESP32_GAME_CONSOLE"

#define CLASSIC_SCAN_TIME_MS    12000
#define BLE_SCAN_TIME_SEC       10

#define MAX_BT_DEVICES          18
#define BT_LABEL_LEN            19
#define BT_ADDR_LEN             18
#define BT_TYPE_LEN             8

#define REPORT_TITLE_LEN        18
#define REPORT_LINE_LEN         22
#define MAX_REPORT_LINES        8

#define NRF_CHANNEL_MIN         0
#define NRF_CHANNEL_MAX         125
#define NRF_MODULE_COUNT        2

// ================= REPORT STATE =================

enum BluetoothReportMode {
  BT_REPORT_SNIFF,
  BT_REPORT_AUDIT,
  BT_REPORT_OPERATION
};

// ================= SHARED STATE =================
//
// Defined once in BluetoothState.cpp.
// Other files use these extern declarations to avoid duplicate RAM use.

extern BluetoothSerial SerialBT;

extern bool classicStarted;
extern bool bleStarted;

extern char btDeviceLabels[MAX_BT_DEVICES][BT_LABEL_LEN];
extern char btDeviceAddresses[MAX_BT_DEVICES][BT_ADDR_LEN];
extern char btDeviceTypes[MAX_BT_DEVICES][BT_TYPE_LEN];

extern int btDeviceRSSI[MAX_BT_DEVICES];
extern bool btDeviceHasRSSI[MAX_BT_DEVICES];

// Extra passive BLE advertisement metadata.
// These fields are read only from public BLE advertisements.
extern int btBleServiceUUIDCount[MAX_BT_DEVICES];
extern int btBleServiceDataCount[MAX_BT_DEVICES];
extern int btBleManufacturerDataLen[MAX_BT_DEVICES];
extern int btBlePayloadLen[MAX_BT_DEVICES];
extern int btBleAddressType[MAX_BT_DEVICES];

extern int btBleTxPower[MAX_BT_DEVICES];
extern bool btBleHasTxPower[MAX_BT_DEVICES];

extern int btBleAppearance[MAX_BT_DEVICES];
extern bool btBleHasAppearance[MAX_BT_DEVICES];

extern int btDeviceCount;

extern char bluetoothResultItemBuffer[REPORT_LINE_LEN];

extern BluetoothReportMode bluetoothReportMode;

extern char bluetoothReportTitle[REPORT_TITLE_LEN];
extern char bluetoothReportLines[MAX_REPORT_LINES][REPORT_LINE_LEN];
extern int bluetoothReportLineCount;

// ================= TEXT HELPERS =================

bool isAllowedOLEDChar(char c);
void copyCleanText(char* dest, const char* src, int maxLen);

char toUpperAscii(char c);
bool stringsEqualIgnoreCase(const char* a, const char* b);

#endif
