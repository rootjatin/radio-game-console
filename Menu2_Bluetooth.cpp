#include "AppState.h"
#include "OLED_Driver.h"
#include "Menu2_Bluetooth.h"

#include "src/Bluetooth/BluetoothAudit.h"
#include "src/Bluetooth/BluetoothPages.h"
#include "src/Bluetooth/BluetoothScan.h"

// ================= BLUETOOTH SUB MENU =================
//
// Labels are named as diagnostics/demo pages because the non-scan pages
// are display-only demos. No pairing, flooding, jamming, or spam is performed.

const char* bluetoothMenu[] = {
  "SNIFF",
  "PAIR AUDIT",
  "BLE BEACON",
  "BLE NAME TEST",
  "2.4G LOAD TEST",
  "RF SWEEP",
  "CHANNEL HOP",
  "NRF LINK TEST",
  "BACK"
};

const int bluetoothMenuCount = sizeof(bluetoothMenu) / sizeof(bluetoothMenu[0]);

int getBluetoothMenuCount() {
  return bluetoothMenuCount;
}

void drawBluetoothMenu() {
  drawMenuScreen("BLUETOOTH", bluetoothMenu, bluetoothMenuCount);
}

// ==================================================
// COMMON ACTIVITY RUNNER
// ==================================================

void runBluetoothActivity(
  BluetoothActivityPart actionPart,
  BluetoothActivityPart resultPart
) {
  actionPart();
  resultPart();
}

// ================= BLUETOOTH MENU ACTION ROUTER =================

void selectBluetoothMenuItem() {
  if (selectedIndex == 0) {
    runBluetoothActivity(
      bluetoothSniffAction,
      bluetoothSniffResult
    );
  }

  else if (selectedIndex == 1) {
    bluetoothPairAuditAction();
  }

  else if (selectedIndex == 2) {
    runBluetoothActivity(
      bluetoothBleBeaconAction,
      bluetoothBleBeaconResult
    );
  }

  else if (selectedIndex == 3) {
    runBluetoothActivity(
      bluetoothBleNameTestAction,
      bluetoothBleNameTestResult
    );
  }

  else if (selectedIndex == 4) {
    runBluetoothActivity(
      bluetoothBandLoadDemoAction,
      bluetoothBandLoadDemoResult
    );
  }

  else if (selectedIndex == 5) {
    runBluetoothActivity(
      bluetoothRfSweepAction,
      bluetoothRfSweepResult
    );
  }

  else if (selectedIndex == 6) {
    runBluetoothActivity(
      bluetoothChannelHopAction,
      bluetoothChannelHopResult
    );
  }

  else if (selectedIndex == 7) {
    runBluetoothActivity(
      bluetoothNrfLinkTestAction,
      bluetoothNrfLinkTestResult
    );
  }

  else if (selectedIndex == 8) {
    goBack();
  }
}
