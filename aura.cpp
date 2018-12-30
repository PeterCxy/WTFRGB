#include "aura.h"

// ==== The Aura Rainbow Effect ====
// TODO: Implement arbitrary color selection
int auraCounter = 0;
int auraWidth = NUM_LEDS / 3;
int auraDecrement = MAX_BRIGHTNESS / auraWidth;

void AuraEffect::onUpdate() {
  auraCounter = (auraCounter + 1) % (auraWidth * 3);  // 3 = R G B
  int cycleProgress = auraDecrement * (auraCounter % auraWidth);

  // Shift all LEDs left by 1 px
  for (int i = 0; i < NUM_LEDS - 1; i++) {
    leds[i] = CRGB(leds[i + 1].r, leds[i + 1].g, leds[i + 1].b);
  }

  // Calculate the color for the last LED
  int last = NUM_LEDS - 1;
  if (auraCounter < auraWidth) {
    // RED
    leds[last] = CRGB(MAX_BRIGHTNESS - cycleProgress, cycleProgress, 0);
  } else if (auraCounter < auraWidth * 2) {
    // GREEN
    leds[last] = CRGB(0, MAX_BRIGHTNESS - cycleProgress, cycleProgress);
  } else {
    // BLUE
    leds[last] = CRGB(cycleProgress, 0, MAX_BRIGHTNESS - cycleProgress);
  }
}