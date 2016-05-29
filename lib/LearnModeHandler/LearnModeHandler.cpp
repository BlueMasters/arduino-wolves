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
#include "LearnModeHandler.h"
#include "RFIDUid.h"
#include "RFIDSensor.h"
#include <Streaming.h>
#include "App.h"

void LearnModeHandler::begin(){
    reset();
}

void LearnModeHandler::tick(){

    // we are only concerned with global modes
    if(app.globalMode != globmode_LEARN) return;

    // NOTE: this supposes that the remote will
    // reset the globalMode upon OK/CANCEL after one tick.
    // if not, we will have inconsistancies.
    // the same goes for rfidSensorStatus

    if(_remote.keypressed() == IR_KEY_CANCEL){
        #ifdef APP_DEBUG
        Serial << "LEARN MODE: cancel " << endl;
        #endif
        // cancel
        reset();

    }else if(_remote.keypressed() == IR_KEY_OK){
        #ifdef APP_DEBUG
        Serial << "LEARN MODE: saving config" << endl;
        #endif
        // save and reset
        app.saveConfig(_config);
        reset();

    }else{
        // still in learn mode, check for new RFID cards
        for (int i = 0; i < NB_OF_QUESTIONS; i++){
            if(_sensors[i].rfidSensorStatus() != NO_CARD){
                int idx = addCard(_sensors[i].cardId());
                addAnswer(i, idx);
            }
        }
    }
}


void LearnModeHandler::reset(){
    _config.cards.len = 0;
    _config.questions.len = NB_OF_QUESTIONS;

    for(int i = 0; i < NB_OF_QUESTIONS; i++){
        _config.questions.question[i].len = 0;
    }
}


int LearnModeHandler::addCard(struct rfidUid card){
    wolvesConfigCards &cards = _config.cards;

    for(int i = 0; i < cards.len; i++){
        if(cards.items[i].equals(&card)){
            // card already listed, nothing to do
            return i;
        }
    }
    // new card, add it to the list:
    cards.items[cards.len].set(card.size, card.data);
    #ifdef APP_DEBUG
    Serial << "LEARN MODE:  added new ";
    cards.items[cards.len].dump();
    #endif
    return (cards.len++);
}


void LearnModeHandler::addAnswer(int q, int idx){

    wolvesConfigQuestion &question = _config.questions.question[q];
    // TODO is this check necessary ?
    for(int i = 0; i < question.len; i++){
        if(question.items[i] == idx){
            // answer already registered !
            return;
        }
    }
    question.items[question.len++] = idx;
    #ifdef APP_DEBUG
    Serial << "LEARN MODE: add answer idx=" << idx << " for question=" << q << endl;
    #endif
}
