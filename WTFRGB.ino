#include "main.h"
#include "effect.h"
#include "aura.h"
#include "breathing.h"
#include "ripple.h"

byte brightness = 100;
// Delay between frames, a.k.a. fps,
// in ms (normally don't use this for speed control, it may cause the animation to look sticky
byte cycle = 40;

// Main color palette
// We support a maximum of 6 colors
#define NUM_PALETTE_COLORS_MAX 6
byte paletteColorNum = 3;
CRGB palette[NUM_PALETTE_COLORS_MAX];

CRGB leds[NUM_LEDS];
CRGB realLeds[NUM_LEDS];

#define EFFECTS_NUM 3
Effect* effects[] = {new BreathingEffect(), new AuraEffect(),
                     new RippleEffect()};
byte curEffect = 2;

void setup() {
  Serial.begin(115200);
  // Initialize the palette with simple RGB
  palette[0] = CRGB(255, 0, 0);
  palette[1] = CRGB(0, 255, 0);
  palette[2] = CRGB(0, 0, 255);

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

  // Load the palette length + palette
  paletteColorNum = EEPROM.read(EEPROM_PALETTE);
  for (int i = 0; i < paletteColorNum; i++) {
    palette[i] = CRGB(
      EEPROM.read(EEPROM_PALETTE + i * 3 + 1),
      EEPROM.read(EEPROM_PALETTE + i * 3 + 2),
      EEPROM.read(EEPROM_PALETTE + i * 3 + 3));
  }

  for (auto& effect : effects) {
    effect->loadFromEEPROM();
  }

  // Call post-init hooks when everything has been loaded.
  for (auto& effect : effects) {
    effect->postInit();
  }
}

void writeAllToEEPROM() {
  EEPROM.update(EEPROM_VERSION_COUNTER, EEPROM_VERSION);
  EEPROM.update(EEPROM_MODE, curEffect);
  EEPROM.update(EEPROM_BRIGHTNESS, brightness);
  EEPROM.update(EEPROM_CYCLE, cycle);

  // Write the palette length + palette
  EEPROM.update(EEPROM_PALETTE, paletteColorNum);
  for (unsigned int i = 0; i < paletteColorNum; i++) {
    EEPROM.update(EEPROM_PALETTE + i * 3 + 1, palette[i].r);
    EEPROM.update(EEPROM_PALETTE + i * 3 + 2, palette[i].g);
    EEPROM.update(EEPROM_PALETTE + i * 3 + 3, palette[i].b);
  }

  for (auto& effect : effects) {
    effect->writeToEEPROM();
  }
}

CRGB *getPaletteColor(unsigned char index) {
  return &palette[index % paletteColorNum];
}

CRGB *nextPaletteColor(unsigned char *counter) {
  (*counter)++;
  if (*counter >= paletteColorNum) {
    *counter = (*counter) % paletteColorNum;
  }
  return getPaletteColor(*counter);
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
      // Note: all numerical parameters of the following commands
      // should be sent in their BINARY form.
      if (strncmp("SAVE", cmdBuf, 4) == 0) {
        // SAVE - save everything to EEPROM
        writeAllToEEPROM();
      } else if (strncmp("MODE ", cmdBuf, 5) == 0) {
        // MODE <mode_id> - switch pattern to mode_id
        curEffect = cmdBuf[5] % EFFECTS_NUM;
      } else if (strncmp("BRIGHT ", cmdBuf, 7) == 0) {
        // BRIGHT <brightness> - change brightness value
        brightness = cmdBuf[7];
      } else if (strncmp("CYCLE ", cmdBuf, 6) == 0) {
        // CYCLE <cycle_period> - set cycle to cycle_period. This only affects FPS.
        cycle = cmdBuf[6];
      } else if (strncmp("PALETTE ", cmdBuf, 8) == 0) {
        // PALETTE <r1> <g1> <b1> <r2> <g2> <b2> .... - set the palette. Maximum of 6 colors.
        paletteColorNum = 255; // Mark that we haven't determined the number yet.
        for (unsigned int i = 0; i < NUM_PALETTE_COLORS_MAX; i++) {
          unsigned int startPos = 8 + i * 3;
          if (startPos >= len || startPos + 2 > len) {
            paletteColorNum = i;
            break;
          } else {
            palette[i] = CRGB(cmdBuf[startPos], cmdBuf[startPos + 1], cmdBuf[startPos + 2]);
          }
        }
        
        if (paletteColorNum == 255) {
          // The previous loop finished without overflowing the command sequence
          // So we have a maximum of 6 colors
          paletteColorNum = 6;
        }

        for (auto& effect : effects) {
          effect->onParamUpdate();
        }
      } else {
        // Delegate command to the current effect
        if (curEffect < EFFECTS_NUM) {
          effects[curEffect]->handleCommand(cmdBuf, len);
        }
      }
    }
  }
}