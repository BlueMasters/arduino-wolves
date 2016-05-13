#include <Arduino.h>
#include <Streaming.h>
#include "App.h"
#include "LED.h"
#include "RGBLED.h"
#include "RFIDSensor.h"
#include "Solenoid.h"

#define N_OF_QUESTIONS 3
#define HEARTBEAT_COUNTER 10000
#define HEARTBEAT_WIDTH     100

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
    SPI.begin();
    app.statusLed.begin(2,3,4);
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        leds[i].begin();
        solenoids[i].begin();
        sensors[i].begin();
    }
    testStatusLed();
    testQuestions();
}

void loop() {
    static long heartbeat = 0;
    for (int i = 0; i < N_OF_QUESTIONS; i++) {
        solenoids[i].tick();
    }
    heartbeat++;
    if (heartbeat > HEARTBEAT_COUNTER) {
        app.statusLed.off();
        heartbeat = 0;
    } else if (heartbeat > HEARTBEAT_COUNTER - HEARTBEAT_WIDTH){
        app.statusLed.setColor(COLOR_BLUE);
    }
}
