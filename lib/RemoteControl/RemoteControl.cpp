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

#include "RemoteControl.h"
#include "App.h"

void RemoteControl::begin(){
    _irrecv.enableIRIn(); // Start the receiver
}

void RemoteControl::tick(){

    long now = millis();
    // handle the confirm mode with tick behind,
    // so the app state is not reset before the
    // other modules have a chance to handle the
    // confirm/cancel event

    if(_state == IR_STATE_WAIT_CONFIRM) handleConfirm();

    // reset key state
    _lastkey = IR_KEY_NONE;

    // handle timeout (only in normal mode)
    if(app.globalMode == globmode_NORMAL &&
       now - _lastrecvtime >= IR_IDLE_TIMEOUT) {
        if(_state == IR_STATE_WAIT_CONFIRM) {
            // if executing command, send the cancel key
            // during one tick before continuing
            _lastkey = IR_KEY_CANCEL;
            resetState();
            return; //
        }else{
            // else, simply reset the state
            resetState();
        }
    }

    // read next
    decode_results recv_key;

    if (_irrecv.decode(&recv_key)) {
        _lastkey = recv_key.value;
        _lastrecvtime = now;
        _irrecv.resume(); // Receive the next value

    }else{
        // no key received, nothing to do
        return;
    }

    // handle cmd
    switch(_state) {
    case IR_STATE_WAIT_PIN: handlePinCode(); break;
    case IR_STATE_WAIT_CMD: handleCmd(); break;
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

void RemoteControl::handlePinCode(){

    int key_nbr = lastKeyToInt();
    if(key_nbr < 0) return; // ignore non number key (TODO)
    int expected = app.pinCode[key_nbr];

    if(key_nbr == expected) {
        _pincode_idx++;
        if(_pincode_idx >= 4) {
            // the whole code has been entered.
            _state = IR_STATE_WAIT_CMD;
            _pincode_idx = 0;
        }
    }else{
        _pincode_idx = 0; // reset
    }
}

void RemoteControl::resetState(){
    app.globalMode = globmode_NORMAL;
    app.configMode = confmode_None;
    _state = IR_STATE_WAIT_PIN;
    _pincode_idx = 0;
}

void RemoteControl::handleCmd(){
    // next state
    _state = IR_STATE_WAIT_CONFIRM;
    switch(_lastkey) {
    case IR_KEY_1:  app.globalMode = globmode_LEARN; break;
    case IR_KEY_2:  app.configMode = confmode_DI; break;
    case IR_KEY_3:  app.configMode = confmode_DF; break;
    default: // no command match, back to wait cmd
        _state = IR_STATE_WAIT_CMD;
    }
}

void RemoteControl::handleConfirm(){
    if(_lastkey == IR_KEY_OK || _lastkey == IR_KEY_CANCEL) {
        // if confirmation or cancel received, back to wait cmd state
        resetState();
        _state = IR_STATE_WAIT_CMD;
    }
}

IRKey RemoteControl::keypressed(){
    return _lastkey;
}
