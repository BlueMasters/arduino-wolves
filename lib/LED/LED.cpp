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

#define IDLE_WHITE_ON 20
#define IDLE_WHITE_OFF (IDLE_WHITE_ON + 2)

#define LED_COLOR_IDLE1  0x333333
#define LED_COLOR_IDLE2  0x000000

int LED::_idleCount = 0;

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
        if (_idleCount < IDLE_WHITE_ON) {
            setColor(LED_COLOR_IDLE1);
            if (_id == NB_OF_QUESTIONS - 1) _idleCount++;
        } else if (_idleCount < IDLE_WHITE_OFF) {
            setColor(LED_COLOR_IDLE2);
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
    analogWrite(_redPin,   255-r);
    analogWrite(_greenPin, 255-g);
    analogWrite(_bluePin,  255-b);
}

void LED::idle(bool idl) {
    _idle = idl;
}

bool LED::isIdle() {
    return _idle;
}
