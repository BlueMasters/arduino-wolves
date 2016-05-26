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

    if(_remote.keypressed() == IR_KEY_CANCEL){
        #ifdef DEBUG
        Serial << "aborting learn mode (cancel)" << endl;
        #endif
        // cancel
        reset();

    }else if(_remote.keypressed() == IR_KEY_OK){
        #ifdef DEBUG
        Serial << "saving config" << endl;
        #endif
        // save and reset
        app.saveConfig(_config);
        reset();

    }else{
        // still in learn mode, check for new RFID cards
        for (int i = 0; i < NB_OF_QUESTIONS; i++){
            if(_sensors[i].changed()){
                int idx = addCard(_sensors[i].cardId());
                addAnswer(i, idx);
            }
        }
    }
}


void LearnModeHandler::reset(){
    _config.cards.len = 0;
    for(int i = 0; i < NB_OF_QUESTIONS; i++){
        _config.questions.question[i].len = 0;
    }
    #ifdef DEBUG
    Serial << "reset temp config" << endl;
    #endif
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
    cards.items[cards.len++].set(card.size, card.data);
    #ifdef DEBUG
    Serial << "added card " << _HEX(card.data[0]) << endl;
    #endif
    return (cards.len - 1);
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
    #ifdef DEBUG
    Serial << "added answer for question " << q << "(" << idx << ")" << endl;
    #endif
}
