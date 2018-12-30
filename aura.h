#ifndef AURA_H
#define AURA_H
#include "main.h"
#include "effect.h"

class AuraEffect : public Effect {
 public:
  void onUpdate();
  void reset();
};

#endif