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

#define PROTO

unsigned LED::_idleCount = 0;

void LED::begin() {
    setColor(LED_COLOR_BLACK);
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    _idle = true;
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
        if (_idleCount < app.idleTicksOn) {
            setColor(app.idleColorOn);
            if (_id == NB_OF_QUESTIONS - 1) _idleCount++;
        } else if (_idleCount < (app.idleTicksOn + app.idleTicksOff)) {
            setColor(app.idleColorOff);
            if (_id == NB_OF_QUESTIONS - 1) _idleCount++;
        } else {
            if (_id == NB_OF_QUESTIONS - 1) _idleCount = 0;
        }
    } else {
        if (_id == NB_OF_QUESTIONS - 1) _idleCount = 0;
        if (_idle) {
            setColor(LED_COLOR_BLACK);
        }
    }
}

void LED::setColor(uint32_t color) {
    int r = (color >> 16) & 0xFF;
    int g = (color >>  8) & 0xFF;
    int b = (color >>  0) & 0xFF;
    #ifdef PROTO
    analogWrite(_redPin,   255-r);
    analogWrite(_greenPin, 255-g);
    analogWrite(_bluePin,  255-b);
    #else
    analogWrite(_redPin,   r);
    analogWrite(_greenPin, g);
    analogWrite(_bluePin,  b);
    #endif
}

void LED::idle(bool idle) {
    _idle = idle;
}

bool LED::isIdle() {
    return _idle;
}
