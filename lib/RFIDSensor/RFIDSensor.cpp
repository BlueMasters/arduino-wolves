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
#include <MFRC522.h>

#ifdef DEBUG
#include "Streaming.h"
#endif

void RFIDSensor::begin() {
    _mfrc522.PCD_Init();
}

bool RFIDSensor::selfTest(){
  #ifdef DEBUG
  _mfrc522.PCD_DumpVersionToSerial();
  #endif
  bool result = _mfrc522.PCD_PerformSelfTest();
  begin(); // init once again, which is required after a self-test
  return result;
}

void RFIDSensor::tick() {
  // always reinitialise this
  _changed = false;

  if(_mfrc522.PICC_IsNewCardPresent() && _mfrc522.PICC_ReadCardSerial()){
    // card detected
    struct rfidUid newCardId;
    newCardId.set(_mfrc522.uid.size, _mfrc522.uid.uidByte);

    if(newCardId.equals(&_cardId)){
      // no changes, do nothing.
      return;
    }

    // this is a new card -> update the status
    _cardId.set(newCardId.size, newCardId.data);
    _changed = true;
    validateNewCard();

    // in debug mode, print the detected card
    #ifdef DEBUG
    Serial << "Card #" << _id << " = ";
    _cardId.dump();
    Serial << (_status == VALID_CARD ? " valid" : " invalid") << '\n';
    #endif
  }
}

void RFIDSensor::validateNewCard(){
  struct rfidUid * cards = app.config.cards.items;

  for(int ans = 0; app.config.questions.question[_id].len; ans++){
    int idx = app.config.questions.question[_id].items[ans];
    if(_cardId.equals(&cards[idx])){
      _status = VALID_CARD;
      return;
    }
  }

  _status = INVALID_CARD;
}

enum rfidSensorStatus RFIDSensor::rfidSensorStatus() {
    return _status;
}

bool RFIDSensor::changed() {
  return _changed;
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
