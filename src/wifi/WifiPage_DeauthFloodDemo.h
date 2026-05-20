
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
 * FILE REPORT: Deauth flood workflow API
 * EXAM CATEGORY: RISKY AND NOT RECOMMENDED FOR LIVE CLASSROOM DEMO
 *
 * What this file does:
 *   - Declares action/result functions for deauth flood workflow page.
 *
 * Expected outcome when run:
 *   - No direct output; implementation handles OLED/counter flow.
 *
 * Viva explanation:
 *   - Better final project direction: passive deauth detection/alerting instead of active flood.
 *
 * TODO / improvement notes:
 *   - Keep as theory-only discussion item if required by syllabus.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

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
