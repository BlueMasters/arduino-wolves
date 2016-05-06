#ifndef SOLENOID__H
#define SOLENOID__H

#include <Arduino.h>
#include "StateMachine.h"
#include "LED.h"
#include "RFIDSensor.h"
#include "WolvesTypes.h"

enum solenoidState {SOLENOID_IDLE, SOLENOID_FIRED, SOLENOID_WAITING};

class Solenoid : public StateMachine {
public:
  Solenoid(int impulsePin, RFIDSensor sensor, LED led) :
    _impulsePin(impulsePin), _sensor(sensor), _led(led) {};
  void begin();
  void tick();

private:
  int _impulsePin;
  RFIDSensor _sensor;
  LED _led;
  solenoidState _state;
  long _timestamp;
  void fire(long t);
};

#endif
