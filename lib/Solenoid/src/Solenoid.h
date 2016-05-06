#ifndef SOLENOID__H
#define SOLENOID__H

#include <Arduino.h>
#include "StateMachine.h"
#include "LED.h"
#include "RFIDSensor.h"
#include "WolvesTypes.h"

class Solenoid : public StateMachine {
public:
  Solenoid(int impulsePin, RFIDSensor sensor, LED led) : _impulsePin(impulsePin), _sensor(sensor), _led(led) {};

private:
  int _impulsePin;
  RFIDSensor _sensor;
  LED _led;
};

#endif
