#ifndef RIPPLE_H
#define RIPPLE_H
#include "main.h"
#include "effect.h"

class RippleEffect : public Effect {
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