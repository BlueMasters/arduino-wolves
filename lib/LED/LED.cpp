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

void LED::begin() {
    off();
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
}

void LED::begin(int redPin, int greenPin) {
    _redPin = redPin;
    _greenPin = greenPin;
    begin();
}

void LED::off() {
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, HIGH);
}

void LED::green() {
    digitalWrite(_redPin, HIGH);
    digitalWrite(_greenPin, LOW);
}

void LED::red() {
    digitalWrite(_greenPin, HIGH);
    digitalWrite(_redPin, LOW);
}
