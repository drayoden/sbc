#include <Adafruit_NeoPixel.h>

// digit array...

#define DATA_PIN  16
#define NUMPIXELS 24
#define PIXEL_FLAGS NEO_GRB + NEO_KHZ800
#define DELAY 10000
#define LEDR 0
#define LEDG 0
#define LEDB 255

// digit position -- first LED index in array (seg) -- L -> R
#define HR10  22
#define HR1   15
#define COL   14
#define MN10  7
#define MN1   0

// digit array
#define DIG 10
#define SEG 7
int seg[DIG][SEG] ={
  {1,1,1,0,1,1,1},  // digit 0
  {0,1,0,0,1,0,0},  // digit 1
  {1,1,0,1,0,1,1},  // digit 2
  {1,1,0,1,1,0,1},  // digit 3
  {0,1,1,1,1,0,0},  // digit 4 
  {1,0,1,1,1,0,1},  // digit 5
  {1,0,1,1,1,1,1},  // digit 6
  {1,1,0,0,1,0,0},  // digit 7
  {1,1,1,1,1,1,1},  // digit 8
  {1,1,1,1,1,0,1}   // digit 9
};


// initialze neopixel...
Adafruit_NeoPixel strip(NUMPIXELS, DATA_PIN, PIXEL_FLAGS);

void setup() {
  Serial.begin(115200);
  Serial.println("setup...");
  strip.show();
  strip.clear();
  strip.setBrightness(50);
}

void loop() {

  // display each digit -- 0 - 9 for positions MN1, MN10 and HR1
  for (int i = 0; i < DIG; i++ ) {

    Serial.print("loop -- digit: ");
    Serial.println(i);
    
    // sets in memory only until pixel.show()
    setDigit(MN1,i);
    setDigit(MN10,i);
    setDigit(HR1,i);  

    strip.show();
    delay(DELAY);
    strip.clear();
  }
}


// setDigit:
// arg1: position -- will be one of the digit position defines
// arg2: digit to display -- 0 => 9 -- some edge cases for some 
void setDigit(int pos, int digit) {
  for ( int j = 0; j < SEG; j++ ) {
    if (seg[digit][j]) {
      strip.setPixelColor(j + pos, strip.Color(LEDR,LEDG,LEDB) );
    }
  }
  
}
