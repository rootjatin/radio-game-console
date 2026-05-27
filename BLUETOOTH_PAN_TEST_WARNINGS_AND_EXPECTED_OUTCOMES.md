# Bluetooth / PAN Test Demo Warnings and Expected Outcomes

This guide is for your viva/demo. It keeps the project honest: safe pages can be
shown live, while active RF stress pages should be explained from source code
only unless your college provides an isolated authorised RF lab.

## Recommended live demo order

### 1. Bluetooth Sniff / Scan — live demo recommended

**Steps**
1. Turn on the ESP32 device.
2. Open the Bluetooth/PAN menu.
3. Select the Sniff/Scan option.
4. Turn on Bluetooth on your own phone.
5. Open phone Bluetooth settings or a BLE scanner app so the phone advertises
   more visibly.
6. Wait until the ESP32 finishes Classic and BLE scans.
7. Open the result list on the OLED.

**Expected outcome**
- OLED shows device names, type, RSSI/address summary, plus RESCAN/BACK.
- Serial Monitor prints Classic/BLE devices and BLE advertisement metadata.

**What to say**
- "This is passive discovery. It does not connect, pair, or modify devices."

---

### 2. Pair Audit / Visibility Audit — live demo recommended

**Steps**
1. Keep your own phone Bluetooth enabled.
2. Select Pair Audit from the Bluetooth/PAN menu.
3. Wait for the scan to complete.
4. Select a detected device from the OLED list.

**Expected outcome**
- OLED shows visible devices with type/RSSI/metadata.
- Serial Monitor prints the detailed visible-device audit.

**What to say**
- "This audits public visibility. A normal scan cannot prove real pairing
  status, so future work should rename this to Visibility Audit or add an
  authorised closed-pair test."

---

### 3. BLE Beacon — live demo recommended with consent

**Steps**
1. Open a BLE scanner app on your own phone.
2. Turn on Bluetooth and nearby-device permission.
3. Select BLE Beacon on the ESP32.
4. Watch for the ESP32 beacon/name/UUID in the phone app.
5. Let the ESP32 stop automatically.

**Expected outcome**
- OLED shows advertising status and countdown.
- BLE scanner shows the beacon advertisement.

**What to say**
- "The beacon advertises from our own ESP32 only. It is a controlled BLE
  advertisement demo."

---

### 4. RF Sweep — optional live demo if nRF24 wiring is stable

**Steps**
1. Connect nRF24L01 modules with stable 3.3 V power and common ground.
2. Select RF Sweep.
3. Wait through the scan.
4. Show the busiest channel result.

**Expected outcome**
- OLED shows scanning channel, rough activity percentage, remaining time, and
  final busiest channel.
- Serial Monitor prints the busiest channel and max activity.

**What to say**
- "This is an approximate carrier-activity teaching demo. nRF24 is not a
  calibrated spectrum analyser."

---

## Do not run live in a normal classroom

### Band Load Demo

**Reason**: active 2.4 GHz traffic can disturb nearby Wi-Fi/Bluetooth/2.4 GHz
systems. Keep it for code explanation only unless the test is inside an
isolated authorised RF lab.

**Expected outcome in isolated lab only**: OLED shows active transmit status,
remaining time, then cleanup; Serial Monitor prints packet count.

### Channel Hop

**Reason**: active rapid 2.4 GHz channel hopping can create RF noise. Explain
from code only during viva.

**Expected outcome in isolated lab only**: OLED shows current channel and
remaining time; Serial Monitor prints packet count.

### nRF Link/Jam Test

**Reason**: the current implementation is closer to a channel stress test than a
true closed link-quality test. Do not run live in class.

**Expected outcome in isolated lab only**: OLED shows selected channel and
remaining time; Serial Monitor prints packet count.

**Best improvement to mention**: convert this page into a closed TX/RX test
between your own two nRF24 modules using ACKs and packet-delivery ratio.

### BLE Name Test

**Reason**: it rapidly rotates advertised names and can clutter BLE scanner apps.
If shown at all, use only your own phone, neutral names, and permission.

**Expected outcome with consent only**: BLE scanner may show changing names from
the ESP32; OLED shows current advertised name and remaining time.

---

## Safe viva sentence

> "Our safe live demo uses passive discovery, visibility audit, BLE beacon, and
> optional passive RF sweep. Active RF stress pages are preserved in the source
> for review, but we do not run them in a classroom because they require an
> isolated authorised lab."
