#ifndef RIPPLE_H
#define RIPPLE_H
#include "effect.h"
#include "main.h"

class RippleEffect : public Effect {
 public:
  void onUpdate();
};

#endif