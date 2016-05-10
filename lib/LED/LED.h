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

class LED {

public:
    LED() {};
    LED(int redPin, int greenPin) : _redPin(redPin), _greenPin(greenPin) {};
    void begin();
    void begin(int redPin, int greenPin);
    void off();
    void red();
    void green();

private:
    int _redPin;
    int _greenPin;
};


#endif
