#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// SPI pins
#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23

// Radio 1
#define NRF1_CE   4
#define NRF1_CSN  5

// Radio 2
#define NRF2_CE   13
#define NRF2_CSN  14

RF24 radio1(NRF1_CE, NRF1_CSN, 1000000);
RF24 radio2(NRF2_CE, NRF2_CSN, 1000000);

const byte ADDR_1_TO_2[6] = "A0001";
const byte ADDR_2_TO_1[6] = "B0001";

uint32_t testNo = 0;

void configureRadio(RF24 &radio) {
  radio.setChannel(108);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MIN);
  radio.setRetries(5, 15);
  radio.setAutoAck(true);

  // We send exactly 32 bytes every time
  radio.setPayloadSize(32);

  radio.flush_rx();
  radio.flush_tx();
  radio.powerUp();
}

bool receiveString(RF24 &rxRadio, char *rxText, uint32_t timeoutMs) {
  uint32_t start = millis();

  memset(rxText, 0, 32);

  while (millis() - start < timeoutMs) {
    if (rxRadio.available()) {
      rxRadio.read(rxText, 32);
      rxText[31] = '\0';   // safety null termination
      return true;
    }
  }

  return false;
}

bool sendAndCheck(
  RF24 &txRadio,
  RF24 &rxRadio,
  const char *txName,
  const char *rxName,
  const char *sendText
) {
  char rxText[32];
  memset(rxText, 0, sizeof(rxText));

  rxRadio.stopListening();
  txRadio.stopListening();

  rxRadio.flush_rx();
  txRadio.flush_tx();

  delay(10);

  rxRadio.startListening();
  delay(10);

  char txText[32];
  memset(txText, 0, sizeof(txText));
  strncpy(txText, sendText, 31);

  Serial.println();
  Serial.print(txName);
  Serial.print(" sending: ");
  Serial.println(txText);

  bool ackOK = txRadio.write(txText, 32);

  Serial.print(txName);
  Serial.print(" write ACK: ");
  Serial.println(ackOK ? "YES" : "NO");

  bool rxOK = receiveString(rxRadio, rxText, 300);

  Serial.print(rxName);
  Serial.print(" received: ");
  if (rxOK) {
    Serial.println(rxText);
  } else {
    Serial.println("NO DATA");
  }

  bool sameString = rxOK && strcmp(txText, rxText) == 0;

  Serial.print("STRING MATCH: ");
  Serial.println(sameString ? "PASS" : "FAIL");

  rxRadio.stopListening();

  return ackOK && rxOK && sameString;
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println();
  Serial.println("====================================");
  Serial.println(" ESP32 TWO NRF STRING MATCH TEST");
  Serial.println("====================================");

  pinMode(NRF1_CSN, OUTPUT);
  pinMode(NRF2_CSN, OUTPUT);
  digitalWrite(NRF1_CSN, HIGH);
  digitalWrite(NRF2_CSN, HIGH);

  pinMode(NRF1_CE, OUTPUT);
  pinMode(NRF2_CE, OUTPUT);
  digitalWrite(NRF1_CE, LOW);
  digitalWrite(NRF2_CE, LOW);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
  delay(100);

  bool r1 = radio1.begin(&SPI);
  bool r2 = radio2.begin(&SPI);

  Serial.print("radio1.begin: ");
  Serial.println(r1 ? "OK" : "FAIL");

  Serial.print("radio2.begin: ");
  Serial.println(r2 ? "OK" : "FAIL");

  Serial.print("radio1 chip: ");
  Serial.println(radio1.isChipConnected() ? "CONNECTED" : "NOT FOUND");

  Serial.print("radio2 chip: ");
  Serial.println(radio2.isChipConnected() ? "CONNECTED" : "NOT FOUND");

  configureRadio(radio1);
  configureRadio(radio2);

  radio1.openWritingPipe(ADDR_1_TO_2);
  radio1.openReadingPipe(1, ADDR_2_TO_1);

  radio2.openWritingPipe(ADDR_2_TO_1);
  radio2.openReadingPipe(1, ADDR_1_TO_2);

  radio1.stopListening();
  radio2.stopListening();

  Serial.println("Setup done.");
}

void loop() {
  testNo++;

  char msg1[32];
  char msg2[32];

  snprintf(msg1, sizeof(msg1), "R1_TO_R2_%03lu", testNo);
  snprintf(msg2, sizeof(msg2), "R2_TO_R1_%03lu", testNo);

  Serial.println();
  Serial.println("---------- TEST START ----------");

  bool pass1 = sendAndCheck(
    radio1,
    radio2,
    "Radio1",
    "Radio2",
    msg1
  );

  delay(300);

  bool pass2 = sendAndCheck(
    radio2,
    radio1,
    "Radio2",
    "Radio1",
    msg2
  );

  Serial.println();
  Serial.print("FINAL RESULT TEST ");
  Serial.print(testNo);
  Serial.print(": ");

  if (pass1 && pass2) {
    Serial.println("PASS BOTH DIRECTIONS");
  } else {
    Serial.println("FAIL");
  }

  Serial.println("----------- TEST END -----------");

  delay(1000);
}
