#include <Arduino.h>
#include "ws2812.h"

void postByte(unsigned char b, unsigned char portMask) {
  for (int i = 7; i >= 0; i--) {
    if ((b >> i) & 0x01) {
      PORTD |= portMask;
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
      PORTD &= ~portMask;
      __asm__("nop\n\t");
    } else {
      PORTD |= portMask;
      __asm__("nop\n\t");
      PORTD &= ~portMask;
      __asm__("nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t""nop\n\t");
    }
  }
}

void postLedUpdate(CRGB *leds, int len, int ledPin) {
  unsigned char portMask = 1 << ledPin;
  cli();
  for (int i = 0; i < len; i++) {
    postByte(leds[i].g, portMask);
    postByte(leds[i].r, portMask);
    postByte(leds[i].b, portMask);
  }
  sei();
}