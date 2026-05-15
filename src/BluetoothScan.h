/*
 * SAFE LIVE DEMO CANDIDATE.
 * Header for passive Bluetooth Classic/BLE discovery helpers.
 */

#ifndef BLUETOOTH_SCAN_H
#define BLUETOOTH_SCAN_H

void startClassicIfNeeded();
void startBLEIfNeeded();

void scanClassicBluetooth();
void scanBLEBluetooth();

void bluetoothSniffAction();
void bluetoothSniffResult();
void scanBluetoothDevices();

#endif
