#include "breathing.h"

// ==== The Breathing Effect ====
#define BREATHING_CYCLE 30
#define HALF_BREATHING_CYCLE (BREATHING_CYCLE / 2)
#define WRAP_OVERFLOW(x) \
  if (x < 0) {           \
    x = 0;               \
  } else if (x > 255) {  \
    x = 255;             \
  }
CRGB breathingColor = CRGB(MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS);
CRGB breathingIncrement = CRGB(breathingColor.r / HALF_BREATHING_CYCLE,
                               breathingColor.g / HALF_BREATHING_CYCLE,
                               breathingColor.b / HALF_BREATHING_CYCLE);
int sign = -1;
int breathingCounter = 0;

void BreathingEffect::onUpdate() {
  if (breathingCounter == HALF_BREATHING_CYCLE) {
    sign = -sign;
  } else if (breathingCounter == BREATHING_CYCLE) {
    sign = -sign;
    breathingCounter = 0;
  }
  breathingCounter++;
  int newR = (int)breathingColor.r + sign * breathingIncrement.r;
  int newG = (int)breathingColor.g + sign * breathingIncrement.g;
  int newB = (int)breathingColor.b + sign * breathingIncrement.b;
  WRAP_OVERFLOW(newR);
  WRAP_OVERFLOW(newG);
  WRAP_OVERFLOW(newB);
  breathingColor = CRGB(newR, newG, newB);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(breathingColor.r, breathingColor.g, breathingColor.b);
  }
}