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
Adafruit_NeoPixel pixels_01 = Adafruit_NeoPixel(2, PIN_01);

uint8_t  mode   = 0, // Current animation effect
         offset = 0; // Position of spinny eyes
uint32_t color  = 0xFF0000; // Start red
uint32_t prevTime;

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

  prevTime = millis();
}

void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {

   case 0:
    random(color, 10);
    break;

   case 1:
    tsunami(color, 50);
    break;
  }

  t = millis();
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;                        // Next mode
    if(mode > 1) {                 // End of modes?
      mode = 0;                    // Start modes over
      color >>= 8;                 // Next color R->G->B
      if(!color) color = 0xFF0000; // Reset to red
    }
    for(i=0; i<32; i++) pixels_16.setPixelColor(i, 0);
    for(i=0; i<24; i++) pixels_12.setPixelColor(i, 0);
    for(i=0; i<16; i++) pixels_08.setPixelColor(i, 0);
    for(i=0; i<2; i++) pixels_01.setPixelColor(i, 0);
    prevTime = t;
  }
}

// Fill the dots one after the other with a color
// Random sparks - just one LED on at a time!
void random(uint32_t c, uint8_t wait) {
  uint8_t i;
  i = random(32);
  pixels_16.setPixelColor(i, color);
  pixels_16.show();

  pixels_12.setPixelColor(i, color);
  pixels_12.show();

  pixels_08.setPixelColor(i, color);
  pixels_08.show();

  pixels_01.setPixelColor(i, color);
  pixels_01.show();

  delay(wait);
  pixels_16.setPixelColor(i, 0);
  pixels_12.setPixelColor(i, 0);
  pixels_08.setPixelColor(i, 0);
  pixels_01.setPixelColor(i, 0);
}

// Spinny wheels (8 LEDs on at a time)
void tsunami(uint32_t c, uint8_t wait) {
  uint8_t i;
  for(i=0; i<16; i++) {
    uint32_t c = 0;
    if(((offset + i + 8) & 7) < 2) c = color; // 4 pixels_16 on...
    pixels_16.setPixelColor(   i, c); // First eye
    pixels_16.setPixelColor(31-i, c); // Second eye (flipped)
  }

  for(i=0; i<12; i++) {
    uint32_t c = 0;
    if(((offset + i + 4) & 7) < 2) c = color; // 4 pixels_12 on...
    pixels_12.setPixelColor(   i, c); // First eye
    pixels_12.setPixelColor(23-i, c); // Second eye (flipped)
  }

  for(i=0; i<8; i++) {
    uint32_t c = 0;
    if(((offset + i + 2) & 7) < 2) c = color; // 4 pixels_08 on...
    pixels_08.setPixelColor(   i, c); // First eye
    pixels_08.setPixelColor(15-i, c); // Second eye (flipped)
  }

  for(i=0; i<2; i++) {
    uint32_t c = 0;
    if(((offset + i) & 7) < 2) c = color; // 4 pixels_01 on...
    pixels_01.setPixelColor(   i, c); // First eye
    pixels_01.setPixelColor(1-i, c); // Second eye (flipped)
  }

  pixels_16.show();
  pixels_12.show();
  pixels_08.show();
  pixels_01.show();
  offset++;
  delay(wait);
}
