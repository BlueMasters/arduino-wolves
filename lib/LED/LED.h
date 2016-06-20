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

class LED : public StateMachine {

public:
    LED() {};
    LED(int id, int redPin, int greenPin, int bluePin) :
        _id(id), _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {};
    void begin();
    void begin(int id, int redPin, int greenPin, int bluePin);
    void tick();
    void off();
    void red();
    void green();
    void white();

    void idle();
    bool isIdle();

private:
    int _id;
    int _redPin;
    int _greenPin;
    int _bluePin;
    bool _idle;
    static int _idleCount;

};

#endif
