/****************************************************************************
 * Copyright 2016 BlueMasters
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#ifndef SOLENOID__H
#define SOLENOID__H

#include <Arduino.h>
#include "StateMachine.h"
#include "LED.h"
#include "RFIDSensor.h"
#include "WolvesTypes.h"

enum solenoidState {
    SOLENOID_IDLE,
    SOLENOID_FROZEN,
    SOLENOID_FIRED,
    SOLENOID_WAITING
};

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
    void release(long t);
};

#endif
