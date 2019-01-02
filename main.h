#ifndef MAIN_H
#define MAIN_H
#include "ws2812.h"
#include "eeprom.h"

#define MAX_BRIGHTNESS 255
#define LED_PIN 2
#define NUM_LEDS 24

extern CRGB leds[NUM_LEDS];

CRGB *getPaletteColor(unsigned char index);
CRGB *nextPaletteColor(unsigned char *counter);

#endif