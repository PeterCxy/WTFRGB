#ifndef BREATHING_H
#define BREATHING_H
#include "main.h"
#include "effect.h"

class BreathingEffect : public Effect {
 private:
  void _reset();
 public:
  void reset();
  void onUpdate();
  void handleCommand(char *cmdBuf, int len);
};

#endif