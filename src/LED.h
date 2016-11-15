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

#ifndef LED__H
#define LED__H

#include <Arduino.h>
#include "StateMachine.h"
#include "Solenoid.h"

#define LED_COLOR_BLACK  0x000000
#define LED_COLOR_RED    0xFF0000
#define LED_COLOR_GREEN  0x00FF00
#define LED_COLOR_WHITE  0xFFFFFF
#define LED_COLOR_ORANGE 0xFF6600

class LED : public StateMachine {

public:
    LED(int redPin, int greenPin, int bluePin, Solenoid& solenoid) :
        _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin), _solenoid(solenoid) {};
    void begin();
    void tick();
    void setColor (uint32_t color);

private:
    int _redPin;
    int _greenPin;
    int _bluePin;
    uint32_t _blinkCycleOrigin;
    Solenoid& _solenoid;

};

#endif
