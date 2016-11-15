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
#include "Solenoid.h"

#undef PROTO

void LED::begin() {
    setColor(LED_COLOR_BLACK);
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LED::tick() {
    if (app.emergency > 0) {
        setColor(LED_COLOR_ORANGE);
    } else if (app.activeCount == 0) {
        if (app.tickCount - _blinkCycleOrigin < app.idleTicksOn) {
            setColor(app.idleColorOn);
        } else if (app.tickCount - _blinkCycleOrigin < (app.idleTicksOn + app.idleTicksOff)) {
            setColor(app.idleColorOff);
        } else {
            _blinkCycleOrigin = app.tickCount;
        }
    } else {
        _blinkCycleOrigin = app.tickCount;
        if (_solenoid.currentAnswer() == TRUE) {
            if (app.globalMode == globmode_LEARN)
                setColor(LED_COLOR_ORANGE);
            else
                setColor(LED_COLOR_GREEN);
        } else if (_solenoid.currentAnswer() == FALSE) {
            if (app.globalMode == globmode_LEARN)
                setColor(LED_COLOR_ORANGE);
            else
                setColor(LED_COLOR_RED);
        } else {
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
