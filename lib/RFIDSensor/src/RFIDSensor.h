#ifndef RFIDSENSOR__H
#define RFIDSENSOR__H

#include <Arduino.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

enum rfidSensorStatus { no_card, invalid_card, valid_card } ;

class RFIDSensor : public StateMachine {
public:
  void begin(int pin);
  boolean hasChanged();
  struct rfidUid cardId();
  enum rfidSensorStatus rfidSensorStatus();

private:
  enum rfidSensorStatus _status;
  bool _changed;
  struct rfidUid _cardId;
  int _pin;
};

#endif
