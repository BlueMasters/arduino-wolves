#include <Arduino.h>
#include "App.h"
#include "LED.h"
#include "RFIDSensor.h"
#include "Solenoid.h"

#define N_OF_QUESTIONS 3

LED leds[N_OF_QUESTIONS] = {
    LED(0,0),
    LED(0,0),
    LED(0,0)
};

RFIDSensor sensors[N_OF_QUESTIONS] = {
    RFIDSensor(0, 0),
    RFIDSensor(0, 0),
    RFIDSensor(0, 0)
};

Solenoid solenoids[N_OF_QUESTIONS] = {
    Solenoid(0, sensors[0], leds[0]),
    Solenoid(0, sensors[1], leds[1]),
    Solenoid(0, sensors[2], leds[2]),
};

void setup() {
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].begin();
        sensors[i].begin();
        solenoids[i].begin();
    }
}


void loop() {

}
