#include <Arduino.h>
#include "App.h"
#include "LED.h"
#include "RFIDSensor.h"
#include "Solenoid.h"

LED leds[] = {
    LED(0,0),
    LED(0,0),
    LED(0,0)
};

RFIDSensor sensors[] = {
    RFIDSensor(0, 0),
    RFIDSensor(0, 0),
    RFIDSensor(0, 0)
};

Solenoid solenoids[] = {
    Solenoid(0, sensors[0], leds[0]),
    Solenoid(0, sensors[1], leds[1]),
    Solenoid(0, sensors[2], leds[2]),
};

void setup() {

}


void loop() {

}
