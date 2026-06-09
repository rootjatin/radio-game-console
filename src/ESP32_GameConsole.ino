#include <Wire.h>

#include "AppState.h"
#include "OLED_Driver.h"

#include "Menu1_Main.h"
#include "Menu2_Bluetooth.h"
#include "Menu3_Wifi.h"

#include "InputKeyboard.h"

// ================= BUTTON PINS =================
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_LEFT   25
#define BTN_RIGHT  26
#define BTN_FIRE   27

#define DEBOUNCE_MS 50

ScreenState currentScreen = SCREEN_MAIN;
ScreenState previousScreen = SCREEN_MAIN;

int selectedIndex = 0;

// ================= BUTTON DEBOUNCE STATE =================

struct ButtonDebounce {
  int pin;
  bool stableState;
  bool lastReading;
  unsigned long lastChangeTime;
};

ButtonDebounce buttons[] = {
  { BTN_UP,    HIGH, HIGH, 0 },
  { BTN_DOWN,  HIGH, HIGH, 0 },
  { BTN_LEFT,  HIGH, HIGH, 0 },
  { BTN_RIGHT, HIGH, HIGH, 0 },
  { BTN_FIRE,  HIGH, HIGH, 0 }
};

const int buttonCount = sizeof(buttons) / sizeof(buttons[0]);

// ================= FORWARD DECLARATIONS =================

void handleButtons();
bool buttonPressed(int pin);

int getCurrentMenuCount();

void selectCurrentItem();
void selectMainSection();
void selectBluetoothSection();
void selectBluetoothSniffSection();
void selectWifiSection();
void selectWifiScanSection();
void selectInputKeyboardSection();
void selectMessageSection();

void goBack();
void goBackFromMessage();
void goBackFromBluetoothSniff();
void goBackFromWifiScan();
void goBackFromInputKeyboard();
void goBackToMain();

void drawCurrentScreen();
void drawMainSection();
void drawBluetoothSection();
void drawBluetoothSniffSection();
void drawWifiSection();
void drawWifiScanSection();
void drawInputKeyboardSection();

void showMessage(const char* title, const char* line1, const char* line2, const char* line3);
void showStartupScreen();

// ================= SETUP =================

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_FIRE, INPUT_PULLUP);

  oledInit();
  oledClearScreen();

  showStartupScreen();
  delay(1200);

  drawCurrentScreen();
}

// ================= LOOP =================

void loop() {
  handleButtons();
}

// ================= BUTTON HANDLING =================

bool buttonPressed(int pin) {
  for (int i = 0; i < buttonCount; i++) {
    if (buttons[i].pin != pin) {
      continue;
    }

    bool reading = digitalRead(pin);

    if (reading != buttons[i].lastReading) {
      buttons[i].lastChangeTime = millis();
      buttons[i].lastReading = reading;
    }

    if ((millis() - buttons[i].lastChangeTime) > DEBOUNCE_MS) {
      if (reading != buttons[i].stableState) {
        buttons[i].stableState = reading;

        if (buttons[i].stableState == LOW) {
          return true;
        }
      }
    }

    return false;
  }

  return false;
}

void handleButtons() {
  int itemCount = getCurrentMenuCount();

  if (buttonPressed(BTN_UP)) {
    if (currentScreen == SCREEN_MESSAGE) return;
    if (itemCount <= 0) return;

    selectedIndex--;

    if (selectedIndex < 0) {
      selectedIndex = itemCount - 1;
    }

    drawCurrentScreen();
    return;
  }

  if (buttonPressed(BTN_DOWN)) {
    if (currentScreen == SCREEN_MESSAGE) return;
    if (itemCount <= 0) return;

    selectedIndex++;

    if (selectedIndex >= itemCount) {
      selectedIndex = 0;
    }

    drawCurrentScreen();
    return;
  }

  if (buttonPressed(BTN_RIGHT)) {
    if (currentScreen == SCREEN_INPUT_KEYBOARD) {
      inputKeyboardRight();     // RIGHT = submit typed string
    } else {
      selectCurrentItem();      // RIGHT = OK on normal menus
    }

    return;
  }

  if (buttonPressed(BTN_FIRE)) {
    selectCurrentItem();        // FIRE = add selected character on keyboard
    return;
  }

  if (buttonPressed(BTN_LEFT)) {
    goBack();                   // LEFT = backspace on keyboard
    return;
  }
}

// ================= MENU COUNT ROUTER =================

int getCurrentMenuCount() {
  switch (currentScreen) {
    case SCREEN_MAIN:
      return getMainMenuCount();

    case SCREEN_BLUETOOTH:
      return getBluetoothMenuCount();

    case SCREEN_BT_SNIFF:
      return getBluetoothSniffMenuCount();

    case SCREEN_WIFI:
      return getWifiMenuCount();

    case SCREEN_WIFI_SCAN:
      return getWifiScanMenuCount();

    case SCREEN_INPUT_KEYBOARD:
      return getInputKeyboardMenuCount();

    default:
      return 0;
  }
}

