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
#include "RFIDSensor.h"

class Solenoid : public StateMachine {
public:
    Solenoid(int impulsePin, RFIDSensor &sensor) :
        _impulsePin(impulsePin), _sensor(sensor){};
    void begin();
    void on();
    void off();
    void tick();
    int selfCheck0();
    int selfCheck1();
    enum triState currentAnswer();


private:
    enum solenoidState {
        SOLENOID_IDLE,
        SOLENOID_FROZEN,
        SOLENOID_FIRED,
        SOLENOID_WAITING
    };
    static bool _mutexSet;
    int _impulsePin;
    RFIDSensor &_sensor;
    solenoidState _state;
    bool _newAnswer;
    enum triState _currentAnswer;
    uint32_t _timestamp;
    uint32_t _delay;
    bool _emergency;

    void fire(long t);
    void release(long t);

};

#endif
