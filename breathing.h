#ifndef BREATHING_H
#define BREATHING_H
#include "main.h"
#include "effect.h"

class BreathingEffect : public Effect {
 CRGB startColor;
 CRGB breathingColor;
 CRGB breathingIncrement;
 int sign;
 int breathingCounter;
 unsigned char paletteIndex;
 bool fullPalette;
 private:
  void stepColor(unsigned int step);
  void updateIncrement();
 public:
  void reset();
  void onParamUpdate();
  void onUpdate();
  void handleCommand(char *cmdBuf, int len);
  void writeToEEPROM();
  void loadFromEEPROM();
};

#endif