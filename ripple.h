#ifndef RIPPLE_H
#define RIPPLE_H
#include "main.h"
#include "effect.h"

#define RIPPLE_CENTERS 2 // TODO: Make this (at least somewhat) configurable

class RippleEffect : public Effect {
 int rippleCounter;
 int rippleCenter[RIPPLE_CENTERS] = {NUM_LEDS / 4, NUM_LEDS / 4 * 3}; // TODO: Make this configurable
 int rippleWidth;
 int rippleDecrement;
 unsigned char paletteCounter;
 CRGB curColor, nextColor;
 private:
  void stepColor();
 public:
  void reset();
  void onUpdate();
  void postInit();
};

#endif