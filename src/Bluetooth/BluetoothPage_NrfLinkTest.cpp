/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 *
 * This page is named as a link test, but the current logic transmits repeated
 * packets on one 2.4 GHz channel. That can interfere with nearby devices if
 * used outside an isolated and authorised lab. Keep the implementation for
 * code-review/viva discussion, but do not run it as a classroom live demo.
 *
 * Recommended viva use:
 *   1. Explain that this is not selected for live demo because it is active RF.
 *   2. Mention that a better final version would use a closed pair of nRF24
 *      modules and measure packet-delivery ratio instead of stressing a band.
 *   3. Demo RF Sweep or BLE Beacon instead.
 *
 * Expected outcome if run only in an isolated authorised lab:
 *   - OLED shows the selected channel and remaining time.
 *   - Serial Monitor prints packet count after completion.
 *
 * TODO / limitation:
 *   - Convert this into a true closed-loop TX/RX reliability test using two
 *     owned modules, ACKs, packet counters, and no broad interference.
 */

// BluetoothPage_NrfLinkTest.cpp

#include "BluetoothPage_NrfLinkTest.h"
#include "BluetoothReportUI.h"
#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// ====================== CONFIG ======================
#define NRF1_CE   4
#define NRF1_CSN  5
#define NRF2_CE   13
#define NRF2_CSN  14

#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23

#define JAM_DURATION_MS     16000     // 16 seconds of jamming
#define JAM_CHANNEL         76        // Common channel (you can change)

// ===================================================

static SPIClass vspi(VSPI);
static RF24 radio1(NRF1_CE, NRF1_CSN);
static RF24 radio2(NRF2_CE, NRF2_CSN);

static uint8_t jamPayload[32];
static volatile bool stopJam = false;

// ===================================================
// Reliability note: these objects are file-local to avoid linker
// collisions with other pages that use the same radio names.
void initRadiosForJam() {
  vspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, -1);

  // === Radio 1 ===
  radio1.begin(&vspi);
  radio1.setAutoAck(false);
  radio1.setPALevel(RF24_PA_MAX);        // Maximum power
  radio1.setDataRate(RF24_2MBPS);
  radio1.setChannel(JAM_CHANNEL);
  radio1.setRetries(0, 0);
  radio1.setCRCLength(RF24_CRC_DISABLED);
  radio1.openWritingPipe(0xF0F0F0F0E1LL);

  // === Radio 2 ===
  radio2.begin(&vspi);
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_MAX);
  radio2.setDataRate(RF24_2MBPS);
  radio2.setChannel(JAM_CHANNEL);
  radio2.setRetries(0, 0);
  radio2.setCRCLength(RF24_CRC_DISABLED);
  radio2.openWritingPipe(0xF0F0F0F0E2LL);

  // Fill payload with noise
  for (int i = 0; i < 32; i++) {
    jamPayload[i] = random(0, 256);
  }
}

// ===================================================
void bluetoothNrfLinkTestAction() {
  stopJam = false;
  
  renderBluetoothOperationStage("NRF LINK TEST", "INITIALIZING", "NRF24 MODULES", "CONFIGURING");
  delay(600);

  initRadiosForJam();

  renderBluetoothOperationStage("NRF LINK TEST", "JAMMING", "2.4GHz BAND", "ACTIVE");
  delay(800);

  unsigned long startTime = millis();
  uint32_t packetCount = 0;

  while (millis() - startTime < JAM_DURATION_MS && !stopJam) {
    // Continuous jamming on both modules
    radio1.writeFast(jamPayload, 32, true);
    radio2.writeFast(jamPayload, 32, true);
    
    packetCount += 2;

    // Update display every 300ms
    if (packetCount % 300 == 0) {
      int remaining = (JAM_DURATION_MS - (millis() - startTime)) / 1000;
      
      renderBluetoothOperationStage("NRF LINK TEST", 
                                    "JAMMING ACTIVE", 
                                    "CH " + String(JAM_CHANNEL), 
                                    String(remaining) + "s left");
    }

    // Small delay to prevent watchdog trigger
    delayMicroseconds(80);
  }

  stopJam = true;

  renderBluetoothOperationStage("NRF LINK TEST", "STOPPING", "JAMMING", "CLEANUP");
  delay(800);

  Serial.printf("NRF Jam Test completed. Packets sent: %lu\n", packetCount);
}

// ===================================================
void bluetoothNrfLinkTestResult() {
  renderBluetoothOperationResult("NRF LINK TEST");
  Serial.println("=== NRF Jam / Link Test Completed ===");
}

// Compatibility wrappers
void bluetoothNrfJamTestAction() {
  bluetoothNrfLinkTestAction();
}

void bluetoothNrfJamTestResult() {
  // Compatibility wrapper: route old menu name to the real result function.
  bluetoothNrfLinkTestResult();
}
