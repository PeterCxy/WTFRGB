#include "main.h"
#include "effect.h"
#include "breathing.h"
#include "aura.h"
#include "ripple.h"

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

Effect* effects[] = {new BreathingEffect(), new AuraEffect(), new RippleEffect()};
int curEffect = 2;

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
      }
    }
  }
}