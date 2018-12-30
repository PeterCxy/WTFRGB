#ifndef WS2812_H
#define WS2812_H

class CRGB {
  public:
    unsigned char r, g, b;
    CRGB() {};
    CRGB(unsigned char r, unsigned char g, unsigned char b) {
      this->r = r;
      this->g = g;
      this->b = b;
    };
};

void postLedUpdate(CRGB *leds, int len, int ledPin);

#endif