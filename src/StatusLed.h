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

#ifndef STATUS_LED__H
#define STATUS_LED__H

#define STATUS_LED_BLACK  0x000000
#define STATUS_LED_RED    0xFF0000
#define STATUS_LED_GREEN  0x00FF00
#define STATUS_LED_BLUE   0x0000FF
#define STATUS_LED_WHITE  0xFFFFFF

#include <Arduino.h>
#include "StateMachine.h"

class StatusLed : public StateMachine {

public:
    StatusLed() {};
    StatusLed(int redPin, int greenPin, int bluePin) :
        _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {};
    void begin();
    void begin(int redPin, int greenPin, int bluePin);
    void setColor(uint32_t color);
    void off();
    void tick();
    int selfCheck();

private:
    int _redPin;
    int _greenPin;
    int _bluePin;
    int _tickCount;
};

#endif
