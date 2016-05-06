#ifndef IRREMOTE__H
#define IRREMOTE__H

#include <Arduino.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

enum IRKey {irkey_ok, irkey_cancel, irkey_prev, irkey_next, irkey_power, irkey_plus, irkey_minus};

class IRRemote : public StateMachine {
public:
  IRRemote(int pin) : _pin(pin) {};
  IRKey keypressed();

private:
  int _pin;
};
#endif
