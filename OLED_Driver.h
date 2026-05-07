#pragma once

#include <Arduino.h>
#include <Wire.h>

// ================= OLED SETTINGS =================
#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Change to 1 if your OLED is SH1106 instead of SSD1306
#define OLED_IS_SH1106 0

extern uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

void oledCommand(uint8_t cmd);
void oledInit();

void clearBuffer();
void sendBuffer();
void oledClearScreen();

void drawPixel(int x, int y, bool color);
void drawRect(int x, int y, int w, int h, bool color);
void fillRect(int x, int y, int w, int h, bool color);

void drawChar(int x, int y, char ch, int scale, bool color);
void drawText(int x, int y, const char* text, int scale, bool color);
void drawTextCentered(const char* text, int y, int scale, bool color);

void drawTopBar(const char* title);
void drawBottomHelp();
void drawMenuScreen(const char* title, const char* items[], int itemCount);
