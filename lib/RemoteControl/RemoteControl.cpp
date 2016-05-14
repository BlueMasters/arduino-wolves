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

#define IR_LED_CHOOSE_CMD 0xFF7400 // orange
#define IR_LED_CONFIG_DF  0xAE56DF // light violet
#define IR_LED_CONFIG_DI  0x400362 // dark violet
#define IR_LED_LEARN      0x3029D6 // bright blue

void RemoteControl::begin(){
    _irrecv.enableIRIn(); // Start the receiver
}

void RemoteControl::tick(){

    long now = millis();

    app.emergency = false; // always reset the flag

    // handle the confirm mode with one tick behind,
    // so the app state is not reset before the
    // other modules have a chance to handle the
    // confirm/cancel event
    if(_state == IR_STATE_CONFIG || _state == IR_STATE_LEARN) {
        if(_lastkey == IR_KEY_OK || _lastkey == IR_KEY_CANCEL) {
            // back to wait cmd state (if timeout, will be handled below)
            resetState();
            setState(IR_STATE_CHOOSE_CMD);
        }
    }

    // reset key state
    _lastkey = IR_KEY_NONE;

    // handle timeout (only in normal mode)
    if(app.globalMode == globmode_NORMAL &&
       now - _lastrecvtime >= IR_IDLE_TIMEOUT) {
        if(_state == IR_STATE_CONFIG) {
            // if executing command, send the cancel key
            // during one tick before continuing
            _lastkey = IR_KEY_CANCEL;
            return; // ensure the cancel key stays for one tick
            // next tick, handleConfirm will be called and the
            // state will be reset
        } else {
            // else, simply reset the state
            // and read next key
            resetState();
        }
    }

    // read next
    decode_results recv_key;

    if (_irrecv.decode(&recv_key)) {
        _lastrecvtime = now;
        _irrecv.resume(); // Receive the next value
        if(recv_key.value == 0xFFFFFFFF) // -1 = key still pressed
            return;
        _lastkey = recv_key.value;
        #ifdef APP_DEBUG
        Serial << "IR Received : " << _HEX(recv_key.value) << "(" << _lastkey << ")" << endl;
        #endif

        if(_lastkey == IR_KEY_POWER) {
            app.emergency = true;
            return;
        }

    } else {
        // no key received, nothing to do
        return;
    }

    // handle cmd
    switch(_state) {
    case IR_STATE_DEFAULT: handlePinCode(); break;
    case IR_STATE_CHOOSE_CMD: handleCmd(); break;
    default: break;
    }

} // end tick

int RemoteControl::lastKeyToInt(){
    switch(_lastkey) {
    case IR_KEY_0: return 0;
    case IR_KEY_1: return 1;
    case IR_KEY_2: return 2;
    case IR_KEY_3: return 3;
    case IR_KEY_4: return 4;
    case IR_KEY_5: return 5;
    case IR_KEY_6: return 6;
    case IR_KEY_7: return 7;
    case IR_KEY_8: return 8;
    case IR_KEY_9: return 9;
    default: return -1;
    }
}

void RemoteControl::resetState(){
    app.globalMode = globmode_NORMAL;
    app.configMode = confmode_None;
    setState(IR_STATE_DEFAULT);
    _pincode_idx = 0;
}

void RemoteControl::handlePinCode(){

    int key_nbr = lastKeyToInt();
    #ifdef APP_DEBUG
    Serial << "Key Number : " << key_nbr << endl;
    #endif
    if(key_nbr < 0) return; // ignore non number key (TODO)
    int expected = app.pinCode[_pincode_idx];

    if(key_nbr == expected) {
        _pincode_idx++;
        if(_pincode_idx >= 4) {
            // the whole code has been entered.
            setState(IR_STATE_CHOOSE_CMD);
            _pincode_idx = 0;
        }
    } else {
        _pincode_idx = 0; // reset
    }
}

void RemoteControl::handleCmd(){
    // next state
    switch(_lastkey) {

    case IR_KEY_1:  // learn
        app.globalMode = globmode_LEARN;
        setState(IR_STATE_LEARN);
        return;

    case IR_KEY_2: // edit Delay of Feedback
        app.configMode = confmode_DF;
        setState(IR_STATE_CONFIG);
        return;

    case IR_KEY_3: // edit Delay of Impulsion
        app.configMode = confmode_DI;
        setState(IR_STATE_CONFIG);
        return;

    }
}

void RemoteControl::setState(enum IRState newState){
    _state = newState;

    switch (newState) {
    case IR_STATE_DEFAULT:
        app.statusLed.off();
        break;
    case IR_STATE_CHOOSE_CMD:
        app.statusLed.setColor(IR_LED_CHOOSE_CMD);
        break;
    case IR_STATE_LEARN:
        app.statusLed.setColor(IR_LED_LEARN);
        break;
    case IR_STATE_CONFIG:
        app.statusLed.setColor(app.configMode == confmode_DF ? IR_LED_CONFIG_DF : IR_LED_CONFIG_DI);
        break;
    }
}


IRKey RemoteControl::keypressed(){
    return _lastkey;
}
