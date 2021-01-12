/*  Running list of useful links:
 *  - Animation demos: https://github.com/atuline/FastLED-Demos/blob/master 
 *  - OTA Tutorial: https://lastminuteengineers.com/esp8266-ota-updates-arduino-ide/
 *  
 */

/*  How to upload over wifi:
 *  1. Download ESP8266 library
 *    - Go to "File > Preferences"
 *    - Add this line to "Additional Board Manager URLs" and click "OK"
 *  2. Set ESP8266 as your board
 *    - Go to "Tools > Board > Boards Manager"
 *    - Search for ESP8266 and install "esp8266" by "ESP8266 Community"
 *    - Go to "Tools > Board"
 *    - Set board to "NodeMCU 1.0 (ESP-12E Module)"
 *  3. Select ESP8266 WiFi port
 *    - Go to "Tools > Port"
 *    - Select the esp8266 port under "Network ports"
 *  4. Upload sketch
 *    - Write program as you normally would on a microcontroller
 *    - DO NOT TOUCH ANY OF THE CODE SURROUNDED BY WARNINGS
 *      - This code configures the ESP8266 for WiFi uploads, so changing it will
 *        break that feature and force you to reprogram the board with by cable
 *    - Make sure the ESP8266 is on
 *    - Upload the sketch
 */

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <NTPClient.h>

#include "wifi_settings.h"

/* // Create a file called wifi_settings.h with these contents. (Changing ssid & password)
 * const char* ssid = "SSID_HERE";
 * const char* password = "PASSWORD_HERE";
 */

#define LED_PIN D4   // LED strip data connected to ESP8266 pin D4
#define NUM_LEDS 100 // 100 total LEDs in the strip

CRGB leds[NUM_LEDS];

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, -28800L); // PST is 8 hrs behind GMT
// 24 hour time
int turnOffHour = 1;
int turnOnHour = 12+5;

void setup() {
  FastLED.addLeds<WS2811, LED_PIN, BRG>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  //Serial.begin(115200);
  /* ******** DO NOT REMOVE THIS LINE PLEASE ******** */
  /* !DANGER! */ wifiSetup(); /* !DANGER! */
  /* OR ELSE YOU WILL NOT BE ABLE TO UPLOAD OVER WIFI */

  timeClient.begin();
  
  fill_solid(leds, NUM_LEDS, CRGB(32, 0, 0));
  FastLED.show();
  delay_ota(1000);
  fill_solid(leds, NUM_LEDS, CRGB(0, 32, 0));
  FastLED.show();
  delay_ota(1000);
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 32));
  FastLED.show();
  delay_ota(1000);
  off();
}

void loop() {
  /* ******** DO NOT REMOVE THIS LINE PLEASE ******** */
  /* !DANGER! */ ArduinoOTA.handle(); /* !DANGER! */
  /* OR ELSE YOU WILL NOT BE ABLE TO UPLOAD OVER WIFI */

  timeClient.update();
  int hr = timeClient.getHours();

  if (hr > turnOffHour && hr < turnOnHour) { // OFF interval
    off();
    delay_ota(10);
  } else {                                   // ON interval
    callAnimation(random8(10));
  }
}

void delay_ota(int amount) {
  ArduinoOTA.handle();
  const int start = millis();  
  while (millis() - start < amount) {
    ArduinoOTA.handle();
    delay(1);
  }
}

void callAnimation(int n) {
  switch (n) {
    case 0:
      blink_it_S();
      break;
    case 1:
    case 2:
      chase_it_S();
      break;
    case 3:    
      fade_it_S();
      break;
    case 4:
    case 5:
      fireworks_S();
      break;
    case 6:
    case 7:
      two_meteors_S();
      break;
    case 8:
    case 9:
      sparkle();
      break;
    default:
      off();
  }
}

void off() {
  FastLED.clear();
  FastLED.show();
}

/*void blink_it() {
  int interval = 3000;
  CRGB red = CRGB(255, 0, 0);
  CRGB green = CRGB(0, 255, 0);

  for (int i = 0; i < 5; i++) {
    fill_solid(leds, NUM_LEDS, blue);
    FastLED.show();
    delay_ota(interval);
    fill_solid(leds, NUM_LEDS, white);
    FastLED.show();
    delay_ota(interval);
  }  
}*/

void blink_it_S() {
  int interval = 3000;
  CRGB blue = CRGB(0, 0, 255);
  CRGB white = CRGB(128, 128, 128);

  for (int i = 0; i < 5; i++) {
    fill_solid(leds, NUM_LEDS, blue);
    FastLED.show();
    delay_ota(interval);
    fill_solid(leds, NUM_LEDS, white);
    FastLED.show();
    delay_ota(interval);
  }  
}

/*void fade_it() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 96; j++) {
      fill_solid(leds, NUM_LEDS, CHSV(j, 255, 255));
      FastLED.show();
      delay_ota(30);
    }
    delay_ota(1000);
    for (int j = 96; j >= 0; j--) {
      fill_solid(leds, NUM_LEDS, CHSV(j, 255, 255));
      FastLED.show();
      delay_ota(30);
    }
    delay_ota(1000);
  }
}*/

void fade_it_S() {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 255; j += 2) { // white to blue
      fill_solid(leds, NUM_LEDS, CHSV(152, j, 200));
      FastLED.show();
      delay_ota(30);
    }
    delay_ota(1000);
    for (int j = 255; j > 0; j -= 2) { // blue to white
      fill_solid(leds, NUM_LEDS, CHSV(152, j, 200));
      FastLED.show();
      delay_ota(30);
    }
    delay_ota(1000);
  }
}

// t goes from 0 to NUM_LEDS
/*void chase(int H, int width, int t) {
  int step_v = 255 / width;
  int curr_v = 0;
  for (int i = t - width; i < t; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(H, 255, curr_v);    
    }
    curr_v += step_v;
  }
}*/

