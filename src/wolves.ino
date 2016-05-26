/****************************************************************************
 * Copyright 2016 BlueMasters
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <Arduino.h>
#include <Streaming.h>
#include <EEPROM.h>
#include "App.h"
#include "Conf0.h"
#include "LED.h"
#include "RGBLED.h"
#include "RFIDSensor.h"
#include "Solenoid.h"
#include "RemoteControl.h"


#define VERSION "0.0.1"

#define HEARTBEAT_COUNTER 10000
#define HEARTBEAT_WIDTH     100

LED leds[NB_OF_QUESTIONS] = {
    LED(23,24),
    LED(33,34),
    LED(43,44)
};

RFIDSensor sensors[NB_OF_QUESTIONS] = {
    RFIDSensor(0, 22, 53),
    RFIDSensor(1, 32, 53),
    RFIDSensor(2, 42, 53)
};

Solenoid solenoids[NB_OF_QUESTIONS] = {
    Solenoid(25, sensors[0], leds[0]),
    Solenoid(35, sensors[1], leds[1]),
    Solenoid(45, sensors[2], leds[2]),
};

RemoteControl remoteCtrl(11);

void checkSolenoids() {
    for (int i = 0; i < NB_OF_QUESTIONS; i++)
        solenoids[i].selfCheck0();
    delay(1000);
    for (int i = 0; i < NB_OF_QUESTIONS; i++)
        solenoids[i].selfCheck1();
    delay(1000);
    for (int i = 0; i < NB_OF_QUESTIONS; i++)
        solenoids[i].selfCheck2();
}

void checkRFIDSensors() {
    int countOk = 0;
    bool sensorOK[NB_OF_QUESTIONS];
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        bool ok = sensors[i].selfCheck();
        sensorOK[i] = ok;
        if (ok) countOk++;
        #ifdef APP_DEBUG
        Serial << "Checking Sensor " << i << ": " << (ok ? "OK" : "FAILED") << endl;
        #endif
    }
    if (countOk != NB_OF_QUESTIONS) {
        for (int i = 0; i < NB_OF_QUESTIONS; i++) {
            if (sensorOK[i]) {
                leds[i].green();
            } else {
                leds[i].red();
            }
        }
        while(1) {
            app.statusLed.setColor(COLOR_RED);
            delay(100);
            app.statusLed.off();
            delay(100);
        }
    }
}

void setup() {
    Serial.begin(9600);
    conf0Configure();
    Serial << "Volves version " << VERSION << endl;
    Serial << "Copyright (c) 2016 BlueMasters Fribourg" << endl;
    SPI.begin();
    app.statusLed.begin(2,3,4);
    remoteCtrl.begin();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].begin();
        solenoids[i].begin();
        sensors[i].begin();
    }
    // app.statusLed.selfCheck();
    // checkSolenoids();
    checkRFIDSensors();
    for (int i = 0; i < 4; i++) {
        app.pinCode[i] = i+1;
    }
    app.DF = 2500;
    app.DI = 1000;
    // app.loadApp();
}

void loop() {
    static long heartbeat = 0;
    remoteCtrl.tick();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        sensors[i].tick();
    }
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        solenoids[i].tick();
    }
    // heartbeat++;
    // if (heartbeat > HEARTBEAT_COUNTER) {
    //     app.statusLed.off();
    //     heartbeat = 0;
    // } else if (heartbeat > HEARTBEAT_COUNTER - HEARTBEAT_WIDTH){
    //     app.statusLed.setColor(COLOR_BLUE);
    // }
}
