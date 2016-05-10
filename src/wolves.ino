#include <Arduino.h>
#include <Streaming.h>
#include "App.h"
#include "LED.h"
#include "RGBLED.h"
#include "RFIDSensor.h"
#include "Solenoid.h"

#define N_OF_QUESTIONS 3

LED leds[N_OF_QUESTIONS] = {
    LED(23,24),
    LED(33,34),
    LED(43,44)
};

RFIDSensor sensors[N_OF_QUESTIONS] = {
    RFIDSensor(0, 22, 53),
    RFIDSensor(1, 32, 53),
    RFIDSensor(2, 42, 53)
};

Solenoid solenoids[N_OF_QUESTIONS] = {
    Solenoid(25, sensors[0], leds[0]),
    Solenoid(35, sensors[1], leds[1]),
    Solenoid(45, sensors[2], leds[2]),
};

void testStatusLed() {
    app.statusLed.setColor(COLOR_RED);
    delay(1000);
    app.statusLed.setColor(COLOR_GREEN);
    delay(1000);
    app.statusLed.setColor(COLOR_BLUE);
    delay(1000);
    app.statusLed.off();
}

void testQuestions() {
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].red();
        solenoids[i].on();
        delay(200);
    }
    delay(1000);
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].green();
        solenoids[i].off();
        delay(200);
    }
    delay(1000);
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].off();
    }
}

void setup() {
    Serial.begin(9600);
    app.statusLed.begin(2,3,4);
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].begin();
        //sensors[i].begin();
        solenoids[i].begin();
    }
    // Testing
    testStatusLed();
    testQuestions();
}



void loop() {}
