#include <SPI.h>

// SPI pins for ESP32
#define SPI_SCK   18
#define SPI_MISO  19
#define SPI_MOSI  23

// nRF module 1 pins
#define NRF1_CE   4
#define NRF1_CSN  5

// nRF module 2 pins
#define NRF2_CE   13
#define NRF2_CSN  14

// nRF24 register addresses
#define REG_CONFIG   0x00
#define REG_RF_CH    0x05
#define REG_RF_SETUP 0x06
#define REG_STATUS   0x07

#define R_REGISTER   0x00
#define W_REGISTER   0x20

uint8_t readRegister(uint8_t csnPin, uint8_t reg) {
  digitalWrite(csnPin, LOW);
  SPI.transfer(R_REGISTER | (reg & 0x1F));
  uint8_t value = SPI.transfer(0xFF);
  digitalWrite(csnPin, HIGH);
  return value;
}

void writeRegister(uint8_t csnPin, uint8_t reg, uint8_t value) {
  digitalWrite(csnPin, LOW);
  SPI.transfer(W_REGISTER | (reg & 0x1F));
  SPI.transfer(value);
  digitalWrite(csnPin, HIGH);
}

void testNRF(const char* name, uint8_t cePin, uint8_t csnPin) {
  Serial.println();
  Serial.print("Testing ");
  Serial.println(name);

  digitalWrite(cePin, LOW);
  digitalWrite(csnPin, HIGH);
  delay(10);

  uint8_t configBefore = readRegister(csnPin, REG_CONFIG);
  uint8_t rfSetup      = readRegister(csnPin, REG_RF_SETUP);
  uint8_t statusReg    = readRegister(csnPin, REG_STATUS);
  uint8_t oldChannel   = readRegister(csnPin, REG_RF_CH);

  Serial.print("CONFIG: 0x");
  Serial.println(configBefore, HEX);

  Serial.print("RF_SETUP: 0x");
  Serial.println(rfSetup, HEX);

  Serial.print("STATUS: 0x");
  Serial.println(statusReg, HEX);

  Serial.print("Old RF_CH: ");
  Serial.println(oldChannel);

  // Write test value to RF channel register
  writeRegister(csnPin, REG_RF_CH, 76);
  delay(5);

  uint8_t newChannel = readRegister(csnPin, REG_RF_CH);

  Serial.print("Test RF_CH after write: ");
  Serial.println(newChannel);

  // Restore old channel
  writeRegister(csnPin, REG_RF_CH, oldChannel);

  if (newChannel == 76) {
    Serial.print(name);
    Serial.println(" CONNECTION OK");
  } else {
    Serial.print(name);
    Serial.println(" CONNECTION FAILED");
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("nRF24L01 SPI Connection Test");

  pinMode(NRF1_CE, OUTPUT);
  pinMode(NRF1_CSN, OUTPUT);
  pinMode(NRF2_CE, OUTPUT);
  pinMode(NRF2_CSN, OUTPUT);

  digitalWrite(NRF1_CE, LOW);
  digitalWrite(NRF1_CSN, HIGH);
  digitalWrite(NRF2_CE, LOW);
  digitalWrite(NRF2_CSN, HIGH);

  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  testNRF("nRF 1", NRF1_CE, NRF1_CSN);
  testNRF("nRF 2", NRF2_CE, NRF2_CSN);
}

void loop() {
}
