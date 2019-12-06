#ifndef config_h
#define config_h

#define WIFI_SSID "WIFI_NAME"
#define WIFI_PASSWORD "WIFI_PASSWORD"

#define MODULE_NUMBER "4"

#define MQTT_SERVER "52.59.197.227"
#define MQTT_PORT 1884

#define MQTT_PREFIX "nvias/MC"

#define SUB_TOPIC MQTT_PREFIX "/M" MODULE_NUMBER "/#"
#define PUB_TOPIC MQTT_PREFIX "/M" MODULE_NUMBER

// Buttons topic listeners

#define BTN_1_PUB_TOPIC PUB_TOPIC "/D1"
#define BTN_2_PUB_TOPIC PUB_TOPIC "/D2"

// LED topic listeners

#define LED_SUB_TOPIC PUB_TOPIC "/D3"

#endif