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

#include <Arduino.h>
#include "Solenoid.h"
#include "App.h"

// Give time for the disk to fall out of the RFID sensor field
#define SOLENOID_WAITING_TIME 200 // msec
#define EMERGENCY_OPEN 2000 // msec

bool Solenoid::_mutexSet = 0;

void Solenoid::begin() {
    off();
    pinMode(_impulsePin, OUTPUT);
    _state = SOLENOID_IDLE;
    _currentAnswer = UNDEFINED;
    _newAnswer = false;
}

void Solenoid::on() {
    digitalWrite(_impulsePin, LOW);
}

void Solenoid::off() {
    digitalWrite(_impulsePin, HIGH);
}

void Solenoid::fire(long t) {
    _timestamp = t;
    on();
}

void Solenoid::release(long t) {
    _timestamp = t;
    off();
}

void Solenoid::tick() {
    // Read sensor state
    enum triState newSensorState = _sensor.currentAnswer();
    // If we have a new card (state is not UNDEFINED), so we save the state in
    // the _currentAnswer attribute. We will receive this information only
    // once, and it might arrive during a "FROZEN" state, so we have to save
    // it here.
    if (newSensorState == TRUE || newSensorState == FALSE) {
        _currentAnswer = newSensorState;
        _newAnswer = true;
    }

    switch (_state) {
    case SOLENOID_IDLE:
        if (app.emergency == NB_OF_QUESTIONS) {
            app.activeCount++;
            _delay = EMERGENCY_OPEN;
            _state = SOLENOID_FROZEN;
            _emergency = true;
        } else if (_newAnswer) {
            app.activeCount++;
            _timestamp = app.now;
            _newAnswer = false;
            _state = SOLENOID_FROZEN;
        } else { // Idle and no reason to change.
            off();
        }
        break;

    case SOLENOID_FROZEN:
        if ((app.now - _timestamp > app.DF || _emergency) && !_mutexSet) {
            _mutexSet = true;
            fire(app.now);
            _delay = _emergency ? EMERGENCY_OPEN : app.DI;
            _state = SOLENOID_FIRED;
            // I decided to not clear the LED here. It will stay
            // a little bit longer and will be cleared in the next state.
        } break;

    case SOLENOID_FIRED:
        if (app.now - _timestamp > _delay) {
            release(app.now);
            _state = SOLENOID_WAITING;
        } break;

    case SOLENOID_WAITING:
        if (app.now - _timestamp > SOLENOID_WAITING_TIME) {
            if (_emergency) {
                _emergency = false;
                app.emergency--;
            }
            _mutexSet = false;
            if (_newAnswer) {
                _timestamp = app.now;
                _newAnswer = false;
                _state = SOLENOID_FROZEN;
            } else {
                app.activeCount--;
                _state = SOLENOID_IDLE;
            }
        } break;
    }
};

int Solenoid::selfCheck0() {
    on();
    delay(200);
    return 0;
}

int Solenoid::selfCheck1() {
    off();
    delay(200);
    return 0;
}

enum triState Solenoid::currentAnswer(){
    if (_state == SOLENOID_FROZEN || _state == SOLENOID_FIRED || _newAnswer) {
        return _currentAnswer;
    } else {
        return UNDEFINED;
    }
}
