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

#ifndef RGBLED__H
#define RGBLED__H

#define COLOR_BLACK  0x000000
#define COLOR_RED    0xFF0000
#define COLOR_GREEN  0x00FF00
#define COLOR_BLUE   0x0000FF
#define COLOR_WHITE  0xFFFFFF

#include <Arduino.h>

class RGBLED {

public:
    RGBLED(int redPin, int greenPin, int bluePin) :
        _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {};
    void begin();
    void setColor(uint32_t color);
    void off();

private:
    int _redPin;
    int _greenPin;
    int _bluePin;
};


#endif
