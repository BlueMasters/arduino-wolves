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
#include "RGBLED.h"

void RGBLED::begin() {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void RGBLED::setColor(uint32_t color) {
    int r = (color >>  0) & 0xFF;
    int g = (color >>  8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    analogWrite(_redPin,   r);
    analogWrite(_greenPin, g);
    analogWrite(_bluePin,  b);
}

void RGBLED::off() {
    setColor(COLOR_BLACK);
}
