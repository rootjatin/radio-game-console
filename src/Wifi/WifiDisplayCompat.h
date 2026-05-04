#ifndef WIFI_DISPLAY_COMPAT_H
#define WIFI_DISPLAY_COMPAT_H

#include "../../OLED_Driver.h"

/**
 * Compatibility declaration for projects where showMessage() is implemented in
 * the main sketch or display module instead of OLED_Driver.h.
 */
void showMessage(
  const char* title,
  const char* line1,
  const char* line2,
  const char* line3
);

#endif
