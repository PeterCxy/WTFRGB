#ifndef RIPPLE_H
#define RIPPLE_H
#include "main.h"
#include "effect.h"

class RippleEffect : public Effect {
  public:
    void onUpdate();
};

#endif