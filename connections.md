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

# nRF24L01 Connections

You can wire the nRF24L01 modules in **two ways**:

1. **With nRF24L01 adapter board** — recommended if you have the adapter.
2. **Without adapter board / bare nRF24L01 module** — use 3.3V only.

The SPI signal pins are the same in both cases.

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

## Option A: nRF24L01 Modules WITH Adapter Board

Use this section if each nRF24L01 module is plugged into an adapter board that accepts **5V input**.

### nRF24L01 Module 1 With Adapter

| nRF24L01 Adapter Pin | ESP32 GPIO / Pin |
| -------------------- | ---------------- |
| CE | GPIO 4 |
| CSN / CS | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | VIN / 5V |
| GND | GND |
| IRQ | Not connected |

### nRF24L01 Module 2 With Adapter

| nRF24L01 Adapter Pin | ESP32 GPIO / Pin |
| -------------------- | ---------------- |
| CE | GPIO 13 |
| CSN / CS | GPIO 14 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | VIN / 5V |
| GND | GND |
| IRQ | Not connected |

### Adapter Power Note

If your nRF adapter board has pins marked **VCC** and **GND**, connect:

```text
Adapter VCC → ESP32 VIN / 5V
Adapter GND → ESP32 GND
```

The adapter board normally regulates the 5V input down to 3.3V for the nRF24L01 module.

---

## Option B: nRF24L01 Modules WITHOUT Adapter Board

Use this section if you are using a **bare nRF24L01 module** without a voltage regulator adapter.

### nRF24L01 Module 1 Without Adapter

| nRF24L01 Pin | ESP32 GPIO / Pin |
| ------------ | ---------------- |
| CE | GPIO 4 |
| CSN / CS | GPIO 5 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | 3V3 |
| GND | GND |
| IRQ | Not connected |

### nRF24L01 Module 2 Without Adapter

| nRF24L01 Pin | ESP32 GPIO / Pin |
| ------------ | ---------------- |
| CE | GPIO 13 |
| CSN / CS | GPIO 14 |
| SCK | GPIO 18 |
| MOSI | GPIO 23 |
| MISO | GPIO 19 |
| VCC | 3V3 |
| GND | GND |
| IRQ | Not connected |

### Bare nRF24L01 Power Warning

A bare nRF24L01 module must use **3.3V only**.

```text
Bare nRF24L01 VCC → ESP32 3V3
Bare nRF24L01 GND → ESP32 GND
```

Do **not** connect a bare nRF24L01 module directly to 5V.

---

## Capacitor Recommendation for nRF24L01

If the nRF24L01 modules are unstable, add a capacitor across the power pins of each module or adapter.

| Capacitor | Connection |
| --------- | ---------- |
| 10µF to 47µF electrolytic capacitor | Between nRF VCC and GND |
| Capacitor + leg | nRF VCC |
| Capacitor - leg | nRF GND |

For adapter boards, place the capacitor between the adapter **VCC** and **GND** pins.

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
4. If using an **nRF24L01 adapter board**, connect adapter **VCC** to **VIN / 5V**.
5. If using a **bare nRF24L01 module**, connect nRF **VCC** to **3V3 only**.
6. All grounds must be connected together.

---

## Final Connection Summary

| Component | Power |
| --------- | ----- |
| ESP32 | 5V from boost converter to VIN / 5V |
| OLED | 3V3 from ESP32 |
| nRF24L01 Module 1 with adapter | VIN / 5V from ESP32 |
| nRF24L01 Module 2 with adapter | VIN / 5V from ESP32 |
| nRF24L01 Module 1 without adapter | 3V3 from ESP32 |
| nRF24L01 Module 2 without adapter | 3V3 from ESP32 |
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

With nRF adapter:
ESP32 VIN / 5V --------------------> nRF Adapter VCC
ESP32 GND -------------------------> nRF Adapter GND

Without nRF adapter:
ESP32 3V3 -------------------------> nRF24L01 VCC
ESP32 GND -------------------------> nRF24L01 GND

Buttons:
GPIO pin --------------------------> Button
Button opposite leg ---------------> GND
```
