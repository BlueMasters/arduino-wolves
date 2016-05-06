#ifndef CAPTOR__H
#define CAPTOR__H

#include <Arduino.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

enum rfidSensorStatus { no_card, invalid_card, valid_card } ;

class RFIDSensor : public StateMachine {
public:
  void begin(int pin);
  boolean hasChanged();
  rfid_uid cardId();
  enum rfidSensorStatus rfidSensorStatus();

private:
  enum rfidSensorStatus _status;
  bool _changed;
  rfid_uid _cardId;
  int _pin;
};

#endif
