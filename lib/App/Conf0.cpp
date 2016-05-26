#include <Arduino.h>
#include <EEPROM.h>
#include "App.h"

#define CONF0_MAGIC        0x900df00d
#define CONF0_EEPROM_START 0x0000

#define CONF0_MAGIC_ADDR  (CONF0_EEPROM_START)
#define CONF0_PIN_ADDR    (CONF0_EEPROM_START + 4)
#define CONF0_DF_ADDR     (CONF0_EEPROM_START + 28)
#define CONF0_DI_ADDR     (CONF0_EEPROM_START + 30)

#define CONF0_MAX_PIN_LEN (CONF0_DF_ADDR - CONF0_PIN_ADDR - 1)

#define CONF0_ENQ          '>'
#define CONF0_ACK          '!'
#define CONF0_REACTION_TIME 500
#define CONF0_BUFFER_SIZE   32

int conf0GetChar() {
    while (Serial.available() <= 0) {};
    return Serial.read();
}

int conf0GetCharTimeout(unsigned long timeout) {
    unsigned long now = millis();
    while ((millis() - now) < timeout) {
        if (Serial.available()) {
            return Serial.read();
        }
    };
    return -1;
}

void conf0ReadEEPROM() {
    uint32_t magic;
    char buffer[CONF0_MAX_PIN_LEN + 1];
    EEPROM.get(CONF0_MAGIC_ADDR, magic);
    if (magic == CONF0_MAGIC) {
        int i = 0;
        while (i < CONF0_MAX_PIN_LEN) {
            char c = EEPROM[CONF0_PIN_ADDR + i];
            if (c == 0) break;
            buffer[i++] = c;
        }
        buffer[i] = '\x00';
        app.pinCode = String(buffer);
        EEPROM.get(CONF0_DF_ADDR + 28, app.DF);
        EEPROM.get(CONF0_DI_ADDR + 30, app.DI);
    }
}

void conf0WriteEEPROM() {
    uint32_t magic = CONF0_MAGIC;
    char buffer[CONF0_MAX_PIN_LEN + 1];
    app.pinCode.toCharArray(buffer, CONF0_MAX_PIN_LEN + 1);

    EEPROM.put(CONF0_MAGIC_ADDR, magic);
    int i = 0;
    while (i < CONF0_MAX_PIN_LEN) {
        char c = buffer[i];
        EEPROM[CONF0_PIN_ADDR + i] = c;
        if (c == 0) break;
        i++;
    }
    EEPROM[CONF0_PIN_ADDR + i] = 0;
    EEPROM.put(CONF0_DF_ADDR + 28, app.DF);
    EEPROM.put(CONF0_DI_ADDR + 30, app.DI);
}

void conf0Configure() {
    char buffer[CONF0_BUFFER_SIZE];
    bool dirty = false;
    Serial.write(CONF0_ENQ);
    int res = conf0GetCharTimeout(CONF0_REACTION_TIME);
    if (res == CONF0_ACK) {
        while(1) {
            char cmd = conf0GetChar();
            if (cmd == 'x') { // Exit
                break;
            } else if (cmd == 'd') { // Dump
                Serial.print("p="); Serial.print(app.pinCode); Serial.print(",");
                Serial.print("f="); Serial.print(app.DF); Serial.print(",");
                Serial.print("i="); Serial.print(app.DI); Serial.print(";");
            } else if (cmd == 'c') { // Config
                char var = conf0GetChar();
                int pos = 0;
                while (1) {
                    char c = conf0GetChar();
                    if (c == ';') {
                        buffer[pos] = '\x00';
                        break;
                    }
                    buffer[pos++] = c;
                    if (pos >= CONF0_BUFFER_SIZE) {
                        pos = -1;
                        break;
                    }
                }
                if (pos <= 0) continue;
                dirty = true;
                if (var == 'p') {
                    app.pinCode = String(buffer);
                } else if (var == 'i') {
                    app.DI = String(buffer).toInt();
                } else if (var == 'f') {
                    app.DF = String(buffer).toInt();
                }
            }
        }
    }
    Serial.println();
    if (dirty) {
        conf0WriteEEPROM();
        Serial.println("New configuration saved");
    }
}
