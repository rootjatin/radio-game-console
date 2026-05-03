#include "BluetoothPage_BandLoadDemo.h"
#include "BluetoothReportUI.h"

#include <Arduino.h>

// ==================================================
// 2.4G LOAD TEST DEMO
// ==================================================
//
// Visual/demo placeholder only. Does not transmit load/flood traffic.

void bluetoothBandLoadDemoAction() {
  renderBluetoothOperationStage("2.4G LOAD TEST", "SYNC NRF 1", "SYNC NRF 2", "CH MAP READY");
  delay(800);

  renderBluetoothOperationStage("2.4G LOAD TEST", "PACKET QUEUE", "BUFFER READY", "RESULT READY");
  delay(800);
}

void bluetoothBandLoadDemoResult() {
  renderBluetoothOperationResult("2.4G LOAD TEST");
}

// Compatibility wrappers for older code.
void bluetoothFloodAction() {
  bluetoothBandLoadDemoAction();
}

void bluetoothFloodResult() {
  bluetoothBandLoadDemoResult();
}
