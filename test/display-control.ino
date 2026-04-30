#include <Wire.h>

// ================= OLED SETTINGS =================
#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// If screen is still noisy, change this from 0 to 1
// 0 = SSD1306 OLED
// 1 = SH1106 OLED
#define OLED_IS_SH1106 0

// ================= BUTTON PINS =================
#define BTN_UP     32
#define BTN_DOWN   33
#define BTN_LEFT   25
#define BTN_RIGHT  26
#define BTN_FIRE   27

uint8_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT / 8];

String lastText = "";

// ================= BASIC OLED FUNCTIONS =================

void oledCommand(uint8_t cmd) {
  Wire.beginTransmission(OLED_ADDR);
  Wire.write(0x00);   // command mode
  Wire.write(cmd);
  Wire.endTransmission();
}

void oledInit() {
  delay(100);

  oledCommand(0xAE); // display off

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
  oledCommand(0x02); // page addressing mode
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
  oledCommand(0xA6); // normal display, not inverted

  oledCommand(0xAF); // display on
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
    oledCommand(0x02); // SH1106 column offset
    oledCommand(0x10);
#else
    oledCommand(0x00);
    oledCommand(0x10);
#endif

    for (int x = 0; x < SCREEN_WIDTH; x += 16) {
      Wire.beginTransmission(OLED_ADDR);
      Wire.write(0x40); // data mode

      for (int i = 0; i < 16; i++) {
        Wire.write(buffer[page * SCREEN_WIDTH + x + i]);
      }

      Wire.endTransmission();
    }
  }
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

// ================= SIMPLE FONT =================

void setFont(uint8_t f[5], uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint8_t e) {
  f[0] = a;
  f[1] = b;
  f[2] = c;
  f[3] = d;
  f[4] = e;
}

void getFont(char ch, uint8_t f[5]) {
  switch (ch) {
    case 'A': setFont(f, 0x7E, 0x11, 0x11, 0x11, 0x7E); break;
    case 'D': setFont(f, 0x7F, 0x41, 0x41, 0x22, 0x1C); break;
    case 'E': setFont(f, 0x7F, 0x49, 0x49, 0x49, 0x41); break;
    case 'F': setFont(f, 0x7F, 0x09, 0x09, 0x09, 0x01); break;
    case 'G': setFont(f, 0x3E, 0x41, 0x49, 0x49, 0x7A); break;
    case 'H': setFont(f, 0x7F, 0x08, 0x08, 0x08, 0x7F); break;
    case 'I': setFont(f, 0x00, 0x41, 0x7F, 0x41, 0x00); break;
    case 'L': setFont(f, 0x7F, 0x40, 0x40, 0x40, 0x40); break;
    case 'N': setFont(f, 0x7F, 0x02, 0x0C, 0x10, 0x7F); break;
    case 'O': setFont(f, 0x3E, 0x41, 0x41, 0x41, 0x3E); break;
    case 'P': setFont(f, 0x7F, 0x09, 0x09, 0x09, 0x06); break;
    case 'R': setFont(f, 0x7F, 0x09, 0x19, 0x29, 0x46); break;
    case 'T': setFont(f, 0x01, 0x01, 0x7F, 0x01, 0x01); break;
    case 'U': setFont(f, 0x3F, 0x40, 0x40, 0x40, 0x3F); break;
    case 'W': setFont(f, 0x7F, 0x20, 0x18, 0x20, 0x7F); break;
    case 'Y': setFont(f, 0x07, 0x08, 0x70, 0x08, 0x07); break;
    default:  setFont(f, 0x00, 0x00, 0x00, 0x00, 0x00); break;
  }
}

void drawChar(int x, int y, char ch, int scale) {
  uint8_t font[5];
  getFont(ch, font);

  for (int col = 0; col < 5; col++) {
    for (int row = 0; row < 7; row++) {
      if (font[col] & (1 << row)) {
        for (int sx = 0; sx < scale; sx++) {
          for (int sy = 0; sy < scale; sy++) {
            drawPixel(x + col * scale + sx, y + row * scale + sy, true);
          }
        }
      }
    }
  }
}

int textLength(const char *text) {
  int len = 0;
  while (text[len] != '\0') len++;
  return len;
}

void drawTextCentered(const char *text, int y, int scale) {
  int len = textLength(text);
  int textWidth = len * 6 * scale;
  int x = (SCREEN_WIDTH - textWidth) / 2;

  for (int i = 0; text[i] != '\0'; i++) {
    drawChar(x + i * 6 * scale, y, text[i], scale);
  }
}

void showText(const char *text) {
  if (lastText == String(text)) return;

  lastText = String(text);

  clearBuffer();
  drawTextCentered(text, 22, 3);
  sendBuffer();
}

// ================= BUTTON READING =================

const char* readButton() {
  if (digitalRead(BTN_UP) == LOW) return "UP";
  if (digitalRead(BTN_DOWN) == LOW) return "DOWN";
  if (digitalRead(BTN_LEFT) == LOW) return "LEFT";
  if (digitalRead(BTN_RIGHT) == LOW) return "RIGHT";
  if (digitalRead(BTN_FIRE) == LOW) return "FIRE";

  return "READY";
}

// ================= SETUP AND LOOP =================

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_FIRE, INPUT_PULLUP);

  oledInit();

  clearBuffer();
  sendBuffer();

  showText("READY");
}

void loop() {
  const char* buttonText = readButton();

  Serial.println(buttonText);
  showText(buttonText);

  delay(50);
}
