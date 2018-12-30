#ifndef MAIN_H
#define MAIN_H
#include <FastLED.h>
#include "eeprom.h"

#define CYCLE 50
#define LED_PIN 2
#define NUM_LEDS 24

extern byte MAX_BRIGHTNESS;
extern CRGB leds[NUM_LEDS];

#endif