#include "color.h"

CRGB blendColor(CRGB color1, CRGB color2, int dividend, int divisor) {
  return CRGB(
    color1.r - color1.r * dividend / divisor + color2.r * dividend / divisor,
    color1.g - color1.g * dividend / divisor + color2.g * dividend / divisor,
    color1.b - color1.b * dividend / divisor + color2.b * dividend / divisor);
}