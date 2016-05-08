#ifndef REMOTE_CONTROL__H
#define REMOTE_CONTROL__H

#include <Arduino.h>
#include <IRremote.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

#define IR_IDLE_TIMEOUT 6000 // in ms

// TODO: proper keybindings
#define IR_KEY_NONE  1
#define IR_KEY_PLUS  2
#define IR_KEY_MINUS 3
#define IR_KEY_POWER 4
#define IR_KEY_0 5
#define IR_KEY_1 6
#define IR_KEY_2 7
#define IR_KEY_3 8
#define IR_KEY_4 9
#define IR_KEY_5 10
#define IR_KEY_6 11
#define IR_KEY_7 12
#define IR_KEY_8 13
#define IR_KEY_9 14
#define IR_KEY_PREV 15
#define IR_KEY_NEXT 16
#define IR_KEY_CANCEL 17
#define IR_KEY_OK     18

typedef unsigned long IRKey;

enum IRState {
  IR_STATE_WAIT_PIN,
  IR_STATE_WAIT_CMD,
  IR_STATE_WAIT_CONFIRM,
};

class RemoteControl : public StateMachine {
public:
  RemoteControl(int pin) : _irrecv(pin), _state(IR_STATE_WAIT_PIN) {};
  void begin();
  virtual void tick();
  IRKey keypressed();

private:
  void handlePinCode();
  void handleCmd();
  void handleConfirm();
  int lastKeyToInt();
  void resetState();

  IRrecv _irrecv;
  long _lastrecvtime;
  IRKey _lastkey;
  IRState _state;
  int _pincode_idx;
};

#endif
