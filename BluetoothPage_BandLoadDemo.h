/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 * Header for the nRF24 2.4 GHz load/stress demo page. Keep for viva/code
 * review; run only in an isolated authorised lab.
 */

#ifndef BLUETOOTH_PAGE_BAND_LOAD_DEMO_H
#define BLUETOOTH_PAGE_BAND_LOAD_DEMO_H

// Visual/demo-only 2.4 GHz load status page.
// Does not transmit load/flood traffic.

void bluetoothBandLoadDemoAction();
void bluetoothBandLoadDemoResult();

// Compatibility wrappers for older code that still uses the old names.
void bluetoothFloodAction();
void bluetoothFloodResult();

#endif
