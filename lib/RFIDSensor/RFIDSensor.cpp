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
#include "RFIDSensor.h"
#include "App.h"
#include <SPI.h>
#include <MFRC522.h>

#ifdef APP_DEBUG
#include <Streaming.h>
#endif

void RFIDSensor::begin() {
    _mfrc522.PCD_Init();
}

void RFIDSensor::begin(int csPin, int resetPin) {
    _mfrc522.PCD_Init(csPin, resetPin);
}

bool RFIDSensor::selfCheck(){
  #ifdef APP_DEBUG
    _mfrc522.PCD_DumpVersionToSerial();
  #endif
    bool result = _mfrc522.PCD_PerformSelfTest();
    begin(); // init once again, which is required after a self-test
    return result;
}

void RFIDSensor::tick() {
    // always reinitialise the status
    _answer = UNDEFINED;
    if(_mfrc522.PICC_IsNewCardPresent() && _mfrc522.PICC_ReadCardSerial()) {
        // card detected
        _cardId.set(_mfrc522.uid.size, _mfrc522.uid.uidByte);
        validateNewCard();

        #ifdef APP_DEBUG
        Serial << "RFID " << _id << ": detected " << (_answer == TRUE ? "valid " : "invalid ");
        _cardId.dump();
        #endif

        // Halt PICC
        _mfrc522.PICC_HaltA();
        // Stop encryption on PCD
        _mfrc522.PCD_StopCrypto1();
    }
}

void RFIDSensor::validateNewCard(){
    struct rfidUid * cards = app.config.cards.items;
    wolvesConfigQuestion &question = app.config.questions.question[_id];

    for(int ans = 0; ans < question.len; ans++) {
        int idx = question.items[ans];
        if(_cardId.equals(&cards[idx])) {
            _answer = TRUE;
            return;
        }
    }

    _answer = FALSE;
}

enum triState RFIDSensor::currentAnswer() {
    return _answer;
}

struct rfidUid RFIDSensor::cardId(){
    return _cardId;
}


// void RFIDSensor::dumpUID(struct rfidUid uid){
//   byte i;
//   for (i = 0; i < uid.size-1; i++) {
//       Serial.print(uid.data[i] < 0x10 ? "0" : "");
//       Serial.print(uid.data[i], HEX);
//       Serial.print(":");
//   }
//   if ((i = uid.size-1) > 0) {
//       Serial.print(uid.data[i] < 0x10 ? "0" : "");
//       Serial.print(uid.data[i], HEX);
//   }
// }
