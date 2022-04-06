#include <Adafruit_NeoPixel.h>

// non-blocking loop with blinking colon!

#define DATA_PIN  16
#define NUMPIXELS 24
#define PIXEL_FLAGS NEO_GRB + NEO_KHZ800

// non-blocking delay stuff:
#define DELAYDIGITS 20000
#define DELAYCOL  1000
unsigned int Lcol = 0;
unsigned int Ldig = 0;

// set colors:
#define LEDR 0
#define LEDG 0
#define LEDB 255

// digit position -- first LED index in array (seg) -- L -> R
#define HR10  22
#define HR1   15
#define COL   14
#define MN10  7
#define MN1   0


// misc.
bool colState = true;
int displayDigit = 0;

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

  // start the 'timer'
  unsigned int now = millis();

  
  // colon blink control...
  if ( now - Lcol >= DELAYCOL ) {
    Lcol = now;
    if ( colState ) {
      strip.setPixelColor(COL,strip.Color(0,0,0)); // black/off
      strip.show();
      colState = false;
    } else {
      strip.setPixelColor(COL,strip.Color(LEDR,LEDG,LEDB));
      strip.show();
      colState = true;
    }
  }
    

  // digit LED control...
  if ( now - Ldig >= DELAYDIGITS ) {
    
    // sets in memory only until pixel.show()
    setDigit(MN1,displayDigit);
    setDigit(MN10,displayDigit);
    setDigit(HR1,displayDigit);
    strip.show();  

    if ( ++displayDigit > DIG ) { displayDigit = 0;}
    Ldig = now;
    strip.show();
      
  }
  
}


// setDigit:
// arg1: position -- will be one of the digit position defines
// arg2: digit to display -- 0 => 9 -- some edge cases for some 
void setDigit(int pos, int digit) {
  for ( int j = 0; j < SEG; j++ ) {
    if (seg[digit][j]) {
      strip.setPixelColor(j + pos, strip.Color(LEDR,LEDG,LEDB) );
    } else {
      strip.setPixelColor(j + pos, strip.Color(0,0,0) );
    }
  }
  
}
