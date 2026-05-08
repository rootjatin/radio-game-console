# ESP32 Game Console GPIO Connections

## Power System

Power source:

```text
3.7V LiPo Battery
   → DPDT ON/OFF Switch
   → 5V Boost Converter
   → ESP32 VIN / 5V Pin
```

> The boost converter output should be set to **5V** before connecting it to the ESP32 **VIN / 5V** pin.

---

## LiPo Battery, DPDT Switch, and Boost Converter

You are using a **6-legged DPDT switch**, but only one side / one set of 3 legs is needed.  
This makes it work like a simple ON/OFF switch.

### DPDT Switch Wiring

Use only **one side of 3 pins** on the DPDT switch.

| DPDT Switch Pin | Connection |
| --------------- | ---------- |
| Middle pin on one side | LiPo Battery **+** |
| One outer pin on the same side | Boost Converter **IN+** |
| Other outer pin on same side | Not connected |
| Other 3 pins of switch | Not connected |

If the switch turns ON in the opposite direction than you want, move the Boost Converter **IN+** wire to the other outer pin.

### Battery to Boost Converter

| Battery / Boost Pin | Connection |
| ------------------- | ---------- |
| LiPo Battery **+** | DPDT switch middle pin |
| DPDT switch outer pin | Boost Converter **IN+** |
| LiPo Battery **-** | Boost Converter **IN-** |

### Boost Converter to ESP32

| Boost Converter Pin | ESP32 Pin |
| ------------------- | --------- |
| OUT+ | VIN / 5V |
| OUT- | GND |

---

## Common Ground

All grounds must be connected together.

| Part | Ground Connection |
| ---- | ----------------- |
| ESP32 GND | Common GND |
| Boost Converter OUT- | Common GND |
| OLED GND | Common GND |
| Button GND legs | Common GND |
| nRF24L01 Module 1 GND | Common GND |
| nRF24L01 Module 2 GND | Common GND |

---

## OLED Screen

| OLED Pin | ESP32 GPIO |
| -------- | ---------- |
| SDA | GPIO 21 |
| SCL / SCK | GPIO 22 |
| VCC | 3V3 |
| GND | GND |

---

## Buttons

Each button has:

- One leg connected to the ESP32 GPIO
- The diagonal / opposite leg connected to GND

| Button | ESP32 GPIO |
| ------ | ---------- |
| UP | GPIO 32 |
| DOWN | GPIO 33 |
| LEFT | GPIO 25 |
| RIGHT | GPIO 26 |
| FIRE | GPIO 27 |

### Button Wiring

| Button Leg | Connection |
| ---------- | ---------- |
| One leg | ESP32 GPIO pin |
| Diagonal / opposite leg | GND |

In code, use the ESP32 internal pull-up resistors:

```cpp
pinMode(32, INPUT_PULLUP);
pinMode(33, INPUT_PULLUP);
pinMode(25, INPUT_PULLUP);
pinMode(26, INPUT_PULLUP);
pinMode(27, INPUT_PULLUP);
```

When a button is pressed, the GPIO reads **LOW**.

---

## nRF24L01 Module 1

| nRF24L01 Pin | ESP32 GPIO |
| ------------ | ---------- |
| CE | GPIO 4 |
| CSN / CS | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | 3V3 |
| GND | GND |
| IRQ | Not connected |

---

## nRF24L01 Module 2

| nRF24L01 Pin | ESP32 GPIO |
| ------------ | ---------- |
| CE | GPIO 13 |
| CSN / CS | GPIO 14 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | 3V3 |
| GND | GND |
| IRQ | Not connected |

---

## Shared SPI Pins for Both nRF24L01 Modules

Both nRF24L01 modules share the same SPI bus.

| SPI Signal | ESP32 GPIO |
| ---------- | ---------- |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |

Each nRF24L01 module has its own separate **CE** and **CSN / CS** pins.

| Module | CE Pin | CSN / CS Pin |
| ------ | ------ | ------------ |
| nRF24L01 Module 1 | GPIO 4 | GPIO 5 |
| nRF24L01 Module 2 | GPIO 13 | GPIO 14 |

---

## Full Power Flow

### Positive Power Line

```text
LiPo Battery +
   → DPDT switch middle pin
   → DPDT switch outer pin when ON
   → Boost Converter IN+
   → Boost Converter OUT+ 5V
   → ESP32 VIN / 5V
```

### Ground Line

```text
LiPo Battery -
   → Boost Converter IN-
   → Boost Converter OUT-
   → ESP32 GND
   → Common GND for OLED, buttons, and nRF24L01 modules
```

---

## Important Power Notes

1. The boost converter output goes to the ESP32 **VIN / 5V** pin.
2. Do **not** connect the 5V boost output directly to the ESP32 **3V3** pin.
3. The OLED screen should be powered from **3V3**.
4. A bare nRF24L01 module should be powered from **3V3**, not 5V.
5. Only connect an nRF24L01 module to 5V if you are using an adapter board that specifically supports 5V input.
6. Add a capacitor across each nRF24L01 power input if the modules are unstable.

Recommended capacitor for each nRF24L01 module:

| Capacitor | Connection |
| --------- | ---------- |
| 10µF to 47µF electrolytic capacitor | Between nRF VCC and GND |
| Capacitor + leg | nRF VCC |
| Capacitor - leg | nRF GND |

---

## Final Connection Summary

| Component | Power |
| --------- | ----- |
| ESP32 | 5V from boost converter to VIN / 5V |
| OLED | 3V3 from ESP32 |
| nRF24L01 Module 1 | 3V3 from ESP32 |
| nRF24L01 Module 2 | 3V3 from ESP32 |
| Buttons | GPIO to GND when pressed |
| Boost Converter | Powered from LiPo through DPDT switch |

---

## Simple Wiring Diagram

```text
        LiPo Battery
       +          -
       |          |
       |          +----------------------+
       |                                 |
       v                                 v
DPDT switch middle pin              Boost IN-
       |
       v
DPDT switch outer pin
       |
       v
Boost IN+

Boost OUT+ 5V ---------------------> ESP32 VIN / 5V
Boost OUT- ------------------------> ESP32 GND

ESP32 3V3 -------------------------> OLED VCC
ESP32 GND -------------------------> OLED GND

ESP32 3V3 -------------------------> nRF24L01 VCC
ESP32 GND -------------------------> nRF24L01 GND

Buttons:
GPIO pin --------------------------> Button
Button opposite leg ---------------> GND
```
