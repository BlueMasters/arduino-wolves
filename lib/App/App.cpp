#include <Streaming.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "App.h"

#define SERIAL_INIT_TIMEOUT 1500
#define SERIAL_TIMEOUT 1500

App app;

#define NB_OF_QUESTIONS 3
#define MAX_CARDS 32
#define MAX_GOOD_ANSWERS 32
#define EEPROM_START_CONF       64
#define EEPROM_ADDR_MAGIC      (EEPROM_START_CONF+0)   // 3 bytes
#define EEPROM_ADDR_MUTEX      (EEPROM_START_CONF+4)   // 1 byte
#define EEPROM_ADDR_PIN        (EEPROM_START_CONF+5)   // 4 bytes
#define EEPROM_ADDR_DF         (EEPROM_START_CONF+9)   // 2 bytes
#define EEPROM_ADDR_DI         (EEPROM_START_CONF+11)  // 2 bytes
#define EEPROM_ADDR_NB_CHIPS   (EEPROM_START_CONF+13)  // 1 byte

void App::loadApp() {

    // check MAGIC
    uint32_t magic;
    EEPROM.get(EEPROM_ADDR_MAGIC, magic);
    if(magic != MAGIC){
        // TODO wrong magic, what do we do ?
        Serial << "WRONG MAGIC !!!!" << _HEX(magic) << " " << _HEX(MAGIC) << endl;
        Serial << "Entering learn mode" << endl;
        app.globalMode = globmode_LEARN;
        return;
    }

    // check mutex
    if(EEPROM.read(EEPROM_ADDR_MUTEX) != 0){
        // TODO mutex set
        Serial << "(load app) MUTEX SET !!!!" << endl;
    }
    // set mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 1);

    // read chips
    int offset = EEPROM_ADDR_NB_CHIPS;
    struct wolvesConfigCards &cards = app.config.cards;

    // nbr of chips
    EEPROM.get(offset,cards.len);
    offset += sizeof(cards.len);

    // array of chips
    for(int c = 0; c < cards.len; c++){
        struct rfidUid &card = cards.items[c];
        // card address size
        EEPROM.get(offset, card.size);
        offset += sizeof(card.size);
        // card address size
        for(int i = 0; i < card.size; i++){
            card.data[i] = EEPROM.read(offset++); // data is of type byte
        }
    }

    // read Q&A
    struct wolvesConfigQuestions &questions = app.config.questions;

    // nbr of questions TODO (fixed... but interesting to have it in rom)
    EEPROM.get(offset, questions.len);
    offset += sizeof(questions.len);

    for(int q = 0; q < questions.len; q++){
        struct wolvesConfigQuestion &question = questions.question[q];
        // nbr of answers for this question
        EEPROM.get(offset, question.len);
        offset += sizeof(question.len);
        // array of answers
        for(int i = 0; i < question.len; i++){
            EEPROM.get(offset, question.items[i]);
            offset += sizeof(question.items[i]);
        }
    }

    // release mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 0);
}

void App::saveConfig(struct wolvesConfig &config){

    // get mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 1);

    // write chips
    int offset = EEPROM_ADDR_NB_CHIPS;
    struct wolvesConfigCards &cards = config.cards;
    // nbr of chips
    EEPROM.put(offset, cards.len);
    offset += sizeof(cards.len);
    // array of chips
    for(int c = 0; c < cards.len; c++){
        struct rfidUid &card = cards.items[c];
        // rfid uid size
        EEPROM.put(offset, card.size);
        offset += sizeof(card.size);
        // rfuid uid data
        for(int i = 0; i < card.size; i++){
            EEPROM.write(offset++, card.data[i]); // data is of type byte
        }
    }

    // write Q&A
    struct wolvesConfigQuestions &questions = config.questions;
    Serial << "nb questions " << config.questions.len << endl;
    // nbr of Q
    EEPROM.put(offset, questions.len);
    offset += sizeof(questions.len);
    // answers for each Q
    for(int q = 0; q < questions.len; q++){
        struct wolvesConfigQuestion &question = questions.question[q];
        // nb answers
        EEPROM.put(offset, question.len);
        offset += sizeof(question.len);
        // array of answers
        for(int i = 0; i < question.len; i++){
            EEPROM.put(offset, question.items[i]);
            offset += sizeof(question.items[i]);
        }
    }

    // check the MAGIC, setting it if missing
    uint32_t magic;
    EEPROM.get(EEPROM_ADDR_MAGIC, magic);
    if(magic != MAGIC){
        magic = MAGIC;
        // write magic
        EEPROM.put(EEPROM_ADDR_MAGIC, magic);
        #ifdef APP_DEBUG
        Serial << "(App::writeConfig) magic missing -> wrote magic " << _HEX(magic) << endl;
        #endif
    }

    // release the mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 0);

    // reload config
    loadApp();
}

void App::dumpConfig(){
    Serial << "CONFIG" << endl;
    Serial << "DF: " << app.DF << " DI: " << app.DI << endl;
    Serial << "pincode: " << app.pinCode << endl;
    Serial << "cards: " << app.config.cards.len << endl;
    for(int i = 0; i < app.config.cards.len; i++) {
        Serial << "  " << _HEX(app.config.cards.items[i].data[0]) << endl;
    }
    Serial << endl;

    Serial << "questions: " << app.config.questions.len << endl;
    for(int i = 0; i < app.config.questions.len; i++) {
        wolvesConfigQuestion &q = app.config.questions.question[0];
        Serial << "  " << q.len << " :";
        for(int j = 0; j < q.len; j++){
            Serial << " " << q.items[j];
        }
    }
    Serial << endl;


}

// TODO: keep those methods anyway ?
void App::saveDF(uint16_t DF){
        EEPROM.write(EEPROM_ADDR_MUTEX, 1);
        EEPROM.put(EEPROM_ADDR_DF, DF);
        EEPROM.write(EEPROM_ADDR_MUTEX, 0);
}

void App::saveDI(uint16_t DI){
        EEPROM.write(EEPROM_ADDR_MUTEX, 1);
        EEPROM.put(EEPROM_ADDR_DI, DI);
        EEPROM.write(EEPROM_ADDR_MUTEX, 0);
}
