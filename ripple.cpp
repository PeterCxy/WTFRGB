#include "ripple.h"

// ==== The Ripple Effect ====
// TODO: Implement arbitrary color selection
int rippleCounter;
#define RIPPLE_CENTERS 2
int rippleCenter[RIPPLE_CENTERS] = {NUM_LEDS / 4, NUM_LEDS / 4 * 3};
int rippleWidth;
int rippleDecrement;

void RippleEffect::reset() {
  rippleCounter = 0;
  //rippleCenter = {NUM_LEDS / 4, NUM_LEDS / 4 * 3}; // TODO: Make this dynamic
  rippleWidth = NUM_LEDS / 3;
  rippleDecrement = MAX_BRIGHTNESS / rippleWidth;
}

void RippleEffect::onUpdate() {
  rippleCounter = (rippleCounter + 1) % (rippleWidth * 3);  // 3 = R G B
  int cycleProgress = rippleDecrement * (rippleCounter % rippleWidth);

  for (int k = 0; k < RIPPLE_CENTERS; k++) {
    int startCell = 0;
    if (k > 0) {
      startCell =
          rippleCenter[k - 1] + (rippleCenter[k] - rippleCenter[k - 1]) / 2;
    }
    int endCell = NUM_LEDS - 1;
    if (k < RIPPLE_CENTERS - 1) {
      endCell = rippleCenter[k] + (rippleCenter[k + 1] - rippleCenter[k]) / 2;
    }
    // Shift every LED left to the center left
    for (int i = startCell; i < rippleCenter[k] - 1; i++) {
      leds[i] = CRGB(leds[i + 1].r, leds[i + 1].g, leds[i + 1].b);
    }

    // Shift every LED right to the center right
    for (int i = endCell; i > rippleCenter[k] - 1; i--) {
      leds[i] = CRGB(leds[i - 1].r, leds[i - 1].g, leds[i - 1].b);
    }

    // Calculate new color for center
    int center = rippleCenter[k] - 1;
    if (rippleCounter < rippleWidth) {
      // RED
      leds[center] = CRGB(MAX_BRIGHTNESS - cycleProgress, cycleProgress, 0);
    } else if (rippleCounter < rippleWidth * 2) {
      // GREEN
      leds[center] = CRGB(0, MAX_BRIGHTNESS - cycleProgress, cycleProgress);
    } else {
      // BLUE
      leds[center] = CRGB(cycleProgress, 0, MAX_BRIGHTNESS - cycleProgress);
    }
  }
}