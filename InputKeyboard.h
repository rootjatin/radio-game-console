#ifndef INPUT_KEYBOARD_H
#define INPUT_KEYBOARD_H

#include "AppState.h"

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
void inputKeyboardRight();
void inputKeyboardCancel();

const char* getInputKeyboardText();

#endif