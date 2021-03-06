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
#include <IRremote.h>
#include <Streaming.h>
#include "RemoteControl.h"
#include "App.h"

void RemoteControl::begin(){
    _irrecv.enableIRIn(); // Start the receiver
}

void RemoteControl::tick(){

    long now = millis();

    // handle ok/cancel one tick late, to allow the
    // other to see the key before resetting the app global mode
    if(app.globalMode == globmode_LEARN){
        handleOkCancel();
    }else if(now - _lastrecvtime >= IR_IDLE_TIMEOUT){
    // handle timeout (only in normal mode)
        _pincode_idx = 0;
    }

    // always reset key state
    _lastkey = IR_KEY_NONE;

    // read next key
    decode_results recv_key;

    if(_irrecv.decode(&recv_key)) { // key received
        _irrecv.resume();
        _lastrecvtime = now;

        if(recv_key.value != 0xFFFFFFFF) { // -1 = key still pressed
            _lastkey = recv_key.value;

            #ifdef APP_DEBUG
            Serial << "IR Received : " << _HEX(recv_key.value) << "(" << _lastkey << ")" << endl;
            #endif

            if(_lastkey == IR_KEY_POWER) {
                app.emergency = NB_OF_QUESTIONS;

            }else if(app.globalMode == globmode_NORMAL){
                handlePinCode();
            }
        }
    }

} // end tick

char RemoteControl::lastKeyToChar(){
    switch(_lastkey) {
    case IR_KEY_0: return '0';
    case IR_KEY_1: return '1';
    case IR_KEY_2: return '2';
    case IR_KEY_3: return '3';
    case IR_KEY_4: return '4';
    case IR_KEY_5: return '5';
    case IR_KEY_6: return '6';
    case IR_KEY_7: return '7';
    case IR_KEY_8: return '8';
    case IR_KEY_9: return '9';
    default: return 0;
    }
}

void RemoteControl::handlePinCode(){

    char key_nbr = lastKeyToChar();
    if(key_nbr == 0) return; // ignore non 'number' key

    char expected = app.pinCode.charAt(_pincode_idx);

    #ifdef APP_DEBUG
    Serial << "Key Number : " << key_nbr << " expected: " << expected << " "  << endl;
    #endif

    if(key_nbr == expected) {
        _pincode_idx++;
        if(_pincode_idx >= app.pinCode.length()) { // the whole code has been entered
            _pincode_idx = 0;
            // switch mode
            app.globalMode = globmode_LEARN;
            // app.statusLed.setColor(IR_LED_LEARN);
            #ifdef APP_DEBUG
            Serial << "switching to learnmode " << endl;
            #endif
        }
    } else {
        _pincode_idx = 0; // wrong digit, reset
    }
}


void RemoteControl::handleOkCancel(){
    if(_lastkey == IR_KEY_OK || _lastkey == IR_KEY_CANCEL){
        #ifdef APP_DEBUG
        Serial << "switch back to normal mode" << endl;
        #endif
        app.globalMode = globmode_NORMAL;
        // app.statusLed.off();
    }
}


IRKey RemoteControl::keypressed(){
    return _lastkey;
}
