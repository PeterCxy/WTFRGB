#include "main.h"
#include "aura.h"
#include "breathing.h"
#include "effect.h"
#include "ripple.h"

byte MAX_BRIGHTNESS = 255;
CRGB leds[NUM_LEDS];

Effect* effects[] = {new BreathingEffect(), new AuraEffect(),
                     new RippleEffect()};
int curEffect = 2;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  loadEEPROM();
}

void loadEEPROM() {
  for (auto& effect : effects) {
    effect->reset();
  }
  if (EEPROM.read(EEPROM_VERSION_COUNTER) != EEPROM_VERSION) {
    writeAllToEEPROM();
    return;
  }
  curEffect = (int) EEPROM.read(EEPROM_MODE);
  for (auto& effect : effects) {
    effect->loadFromEEPROM();
  }
}

void writeAllToEEPROM() {
  EEPROM.put(EEPROM_VERSION_COUNTER, EEPROM_VERSION);
  EEPROM.put(EEPROM_MODE, (byte) curEffect);
  for (auto& effect : effects) {
    effect->writeToEEPROM();
  }
}

void loop() {
  processCommand();
  effects[curEffect]->onUpdate();
  FastLED.show();
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
        curEffect = atoi(&cmdBuf[5]);
        EEPROM.put(EEPROM_MODE, (byte) curEffect);
      }
    }
  }
}