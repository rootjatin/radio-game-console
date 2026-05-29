/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 *
 * This page rapidly hops nRF24L01 radios across 2.4 GHz channels. It is useful
 * as a controlled-lab concept demonstration, but it can create RF noise and may
 * disturb nearby 2.4 GHz devices. Keep this code for explanation, but avoid
 * running it in the classroom.
 *
 * Recommended viva use:
 *   1. Show the channel-hop loop and explain how channels 0-125 map to the
 *      2.4 GHz ISM band.
 *   2. Explain that the live run is skipped for safety and consent reasons.
 *   3. Use RF Sweep / Bluetooth Scan as the safe live demos instead.
 *
 * Expected outcome if run only in an isolated authorised lab:
 *   - OLED shows current channel and remaining time.
 *   - Serial Monitor prints total packet count after completion.
 *
 * TODO / limitation:
 *   - Add a safe-mode gate and a dedicated shielded test box.
 *   - Add channel dwell-time logging so the result is easier to explain.
 */

// BluetoothPage_ChannelHop.cpp

#include "BluetoothPage_ChannelHop.h"
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

#define HOP_DURATION_MS     18000     // 18 seconds total
#define HOP_INTERVAL_MS     45        // Very fast hopping (adjust as needed)

// ===================================================

static SPIClass vspi(VSPI);
static RF24 radio1(NRF1_CE, NRF1_CSN);
static RF24 radio2(NRF2_CE, NRF2_CSN);

static uint8_t dummyPayload[32];
static volatile bool stopHopping = false;

// ===================================================
// Reliability note: these objects are file-local to avoid linker
// collisions with other pages that use the same radio names.
void initRadiosForHopping() {
  vspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, -1);

  // Radio 1 - High power, fast
  radio1.begin(&vspi);
  radio1.setAutoAck(false);
  radio1.setPALevel(RF24_PA_MAX);
  radio1.setDataRate(RF24_2MBPS);
  radio1.setRetries(0, 0);
  radio1.setCRCLength(RF24_CRC_DISABLED);
  radio1.openWritingPipe(0xF0F0F0F0E1LL);

  // Radio 2 - Different pipe
  radio2.begin(&vspi);
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_MAX);
  radio2.setDataRate(RF24_2MBPS);
  radio2.setRetries(0, 0);
  radio2.setCRCLength(RF24_CRC_DISABLED);
  radio2.openWritingPipe(0xF0F0F0F0E2LL);

  // Fill dummy payload
  for (int i = 0; i < 32; i++) {
    dummyPayload[i] = random(0, 256);
  }
}

// ===================================================
void bluetoothChannelHopAction() {
  stopHopping = false;
  
  renderBluetoothOperationStage("CHANNEL HOP", "INITIALIZING", "NRF24 RADIOS", "CONFIGURING");
  delay(600);

  initRadiosForHopping();

  renderBluetoothOperationStage("CHANNEL HOP", "HOPPING", "ACTIVE", "2.4GHz BAND");
  delay(800);

  unsigned long startTime = millis();
  uint8_t channel = 0;
  uint32_t packetCount = 0;

  while (millis() - startTime < HOP_DURATION_MS && !stopHopping) {
    // Hop across all channels (0-125)
    channel = (channel + 1) % 126;

    // Radio 1 hops on even channels
    radio1.setChannel(channel);
    radio1.writeFast(dummyPayload, 32, true);

    // Radio 2 hops on odd channels + offset for better coverage
    radio2.setChannel((channel + 63) % 126);
    radio2.writeFast(dummyPayload, 32, true);

    packetCount += 2;

    // Update display every few hops
    if (packetCount % 40 == 0) {
      int remaining = (HOP_DURATION_MS - (millis() - startTime)) / 1000;
      
      renderBluetoothOperationStage("CHANNEL HOP", 
                                    "HOPPING CH", 
                                    "CH: " + String(channel), 
                                    String(remaining) + "s left");
    }

    delay(HOP_INTERVAL_MS);
  }

  stopHopping = true;

  renderBluetoothOperationStage("CHANNEL HOP", "STOPPING", "RADIOS", "CLEANUP");
  delay(800);

  Serial.printf("Channel Hop completed. Total packets sent: %lu\n", packetCount);
}

// ===================================================
void bluetoothChannelHopResult() {
  renderBluetoothOperationResult("CHANNEL HOP");
  Serial.println("=== Channel Hop Test Completed ===");
}