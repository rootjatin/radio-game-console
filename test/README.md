# ESP32 Game Console GPIO Connections

## OLED Screen

| OLED Pin  | ESP32 GPIO |
| --------- | ---------: |
| SDA       |    GPIO 21 |
| SCL / SCK |    GPIO 22 |
| VCC       |        3V3 |
| GND       |        GND |

## Buttons

| Button | ESP32 GPIO |
| ------ | ---------: |
| UP     |    GPIO 32 |
| DOWN   |    GPIO 33 |
| LEFT   |    GPIO 25 |
| RIGHT  |    GPIO 26 |
| FIRE   |    GPIO 27 |

Each button: one leg to GPIO, diagonal/opposite leg to GND.

## nRF24L01 Module 1

| nRF 1 Pin |    ESP32 GPIO |
| --------- | ------------: |
| CE        |        GPIO 4 |
| CSN / CS  |        GPIO 5 |
| SCK       |       GPIO 18 |
| MOSI      |       GPIO 23 |
| MISO      |       GPIO 19 |
| VCC       |      VIN / 5V |
| GND       |           GND |
| IRQ       | Not connected |

## nRF24L01 Module 2

| nRF 2 Pin |    ESP32 GPIO |
| --------- | ------------: |
| CE        |       GPIO 13 |
| CSN / CS  |       GPIO 14 |
| SCK       |       GPIO 18 |
| MOSI      |       GPIO 23 |
| MISO      |       GPIO 19 |
| VCC       |      VIN / 5V |
| GND       |           GND |
| IRQ       | Not connected |

## Shared SPI Pins for Both nRF Modules

| SPI Signal | ESP32 GPIO |
| ---------- | ---------: |
| SCK        |    GPIO 18 |
| MOSI       |    GPIO 23 |
| MISO       |    GPIO 19 |
