/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 * Header for the nRF24 link/stress page. Recommended improvement is to convert
 * this into a closed TX/RX reliability test between owned modules.
 */

#ifndef BLUETOOTH_PAGE_NRF_LINK_TEST_H
#define BLUETOOTH_PAGE_NRF_LINK_TEST_H

// Real nRF24L01 Jamming / Link Test implementation

void bluetoothNrfLinkTestAction();
void bluetoothNrfLinkTestResult();

// Compatibility wrappers for older code
void bluetoothNrfJamTestAction();
void bluetoothNrfJamTestResult();

#endif