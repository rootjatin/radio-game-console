/*
 * RISKY BUT WORKS, SO DON'T RUN LIVE IN CLASS.
 *
 * This page generates heavy 2.4 GHz traffic using nRF24L01 radios. It may
 * disturb nearby Wi-Fi/Bluetooth/2.4 GHz devices if executed outside an
 * isolated and authorised lab setup. Keep the code for viva/code review, but
 * do not run this test during a normal classroom demo.
 *
 * Recommended viva use:
 *   1. Explain this as a stress/load-test concept for a controlled RF lab.
 *   2. Show the source code and configuration values.
 *   3. Tell the examiner that the live demo is intentionally skipped because
 *      it can affect other devices in the room.
 *
 * Expected outcome if run only in an isolated lab:
 *   - OLED shows initialization, active transmit status, remaining time, then
 *     cleanup.
 *   - Serial Monitor prints packet-count information at the end.
 *
 * TODO / limitation:
 *   - Add a physical lab-mode switch or PIN confirmation before allowing this
 *     action to run.
 *   - Add RF shielding/attenuators and measurement equipment before treating
 *     the result as scientific data.
 */

// BluetoothPage_BandLoad_Demo.cpp  →  Rename or replace with real version

#include "BluetoothPage_BandLoadDemo.h"
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

// How aggressive do you want the flood?
#define PACKET_SIZE        32     // Max is 32 bytes
#define FLOOD_DURATION_MS  15000  // Default 15 seconds (you can change)
#define PACKETS_PER_SECOND 800    // Very aggressive on ESP32 + nRF24

// ===================================================

static SPIClass vspi(VSPI);
static RF24 radio1(NRF1_CE, NRF1_CSN);
static RF24 radio2(NRF2_CE, NRF2_CSN);

static uint8_t dummyPayload[PACKET_SIZE];

static volatile bool stopFlood = false;

// ===================================================
// Reliability note: these objects are file-local to avoid linker
// collisions with other pages that use the same radio names.
void initRadiosForFlood() {
  vspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, -1);

  // Radio 1
  radio1.begin(&vspi);
  radio1.setAutoAck(false);
  radio1.setPALevel(RF24_PA_MAX);        // Maximum power
  radio1.setDataRate(RF24_2MBPS);        // Fastest rate
  radio1.setChannel(76);                 // You can randomize or sweep later
  radio1.setRetries(0, 0);
  radio1.setCRCLength(RF24_CRC_DISABLED);
  radio1.openWritingPipe(0xF0F0F0F0E1LL); // Dummy address

  // Radio 2
  radio2.begin(&vspi);
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_MAX);
  radio2.setDataRate(RF24_2MBPS);
  radio2.setChannel(42);                 // Different channel for more coverage
  radio2.setRetries(0, 0);
  radio2.setCRCLength(RF24_CRC_DISABLED);
  radio2.openWritingPipe(0xF0F0F0F0E2LL);

  // Fill payload with random noise
  for (int i = 0; i < PACKET_SIZE; i++) {
    dummyPayload[i] = random(0, 256);
  }
}

// ===================================================
void floodTask(void* pvParameters) {
  RF24* radio = (RF24*)pvParameters;
  uint32_t packetCount = 0;
  unsigned long startTime = millis();

  while (!stopFlood && (millis() - startTime < FLOOD_DURATION_MS)) {
    radio->writeFast(dummyPayload, PACKET_SIZE, true);  // Non-blocking
    packetCount++;

    // Small yield to keep ESP32 responsive
    if (packetCount % 200 == 0) {
      delayMicroseconds(50);
    }
  }

  Serial.printf("Radio finished. Packets sent: %lu\n", packetCount);
  vTaskDelete(NULL);
}

// ===================================================
void bluetoothBandLoadDemoAction() {
  stopFlood = false;
  
  renderBluetoothOperationStage("2.4G LOAD TEST", "INITIALIZING", "RADIOS", "CONFIGURING");
  delay(600);

  initRadiosForFlood();

  renderBluetoothOperationStage("2.4G LOAD TEST", "FLOODING", "BAND", "ACTIVE");
  delay(800);

  // Start flooding on both modules in parallel
  xTaskCreate(floodTask, "Flood1", 4096, &radio1, 1, NULL);
  xTaskCreate(floodTask, "Flood2", 4096, &radio2, 1, NULL);

  // Show live status for the duration
  unsigned long start = millis();
  while (millis() - start < FLOOD_DURATION_MS) {
    renderBluetoothOperationStage("2.4G LOAD TEST", 
                                  "TRANSMITTING", 
                                  "MAX POWER", 
                                  String(FLOOD_DURATION_MS - (millis() - start)) + "ms left");
    delay(400);
    
    if (stopFlood) break;
  }

  stopFlood = true;
  delay(500);

  renderBluetoothOperationStage("2.4G LOAD TEST", "STOPPING", "RADIOS", "CLEANUP");
  delay(800);
}

// ===================================================
void bluetoothBandLoadDemoResult() {
  renderBluetoothOperationResult("2.4G LOAD TEST");
  
  Serial.println("=== 2.4GHz Band Load Test Completed ===");
}

// Compatibility wrappers
void bluetoothFloodAction() {
  bluetoothBandLoadDemoAction();
}

void bluetoothFloodResult() {
  bluetoothBandLoadDemoResult();
}