/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 * Header for BLE advertisement-name rotation. Use only with consent and neutral
 * names; prefer BLE Beacon for live viva demo.
 */

#ifndef BLUETOOTH_PAGE_BLE_NAME_TEST_H
#define BLUETOOTH_PAGE_BLE_NAME_TEST_H

// Real BLE Name Spamming (Connectable Version)

void bluetoothBleNameTestAction();
void bluetoothBleNameTestResult();

// Compatibility wrappers for older code
void bluetoothBleNameSpamAction();
void bluetoothBleNameSpamResult();

#endif