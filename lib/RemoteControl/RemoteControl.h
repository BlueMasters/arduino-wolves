#ifndef REMOTE_CONTROL__H
#define REMOTE_CONTROL__H

#include <Arduino.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

enum IRKey {irkey_ok, irkey_cancel, irkey_prev, irkey_next, irkey_power, irkey_plus, irkey_minus};

class RemoteControl : public StateMachine {
public:
  RemoteControl(int pin) : _pin(pin) {};
  IRKey keypressed();

private:
  int _pin;
};

#endif
