/*
IAN SCHWARZKOPF - TRON DISC - CSC 235 FINAL

 - uses code from menucommands from Soundboard example library
 - uses code from strandtest from NeoPixel example library

*/

#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"
#include <Adafruit_NeoPixel.h>
#include <CapacitiveSensor.h>

#define SFX_TX 5
#define SFX_RX 6
#define SFX_RST 4

SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);
CapacitiveSensor cs_4_2 = CapacitiveSensor(10, 12);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

int n = 0;
int numLeds = 35;
int ledPin = 2;

Adafruit_NeoPixel strip(numLeds, ledPin);

uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t tronBlue = strip.Color(0, 234, 255);

int turnOn = 5;

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit Sound Board!");

  // softwareserial at 9600 baud
  ss.begin(9600);
  // can also do Serial1.begin(9600)

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1)
      ;
  }
  Serial.println("SFX board found");

  strip.begin();
  strip.setBrightness(50);
  strip.clear();
  strip.show();
}


void loop() {
  flushInput();

  while (!Serial.available()) {

    if (turnOn == 1) {
      bouncingBrightness(tronBlue, 100);
      strip.show();
    }
    if (turnOn == 2) {
      theaterChase(tronBlue, 75);
      strip.show();
    }
    if (turnOn == 3) {
      colorWipe(tronBlue, 50);
      strip.show();
    }
    if (turnOn == 4) {
      theaterChaseRainbow(5);
      strip.show();
    }
    if (turnOn == 5) {
      strip.clear();
      strip.show();
    }
    if (turnOn == 6) {
      strip.clear();
      strip.show();
      turnOn = 1;
    }

    long total1 = cs_4_2.capacitiveSensor(30);
    Serial.println(total1);
    if (total1 > 40) {
      turnOn = turnOn + 1;
      n = 0;
      
      delay(500);
      break;
    }
  }

  Serial.println(n);
  if (!sfx.playTrack((uint8_t)n)) {
    Serial.println("Failed to play track?");
  }
}

void flushInput() {
  // Read all available serial input to flush pending data.
  uint16_t timeoutloop = 0;
  while (timeoutloop++ < 40) {
    while (ss.available()) {
      ss.read();
      timeoutloop = 0;  // If char was received reset the timer
    }
    delay(1);
  }
}

//changes brightness of neopixels
void bouncingBrightness(uint32_t aColor, int interval) {
  static int i = 0;
  static bool isIncreasing = true;
  static unsigned long startTime = millis();
  unsigned long endTime = millis();

  if (endTime - startTime >= interval) {
    if (isIncreasing == true) {
      strip.setBrightness(i);
      solidColor(aColor);
      strip.show();
      startTime = millis();
      i = i + 10;
    } else if (isIncreasing == false) {
      strip.setBrightness(i);
      solidColor(aColor);
      strip.show();
      startTime = millis();
      i = i - 10;
    }
  }
  if (i >= 100) {
    isIncreasing = false;
  }
  if (i <= 20) {
    isIncreasing = true;
  }
}

//set neopixels to one color
void solidColor(uint32_t color) {
  for (int i = 0; i < numLeds; i++) {
    strip.setPixelColor(i, color);
  }
}

//one pixel moves around disc
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { 
    strip.setPixelColor(i, color);
    strip.setPixelColor(i-1, LOW);        
    strip.show();                          
    delay(wait);                          
  }
}

//moves around disc
void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) { 
    for(int b=0; b<2; b++) { 
      strip.clear();       
    
      for(int c=b; c<strip.numPixels(); c += 2) {
        strip.setPixelColor(c, color); 
      }
      strip.show();
      delay(wait); 
    }
  }
}

//rainbox effect around disc
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0; 
  for(int a=0; a<30; a++) { 
    for(int b=0; b<1; b++) { 
      strip.clear();       
     
      for(int c=b; c<strip.numPixels(); c += 1) {
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); 
        strip.setPixelColor(c, color); 
      }
      strip.show();                
      delay(wait);                 
      firstPixelHue += 65536 / 90; 
    }
  }
}