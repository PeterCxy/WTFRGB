#include "ripple.h"
#include "color.h"

// ==== The Ripple Effect ====

void RippleEffect::reset() {
  paletteCounter = 0;
  rippleCounter = 0;
  rippleWidth = NUM_LEDS / 3;
  rippleDecrement = MAX_BRIGHTNESS / rippleWidth;
}

void RippleEffect::postInit() {
  this->stepColor();
}

void RippleEffect::stepColor() {
  curColor = *getPaletteColor(paletteCounter);
  nextColor = *nextPaletteColor(&paletteCounter);
}

void RippleEffect::onUpdate() {
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
    leds[center] = blendColor(curColor, nextColor, rippleCounter, rippleWidth);
  }

  if (rippleCounter == rippleWidth - 1) {
    this->stepColor();
  }

  rippleCounter = (rippleCounter + 1) % rippleWidth;
}