#include "BluetoothPage_BleBeacon.h"
#include "BluetoothReportUI.h"

#include <Arduino.h>

// ==================================================
// BLE BEACON DEMO
// ==================================================
//
// Visual/demo placeholder only. Does not advertise BLE packets.

void bluetoothBleBeaconAction() {
  renderBluetoothOperationStage("BLE BEACON", "ADV NAME SET", "UUID READY", "INTERVAL OK");
  delay(800);

  renderBluetoothOperationStage("BLE BEACON", "PAYLOAD SET", "ADV READY", "RESULT READY");
  delay(800);
}

void bluetoothBleBeaconResult() {
  renderBluetoothOperationResult("BLE BEACON");
}
