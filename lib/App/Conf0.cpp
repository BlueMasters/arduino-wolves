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

#include "AsnLMsg.h"
#include "AsnLWriter.h"
#include "AsnLParser.h"
#include <EEPROM.h>

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

struct {
    String pinCode;
    int16_t DF;
    int16_t DI;
} app;

int conf0ReadEEPROM() {
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
        return 0;
    } else {
        return -1;
    }
}

int conf0WriteEEPROM() {
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
    return 0;
}

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

bool checkToken(AsnLParser& p, int token) {
    int t = p.nextToken();
    return t == token;
}

void result(AsnLParser& p, AsnLWriter& w, int code) {
    p.close();
    w.init();
    w.structure();
    w.integer(1, code);
    w.endStructure();
    w.msg().writeToSerial();
}

void dumpVars(AsnLParser& p, AsnLWriter& w) {
    if (!checkToken(p, ASNL_END_STRUCT)) {
        result(p, w, 0); return;
    }
    if (!checkToken(p, ASNL_NIL)) {
        result(p, w, 0); return;
    }

    w.init();
    w.structure();
    w.integer(1, 1); // Status OK
    w.structure();
    w.string(app.pinCode);
    w.integer(2, app.DF);
    w.integer(2, app.DI);
    w.endStructure();
    w.endStructure();

    w.msg().writeToSerial();
    digitalWrite(13, LOW);

}

static void setPinCode(AsnLParser& p, AsnLWriter& w) {
    char pinCode[8];
    if (checkToken(p, ASNL_STRING)) {
        if (!p.readString(pinCode, 8)) {
            result(p, w, 0); return; // failed to read string
        }
    } else {
        result(p, w, 0); return; // not a string
    }
    if (!checkToken(p, ASNL_END_STRUCT)) {
        result(p, w, 0); return;
    }
    if (!checkToken(p, ASNL_NIL)) {
        result(p, w, 0); return;
    }
    app.pinCode = String(pinCode);
    result(p, w, 1);
}

static void setDF(AsnLParser& p, AsnLWriter& w) {
    int16_t value;
    if (checkToken(p, ASNL_INT)) {
        if (p.readInt(&value) != 0) {
            result(p, w, 0); return; // failed to read int
        }
    } else {
        result(p, w, 0); return; // not an int
    }
    if (!checkToken(p, ASNL_END_STRUCT)) {
        result(p, w, 0); return;
    }
    if (!checkToken(p, ASNL_NIL)) {
        result(p, w, 0); return;
    }
    app.DF = value;
    result(p, w, 1);
}

static void setDI(AsnLParser& p, AsnLWriter& w) {
    int16_t value;
    if (checkToken(p, ASNL_INT)) {
        if (p.readInt(&value) != 0) {
            result(p, w, 0); return; // failed to read int

        }
    } else {
        result(p, w, 0); return; // not an int
    }
    if (!checkToken(p, ASNL_END_STRUCT)) {
        result(p, w, 0); return;
    }
    if (!checkToken(p, ASNL_NIL)) {
        result(p, w, 0); return;
    }
    app.DI = value;
    result(p, w, 1);
}

static void save(AsnLParser& p, AsnLWriter& w, bool dirty) {
    if (dirty) {
        conf0WriteEEPROM();
    }
    result(p, w, 1);
}

static void conf0Dialog() {
    bool dirty = false;

    AsnLMsg msg = AsnLMsg(CONF0_BUFFER_SIZE);
    AsnLParser p = AsnLParser(msg);
    AsnLWriter w = AsnLWriter(msg);
    while (1) {
        msg.readFromSerial();
        p.init();
        if (!checkToken(p, ASNL_STRUCT)) { // invalid message (not a structure)
            result(p, w, 0);
            continue;
        }
        if (!checkToken(p, ASNL_INT)) { // invalid message (not and integer)
            result(p, w, 0);
            continue;
        }
        int command;
        p.readInt(&command);
        switch (command) {
        case 'd': dumpVars(p, w); break;
        case 'p': setPinCode(p, w); dirty = true; break;
        case 'i': setDI(p, w); dirty = true; break;
        case 'f': setDF(p, w); dirty = true; break;
        case 's': save(p, w, dirty); dirty = false; break;
        case 'x': result(p, w, 1); return; // exit
        default: result(p, w, 0); break; // invalid command
        }
    }
}

void conf0Configure() {
    app.pinCode = "000";
    app.DF = 2000;
    app.DI = 1000;
    conf0ReadEEPROM();
    Serial.write(CONF0_ENQ);
    int res = conf0GetCharTimeout(CONF0_REACTION_TIME);
    if (res == CONF0_ACK) {
        conf0Dialog();
    }
}
