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
 * FILE REPORT: Probe traffic workflow API
 * EXAM CATEGORY: RISKY CONCEPT — DO NOT DEMO AS REAL PROBE FLOOD IN CLASS
 *
 * What this file does:
 *   - Declares action/result functions for probe traffic workflow page.
 *
 * Expected outcome when run:
 *   - No direct output; implementation handles OLED/counter flow.
 *
 * Viva explanation:
 *   - Prefer passive monitoring for a safer final project.
 *
 * TODO / improvement notes:
 *   - Use for explaining probe traffic concept and limits.
 *
 * Documentation note:
 *   - This file was documented from the original uploaded code structure.
 *   - No existing page/menu function has been removed by this documentation pass.
 */

#ifndef WIFI_PAGE_PROBE_TRAFFIC_DEMO_H
#define WIFI_PAGE_PROBE_TRAFFIC_DEMO_H

/**
 * Probe traffic workflow page.
 *
 * Counts probe-style progress events and periodically refreshes the OLED so the
 * user can see the flow without over-drawing the display.
 */
void wifiProbeFloodAction();
void wifiProbeFloodResult();

#endif
