#include "InputKeyboard.h"
#include "OLED_Driver.h"
#include "AppState.h"

#include <Arduino.h>
#include <string.h>

#define INPUT_TEXT_LEN 17

const char inputKeys[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.-/ ";
const int INPUT_KEY_COUNT = sizeof(inputKeys) - 1;

char inputKeyboardTitle[18];
char inputKeyboardPrompt[18];
char inputKeyboardText[INPUT_TEXT_LEN];

InputKeyboardDoneCallback inputDoneCallback = NULL;
ScreenState inputReturnScreen = SCREEN_MAIN;

void inputCopyText(char* dest, const char* src, int maxLen) {
  int i = 0;

  if (src == NULL) {
    dest[0] = '\0';
    return;
  }

  while (src[i] != '\0' && i < maxLen - 1) {
    dest[i] = src[i];
    i++;
  }

  dest[i] = '\0';
}

void inputAddChar(char c) {
  int len = strlen(inputKeyboardText);

  if (len >= INPUT_TEXT_LEN - 1) {
    return;
  }

  inputKeyboardText[len] = c;
  inputKeyboardText[len + 1] = '\0';
}

void inputDeleteChar() {
  int len = strlen(inputKeyboardText);

  if (len <= 0) {
    return;
  }

  inputKeyboardText[len - 1] = '\0';
}

void inputSubmitText() {
  InputKeyboardDoneCallback callback = inputDoneCallback;

  currentScreen = inputReturnScreen;
  selectedIndex = 0;

  if (callback != NULL) {
    callback(inputKeyboardText);
  } else {
    drawCurrentScreen();
  }
}

void getSelectedKeyboardLabel(char* label, int maxLen) {
  if (selectedIndex < 0) {
    selectedIndex = 0;
  }

  if (selectedIndex >= INPUT_KEY_COUNT) {
    selectedIndex = INPUT_KEY_COUNT - 1;
  }

  char c = inputKeys[selectedIndex];

  if (c == ' ') {
    inputCopyText(label, "SPACE", maxLen);
  } else {
    label[0] = c;
    label[1] = '\0';
  }
}

void openInputKeyboard(
  const char* title,
  const char* prompt,
  InputKeyboardDoneCallback doneCallback,
  ScreenState returnScreen
) {
  inputCopyText(inputKeyboardTitle, title, sizeof(inputKeyboardTitle));
  inputCopyText(inputKeyboardPrompt, prompt, sizeof(inputKeyboardPrompt));

  inputKeyboardText[0] = '\0';

  inputDoneCallback = doneCallback;
  inputReturnScreen = returnScreen;

  currentScreen = SCREEN_INPUT_KEYBOARD;
  selectedIndex = 0;

  drawCurrentScreen();
}

int getInputKeyboardMenuCount() {
  return INPUT_KEY_COUNT;
}

const char* getInputKeyboardText() {
  return inputKeyboardText;
}

void drawInputKeyboard() {
  char keyLabel[18];

  if (selectedIndex < 0) {
    selectedIndex = 0;
  }

  if (selectedIndex >= INPUT_KEY_COUNT) {
    selectedIndex = INPUT_KEY_COUNT - 1;
  }

  drawTopBar(inputKeyboardTitle);

  drawText(2, 15, inputKeyboardPrompt, 1, true);

  drawText(2, 27, "IN:", 1, true);
  drawText(26, 27, inputKeyboardText, 1, true);

  getSelectedKeyboardLabel(keyLabel, sizeof(keyLabel));

  fillRect(0, 40, 128, 12, true);

  char selectedLine[18];
  snprintf(selectedLine, sizeof(selectedLine), "[ %s ]", keyLabel);
  drawTextCentered(selectedLine, 42, 1, false);

  drawText(0, 56, "SEL ADD L DEL R OK", 1, true);
}

void selectInputKeyboardItem() {
  if (selectedIndex < 0) {
    selectedIndex = 0;
  }

  if (selectedIndex >= INPUT_KEY_COUNT) {
    selectedIndex = INPUT_KEY_COUNT - 1;
  }

  inputAddChar(inputKeys[selectedIndex]);
  drawCurrentScreen();
}

void inputKeyboardBack() {
  inputDeleteChar();
  drawCurrentScreen();
}

void inputKeyboardRight() {
  inputSubmitText();
}

void inputKeyboardCancel() {
  currentScreen = inputReturnScreen;
  selectedIndex = 0;
  drawCurrentScreen();
}