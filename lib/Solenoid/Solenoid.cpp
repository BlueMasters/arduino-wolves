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
#define EMERGENCY_OPEN 5000 // msec

bool Solenoid::_mutexSet = 0;

void Solenoid::begin() {
    off();
    pinMode(_impulsePin, OUTPUT);
    _state = SOLENOID_IDLE;
    _currentAnswer = UNDEFINED;
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
    long now = millis();
    // Read sensor state
    enum triState newSensorState = _sensor.rfidSensorStatus();
    // If we have a card, so we save this in the _currentAnswer attribute.
    // We will receive this information only once, and it might arrive
    // during a "FROZEN" state, so we have to save it.
    if (newSensorState == TRUE || newSensorState == FALSE) {
        _currentAnswer = newSensorState;
    }

    switch (_state) {
    case SOLENOID_IDLE:
        if (app.emergency) {
            app.activeCount++;
            fire(now);
            _delay = EMERGENCY_OPEN;
            _state = SOLENOID_FIRED;
        } else if (_currentAnswer == TRUE) {
            app.activeCount++;
            _timestamp = now;
            _state = SOLENOID_FROZEN;
            _currentAnswer = FALSE;
        } else if (_currentAnswer == FALSE) {
            app.activeCount++;
            _timestamp = now;
            _state = SOLENOID_FROZEN;
            _currentAnswer = UNDEFINED;
        } else { // Idle and no reason to change.
            off();
        }
        break;

    case SOLENOID_FROZEN:
        if (now - _timestamp > app.DF && !_mutexSet) {
            _mutexSet = true;
            fire(now);
            _delay = app.DI;
            _state = SOLENOID_FIRED;
            // I decided to not clear the LED here. It will stay
            // a little bit longer and will be cleared in the next state.
        } break;

    case SOLENOID_FIRED:
        if (now - _timestamp > _delay) {
            release(now);
            _state = SOLENOID_WAITING;
        } break;

    case SOLENOID_WAITING:
        if (now - _timestamp > SOLENOID_WAITING_TIME) {
            app.activeCount--;
            _mutexSet = false;
            _state = SOLENOID_IDLE;
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

int Solenoid::selfCheck2() {
    return 0;
}

enum triState Solenoid::currentAnswer(){
    return _currentAnswer;
}
