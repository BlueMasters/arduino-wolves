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

#ifndef RFIDSENSOR__H
#define RFIDSENSOR__H

#include <Arduino.h>
#include "App.h"
#include "StateMachine.h"
#include "RFIDUid.h"
#include <MFRC522.h>

class RFIDSensor : public StateMachine {
public:
    RFIDSensor(int id) : _id(id) {};
    RFIDSensor(int id, int csPin, int resetPin) :
        _id(id), _answer(UNDEFINED), _mfrc522(csPin, resetPin) {};

    struct rfidUid cardId();
    enum triState currentAnswer();
    void begin();
    void begin(int csPin, int resetPin);
    bool selfCheck();
    int id(){
        return _id;
    };
    void tick();

private:
    int _id;
    enum triState _answer;
    MFRC522 _mfrc522;
    struct rfidUid _cardId;

    void validateNewCard();
};

#endif
