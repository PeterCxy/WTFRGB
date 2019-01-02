#include <string.h>
#include "breathing.h"
#include "eeprom.h"

// ==== The Breathing Effect ====
#define BREATHING_CYCLE 30
#define HALF_BREATHING_CYCLE (BREATHING_CYCLE / 2)
#define WRAP_OVERFLOW(x) \
  if (x < 0) {           \
    x = 0;               \
  } else if (x > 255) {  \
    x = 255;             \
  }
CRGB startColor;
CRGB breathingColor;
CRGB breathingIncrement;
int sign;
int breathingCounter;
unsigned char paletteIndex;

void BreathingEffect::reset() {
  sign = -1;
  breathingCounter = 0;
  paletteIndex = 0;
  this->onParamUpdate();
}

// Called when parameter is changed via command line or doing a full reset
// DO NOT reset the full animation status here (e.g. the counter)
void BreathingEffect::onParamUpdate() {
  startColor = *getPaletteColor(paletteIndex);
  breathingIncrement = CRGB(startColor.r / HALF_BREATHING_CYCLE,
                            startColor.g / HALF_BREATHING_CYCLE,
                            startColor.b / HALF_BREATHING_CYCLE);
  // Keep the current progress while resetting to new color
  if (breathingCounter < HALF_BREATHING_CYCLE) {
    breathingColor = startColor;
    this->stepColor(breathingCounter);
  } else {
    breathingColor = CRGB(0, 0, 0);
    this->stepColor(breathingCounter - HALF_BREATHING_CYCLE);
  }
}

void BreathingEffect::handleCommand(char *cmdBuf, int len) {
  if (strncmp("COLOR ", cmdBuf, 6) == 0) {
    // COLOR <index_in_palette> - Change breathing color
    paletteIndex = cmdBuf[6];
    this->onParamUpdate();
  }
}

void BreathingEffect::writeToEEPROM() {
  EEPROM.update(EEPROM_BREATHING_COLOR, paletteIndex);
}

void BreathingEffect::loadFromEEPROM() {
  paletteIndex = EEPROM.read(EEPROM_BREATHING_COLOR);
  this->onParamUpdate();
}

void BreathingEffect::onUpdate() {
  if (breathingCounter == HALF_BREATHING_CYCLE) {
    sign = -sign;
  } else if (breathingCounter == BREATHING_CYCLE) {
    sign = -sign;
    breathingCounter = 0;
  }
  breathingCounter++;
  this->stepColor(1);
}

void BreathingEffect::stepColor(unsigned int step) {
  int newR = (int)breathingColor.r + step * sign * breathingIncrement.r;
  int newG = (int)breathingColor.g + step * sign * breathingIncrement.g;
  int newB = (int)breathingColor.b + step * sign * breathingIncrement.b;
  WRAP_OVERFLOW(newR);
  WRAP_OVERFLOW(newG);
  WRAP_OVERFLOW(newB);
  breathingColor = CRGB(newR, newG, newB);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(breathingColor.r, breathingColor.g, breathingColor.b);
  }
}