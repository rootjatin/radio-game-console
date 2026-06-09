#pragma once

enum ScreenState {
  SCREEN_MAIN,
  SCREEN_BLUETOOTH,
  SCREEN_INPUT_KEYBOARD,
  SCREEN_WIFI,
  SCREEN_BT_SNIFF,
  SCREEN_WIFI_SCAN,   
  SCREEN_MESSAGE
};

extern ScreenState currentScreen;
extern ScreenState previousScreen;

extern int selectedIndex;

void drawCurrentScreen();
void showMessage(const char* title, const char* line1, const char* line2, const char* line3);
void goBack();

int getCurrentMenuCount();