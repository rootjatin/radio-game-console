/*
 * CODE WORKS, BUT DON'T RUN THAT LIVE IN CLASSROOM.
 * USE THIS ONLY FOR AUTHORIZED EQUIPMENT TESTING IN A CONTROLLED LAB.
 *
 * Medical/critical-equipment testing rule:
 *   - Use only your own isolated test router/device, dummy client, or approved lab equipment.
 *   - Do not run near classmates, public networks, patient-care devices, or production systems.
 *   - Keep Serial Monitor open and record operator, date, target device, and professor approval.
 *   - Stop immediately if unintended devices are affected.
 */

/*
 * FILE REPORT: Targeted deauth workflow page
 * EXAM CATEGORY: RISKY AND NOT RECOMMENDED FOR LIVE CLASSROOM DEMO
 *
 * What this file does:
 *   - Accepts a target SSID/name from keyboard and shows staged OLED workflow messages.
 *   - This file does not include low-level packet transmit calls in the uploaded source.
 *
 * Expected outcome when run:
 *   - OLED asks for target, then shows TARGET SELECTED, CLIENT FLOW IN PROGRESS, and TARGET FLOW DONE.
 *   - Expected observable result is screen workflow only unless other project code performs authorized testing.
 *
 * Viva explanation:
 *   - Risk if made active: targeted deauth can disrupt someone else’s connection.
 *   - Recommendation: ask professor to remove from live practical or keep as code-review-only.
 *   - Future improvement: convert to passive PMF/security-capability audit.
 *
 * TODO / improvement notes:
 *   - Use as theory explanation for why management-frame protection matters.
 *   - Do not demo against real networks or classmates.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#include "WifiPage_DeauthTargetDemo.h"
#include "WifiReportUI.h"
#include "../../InputKeyboard.h"
#include "../../AppState.h"
#include "WifiState.h"

#include <Arduino.h>
#include <string.h>

namespace {
const unsigned long kStageDelayMs = 900;

void showAuthorizedEquipmentWarning() {
  renderWifiOperationStage("DEAUTH TARGET", "AUTHORIZED LAB", "EQUIPMENT ONLY", "DO NOT RUN IN CLASS");
  delay(1200);
  renderWifiOperationStage("DEAUTH TARGET", "CONFIRM TARGET", "OWN TEST DEVICE", "SERIAL LOG ON");
  delay(1200);
}
}

void wifiDeauthTargetResult() {
  renderWifiOperationResult("DEAUTH TARGET");
}

void wifiDeauthTargetAfterInput(const char* inputText) {
  char cleanTarget[WIFI_LABEL_LEN];
  wifiCopyCleanText(cleanTarget, inputText, sizeof(cleanTarget));

  if (strlen(cleanTarget) == 0) {
    wifiCopyCleanText(cleanTarget, "UNKNOWN_TARGET", sizeof(cleanTarget));
  }

  renderWifiOperationStage("DEAUTH TARGET", "TARGET SELECTED", cleanTarget, "LOCKING");
  delay(kStageDelayMs);

  renderWifiOperationStage("DEAUTH TARGET", "CLIENT FLOW", "IN PROGRESS", "COUNTER READY");
  delay(kStageDelayMs);

  renderWifiOperationStage("DEAUTH TARGET", "TARGET FLOW DONE", cleanTarget, "COMPLETE");
  delay(kStageDelayMs);

  wifiDeauthTargetResult();
}

void wifiDeauthTargetAction() {
  showAuthorizedEquipmentWarning();
  openInputKeyboard(
    "DEAUTH TARGET",
    "ENTER TARGET SSID",
    wifiDeauthTargetAfterInput,
    SCREEN_WIFI
  );
}
