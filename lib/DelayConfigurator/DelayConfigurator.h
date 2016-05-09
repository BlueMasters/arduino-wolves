#ifndef DELAY_CONFIGURATOR__H
#define DELAY_CONFIGURATOR__H

#include <Arduino.h>
#include "StateMachine.h"
#include "RemoteControl.h"
#include "App.h"

class DelayConfigurator : public StateMachine {
public:
DelayConfigurator(uint8_t * delayToConfigure, enum configMode myMode,
                  RemoteControl * remote, uint8_t min, uint8_t max, uint8_t increment)
    : _target(delayToConfigure), _myMode(myMode), _remote(remote),
    _min(min + increment), _max(max - increment), _increment(increment) {
}
virtual void tick();

private:
void save();
void enable();

uint8_t * _target;
enum configMode _myMode;
RemoteControl * _remote;
uint8_t _min;
uint8_t _max;
uint8_t _increment;
bool _enabled;
uint8_t _tempvalue;
};

#endif
