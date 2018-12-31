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

void BreathingEffect::reset() {
  startColor = CRGB(MAX_BRIGHTNESS, MAX_BRIGHTNESS, MAX_BRIGHTNESS);
  this->_reset();
}

void BreathingEffect::_reset() {
  breathingColor = startColor;
  breathingIncrement = CRGB(startColor.r / HALF_BREATHING_CYCLE,
                            startColor.g / HALF_BREATHING_CYCLE,
                            startColor.b / HALF_BREATHING_CYCLE);
  sign = -1;
  breathingCounter = 0;
}

void BreathingEffect::handleCommand(char *cmdBuf, int len) {
  if (len >= 9 && strncmp("COLOR ", cmdBuf, 6) == 0) {
    // COLOR <r> <g> <b> - Change breathing color
    startColor = CRGB(cmdBuf[6], cmdBuf[7], cmdBuf[8]);
    this->_reset();
  }
}

void BreathingEffect::writeToEEPROM() {
  EEPROM.update(EEPROM_BREATHING_COLOR_3BYTE, startColor.r);
  EEPROM.update(EEPROM_BREATHING_COLOR_3BYTE + 1, startColor.g);
  EEPROM.update(EEPROM_BREATHING_COLOR_3BYTE + 2, startColor.b);
}

void BreathingEffect::loadFromEEPROM() {
  startColor = CRGB(
    EEPROM.read(EEPROM_BREATHING_COLOR_3BYTE),
    EEPROM.read(EEPROM_BREATHING_COLOR_3BYTE + 1),
    EEPROM.read(EEPROM_BREATHING_COLOR_3BYTE + 2));
  this->_reset();
}

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