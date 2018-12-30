#ifndef BREATHING_H
#define BREATHING_H
#include "main.h"
#include "effect.h"

class BreathingEffect : public Effect {
 public:
  void reset();
  void onUpdate();
};

#endif