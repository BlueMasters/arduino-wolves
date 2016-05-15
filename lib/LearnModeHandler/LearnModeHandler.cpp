#include <Arduino.h>
#include "LearnModeHandler.h"
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
        // cancel
        reset();

    }else if(_remote.keypressed() == IR_KEY_OK){
        // save and reset
        app.saveConfig(_config);
        reset();

    }else{
        // still in learn mode, check for new RFID cards
        for (int i = 0; i < N_OF_QUESTIONS; i++){
            if(_sensors[i].changed()){
                int idx = addCard(_sensors[i].cardId());
                addAnswer(i, idx);
            }
        }
    }
}


void LearnModeHandler::reset(){
    _config.cards.len = 0;
    for(int i = 0; i < N_OF_QUESTIONS; i++){
        _config.questions.question[i].len = 0;
    }
}


int LearnModeHandler::addCard(struct rfidUid card){
    int &len = _config.cards.len;

    for(int i = 0; i < len; i++){
        if(_config.cards.items[i].equals(&card)){
            // card already listed, nothing to do
            return i;
        }
    }
    // new card, add it to the list:
    _config.cards.items[len].set(card.size, card.data);
    len++;
    return (len - 1);

}


void LearnModeHandler::addAnswer(int q, int idx){
    int &len = _config.questions.question[q].len;
    // TODO is this check necessary ?
    for(int i = 0; i < len; i++){
        if(_config.questions.question[q].items[i] == idx){
            // answer already registered !
            return;
        }
    }
    _config.questions.question[q].items[len] = idx;
    len++;
}
