#ifndef RFIDSENSOR__H
#define RFIDSENSOR__H

#include <Arduino.h>
#include "StateMachine.h"
#include "WolvesTypes.h"

enum rfidSensorStatus { NO_CARD, INVALID_CARD, VALID_CARD } ;

class RFIDSensor : public StateMachine {
public:
  RFIDSensor(int csPin, int resetPin) : _csPin(csPin), _resetPin(resetPin) {};
  boolean hasChanged();
  struct rfidUid cardId();
  enum rfidSensorStatus rfidSensorStatus();
  void begin();
  void tick();

private:
  int _csPin;
  int _resetPin;
  enum rfidSensorStatus _status;
  bool _changed;
  struct rfidUid _cardId;
};

#endif