// ================= SELECT ROUTER =================

void selectCurrentItem() {
  switch (currentScreen) {
    case SCREEN_MAIN:
      selectMainSection();
      break;

    case SCREEN_BLUETOOTH:
      selectBluetoothSection();
      break;

    case SCREEN_BT_SNIFF:
      selectBluetoothSniffSection();
      break;

    case SCREEN_WIFI:
      selectWifiSection();
      break;

    case SCREEN_WIFI_SCAN:
      selectWifiScanSection();
      break;

    case SCREEN_INPUT_KEYBOARD:
      selectInputKeyboardSection();
      break;

    case SCREEN_MESSAGE:
      selectMessageSection();
      break;
  }
}

void selectMainSection() {
  selectMainMenuItem();
}

void selectBluetoothSection() {
  selectBluetoothMenuItem();
}

void selectBluetoothSniffSection() {
  selectBluetoothSniffMenuItem();
}

void selectWifiSection() {
  selectWifiMenuItem();
}

void selectWifiScanSection() {
  selectWifiScanMenuItem();
}

void selectInputKeyboardSection() {
  selectInputKeyboardItem();
}

void selectMessageSection() {
  currentScreen = previousScreen;
  drawCurrentScreen();
}

// ================= BACK ROUTER =================

void goBack() {
  if (currentScreen == SCREEN_MAIN) {
    return;
  }

  if (currentScreen == SCREEN_MESSAGE) {
    goBackFromMessage();
    return;
  }

  if (currentScreen == SCREEN_BT_SNIFF) {
    goBackFromBluetoothSniff();
    return;
  }

  if (currentScreen == SCREEN_WIFI_SCAN) {
    goBackFromWifiScan();
    return;
  }

  if (currentScreen == SCREEN_INPUT_KEYBOARD) {
    goBackFromInputKeyboard();
    return;
  }

  goBackToMain();
}

void goBackFromMessage() {
  currentScreen = previousScreen;
  drawCurrentScreen();
}

void goBackFromBluetoothSniff() {
  currentScreen = SCREEN_BLUETOOTH;
  selectedIndex = 0;
  drawCurrentScreen();
}

void goBackFromWifiScan() {
  currentScreen = SCREEN_WIFI;
  selectedIndex = 0;
  drawCurrentScreen();
}

void goBackFromInputKeyboard() {
  inputKeyboardBack();
}

void goBackToMain() {
  currentScreen = SCREEN_MAIN;
  selectedIndex = 0;
  drawCurrentScreen();
}

// ================= DRAW ROUTER =================

void drawCurrentScreen() {
  clearBuffer();

  switch (currentScreen) {
    case SCREEN_MAIN:
      drawMainSection();
      break;

    case SCREEN_BLUETOOTH:
      drawBluetoothSection();
      break;

    case SCREEN_BT_SNIFF:
      drawBluetoothSniffSection();
      break;

    case SCREEN_WIFI:
      drawWifiSection();
      break;

    case SCREEN_WIFI_SCAN:
      drawWifiScanSection();
      break;

    case SCREEN_INPUT_KEYBOARD:
      drawInputKeyboardSection();
      break;

    default:
      break;
  }

  sendBuffer();
}

void drawMainSection() {
  drawMainMenu();
}

void drawBluetoothSection() {
  drawBluetoothMenu();
}

void drawBluetoothSniffSection() {
  drawBluetoothSniffMenu();
}

void drawWifiSection() {
  drawWifiMenu();
}

void drawWifiScanSection() {
  drawWifiScanMenu();
}

void drawInputKeyboardSection() {
  drawInputKeyboard();
}

// ================= MESSAGE SCREEN =================

void showMessage(const char* title, const char* line1, const char* line2, const char* line3) {
  previousScreen = currentScreen;
  currentScreen = SCREEN_MESSAGE;

  clearBuffer();

  drawTopBar(title);

  drawText(6, 18, line1, 1, true);
  drawText(6, 30, line2, 1, true);
  drawText(6, 42, line3, 1, true);

  drawText(0, 56, "LEFT BACK  FIRE OK", 1, true);

  sendBuffer();
}

// ================= STARTUP SCREEN =================

void showStartupScreen() {
  clearBuffer();

  drawTextCentered("ESP32 RADIO", 8, 1, true);
  drawTextCentered("GAME CONSOLE", 20, 1, true);

  drawRect(12, 34, 104, 16, true);
  drawTextCentered("LOADING", 39, 1, true);

  drawTextCentered("OLED READY", 56, 1, true);

  sendBuffer();
}