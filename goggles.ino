// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.

#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif

#define PIN_16 11
#define PIN_12 10
#define PIN_08 9
#define PIN_01 6

Adafruit_NeoPixel pixels_16 = Adafruit_NeoPixel(32, PIN_16);
Adafruit_NeoPixel pixels_12 = Adafruit_NeoPixel(24, PIN_12);
Adafruit_NeoPixel pixels_08 = Adafruit_NeoPixel(16, PIN_08);
Adafruit_NeoPixel pixels_01 = Adafruit_NeoPixel(2,  PIN_01);

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prev_time;

void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels_16.begin();
  pixels_16.setBrightness(85); // 1/3 brightness

  pixels_12.begin();
  pixels_12.setBrightness(85); // 1/3 brightness

  pixels_08.begin();
  pixels_08.setBrightness(85); // 1/3 brightness

  pixels_01.begin();
  pixels_01.setBrightness(85); // 1/3 brightness

  prev_time = millis();
}

void loop() {
  uint32_t time;

  switch(mode) {

   case 0:
    random(color, 10);
    break;

   case 1:
    tsunami(color, 50);
    break;

   case 2:
    tsunami(color, 50);
    break;
  }

  time = millis();

  // if((time - prev_time) > 8000) { // Every 8 seconds...
  if((time - prev_time) > 2000) {    // Every 2 seconds...
    mode++;                         // Next mode
    if(mode > 2) {                  // End of modes?
      mode = 0;                     // Start modes over
      color >>= 8;                  // Next color R->G->B
      if(!color) color = 0xFF0000;  // Reset to red
    }

    prev_time = time;
  }
}

// Fill the dots one after the other with a color
// Random sparks - just one LED on at a time!
void random(uint32_t color, uint8_t wait) {
  uint8_t i = random(32);
  pixels_16.setPixelColor(i, color);
  pixels_12.setPixelColor(i, color);
  pixels_08.setPixelColor(i, color);
  pixels_01.setPixelColor(i, color);

  show(wait);
  clearPixel(i);
}

// Spinny wheels (7 LEDs on at a time)
void tsunami(uint32_t color, uint8_t wait) {
  // uint8_t i;
  ++offset;

  // uint32_t c = ( ( ( offset + i ) & x ) < y ) ? color : 0
  // offset: each frame for a color; given a duration of `wait`
  // i: position of each pixel per ring
  // x: the speed of the rotation
  // y: how many pixels are visible per frame
  // c: determine color for a given pixel within a frame; defaults to OFF

  // for(i=1; i<17; i++) {
  //   uint32_t c = (((offset + i) & 4) < 2) ? color : 0;  // 4 pixels_16 on...
  //   pixels_16.setPixelColor(   i, c);                   // First eye
  //   pixels_16.setPixelColor(31-i, c);                   // Second eye (flipped)
  // }

  // for(i=0; i<12; i++) {
  //   uint32_t c = (((offset + i) & 6) < 1) ? color : 0;  // 4 pixels_12 on...
  //   pixels_12.setPixelColor(   i, c);                   // First eye
  //   pixels_12.setPixelColor(23-i, c);                   // Second eye (flipped)
  // }

  // for(i=0; i<8; i++) {
  //   uint32_t c = (((offset + i) & 5) < 1) ? color : 0;  // 4 pixels_08 on...
  //   pixels_08.setPixelColor(   i, c);                   // First eye
  //   pixels_08.setPixelColor(15-i, c);                   // Second eye (flipped)
  //   // waterfall
  //   // pixels_08.setPixelColor(8-i, c);                    // Second eye (flipped)
  // }

  // // BUG: does not flash pixels_01.first_eye @ offset(0); red  
  // for(i=0; i<2; i++) {
  //   uint32_t c = (((offset + i) & 2) < 2) ? color : 0;  // 2 pixels_01 on...
  //   pixels_01.setPixelColor(i,   c);                    // First eye
  //   pixels_01.setPixelColor(1-i, c);                    // Second eye (flipped)
  // }

  tsunamiHelper(pixels_16, 4, 2, color, offset);
  tsunamiHelper(pixels_12, 6, 1, color, offset);
  tsunamiHelper(pixels_08, 5, 1, color, offset);
  tsunamiHelper(pixels_01, 2, 2, color, offset);

  show(wait);
}

void tsunamiHelper(Adafruit_NeoPixel ring, uint8_t speed, uint8_t visible, uint32_t color, uint8_t offset) {
  uint8_t i;
  uint8_t num_pixels = ring.numPixels();

  for(i=0; i<num_pixels; i++) {
    uint32_t c = (((offset + i) & speed) < visible) ? color : 0;  // 2 pixels_01 on...
    ring.setPixelColor(i,   c);                    // First eye
    // ring.setPixelColor((num_pixels/2)-i, c);                    // Second eye (flipped)
    ring.setPixelColor(num_pixels-i, c);                    // Second eye (flipped)
  }
}

void show(uint8_t wait) {
  pixels_16.show();
  pixels_12.show();
  pixels_08.show();
  pixels_01.show();
  
  delay(wait);
}

void clearPixel(uint8_t i) {
  pixels_16.setPixelColor(i, 0);
  pixels_12.setPixelColor(i, 0);
  pixels_08.setPixelColor(i, 0);
  pixels_01.setPixelColor(i, 0);
}

void clearAll() {
  pixels_16.clear();
  pixels_12.clear();
  pixels_08.clear();
  pixels_01.clear();
}
