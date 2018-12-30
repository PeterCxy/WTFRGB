#ifndef BREATHING_H
#define BREATHING_H
#include "effect.h"
#include "main.h"

class BreathingEffect : public Effect {
 public:
  void onUpdate();
};

#endif