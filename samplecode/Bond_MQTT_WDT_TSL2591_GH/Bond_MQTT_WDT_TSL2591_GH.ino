/***************************************************
  Adafruit MQTT Library ESP32 Adafruit IO SSL/TLS example

  Use the latest version of the ESP32 Arduino Core:
    https://github.com/espressif/arduino-esp32

  Works great with Adafruit Huzzah32 Feather and Breakout Board:
    https://www.adafruit.com/product/3405
    https://www.adafruit.com/products/4172

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  Modified by Brent Rubell for Adafruit Industries
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "esp_system.h"

/* TSL2591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>

/******************************************************************/


const int wdtTimeout = 60000;  //time in ms to trigger the watchdog
hw_timer_t *timer = NULL;

void ARDUINO_ISR_ATTR resetModule() {
  ets_printf("reboot\n");
  esp_restart();
}

#define PIN        2
//#define LIGHT_SENSOR   3


unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long prevMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 1000;           // interval at which to blink (milliseconds)
const long interv = 20000;           // interval at which to blink (milliseconds)

int light = 0;
// How many NeoPixels are attached
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // pass in a number for the sensor identifier (for your use later)
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "<WiFi SSID>"
#define WLAN_PASS       "<WiFi Password>"

/************************* Adafruit.io Setup *********************************/

#define MQTT_SERVER      "<MQTT Server IP on the Network>"
#define MQTT_SERVERPORT  1883

/************ Global State (you don't need to change this!) ******************/

// Create an  WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Publish lightIntensity = Adafruit_MQTT_Publish(&mqtt,  "/feeds/message/ambient/light");


/*************************** Sketch Code ************************************/

/**
Name: Configure Sensor TSL2591
Description: Set gain and integration time with sensor

*/
void configureSensor(void) {
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  // tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)


}

/**
Name: Initialize NeoPixel
Description: Set brightness

*/
void _init_heartbeat() {
  pixels.setBrightness(255);
}


/**
Name: Heartbeat functionality
Description: Blink the Neo Pixel on specific color and duration

*/
void heartbeat() {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(150, 0, 150));
  pixels.show();
  delay(50);

  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  //delay(5000);
}

/**
Name: Setup
Description: Centralised initial configuration

*/
void setup() {
  Serial.begin(115200);
  delay(10);

  if (tsl.begin()) {
    Serial.println(F("Found a TSL2591 sensor"));
  } else {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
  /* Configure the sensor */
  configureSensor();

  timer = timerBegin(0, 80, true);                  //timer 0, div 80
  timerAttachInterrupt(timer, &resetModule, true);  //attach callback
  timerAlarmWrite(timer, wdtTimeout * 1000, false); //set time in us
  timerAlarmEnable(timer);                          //enable interrupt

  //pinMode(LED_PIN, OUTPUT);
  
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
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  pixels.begin();
  _init_heartbeat();
  MQTT_connect();
  sendLightData();

}

/**
Name: Main loop
Description: Code orchestration that run in circles

*/
void loop() {
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(10000);
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

  unsigned long currentMillis = millis();

  if (currentMillis - prevMillis >= interv) {
    // save the last time you blinked the LED
    prevMillis = currentMillis;

    timerWrite(timer, 0); //reset timer (feed watchdog)
    
    sendLightData();
  }
}


/**
Name: MQTT Publish
Description: Read sensor and send MQTT packet with data

*/
void sendLightData() {
  light = tsl.getLuminosity(TSL2591_VISIBLE);//analogRead(LIGHT_SENSOR);
  Serial.print("Light: ");Serial.println(light);
  

  String dat = "";

  //memset(dat, 0, sizeof(1110));

  dat += "{\"l\" : ";
  dat += String(light);

  dat += "}";
  
  Serial.println(dat);
  int z = dat.length();
  Serial.print("Size of Data : "); Serial.println(z); 
  
  char c[z+1];
  dat.toCharArray(c, z+1);

  Serial.print("c=");
  Serial.println(c);
  
  uint8_t a[z+1];
  for (int i = 0; i <= z; i++) {
    a[i] = (uint8_t)c[i];//Serial.print(interim[i]);
  }


  lightIntensity.publish(c);
}

/**
Name: MQTT Connect
Description: Function to connect and reconnect as necessary to the MQTT server.
Should be called in the loop function and it will take care if connecting.
*/
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    ledCheck();
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}


/**
Name: Blink Neo Pixel
Description: Blink the indicator for operation of the unit through heartbeat

*/
void ledCheck() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    heartbeat();
  }  
}
