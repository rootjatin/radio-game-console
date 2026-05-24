/*
 * FILE REPORT: Wi-Fi OLED report renderer implementation
 * EXAM CATEGORY: CORE HELPER FILE — KEEP
 *
 * What this file does:
 *   - Draws a title bar and up to three information lines on the 128x64 OLED.
 *   - Used by most Wi-Fi/PAN test pages for consistent progress/result screens.
 *
 * Expected outcome when run:
 *   - When called, OLED updates with title, stage text, and optional final status lines.
 *
 * Viva explanation:
 *   - Future improvement: add optional String overload if more pages pass Arduino String expressions directly.
 *   - Future improvement: add automatic text scrolling/truncation for long SSIDs.
 *
 * TODO / improvement notes:
 *   - This helper standardizes UI so every Wi-Fi page looks consistent during demo.
 *   - It does not perform network actions; it only renders information.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#include "WifiReportUI.h"
#include "WifiDisplayCompat.h"

void renderWifiOperationStage(
  const char* title,
  const char* line1,
  const char* line2,
  const char* line3
) {
  clearBuffer();
  drawTopBar(title ? title : "WIFI");

  if (line1 && line1[0] != '\0') drawText(6, 18, line1, 1, true);
  if (line2 && line2[0] != '\0') drawText(6, 32, line2, 1, true);
  if (line3 && line3[0] != '\0') drawText(6, 46, line3, 1, true);

  sendBuffer();
}

void renderWifiOperationResult(const char* title) {
  showMessage(
    title ? title : "WIFI",
    "OPERATION COMPLETE",
    "CHECK SERIAL LOG",
    "BACK TO MENU"
  );
}
