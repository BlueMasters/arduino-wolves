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
    pinMode(_red, OUTPUT);
    pinMode(_green, OUTPUT);
}

void LED::off() {
    digitalWrite(_red, LOW);
    digitalWrite(_green, LOW);
}

void LED::green() {
    digitalWrite(_red, LOW);
    digitalWrite(_green, HIGH);
}

void LED::red() {
    digitalWrite(_green, LOW);
    digitalWrite(_red, HIGH);
}
