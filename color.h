#ifndef COLOR_H
#define COLOR_H
#include "ws2812.h"

CRGB blendColor(CRGB color1, CRGB color2, int dividend, int divisor);
#endif