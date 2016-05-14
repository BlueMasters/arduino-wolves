#include "App.h"
#include <Streaming.h>
#include <EEPROM.h>

#define SERIAL_INIT_TIMEOUT 1500
#define SERIAL_TIMEOUT 1500

App app;

void App::readConfig() {
    Serial << "Press ENTER now if you want to reset the PIN code." << endl;
    Serial.setTimeout(SERIAL_INIT_TIMEOUT);
    if (Serial.read() == '\r') {
        Serial << "Enter the new PIN and press ENTER." << endl;
        
    }
    Serial.setTimeout(SERIAL_TIMEOUT);
}
