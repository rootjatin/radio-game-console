#include "AppState.h"
#include "OLED_Driver.h"
#include "Menu1_Main.h"

const char* mainMenu[] = {
  "BLUETOOTH",
  "WIFI"
};

const int mainMenuCount = sizeof(mainMenu) / sizeof(mainMenu[0]);

int getMainMenuCount() {
  return mainMenuCount;
}

void drawMainMenu() {
  drawMenuScreen("MAIN MENU", mainMenu, mainMenuCount);
}

void selectMainMenuItem() {
  if (selectedIndex == 0) {
    currentScreen = SCREEN_BLUETOOTH;
  } 
  else if (selectedIndex == 1) {
    currentScreen = SCREEN_WIFI;
  }

  selectedIndex = 0;
  drawCurrentScreen();
}
