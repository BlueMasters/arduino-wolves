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
 *****************************************************************************
 *
 * Wolves.ino. This is the main program for the Arduino Wolves project
 *
 *****************************************************************************/

#define VERSION "1.1.3b"
#define ONBOARD_LED 13

#include <Arduino.h>
#include <Streaming.h>
#include "App.h"
#include "Conf0.h"
#include "LED.h"
#include "StatusLed.h"
#include "RFIDSensor.h"
#include "RFIDUid.h"
#include "Solenoid.h"
#include "RemoteControl.h"
#include "LearnModeHandler.h"
#include "AsnLMsg.h"
#include "EEPROM.h"

/***************************************************************************
 * CAVEAT: The librairie RemoteControl uses the Timer2. So pins 9 and 10
 * are not available for PWM
 ***************************************************************************/

// The RGB status LED (common anode) is connected to the pins 4 (red),
// 3 (green) and 2 (blue). Connect the anode to 5V and the cathodes,
// through a resistor, to the arduino pins
#define STATUS_LED_R 2
#define STATUS_LED_G 3
#define STATUS_LED_B 4

// sensors are the RFID sensors. The first argument of the RFIDSensor
// constructor represents the number of the sensor. The second argument
// represents the pin used for the chip select (SDA). The last argument
// represent the pin used for the reset.
// On the MFRC522 module, connect all GND together and th the ground (0V);
// connect all 3.3V pins the the 3.3V pin of the arduino; connect all SCK
// pins to the pin 52 of the arduino mega; connect all MOSI pins to the
// pin 51 of the arduino mega; connect all MISO pins to the pin 50 of the
// arduino mega; connect all RST to the pin 53 of the arduino mega. Note
// that the pin IRQ is not connected.
RFIDSensor sensors[NB_OF_QUESTIONS] = {
//  RFIDSensor(0, 22, 53), // connect the SDA of the 1st sensor to pin 22
//  RFIDSensor(1, 32, 53), // connect the SDA of the 2nd sensor to pin 32
    RFIDSensor(0, 32, 53), // connect the SDA of the 1st sensor to pin 32
    RFIDSensor(1, 22, 53), // connect the SDA of the 2nd sensor to pin 22
    RFIDSensor(2, 42, 53)  // connect the SDA of the 3rd sensor to pin 42
};

// solenoids are activates through a relay. The constructor assigns a pin
// and ling the solenoid with the sensor and the led.
Solenoid solenoids[NB_OF_QUESTIONS] = {
//  Solenoid(26, sensors[0]), // connect the 1st relay to the pins 26
//  Solenoid(36, sensors[1]), // connect the 2nd relay to the pins 36
    Solenoid(36, sensors[0]), // connect the 1st relay to the pins 36
    Solenoid(26, sensors[1]), // connect the 2nd relay to the pins 26
    Solenoid(47, sensors[2]), // connect the 3rd relay to the pins 47
};

// leds are the red/green LEDs connected at each sensor. The arguments
// of the LED constructors represent the Arduino pins where the leds are
// connected. The program is made for common anode LEDs (connect the anode
// to 5V and the cathodes, through a resistor, to the arduino pins).
LED leds[NB_OF_QUESTIONS] = {
//  LED( 5,  6,  7, solenoids[0]), // connect the LED0 to the pins  5,  6,  7 (RGB)
//  LED( 8, 45, 46, solenoids[1]), // connect the LED1 to the pins  8, 45, 46 (RGB)
    LED( 8, 45, 46, solenoids[0]), // connect the LED0 to the pins  8, 45, 46 (RGB)
    LED( 5,  6,  7, solenoids[1]), // connect the LED1 to the pins  5,  6,  7 (RGB)
    LED(11, 12, 44, solenoids[2])  // connect the LED2 to the pins 11, 12, 44 (RGB)
};

// The IR Sensor for the remote controller is connected to the pin 40
RemoteControl remoteCtrl(40);

LearnModeHandler learnModeHandler(remoteCtrl, sensors);

void checkSolenoids() {
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].setColor(LED_COLOR_RED);
        solenoids[i].selfCheck0();
    }
    delay(1000);
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].setColor(LED_COLOR_GREEN);
        solenoids[i].selfCheck1();
    }
    delay(1000);
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].setColor(LED_COLOR_BLACK);
    }
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
                leds[i].setColor(LED_COLOR_GREEN);
            } else {
                leds[i].setColor(LED_COLOR_RED);
            }
        }
        while(1) {
            // error, so blink the status LED forever
            app.statusLed.setColor(STATUS_LED_RED);
            delay(100);
            app.statusLed.off();
            delay(100);
        }
    }
}

void setup() {
    pinMode(ONBOARD_LED, OUTPUT);
    digitalWrite(ONBOARD_LED, HIGH);
    Serial.begin(9600);
    conf0Configure();
    digitalWrite(ONBOARD_LED, LOW);
    Serial << "Volves version " << VERSION << endl <<endl;
    Serial << "**************************************************" << endl;
    Serial << "* Copyright (c) 2016 BlueMasters Fribourg        *" << endl;
    Serial << "*  - Lucy Linder                                 *" << endl;
    Serial << "*  - Jacques Supcik                              *" << endl;
    Serial << "**************************************************" << endl;
    Serial << "* Made for : Musee d'histoire naturelle Fribourg *" << endl;
    Serial << "* http://www.mhnf.ch/                            *" << endl;
    Serial << "**************************************************" << endl;
    Serial << "* The source code is available on github         *" << endl;
    Serial << "* https://github.com/BlueMasters/arduino-wolves  *" << endl;
    Serial << "**************************************************" << endl;
    Serial << endl;
    SPI.begin();

    app.statusLed.begin(STATUS_LED_R, STATUS_LED_G, STATUS_LED_B);

    remoteCtrl.begin();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].begin();
        solenoids[i].begin();
        sensors[i].begin();
    }
    Serial << "Running self-checks..." << endl;
    app.statusLed.selfCheck();
    checkSolenoids();
    checkRFIDSensors();

    Serial << "Self-check OK." << endl << endl;
    app.loadApp();
    learnModeHandler.begin();
    #ifdef DEBUG
    if(app.globalMode == globmode_NORMAL) app.dump();
    #endif
}

void loop() {
    app.step();
    remoteCtrl.tick();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        sensors[i].tick();
    }
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        solenoids[i].tick();
    }
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].tick();
    }
    learnModeHandler.tick();
    app.statusLed.tick();
}
