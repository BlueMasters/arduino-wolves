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
 
#ifndef REMOTE_CONTROL__H
#define REMOTE_CONTROL__H

#include <Arduino.h>
#include <IRremote.h>
#include "StateMachine.h"
#include "App.h"

#define IR_IDLE_TIMEOUT 6000 // in ms

// a complete list here:
// http://www.instructables.com/id/Arduino-Infrared-Remote-tutorial/step6/Record-button-codes/


#define IR_KEY_NONE  1
#define IR_KEY_POWER 0xFF629D
#define IR_KEY_CH    0xFFE21D
#define IR_KEY_PREV  0xFF22DD
#define IR_KEY_PLAY  0xFFC23D
#define IR_KEY_NEXT  0xFF02FD
#define IR_KEY_MINUS 0xFFE01F
#define IR_KEY_PLUS  0xFFA857

#define IR_KEY_EQ  0xFF906F
#define IR_KEY_100 0xFF9867
#define IR_KEY_200 0xFFB04F

#define IR_KEY_0  0xFF6897
#define IR_KEY_1  0xFF30CF
#define IR_KEY_2  0xFF18E7
#define IR_KEY_3  0xFF7A85
#define IR_KEY_4  0xFF10EF
#define IR_KEY_5  0xFF38C7
#define IR_KEY_6  0xFF5AA5
#define IR_KEY_7  0xFF42BD
#define IR_KEY_8  0xFF48B5
#define IR_KEY_9  0xFF52AD

#define IR_KEY_CANCEL 0xFF22DD // prev
#define IR_KEY_OK     0xFFC23D // play


typedef unsigned long IRKey;

class RemoteControl : public StateMachine {
public:
        RemoteControl(int pin) : _irrecv(pin) {};
        void begin();
        virtual void tick();
        IRKey keypressed();

private:
        void handlePinCode();
        void handleOkCancel();
        char lastKeyToChar();
        IRrecv _irrecv;
        IRKey _lastkey;
        long _lastrecvtime;
        unsigned _pincode_idx;
};

#endif
