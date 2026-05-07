
#include "OLED_Driver.h"
#include "AppState.h"

uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

// ================= BASIC OLED FUNCTIONS =================

void oledCommand(uint8_t cmd) {
  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x00);
  Wire.write(cmd);
  Wire.endTransmission();
}

void oledInit() {
  delay(100);

  oledCommand(0xAE);

  oledCommand(0xD5);
  oledCommand(0x80);

  oledCommand(0xA8);
  oledCommand(0x3F);

  oledCommand(0xD3);
  oledCommand(0x00);

  oledCommand(0x40);

#if OLED_IS_SH1106 == 0
  oledCommand(0x8D);
  oledCommand(0x14);

  oledCommand(0x20);
  oledCommand(0x02);
#else
  oledCommand(0xAD);
  oledCommand(0x8B);
#endif

  oledCommand(0xA1);
  oledCommand(0xC8);

  oledCommand(0xDA);
  oledCommand(0x12);

  oledCommand(0x81);
  oledCommand(0xCF);

  oledCommand(0xD9);
  oledCommand(0xF1);

  oledCommand(0xDB);
  oledCommand(0x40);

  oledCommand(0xA4);
  oledCommand(0xA6);

  oledCommand(0xAF);
}

void clearBuffer() {
  for (int i = 0; i < sizeof(buffer); i++) {
    buffer[i] = 0;
  }
}

void sendBuffer() {
  for (int page = 0; page < 8; page++) {
    oledCommand(0xB0 + page);

#if OLED_IS_SH1106 == 1
    oledCommand(0x02);
    oledCommand(0x10);
#else
    oledCommand(0x00);
    oledCommand(0x10);
#endif

    for (int x = 0; x < SCREEN_WIDTH; x += 16) {
      Wire.beginTransmission(OLED_ADDR);
      Wire.write(0x40);

      for (int i = 0; i < 16; i++) {
        Wire.write(buffer[page * SCREEN_WIDTH + x + i]);
      }

      Wire.endTransmission();
    }
  }
}

void oledClearScreen() {
  clearBuffer();
  sendBuffer();
}

void drawPixel(int x, int y, bool color) {
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;

  int index = x + (y / 8) * SCREEN_WIDTH;

  if (color) {
    buffer[index] |= (1 << (y % 8));
  } else {
    buffer[index] &= ~(1 << (y % 8));
  }
}

void drawRect(int x, int y, int w, int h, bool color) {
  for (int i = x; i < x + w; i++) {
    drawPixel(i, y, color);
    drawPixel(i, y + h - 1, color);
  }

  for (int j = y; j < y + h; j++) {
    drawPixel(x, j, color);
    drawPixel(x + w - 1, j, color);
  }
}

void fillRect(int x, int y, int w, int h, bool color) {
  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
      drawPixel(i, j, color);
    }
  }
}

// ================= FONT =================

void setFont(uint8_t f[5], uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) {
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
  f[4] = e;
}

