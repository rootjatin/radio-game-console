#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include "AppState.h"

// ================= INPUT KEYBOARD =================
//
// Reusable text input screen.
// Can be used by Bluetooth, Wi-Fi, game, or other activity sections.

typedef void (*InputKeyboardDoneCallback)(const char* inputText);

void openInputKeyboard(
  const char* title,
  const char* prompt,
  InputKeyboardDoneCallback doneCallback,
  ScreenState returnScreen
);

int getInputKeyboardMenuCount();
void drawInputKeyboard();
void selectInputKeyboardItem();
void inputKeyboardBack();

const char* getInputKeyboardText();

#endif