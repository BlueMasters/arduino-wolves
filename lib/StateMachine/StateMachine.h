#ifndef STATE_MACHINE__H
#define STATE_MACHINE__H

#include <Arduino.h>

class StateMachine {
public:
virtual void tick() = 0;
};

#endif
