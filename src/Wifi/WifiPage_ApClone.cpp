#include "WifiPage_ApClone.h"
#include "WifiReportUI.h"
#include "../../InputKeyboard.h"
#include "../../AppState.h"
#include "WifiState.h"

#include <Arduino.h>
#include <string.h>

namespace {
const unsigned long kStageDelayMs = 900;
}

void wifiApCloneSpamResult() {
  renderWifiOperationResult("AP CLONE");
}

void wifiApCloneSpamAfterInput(const char* inputText) {
  char cleanSSID[WIFI_LABEL_LEN];
  wifiCopyCleanText(cleanSSID, inputText, sizeof(cleanSSID));

  if (strlen(cleanSSID) == 0) {
    wifiCopyCleanText(cleanSSID, "CLONED_AP", sizeof(cleanSSID));
  }

  renderWifiOperationStage("AP CLONE", "SSID SELECTED", cleanSSID, "PREPARING");
  delay(kStageDelayMs);

  renderWifiOperationStage("AP CLONE", "QUEUE READY", cleanSSID, "VERIFY SETTINGS");
  delay(kStageDelayMs);

  renderWifiOperationStage("AP CLONE", "CLONE FLOW", "COMPLETE", cleanSSID);
  delay(kStageDelayMs);

  wifiApCloneSpamResult();
}

void wifiApCloneSpamAction() {
  openInputKeyboard(
    "AP CLONE",
    "ENTER SSID TO CLONE",
    wifiApCloneSpamAfterInput,
    SCREEN_WIFI
  );
}
