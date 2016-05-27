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
#include "StatusLed.h"
#include "App.h"
#ifdef DEBUG
#include <Stream.h>
#endif

#define HEARTBEAT_COUNTER  100
#define HEARTBEAT_WIDTH      1
#define COLOR_CHOOSE_CMD   0xFF7400 // orange
#define COLOR_LEARN        0x3029D6 // bright blue
#define COLOR_HEART_BEAT   0x000033 // dark blue

void StatusLed::begin() {
    setColor(COLOR_BLACK);
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
    _tickCount = 0;
}

void StatusLed::begin(int redPin, int greenPin, int bluePin) {
    _redPin = redPin;
    _greenPin = greenPin;
    _bluePin = bluePin;
    begin();
}

void StatusLed::setColor(uint32_t color) {
    int r = (color >>  0) & 0xFF;
    int g = (color >>  8) & 0xFF;
    int b = (color >> 16) & 0xFF;
    analogWrite(_redPin,   255-r);
    analogWrite(_greenPin, 255-g);
    analogWrite(_bluePin,  255-b);
}

void StatusLed::off() {
    setColor(COLOR_BLACK);
}

int StatusLed::selfCheck() {
    setColor(COLOR_RED);
    delay(1000);
    setColor(COLOR_GREEN);
    delay(1000);
    setColor(COLOR_BLUE);
    delay(1000);
    off();
    return 0;
}

void StatusLed::tick() {
    _tickCount++;
    if (_tickCount > HEARTBEAT_COUNTER) {
        _tickCount = 0;
    }
    int heartBeatLed = (_tickCount > (HEARTBEAT_COUNTER - HEARTBEAT_WIDTH));

    if (app.globalMode == globmode_LEARN) {
        setColor(COLOR_LEARN);
    } else if (app.globalMode == globmode_NORMAL) {
        if (heartBeatLed) setColor(COLOR_HEART_BEAT); else off();
    } else {
        if (heartBeatLed) setColor(COLOR_RED); else off();
    }

}
