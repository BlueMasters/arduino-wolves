#include <Arduino.h>
#include "DelayConfigurator.h"
#include "RemoteControl.h"
#include "App.h"

void DelayConfigurator::tick(){
    if(_enabled) {
        IRKey key = _remote->keypressed();

        switch(key) {
        case IR_KEY_CANCEL:
            _enabled = false;
            break;
        case IR_KEY_OK:
            save();
            break;
        case IR_KEY_PLUS:
            if(_tempvalue < _max) _tempvalue += _increment;
            break;
        case IR_KEY_MINUS:
            if(_tempvalue > _min) _tempvalue -= _increment;
            break;
        }
    }else{
        if(app.configMode == _myMode) {
            enable();
        }
    }
}

void DelayConfigurator::save(){
    // write to eeprom TODO
    // update RAM
    *_target = _tempvalue;
}

void DelayConfigurator::enable(){
    _enabled = true;
    _tempvalue = *_target;
}
