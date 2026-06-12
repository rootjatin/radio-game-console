#ifndef WIFI_PAGE_DEAUTH_FLOOD_DEMO_H
#define WIFI_PAGE_DEAUTH_FLOOD_DEMO_H

/**
 * Deauth flood workflow page.
 *
 * Maintains the original counter/status sequence and renders progress to the
 * OLED. Constants in the .cpp control page duration and report density.
 */
void wifiDeauthFloodAction();
void wifiDeauthFloodResult();

#endif
