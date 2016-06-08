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

#include <Arduino.h>
#include <Streaming.h>
#include <EEPROM.h>
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

#define VERSION "1.0.0"
#define ONBOARD_LED 13

// leds are the red/green LEDs connected at each sensor. The arguments
// of the LED constructors represent the Arduino pins where the leds are
// connected. The program is made for common anode LEDs (connect the anode
// to 5V and the cathodes, through a resistor, to the arduino pins).
LED leds[NB_OF_QUESTIONS] = {
    LED(23,24), // connect the 1st LED to the pins 23 (red) and 24 (green)
    LED(33,34), // connect the 2nd LED to the pins 33 (red) and 34 (green)
    LED(43,44)  // connect the 3rd LED to the pins 43 (red) and 44 (green)
};

// The RGB status LED (common anode) is connected to the pins 2 (red),
// 3 (green) and 4 (blue). Connect the anode to 5V and the cathodes,
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
    RFIDSensor(0, 22, 53), // connect the SDA of the 1st sensor to pin 22
    RFIDSensor(1, 32, 53), // connect the SDA of the 2nd sensor to pin 32
    RFIDSensor(2, 42, 53)  // connect the SDA of the 3rd sensor to pin 42
};

// solenoids are activates through a relay. The constructor assigns a pin
// and ling the solenoid with the sensor and the led.
Solenoid solenoids[NB_OF_QUESTIONS] = {
    Solenoid(25, sensors[0], leds[0]), // connect the 1st relay to the pins 25
    Solenoid(35, sensors[1], leds[1]), // connect the 2nd relay to the pins 35
    Solenoid(45, sensors[2], leds[2]), // connect the 3rd relay to the pins 45
};

// The IR Sensor for the remote controller is connected to the pin 11
RemoteControl remoteCtrl(11);

LearnModeHandler learnModeHandler(remoteCtrl, sensors);

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
            // error, so blink the status LED forever
            app.statusLed.setColor(COLOR_RED);
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
    Serial << "Volves version " << VERSION << endl;
    Serial << "Copyright (c) 2016 BlueMasters Fribourg" << endl;
    SPI.begin();
    app.statusLed.begin(STATUS_LED_R, STATUS_LED_G, STATUS_LED_B);
    remoteCtrl.begin();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        leds[i].begin();
        solenoids[i].begin();
        sensors[i].begin();
    }
    Serial << "Self-check running..." << endl;
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
    remoteCtrl.tick();
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        sensors[i].tick();
    }
    for (int i = 0; i < NB_OF_QUESTIONS; i++) {
        solenoids[i].tick();
    }
    learnModeHandler.tick();
    app.statusLed.tick();
}
