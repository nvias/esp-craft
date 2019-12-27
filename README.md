Arduino CLI
===========

Compile
-------
 - NodeMCU `arduino-cli compile -b esp8266:esp8266:nodemcuv2`
 - WeMOS D1 `arduino-cli compile -b esp8266:esp8266:d1`
 - WeMOS D1 mini `arduino-cli compile -b esp8266:esp8266:d1_mini`
 
Upload
------
 - NodeMCU `arduino-cli upload -p /dev/ttyUSB0 -b esp8266:esp8266:nodemcuv2:baud=921600`
 - WeMOS D1 `arduino-cli upload -p /dev/ttyUSB0 -b esp8266:esp8266:d1:baud=921600`
 - WeMOS D1 mini `arduino-cli upload -p /dev/ttyUSB0 -b esp8266:esp8266:d1_mini:baud=921600`
  
 Setup WiFi and Module ID
 ------------------------
 - Setup wifi in config.h JMENO_WIFI a HESLO_NA_WIFI a MODULE_ID
```
#define WIFI_SSID "JMENO_WIFI"
#define WIFI_PASSWORD "HESLO_NA_WIFI"
#define MODULE_NUMBER "MODULE_ID"
```

- open ESPCraft.ino v Arduino ide
- upload the firmware to the ESP board
