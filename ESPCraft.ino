#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>
#include "PubSubClient.h"

#define BTN 14

const char* ssid = "JMENO_WIFI";
const char* password = "HELSO_WIFI";

const char* mqtt_server = "52.59.197.227";
const uint16_t mqtt_port = 1884;
char* subTopic = "nvias/MC/M7/#";
char* bcTopic = "nvias/MC/M7";

WiFiClient espClient;
PubSubClient client(espClient);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 0, NEO_GRB + NEO_KHZ800);

void handleMessage(byte* payload){
  char* rawColors = strrchr((char*)payload, '(');

  String colors[3] = {"", "", ""};
  
  if((char)payload[0] == '1'){
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

    Serial.print(colors[0] + "," + colors[1] + "," + colors[2]);
    
    pixels.setPixelColor(0, pixels.Color(colors[0].toInt(), colors[1].toInt(), colors[2].toInt()));
    pixels.show();
  }else{
    Serial.print("0");
    pixels.setPixelColor(0, pixels.Color(0,0,0));
    pixels.show();
  }
}

void callback(char* topic, byte* payload, unsigned int len) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  const char* pin = strrchr(topic, '/');
  
  if(strcmp(pin, "/D3") == 0){
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
      client.publish(bcTopic, "hello world");
      
      Serial.print("Subscribing to ");
      Serial.println(subTopic);
      client.subscribe(subTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
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

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);  

  pixels.begin();
}

bool state = false;
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

  if(!digitalRead(BTN)){
    delay(20);
    if(!digitalRead(BTN)){
      client.publish("nvias/MC/M7/D5", state ? "1" : "0");
      state = !state;
      digitalWrite(BUILTIN_LED, 1);
      delay(100);
      digitalWrite(BUILTIN_LED, 0);
      while(!digitalRead(BTN));
    }
  }
}
