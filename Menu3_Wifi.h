#ifndef MENU3_WIFI_H
#define MENU3_WIFI_H

// ================= WIFI MENU =================

int getWifiMenuCount();
void drawWifiMenu();
void selectWifiMenuItem();

// ================= COMMON WIFI ACTIVITY RUNNER =================

typedef void (*WifiActivityPart)();

void runWifiActivity(
  WifiActivityPart actionPart,
  WifiActivityPart resultPart
);

// ================= ACTIVITY 1: WIFI STATUS =================

void wifiStatusAction();
void wifiStatusResult();
void showWifiStatus();

// ================= ACTIVITY 2: SCAN NETWORKS =================

void drawWifiScanningScreen();

void wifiScanAction();
void wifiScanResult();
void scanWifiNetworks();

int getWifiScanMenuCount();
void drawWifiScanMenu();
void selectWifiScanMenuItem();

// ================= DISPLAY / DEMO PAGES =================
// These pages keep the same public function names as your original file.
// They are display-only and do not transmit, jam, spoof, deauth, flood, or create APs.

void wifiBeaconListAction();
void wifiBeaconListResult();

void wifiBeaconRandomAction();
void wifiBeaconRandomResult();

void wifiRickRollBeaconAction();
void wifiRickRollBeaconResult();

void wifiApCloneSpamAction();
void wifiApCloneSpamResult();
void wifiApCloneSpamAfterInput(const char* inputText);

void wifiEvilPortalAction();
void wifiEvilPortalResult();
void wifiEvilPortalAfterInput(const char* inputText);

void wifiProbeFloodAction();
void wifiProbeFloodResult();

void wifiDeauthTargetAction();
void wifiDeauthTargetResult();
void wifiDeauthTargetAfterInput(const char* inputText);

void wifiDeauthFloodAction();
void wifiDeauthFloodResult();

void wifiTwoFourGSweepAction();
void wifiTwoFourGSweepResult();

void wifiChannelHopAction();
void wifiChannelHopResult();

void wifiNrfJamTestAction();
void wifiNrfJamTestResult();

#endif
