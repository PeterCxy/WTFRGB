#ifndef AURA_H
#define AURA_H
#include "main.h"
#include "effect.h"

class AuraEffect : public Effect {
 unsigned char paletteCounter;
 CRGB curColor, nextColor;
 private:
  void stepColor();
 public:
  void onUpdate();
  void postInit();
  void reset();
};

#endif