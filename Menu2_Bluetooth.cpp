#include "AppState.h"
#include "OLED_Driver.h"
#include "Menu2_Bluetooth.h"

#include "src/Bluetooth/BluetoothAudit.h"
#include "src/Bluetooth/BluetoothPages.h"
#include "src/Bluetooth/BluetoothScan.h"

// These functions are defined in your main .ino / main cpp file.
extern void goBack();
extern void showMessage(
  const char* title,
  const char* line1,
  const char* line2,
  const char* line3
);

// ================= BLUETOOTH / PAN MENU =================
//
// Important:
// Adding .cpp/.h files under src/Bluetooth does NOT automatically add them
// to the OLED menu. Every item must be listed here and routed below.
//
// This version makes every PAN/Bluetooth page from your uploaded zip visible.
// Safe/passive/controlled pages run directly. Active RF stress pages are kept
// in the menu as LAB ONLY so your menu shows the full test plan without making
// the device a one-click RF flood/jam transmitter.

struct BluetoothMenuItem {
  const char* label;
  BluetoothActivityPart action;
  BluetoothActivityPart result;
  bool labOnly;
  const char* labReason;
};

void runBluetoothActivity(BluetoothActivityPart actionPart, BluetoothActivityPart resultPart);
static void showLabOnlyPage(const char* title, const char* reason);

const BluetoothMenuItem bluetoothMenuItems[] = {
  // Live-safe / normal PAN test pages
  { "SNIFF SCAN",        bluetoothSniffAction,       bluetoothSniffResult,       false, "" },
  { "VISIBILITY AUDIT",  bluetoothPairAuditAction,   NULL,                       false, "" },
  { "BLE BEACON",        bluetoothBleBeaconAction,   bluetoothBleBeaconResult,   false, "" },
  { "RF SWEEP",          bluetoothRfSweepAction,     bluetoothRfSweepResult,     false, "" },

  // Present in your uploaded zip, but risky as written
  { "BLE NAME TEST",     bluetoothBleNameTestAction, bluetoothBleNameTestResult, false, "" },  // ← Only this line changed (true → false)

  { "2.4G LOAD TEST",    bluetoothBandLoadDemoAction,bluetoothBandLoadDemoResult,true,  "RF LAB ONLY" },
  { "CHANNEL HOP",       bluetoothChannelHopAction,  bluetoothChannelHopResult,  true,  "RF LAB ONLY" },
  { "NRF LINK TEST",     bluetoothNrfLinkTestAction, bluetoothNrfLinkTestResult, true,  "CLOSED LAB ONLY" },

  { "BACK",              NULL,                       NULL,                       false, "" }
};

const int bluetoothMenuCount = sizeof(bluetoothMenuItems) / sizeof(bluetoothMenuItems[0]);

// drawMenuScreen() expects const char* labels[], so keep a label-only array.
const char* bluetoothMenu[bluetoothMenuCount] = {
  "SNIFF SCAN",
  "VISIBILITY AUDIT",
  "BLE BEACON",
  "RF SWEEP",
  "BLE NAME TEST",
  "2.4G LOAD TEST",
  "CHANNEL HOP",
  "NRF LINK TEST",
  "BACK"
};

int getBluetoothMenuCount() {
  return bluetoothMenuCount;
}

void drawBluetoothMenu() {
  // Changed title helps you confirm the correct file is being compiled.
  drawMenuScreen("BT/PAN TESTS", bluetoothMenu, bluetoothMenuCount);
}

void runBluetoothActivity(BluetoothActivityPart actionPart, BluetoothActivityPart resultPart) {
  if (actionPart != NULL) {
    actionPart();
  }

  if (resultPart != NULL) {
    resultPart();
  }
}

static void showLabOnlyPage(const char* title, const char* reason) {
  showMessage(
    title,
    "TEST IS VISIBLE",
    reason,
    "USE SHIELDED LAB"
  );
}

void selectBluetoothMenuItem() {
  if (selectedIndex < 0 || selectedIndex >= bluetoothMenuCount) {
    selectedIndex = 0;
    drawBluetoothMenu();
    return;
  }

  // Last item is BACK.
  if (selectedIndex == bluetoothMenuCount - 1) {
    goBack();
    return;
  }

  const BluetoothMenuItem& item = bluetoothMenuItems[selectedIndex];

  if (item.labOnly) {
    // The page is deliberately visible in the menu, but not launched directly.
    // Use this slot for manager/viva review, or replace the underlying page
    // with a closed-loop test that only talks to your own receiver.
    showLabOnlyPage(item.label, item.labReason);
    return;
  }

  runBluetoothActivity(item.action, item.result);
}
