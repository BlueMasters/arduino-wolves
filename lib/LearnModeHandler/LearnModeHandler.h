#ifndef LEARNMODE_HANDLER__H
#define LEARNMODE_HANDLER__H

#include <Arduino.h>
#include "StateMachine.h"
#include "RemoteControl.h"
#include "RFIDSensor.h"
#include "App.h"

class LearnModeHandler : public StateMachine {
public:
    LearnModeHandler(RemoteControl &remote, RFIDSensor *sensors) :
        _remote(remote), _sensors(sensors) {};
    void begin();
    void tick();

private:
    int addCard(struct rfidUid card);
    void addAnswer(int question, int idx);
    void reset();
    RemoteControl &_remote;
    RFIDSensor *_sensors;
    struct wolvesConfig _config;
};

#endif
