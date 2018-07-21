#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID      "lmg"
#define WLAN_PASS      "09870987"

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

// Setup the MQTT client class by passing in the wiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish moisture = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/soil_moisture");
Adafruit_MQTT_Publish broker = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/broker");


/*************************** Sketch Code ************************************/

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(D4,OUTPUT);
  pinMode(D3,OUTPUT);

}

float x,y,t,h;

void loop() {

  MQTT_connect();

  Serial.print(F("\nSending soil moisture val "));
  y=analogRead(A0);
  x=y;
  Serial.print("...");
  Serial.print(x);
  if (! moisture.publish(x)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
    if(x > 750)
    {
      if (! broker.publish(x)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
      digitalWrite(D4,LOW);
      digitalWrite(D3,HIGH);
    }
   }
   else{
        digitalWrite(D3,LOW);
        digitalWrite(D4,HIGH);
   }
  }
  delay(30000);
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
