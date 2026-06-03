/*
 * SAFE LIVE DEMO CANDIDATE.
 *
 * This page passively samples carrier activity across nRF24L01 channels. It is
 * much safer to demo than active load/hop tests because it listens instead of
 * intentionally creating traffic.
 *
 * Classroom demo steps:
 *   1. Connect the nRF24L01 modules with stable 3.3 V power and common GND.
 *   2. Open the Bluetooth/PAN menu and select RF Sweep.
 *   3. Let it scan for the full duration.
 *   4. Show the busiest channel and explain it as approximate activity, not a
 *      calibrated spectrum-analyser reading.
 *
 * Expected outcome:
 *   - OLED shows scanning channel, rough signal percentage, remaining time, and
 *     final busiest channel.
 *   - Serial Monitor prints the busiest channel and max activity.
 *
 * TODO / limitation:
 *   - nRF24 carrier-detect is approximate, so use it as a teaching demo, not a
 *     precise RF measurement tool.
 */

// BluetoothPage_RfSweep.cpp

#include "BluetoothPage_RfSweep.h"
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

#define SWEEP_DURATION_MS   20000     // 20 seconds total sweep
#define SAMPLES_PER_CHANNEL 8         // More samples = better accuracy

// ===================================================

static SPIClass vspi(VSPI);
static RF24 radio1(NRF1_CE, NRF1_CSN);
static RF24 radio2(NRF2_CE, NRF2_CSN);

static volatile bool stopSweep = false;

// ===================================================
// Reliability note: these objects are file-local to avoid linker
// collisions with other pages that use the same radio names.
void initRadiosForSweep() {
  vspi.begin(SPI_SCK, SPI_MISO, SPI_MOSI, -1);

  radio1.begin(&vspi);
  radio1.setAutoAck(false);
  radio1.setPALevel(RF24_PA_LOW);
  radio1.setDataRate(RF24_2MBPS);
  radio1.setCRCLength(RF24_CRC_DISABLED);

  radio2.begin(&vspi);
  radio2.setAutoAck(false);
  radio2.setPALevel(RF24_PA_LOW);
  radio2.setDataRate(RF24_2MBPS);
  radio2.setCRCLength(RF24_CRC_DISABLED);
}

// ===================================================
int getChannelActivity(RF24 &radio, uint8_t channel) {
  radio.setChannel(channel);
  radio.startListening();
  delayMicroseconds(180);        // Give time to detect signal

  int signalStrength = 0;
  
  for (int i = 0; i < SAMPLES_PER_CHANNEL; i++) {
    if (radio.testCarrier()) {
      signalStrength += 25;      // Strong carrier detected
    }
    delayMicroseconds(60);
  }

  radio.stopListening();
  return constrain(signalStrength, 0, 100);
}

// ===================================================
void bluetoothRfSweepAction() {
  stopSweep = false;
  
  renderBluetoothOperationStage("RF SWEEP", "INITIALIZING", "NRF24 RADIOS", "CONFIGURING");
  delay(600);

  initRadiosForSweep();

  renderBluetoothOperationStage("RF SWEEP", "SCANNING", "2.4GHz BAND", "ACTIVE");
  delay(800);

  unsigned long startTime = millis();
  uint8_t currentChannel = 0;
  int activity[126] = {0};        // Store activity per channel

  while (millis() - startTime < SWEEP_DURATION_MS && !stopSweep) {
    currentChannel = (currentChannel + 1) % 126;

    // Use both radios alternately for better sampling
    int act = (currentChannel % 2 == 0) ? 
              getChannelActivity(radio1, currentChannel) : 
              getChannelActivity(radio2, currentChannel);

    activity[currentChannel] = max(activity[currentChannel], act);

    // Update display every 8 channels
    if (currentChannel % 8 == 0) {
      int progress = (millis() - startTime) * 100 / SWEEP_DURATION_MS;
      int remaining = (SWEEP_DURATION_MS - (millis() - startTime)) / 1000;

      String status = "CH:" + String(currentChannel) + " | SIG:" + String(act) + "%";

      renderBluetoothOperationStage("RF SWEEP", 
                                    "SCANNING CH " + String(currentChannel), 
                                    status, 
                                    String(remaining) + "s left");
    }

    delay(12);   // Smooth sweeping speed
  }

  renderBluetoothOperationStage("RF SWEEP", "ANALYZING", "FINDING PEAKS", "GENERATING MAP");
  delay(1200);

  // Find busiest channels (simple analysis)
  int maxActivity = 0;
  uint8_t busiestChannel = 0;
  
  for (int i = 0; i < 126; i++) {
    if (activity[i] > maxActivity) {
      maxActivity = activity[i];
      busiestChannel = i;
    }
  }

  Serial.printf("RF Sweep completed. Busiest channel: %d (%d%% activity)\n", 
                busiestChannel, maxActivity);

  renderBluetoothOperationStage("RF SWEEP", 
                                "SWEEP DONE", 
                                "BUSIEST: CH " + String(busiestChannel), 
                                "MAX " + String(maxActivity) + "%");
  delay(2000);
}

// ===================================================
void bluetoothRfSweepResult() {
  renderBluetoothOperationResult("RF SWEEP");
  Serial.println("=== RF Sweep Completed ===");
}