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
#define SOLENOID_WAITING_TIME 1000 // msec

void Solenoid::begin() {
    off();
    pinMode(_impulsePin, OUTPUT);
    _state = SOLENOID_IDLE;
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
    _led.off();
}

void Solenoid::tick() {
    long now = millis();

    switch (_state) {
    case SOLENOID_IDLE:
        if (app.emergency) {
            fire(now);
            _state = SOLENOID_FIRED;
            _led.green();
        } else if (_sensor.rfidSensorStatus() == VALID_CARD) {
            _led.green();
            _state = SOLENOID_FROZEN;
        } else if (_sensor.rfidSensorStatus() == INVALID_CARD) {
            _led.red();
            _state = SOLENOID_FROZEN;
        } else { // Idle and no reason to change.
            off();
            _led.off();
        }
        break;

    case SOLENOID_FROZEN:
        if (now - _timestamp > app.DF) {
            fire(now);
            _state = SOLENOID_FIRED;
            // I decided to not clear the LED here. It will stay
            // a little bit longer and will be cleared in the next state.
        } break;

    case SOLENOID_FIRED:
        if (now - _timestamp > app.DI) {
            release(now);
            _state = SOLENOID_WAITING;
        } break;

    case SOLENOID_WAITING:
        if (now - _timestamp > SOLENOID_WAITING_TIME) {
            _state = SOLENOID_IDLE;
        } break;

    default: {
        _state = SOLENOID_IDLE;
        break;
    }
    }
};