void getFont(char ch, uint8_t f[5]) {
  if (ch >= 'a' && ch <= 'z') {
    ch = ch - 32;
  }

  switch (ch) {
    case 'A': setFont(f, 0x7E, 0x11, 0x11, 0x11, 0x7E); break;
    case 'B': setFont(f, 0x7F, 0x49, 0x49, 0x49, 0x36); break;
    case 'C': setFont(f, 0x3E, 0x41, 0x41, 0x41, 0x22); break;
    case 'D': setFont(f, 0x7F, 0x41, 0x41, 0x22, 0x1C); break;
    case 'E': setFont(f, 0x7F, 0x49, 0x49, 0x49, 0x41); break;
    case 'F': setFont(f, 0x7F, 0x09, 0x09, 0x09, 0x01); break;
    case 'G': setFont(f, 0x3E, 0x41, 0x49, 0x49, 0x7A); break;
    case 'H': setFont(f, 0x7F, 0x08, 0x08, 0x08, 0x7F); break;
    case 'I': setFont(f, 0x00, 0x41, 0x7F, 0x41, 0x00); break;
    case 'J': setFont(f, 0x20, 0x40, 0x41, 0x3F, 0x01); break;
    case 'K': setFont(f, 0x7F, 0x08, 0x14, 0x22, 0x41); break;
    case 'L': setFont(f, 0x7F, 0x40, 0x40, 0x40, 0x40); break;
    case 'M': setFont(f, 0x7F, 0x02, 0x04, 0x02, 0x7F); break;
    case 'N': setFont(f, 0x7F, 0x04, 0x08, 0x10, 0x7F); break;
    case 'O': setFont(f, 0x3E, 0x41, 0x41, 0x41, 0x3E); break;
    case 'P': setFont(f, 0x7F, 0x09, 0x09, 0x09, 0x06); break;
    case 'Q': setFont(f, 0x3E, 0x41, 0x51, 0x21, 0x5E); break;
    case 'R': setFont(f, 0x7F, 0x09, 0x19, 0x29, 0x46); break;
    case 'S': setFont(f, 0x46, 0x49, 0x49, 0x49, 0x31); break;
    case 'T': setFont(f, 0x01, 0x01, 0x7F, 0x01, 0x01); break;
    case 'U': setFont(f, 0x3F, 0x40, 0x40, 0x40, 0x3F); break;
    case 'V': setFont(f, 0x1F, 0x20, 0x40, 0x20, 0x1F); break;
    case 'W': setFont(f, 0x7F, 0x20, 0x18, 0x20, 0x7F); break;
    case 'X': setFont(f, 0x63, 0x14, 0x08, 0x14, 0x63); break;
    case 'Y': setFont(f, 0x07, 0x08, 0x70, 0x08, 0x07); break;
    case 'Z': setFont(f, 0x61, 0x51, 0x49, 0x45, 0x43); break;

    case '0': setFont(f, 0x3E, 0x51, 0x49, 0x45, 0x3E); break;
    case '1': setFont(f, 0x00, 0x42, 0x7F, 0x40, 0x00); break;
    case '2': setFont(f, 0x42, 0x61, 0x51, 0x49, 0x46); break;
    case '3': setFont(f, 0x21, 0x41, 0x45, 0x4B, 0x31); break;
    case '4': setFont(f, 0x18, 0x14, 0x12, 0x7F, 0x10); break;
    case '5': setFont(f, 0x27, 0x45, 0x45, 0x45, 0x39); break;
    case '6': setFont(f, 0x3C, 0x4A, 0x49, 0x49, 0x30); break;
    case '7': setFont(f, 0x01, 0x71, 0x09, 0x05, 0x03); break;
    case '8': setFont(f, 0x36, 0x49, 0x49, 0x49, 0x36); break;
    case '9': setFont(f, 0x06, 0x49, 0x49, 0x29, 0x1E); break;

    case ':': setFont(f, 0x00, 0x36, 0x36, 0x00, 0x00); break;
    case '-': setFont(f, 0x08, 0x08, 0x08, 0x08, 0x08); break;
    case '/': setFont(f, 0x20, 0x10, 0x08, 0x04, 0x02); break;
    case '.': setFont(f, 0x00, 0x60, 0x60, 0x00, 0x00); break;
    case '>': setFont(f, 0x41, 0x22, 0x14, 0x08, 0x00); break;
    case ' ': setFont(f, 0x00, 0x00, 0x00, 0x00, 0x00); break;

    default:
      setFont(f, 0x00, 0x00, 0x00, 0x00, 0x00);
      break;
  }
}

void drawChar(int x, int y, char ch, int scale, bool color) {
  uint8_t font[5];
  getFont(ch, font);

  for (int col = 0; col < 5; col++) {
    for (int row = 0; row < 7; row++) {
      if (font[col] & (1 << row)) {
        for (int sx = 0; sx < scale; sx++) {
          for (int sy = 0; sy < scale; sy++) {
            drawPixel(x + col * scale + sx, y + row * scale + sy, color);
          }
        }
      }
    }
  }
}

void drawText(int x, int y, const char* text, int scale, bool color) {
  for (int i = 0; text[i] != '\0'; i++) {
    drawChar(x + i * 6 * scale, y, text[i], scale, color);
  }
}

int textLength(const char* text) {
  int len = 0;

  while (text[len] != '\0') {
    len++;
  }

  return len;
}

void drawTextCentered(const char* text, int y, int scale, bool color) {
  int len = textLength(text);
  int textWidth = len * 6 * scale;
  int x = (SCREEN_WIDTH - textWidth) / 2;

  drawText(x, y, text, scale, color);
}

// ================= MENU DRAWING =================

void drawTopBar(const char* title) {
  fillRect(0, 0, 128, 12, true);

  drawText(3, 2, title, 1, false);

  // small radio/signal bars
  fillRect(108, 8, 3, 2, false);
  fillRect(113, 6, 3, 4, false);
  fillRect(118, 4, 3, 6, false);
  fillRect(123, 2, 3, 8, false);
}

// ================= FOOTER =================
//
// This appears below menu lists.
// It will show under Bluetooth and Wi-Fi lists.

void drawBottomHelp() {
  drawTextCentered("JATIN SHARMA", 56, 1, true);
}

// ================= SCROLLING MENU DRAWING =================
//
// Shows 3 rows at a time.
// Needed because Bluetooth and Wi-Fi menus have many items.

void drawMenuScreen(const char* title, const char* items[], int itemCount) {
  drawTopBar(title);

  int visibleRows = 3;
  int startIndex = 0;

  if (selectedIndex >= visibleRows) {
    startIndex = selectedIndex - visibleRows + 1;
  }

  int startY = 18;
  int rowHeight = 12;

  for (int row = 0; row < visibleRows; row++) {
    int itemIndex = startIndex + row;

    if (itemIndex >= itemCount) {
      break;
    }

    int y = startY + row * rowHeight;

    if (itemIndex == selectedIndex) {
      fillRect(0, y - 2, 128, 11, true);
      drawText(6, y, items[itemIndex], 1, false);
    } else {
      drawText(6, y, items[itemIndex], 1, true);
    }
  }

  drawBottomHelp();
}
