#include <Arduino.h>
#include "DelayConfigurator.h"
#include "RemoteControl.h"
#include "Streaming.h"
#include "App.h"

void DelayConfigurator::tick(){
    if(_enabled) {
        IRKey key = _remote.keypressed();

   switch(key){
       // note: here, we rely on the RemoteControl module to update
       // the configMode, to be sure it stays coherent
     case IR_KEY_CANCEL: _enabled = false; break;
     case IR_KEY_OK: save(); break;
     case IR_KEY_PLUS: increment(); break;
     case IR_KEY_MINUS: decrement(); break;
   }
 }else{
   if(app.configMode == _myMode){
     enable();
   }
 }
}

void DelayConfigurator::save(){
    // write to eeprom TODO
    // update RAM
    _target = _tempvalue;
}

void DelayConfigurator::enable(){
    _enabled = true;
    _tempvalue = _target;
}

void DelayConfigurator::increment(){
    if(_tempvalue < _max) _tempvalue += _increment;
    #ifdef APP_DEBUG
    Serial << "  " << _myMode << ": increment -> " << _tempvalue << endl;
    #endif
}

void DelayConfigurator::decrement(){
    if(_tempvalue > _min) _tempvalue -= _increment;
    #ifdef APP_DEBUG
    Serial << "  " << _myMode << ": decrement -> " << _tempvalue << endl;
    #endif
}
