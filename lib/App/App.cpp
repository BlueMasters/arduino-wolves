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
#define EEPROM_ADDR_MAGIC      0  // 3 bytes
#define EEPROM_ADDR_MUTEX      4  // 1 byte
#define EEPROM_ADDR_PIN        5  // 4 bytes
#define EEPROM_ADDR_DF         9  // 2 bytes
#define EEPROM_ADDR_DI        11  // 2 bytes
#define EEPROM_ADDR_NB_CHIPS  13  // 1 byte

void App::loadApp() {

    // check MAGIC
    uint32_t magic;
    EEPROM.get(0, magic);
    if(magic != MAGIC){
        // TODO wrong magic, what do we do ?
        Serial << "WRONG MAGIC !!!!" << _HEX(magic) << " " << _HEX(MAGIC) << endl;
        while(1);
    }

    // check mutex
    if(EEPROM.read(EEPROM_ADDR_MUTEX) != 0){
        // TODO mutex set
        Serial << "(load app) MUTEX SET !!!!" << endl;
    }
    // set mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 1);

    // read pin
    int offset = EEPROM_ADDR_PIN;
    for(int i = 0; i < 4; i++){
        EEPROM.get(offset++, app.pinCode[i]);
    }

    // read DF and DI
    EEPROM.get(EEPROM_ADDR_DF, app.DF);
    EEPROM.get(EEPROM_ADDR_DI, app.DI);

    // read chips
    offset = EEPROM_ADDR_NB_CHIPS;
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

    // nbr of questions
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

    // check mutex
    if(EEPROM.read(EEPROM_ADDR_MUTEX) != 0){
        // TODO
        Serial << "(save config) MUTEX SET!!!" << endl;
        while(1);
    }

    // get mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 1);

    // write chips
    int offset = EEPROM_ADDR_NB_CHIPS;
    struct wolvesConfigCards &cards = app.config.cards;
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
    EEPROM.get(0, magic);
    if(magic != MAGIC){
        magic = MAGIC;
        // write magic
        EEPROM.put(0, magic);
        #ifdef APP_DEBUG
        Serial << "(App::writeConfig) magic missing -> wrote magic " << _HEX(magic) << endl;
        #endif
    }

    // release the mutex
    EEPROM.write(EEPROM_ADDR_MUTEX, 0);

    uint32_t state;
    EEPROM.get(0, state);

    if((state >> 8) != MAGIC){
        state = MAGIC << 8;
        // write magic
        EEPROM.put(0, state);

    }

    // reload config
    loadApp();
}


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
