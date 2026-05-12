Wi-Fi module split layout

Place these files in your existing ESP32_game_console sketch folder.

Important:
- Keep Menu3_Wifi.h and Menu3_Wifi.cpp in the sketch root.
- Keep the Wi-Fi folder under src:
    ESP32_game_console/src/Wifi/...
- Arduino IDE / Arduino CLI compile C/C++ files recursively under the sketch's src folder.
- Do not #include .cpp files.
- Only include .h files.

This split keeps one shared state file:
    src/Wifi/WifiState.cpp

That avoids duplicate RAM usage. Do not define the Wi-Fi arrays again elsewhere.

Functionality preserved from your provided Menu3_Wifi.cpp:
- WIFI STATUS checks local Wi-Fi state.
- SCAN NETWORKS performs a real Wi-Fi scan.
- Other pages are display-only demos and do not transmit, jam, spoof, deauth, flood, or create APs.

Page headers:
- WifiPage_BeaconList.h
- WifiPage_BeaconRandom.h
- WifiPage_RickRollBeacon.h
- WifiPage_ApClone.h
- WifiPage_PortalDemo.h
- WifiPage_ProbeTrafficDemo.h
- WifiPage_DeauthTargetDemo.h
- WifiPage_DeauthFloodDemo.h
- WifiPage_BandSweep.h
- WifiPage_ChannelHop.h
- WifiPage_NrfLinkTest.h

Aggregate include:
- WifiPages.h includes all page headers for the menu/router.


Compile fix:
- Added src/Wifi/WifiDisplayCompat.h.
- This explicitly declares showMessage(...) for .cpp files under src/Wifi because Arduino does not auto-generate prototypes there.
