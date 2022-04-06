#include <Adafruit_NeoPixel.h>

// simple test to get started...

#define DATA_PIN  16
#define NUMPIXELS 24
#define PIXEL_FLAGS NEO_GRB + NEO_KHZ800
#define DELAY 500

// initialze neopixel...
Adafruit_NeoPixel strip(NUMPIXELS, DATA_PIN, PIXEL_FLAGS);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);
}

void loop() {
  // strip.clear();

  for(int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0,0,255));
    strip.show();
    strip.clear();
    delay(DELAY);
  }
}
