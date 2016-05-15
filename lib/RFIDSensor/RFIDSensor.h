#ifndef RFIDSENSOR__H
#define RFIDSENSOR__H

#include <Arduino.h>
#include "StateMachine.h"
#include "RFIDUid.h"
#include <MFRC522.h>

enum rfidSensorStatus { NO_CARD, INVALID_CARD, VALID_CARD };

class RFIDSensor : public StateMachine {
public:
    RFIDSensor(int id) : _id(id) {};
    RFIDSensor(int id, int csPin, int resetPin) :
        _id(id), _status(NO_CARD), _changed(false), _mfrc522(csPin, resetPin) {};

    bool changed();
    struct rfidUid cardId();
    enum rfidSensorStatus rfidSensorStatus();
    void begin();
    void begin(int csPin, int resetPin);
    bool selfCheck();
    int id(){
        return _id;
    };
    void tick();

private:
    int _id;
    enum rfidSensorStatus _status;
    bool _changed;
    MFRC522 _mfrc522;
    struct rfidUid _cardId;

    void validateNewCard();
};

#endif
