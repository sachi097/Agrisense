
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "lmg"
#define WLAN_PASS       "09870987"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "sachi_097"
#define AIO_KEY         "07957f286c7e4f97aec37559fe6fb469"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/broker");

/*************************** Sketch Code ************************************/


void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  pinMode(D3,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D6,OUTPUT);
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&onoff);
}

uint32_t x=0;

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoff) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoff.lastread);
      Serial.println("Motor : ON");
      digitalWrite(D3,LOW);
      digitalWrite(D6,HIGH);
      digitalWrite(D4,HIGH);
    }
  }
      Serial.println("Motor : OFF");
      digitalWrite(D4,LOW);
      digitalWrite(D6,LOW);
      digitalWrite(D3,HIGH);
}


void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
