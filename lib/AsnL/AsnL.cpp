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

#include "AsnL.h"
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

AsnL::AsnL(int bufferSize) {
    msg = (unsigned char*) malloc(bufferSize);
    msgLen = 0;
    fix = 0;
}

AsnL::~AsnL() {
    if (msg != 0) {
        free(msg);
    }
}

//----------------------------------------------------------------------------
// Writer
//----------------------------------------------------------------------------

void AsnL::InitWriter() {
    msgLen = 0;
    fix = 0;
}

void AsnL::Int(int len, int value) {
    msg[msgLen] = ASNL_INT;
    msg[msgLen+1] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        msg[msgLen + 1 + len - i] = (unsigned char)(value % 256);
        value = value / 256;
    }
    msgLen += len + 2;
}

void AsnL::Uint(int len, unsigned int value) {
    msg[msgLen] = ASNL_UINT;
    msg[msgLen+1] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        msg[msgLen + 1 + len - i] = (unsigned char)(value % 256);
        value = value / 256;
    }
    msgLen += len + 2;
}

void AsnL::String(char* txt) {
    msg[msgLen++] = ASNL_STRING;
    int len = strlen(txt);
    msg[msgLen++] = (unsigned char) len;
    for (int i = 0; i < len; i++) {
        msg[msgLen++] = (unsigned char)(txt[i]);
    }
}

void AsnL::Struct() {
    msg[msgLen++] = ASNL_STRUCT;
    // Here we use the "length" field to chain the struct tokens.
    // This will be "fixed" with the EndStruct function.
    msg[msgLen] = (unsigned char) fix;
    fix = msgLen++;
}

void AsnL::EndStruct() {
    if (fix > 0) {
        // Fix the "length" field and pop the address of the previous
        // "struct".
        int i = (int)msg[fix];
        msg[fix] = (unsigned int)(msgLen - fix - 1);
        fix = i;
    }
}

void AsnL::CloseWriter() {
    while (fix > 0) {
        int i = (int)msg[fix];
        msg[fix] = (unsigned int)(msgLen - fix - 1);
        fix = i;
    }
}

//----------------------------------------------------------------------------
// Parser
//----------------------------------------------------------------------------

void AsnL::InitParser() {
    pos = 0;
    fix = 0;
}

int AsnL::NextToken() {
    if (fix > 0 && pos > (fix + msg[fix])) {
        // We reached the end of a structure. So we "pop" the address
        // of the previous "struct".
        int i = (int)msg[fix-1];
        msg[fix-1] = ASNL_STRUCT;
        fix = i;
        return ASNL_END_STRUCT;
    }
    if (pos >= msgLen) {
        return ASNL_NIL;
    }

    int type = (int)msg[pos++];
    int len = (int)msg[pos++];
    valPtr = pos;
    switch(type) {
    case ASNL_INT:
        pos += len;
        break;
    case ASNL_STRING:
        pos += len;
        break;
    case ASNL_STRUCT:
        // We use the "type" byte (ASNL_STRUCT) to chain the "structs"
        msg[pos-2] = (unsigned char)fix;
        fix = pos-1;
        break;
    }
    return type;
}

void AsnL::ReadInt(int* value) {
    int len = msg[valPtr-1];
    *value = 0;
    for (int i = 0; i < len; i++) {
        *value = *value * 256 + msg[valPtr+i];
    }
}

void AsnL::ReadString(char* buffer, int len) {
    if (len > msg[valPtr-1]+1) {
        memcpy((void*)buffer, (void*)&msg[valPtr], msg[valPtr-1]);
        buffer[msg[valPtr-1]] = 0;
    }
}

void AsnL::AbortParser() {
    while (fix > 0) {
        int i = (int)msg[fix-1];
        msg[fix-1] = ASNL_STRUCT;
        fix = i;
    }
}

//----------------------------------------------------------------------------
// Util
//----------------------------------------------------------------------------

int AsnL::FixOk() {
    return fix == 0;
}

void AsnL::Dump() {
    for (int i = 0; i < msgLen; i++) {
        int x = msg[i];
        int xh = (x / 16) % 16;
        int xl = x % 16;
        Serial.print("0x");
        Serial.print((char)(xh < 10 ? ('0' + xh) : ('a' - 10 + xh)));
        Serial.print((char)(xl < 10 ? ('0' + xl) : ('a' - 10 + xl)));
        Serial.print(' ');
        if (i % 8 == 7) {
            Serial.println();
        }
    }
    Serial.println();
}
