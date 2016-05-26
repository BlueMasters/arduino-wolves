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

#ifndef ASN_L__H
#define ASN_L__H

#include <Arduino.h>

// ASN Types
#define  ASNL_INT           'I'
#define  ASNL_UINT          'U'
#define  ASNL_STRING        '"'
#define  ASNL_STRUCT        '{'

// Tokens
#define  ASNL_END_STRUCT    '}'
#define  ASNL_NIL           '0'

class AsnL {

public:
    AsnL(int bufferSize);
    ~AsnL();

    void InitWriter();
    void Int(int len, int value);
    void Uint(int len, unsigned int value);
    void String(char* txt);
    void Struct();
    void EndStruct();
    void CloseWriter();

    void InitParser();
    int NextToken();
    void ReadInt(int* value);
    void ReadString(char* buffer, int len);
    void AbortParser();

    int FixOk();
    void Dump();

private:
    unsigned char* msg;
    int msgLen;
    int fix;
    int pos;
    int valPtr;

};

#endif
