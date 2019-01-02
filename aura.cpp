#include "aura.h"
#include "color.h"

// ==== The Aura Rainbow Effect ====
int auraCounter = 0;
int auraWidth;
unsigned char paletteCounter;
CRGB curColor, nextColor;

void AuraEffect::reset() {
  auraCounter = 0;
  paletteCounter = 0;
  auraWidth = NUM_LEDS;
}

void AuraEffect::stepColor() {
  curColor = *getPaletteColor(paletteCounter);
  nextColor = *nextPaletteColor(&paletteCounter);
}

void AuraEffect::postInit() {
  this->stepColor();
}

void AuraEffect::onUpdate() {
  // Shift all LEDs left by 1 px
  for (int i = 0; i < NUM_LEDS - 1; i++) {
    leds[i] = CRGB(leds[i + 1].r, leds[i + 1].g, leds[i + 1].b);
  }

  // Calculate the color for the last LED
  int last = NUM_LEDS - 1;
  leds[last] = blendColor(curColor, nextColor, auraCounter, auraWidth);

  if (auraCounter == (auraWidth - 1)) {
    this->stepColor();
  }

  // Increment the counter
  auraCounter = (auraCounter + 1) % auraWidth;
}