#include <Streaming.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "App.h"

#define SERIAL_INIT_TIMEOUT 1500
#define SERIAL_TIMEOUT 1500

App app;

void App::loadApp() {
    Serial << "Press ENTER now if you want to reset the PIN code." << endl;
    Serial.setTimeout(SERIAL_INIT_TIMEOUT);
    if (Serial.read() == '\r') {
        Serial << "Enter the new PIN and press ENTER." << endl;

    }
    Serial.setTimeout(SERIAL_TIMEOUT);
}
