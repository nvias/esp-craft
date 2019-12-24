#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include <stdlib.h>
#include "PubSubClient.h"
#include "config.h"

#define BTN1 D1
#define BTN2 D2

#define LED_PIN D3
#define LED_COUNT 2

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* mqtt_server = MQTT_SERVER;
const uint16_t mqtt_port = MQTT_PORT;

char* subTopic = SUB_TOPIC;
char* pubTopic = PUB_TOPIC;

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void handleMessage(byte* payload){
    char* rawColors = strrchr((char*)payload, '(');
    String sPayload = (char *)payload;

    String colors[3] = {"", "", ""};

    for(int i = 1, x = 0; i < strlen(rawColors); i++){
      if(rawColors[i] == ',') {
        x++;
        continue;
      }

      if(rawColors[i] == ')') break;

      if(x == 0){
        colors[x] += (String)rawColors[i];
      } else if(x == 1){
        colors[x] += (String)rawColors[i];
      }else if(x == 2){
        colors[x] += (String)rawColors[i];
      }
    }

    char c = sPayload.charAt(0);
    int pin = (int)c - 48;

    Serial.print(pin);
    Serial.println(": " + colors[0] + "," + colors[1] + "," + colors[2]);

    if (pin >= 0 && pin <= 1){
        pixels.setPixelColor(pin, pixels.Color(colors[0].toInt(), colors[1].toInt(), colors[2].toInt()));
        pixels.show();
    }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  delay(50);
  digitalWrite(BUILTIN_LED, 1);

  Serial.begin(115200);
  pixels.begin();

  startup();

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

bool state1 = false;
bool state2 = false;
void loop() {
  if (!client.connected()) {
    reconnect();

    digitalWrite(BUILTIN_LED, 0);
  }
  
  client.loop();

//////////////////////////////////////// BTN HANDLING

  if(!digitalRead(BTN1)){
    delay(20);
    if(!digitalRead(BTN1)){
      client.publish(BTN_1_PUB_TOPIC, state1 ? "1" : "0");

      state1 = !state1;

      digitalWrite(BUILTIN_LED, 1);
      delay(100);
      digitalWrite(BUILTIN_LED, 0);

      while(!digitalRead(BTN1));
    }
  }

  if(!digitalRead(BTN2)){
    delay(20);
    if(!digitalRead(BTN2)){
      client.publish(BTN_2_PUB_TOPIC, state2 ? "1" : "0");

      state2 = !state2;

      digitalWrite(BUILTIN_LED, 1);
      delay(100);
      digitalWrite(BUILTIN_LED, 0);

      while(!digitalRead(BTN2));
    }
  }
}

void callback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  if(strcmp(topic, LED_SUB_TOPIC) == 0){
    handleMessage(payload);
  }

  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      String helloMsg = "[M" + (String)MODULE_NUMBER + "] Logged in.";

      char msg[200];
      helloMsg.toCharArray(msg, helloMsg.length());

      client.publish(pubTopic, msg);

      Serial.print("Subscribing to ");
      Serial.println(subTopic);
      client.subscribe(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void startup(){
  pixels.setPixelColor(0, 0, 0, 200);
  pixels.show();
  delay(500);

  pixels.setPixelColor(0, 0, 0, 0);
  pixels.show();
  delay(500);

  pixels.setPixelColor(1, 0, 0, 200);
  pixels.show();
  delay(500);

  pixels.setPixelColor(1, 0, 0, 0);
  pixels.show();
  delay(500);
}
