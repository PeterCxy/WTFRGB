#include "main.h"
#include "effect.h"
#include "aura.h"
#include "breathing.h"
#include "ripple.h"

byte brightness = 100;
// Delay between frames, a.k.a. fps,
// in ms (normally don't use this for speed control, it may cause the animation to look sticky
byte cycle = 40;
CRGB leds[NUM_LEDS];
CRGB realLeds[NUM_LEDS];

#define EFFECTS_NUM 3
Effect* effects[] = {new BreathingEffect(), new AuraEffect(),
                     new RippleEffect()};
byte curEffect = 2;

void setup() {
  Serial.begin(115200);
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
  curEffect = EEPROM.read(EEPROM_MODE);
  brightness = EEPROM.read(EEPROM_BRIGHTNESS);
  cycle = EEPROM.read(EEPROM_CYCLE);
  for (auto& effect : effects) {
    effect->loadFromEEPROM();
  }
}

void writeAllToEEPROM() {
  EEPROM.update(EEPROM_VERSION_COUNTER, EEPROM_VERSION);
  EEPROM.update(EEPROM_MODE, curEffect);
  EEPROM.update(EEPROM_BRIGHTNESS, brightness);
  EEPROM.update(EEPROM_CYCLE, cycle);
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
  delay((int) cycle + 15);
}

void processCommand() {
  // Try reading from the serial
  char cmdBuf[50];
  if (Serial.available()) {
    int len = Serial.readBytesUntil('\n', cmdBuf, 50);
    if (len > 0) {
      // A command might be available to execute
      cmdBuf[len] = '\0';
      if (strncmp("SAVE", cmdBuf, 4) == 0) {
        writeAllToEEPROM();
      } else if (strncmp("MODE ", cmdBuf, 5) == 0) {
        curEffect = cmdBuf[5] % EFFECTS_NUM;
      } else if (strncmp("BRIGHT ", cmdBuf, 7) == 0) {
        brightness = cmdBuf[7];
      } else if (strncmp("CYCLE ", cmdBuf, 6) == 0) {
        cycle = cmdBuf[6];
      }
    }
  }
}