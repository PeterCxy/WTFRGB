#include "aura.h"
#include "breathing.h"
#include "effect.h"
#include "main.h"
#include "ripple.h"

byte brightness = 100;
CRGB leds[NUM_LEDS];
CRGB realLeds[NUM_LEDS];

#define EFFECTS_NUM 3
Effect* effects[] = {new BreathingEffect(), new AuraEffect(),
                     new RippleEffect()};
int curEffect = 2;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    realLeds[i] = CRGB(0, 0, 0);
  }

  pinMode(LED_PIN, OUTPUT);
  loadEEPROM(false);
}

void loadEEPROM(bool forceReset) {
  for (auto& effect : effects) {
    effect->reset();
  }
  if (forceReset || EEPROM.read(EEPROM_VERSION_COUNTER) != EEPROM_VERSION) {
    writeAllToEEPROM();
    return;
  }
  curEffect = (int)EEPROM.read(EEPROM_MODE);
  brightness = EEPROM.read(EEPROM_BRIGHTNESS);
  for (auto& effect : effects) {
    effect->loadFromEEPROM();
  }
}

void writeAllToEEPROM() {
  EEPROM.put(EEPROM_VERSION_COUNTER, EEPROM_VERSION);
  EEPROM.put(EEPROM_MODE, (byte)curEffect);
  EEPROM.put(EEPROM_BRIGHTNESS, brightness);
  for (auto& effect : effects) {
    effect->writeToEEPROM();
  }
}

void loop() {
  processCommand();
  if (curEffect < EFFECTS_NUM) effects[curEffect]->onUpdate();

  // Apply brightness transformation to the led array
  for (int i = 0; i < NUM_LEDS; i++) {
    realLeds[i] =
        CRGB(leds[i].r * brightness / 255, leds[i].g * brightness / 255,
             leds[i].b * brightness / 255);
  }

  postLedUpdate(realLeds, NUM_LEDS, LED_PIN);
  delay(CYCLE);
}

void processCommand() {
  // Try reading from the serial
  char cmdBuf[50];
  if (Serial.available()) {
    int len = Serial.readBytesUntil('\n', cmdBuf, 50);
    if (len > 0) {
      // A command might be available to execute
      if (strncmp("MODE ", cmdBuf, 5) == 0) {
        curEffect = atoi(&cmdBuf[5]) % EFFECTS_NUM;
        EEPROM.put(EEPROM_MODE, (byte)curEffect);
      } else if (strncmp("BRIGHT ", cmdBuf, 7) == 0) {
        brightness = atoi(&cmdBuf[7]) % 256;
        EEPROM.put(EEPROM_BRIGHTNESS, brightness);
      }
    }
  }
}