// t goes from 0 to NUM_LEDS
void chase_S(int S, int width, int t) {
  int step_v = 255 / width;
  int curr_v = 0;
  for (int i = t - width; i < t; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(152, S, curr_v);    
    }
    curr_v += step_v;
  }
}

/*void reverse_chase(int H, int width, int t) {
  t = NUM_LEDS - t;
  int step_v = -255 / width;
  int curr_v = 255;
  for (int i = t; i < t + width; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(H, 255, curr_v);    
    }
    curr_v += step_v;
  }
}*/

void reverse_chase_S(int S, int width, int t) {
  t = NUM_LEDS - t;
  int step_v = -255 / width;
  int curr_v = 255;
  for (int i = t; i < t + width; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(152, S, curr_v);    
    }
    curr_v += step_v;
  }
}

/*void chase_it() {
  const int width = 20;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; i < NUM_LEDS + width; i++) {
      chase(0, width, i);
      reverse_chase(96, width, i);
      FastLED.show();
      delay_ota(30);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
    for (int i = 0; i < NUM_LEDS + width; i++) {
      chase(96, width, i);
      reverse_chase(0, width, i);
      FastLED.show();
      delay_ota(30);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
  }
}*/

void chase_it_S() {
  const int width = 20;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; i < NUM_LEDS + width; i++) {
      chase_S(0, width, i);
      reverse_chase_S(255, width, i);
      FastLED.show();
      delay_ota(30);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
    for (int i = 0; i < NUM_LEDS + width; i++) {
      chase_S(255, width, i);
      reverse_chase_S(0, width, i);
      FastLED.show();
      delay_ota(30);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
  }
}

/*void firework_at(int center, int H, int t) {
  if (t < 0 || t > 255) {
    return;
  }
  int width = lerp8by8(1, 10, (uint8_t) (t < 127 ? t * 2 : 255));
  int currV = lerp8by8(255, 0, (uint8_t) t);
  for (int i = center - width; i < center + width; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(H, 255, currV);
    }
  }  
}*/

void firework_at_S(int center, int S, int t) {
  if (t < 0 || t > 255) {
    return;
  }
  int width = lerp8by8(1, 10, (uint8_t) (t < 127 ? t * 2 : 255));
  int currV = lerp8by8(255, 0, (uint8_t) t);
  for (int i = center - width; i < center + width; i++) {
    if (i >= 0 && i < NUM_LEDS) {
      leds[i] += CHSV(152, S, currV);
    }
  }  
}

/*void fireworks() {
  for (int i = 0; i < 10; i++) {
    const int f1_center = random8(NUM_LEDS - 20) + 10;
    const int f2_center = random8(NUM_LEDS - 20) + 10;
    const int f3_center = random8(NUM_LEDS - 20) + 10;
    const int f4_center = random8(NUM_LEDS - 20) + 10;
    for (int j = 0; j < 256 + 128; j++) {
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
      firework_at(f1_center, 96, j);
      firework_at(f2_center, 0, j - 64);
      firework_at(f3_center, 96, j - 128);
      firework_at(f3_center, 0, j - 32);
      FastLED.show();
      delay_ota(1);      
    }
  }
}*/

void fireworks_S() {
  for (int i = 0; i < 10; i++) {
    const int f1_center = random8(NUM_LEDS - 20) + 10;
    const int f2_center = random8(NUM_LEDS - 20) + 10;
    const int f3_center = random8(NUM_LEDS - 20) + 10;
    const int f4_center = random8(NUM_LEDS - 20) + 10;
    for (int j = 0; j < 256 + 128; j++) {
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
      firework_at_S(f1_center, 255, j);
      firework_at_S(f2_center, 0, j - 64);
      firework_at_S(f3_center, 255, j - 128);
      firework_at_S(f3_center, 0, j - 32);
      FastLED.show();
      delay_ota(1);      
    }
  }
}

/*void two_meteors() {
  const int width = 30;
  const int second_offset = 50;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; i < NUM_LEDS + width + second_offset; i++) {
      chase(0, width, i);
      chase(96, width, i - second_offset);
      FastLED.show();
      delay_ota(15);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
  }  
}*/

void two_meteors_S() {
  const int width = 30;
  const int second_offset = 50;
  for (int i = 0; i < 5; i++) {
    for (int i = 0; i < NUM_LEDS + width + second_offset; i++) {
      chase_S(0, width, i);
      chase_S(255, width, i - second_offset);
      FastLED.show();
      delay_ota(15);
      fill_solid(leds, NUM_LEDS, CRGB(0,0,0));
    }
  }  
}

void sparkle() {
  int duration = 10000; // ms
  int interval = 50; // ms
  int num = 2;
  for (int t = 0; t < duration / interval; t++) {
    for (int n = 0; n < num; n++) {
        leds[random(NUM_LEDS)] += CRGB(48, 48, 48*random8(32, 96)/32);
      }
    /*for (int i = 0; i < NUM_LEDS; i++) {
      if (random8(128) == 0) {
        uint8_t value = random8(32, 128);
        leds[i] += CRGB(value, value, value*random8(32, 64)/32);
      }
    }*/
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, 10);
    delay_ota(interval);
  }
}

/* ******** !IMPORTANT! ******** */
/* DO NOT TOUCH THIS METHOD, LEST YOU FORFEIT YOUR WIFI PRIVILEGES */
/* PUT ALL OF YOUR STUFF ABOVE THIS METHOD AND LEAVE THIS AT THE BOTTOM. */
/* THIS IS FOR ORGANIZATIONAL PURPOSES. */
void wifiSetup() {
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
/* BOTTOM OF FILE */
