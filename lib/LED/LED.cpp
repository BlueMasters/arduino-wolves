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
#include "LED.h"
#include "App.h"

#define IDLE_WHITE_ON 100
#define IDLE_WHITE_OFF 105

int LED::_idleCount = 0;

void LED::begin() {
    off();
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LED::begin(int id, int redPin, int greenPin, int bluePin) {
    _id = id;
    _redPin = redPin;
    _greenPin = greenPin;
    _bluePin = bluePin;
    _idle = true;
    begin();
}

void LED::tick() {
    if (app.allIdle) {
        if (_idleCount < IDLE_WHITE_ON) {
            white();
            _idleCount++;
        } else if (_idleCount < IDLE_WHITE_OFF) {
            off();
            _idleCount++;
        } else {
            _idleCount = 0;
        }
    } else {
        _idleCount = 0;
        if (_idle) {
            off();
        }
    }
}

void LED::off() {
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_bluePin, HIGH);
    _idle = false;
}

void LED::idle() {
    _idle = true;
}

bool LED::isIdle() {
    return _idle;
}

void LED::green() {
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, LOW);
    digitalWrite(_bluePin, HIGH);
    _idle = false;
}

void LED::red() {
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_redPin, LOW);
    digitalWrite(_bluePin, HIGH);
    _idle = false;
}

void LED::white() {
    digitalWrite(_greenPin, LOW);
    digitalWrite(_redPin, LOW);
    digitalWrite(_bluePin, LOW);
    _idle = false;
}
