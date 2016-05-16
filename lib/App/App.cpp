#include <Streaming.h>
#include <Arduino.h>
#include <EEPROM.h>
#include "App.h"

#define SERIAL_INIT_TIMEOUT 1500
#define SERIAL_TIMEOUT 1500

App app;

#define EEPROM_ADDR_MAGIC      0  // 3 bytes
#define EEPROM_ADDR_MUTEX      3  // 1 byte
#define EEPROM_ADDR_PIN        4  // 4 bytes
#define EEPROM_ADDR_DF         8  // 2 bytes
#define EEPROM_ADDR_DI        10  // 2 bytes
#define EEPROM_ADDR_NB_CHIPS  12  // 1 byte

void App::loadApp() {

    uint32_t state;
    EEPROM.get(0, state);

    if((state >> 8) != MAGIC){
        // TODO wrong magic
        app.statusLed.setColor(COLOR_RED);
        while(1);

    }else if((state & 0xFF) != 0){
        // TODO mutex set
    }

    // read pin
    for(int i = 0; i < 4; i++){
        EEPROM.get(EEPROM_ADDR_PIN + i, app.pinCode[i]);
    }

    // read df and di
    EEPROM.get(EEPROM_ADDR_DF, app.DF);
    EEPROM.get(EEPROM_ADDR_DI, app.DI);

    // read chips
    int offset = EEPROM_ADDR_NB_CHIPS;

    struct wolvesConfigCards &cards = app.config.cards;
    EEPROM.get(offset++,cards.len);

    for(int c = 0; c < cards.len; c++){
        // read one rfuid address (size + data[size])
        struct rfidUid &card = cards.items[c];
        EEPROM.get(offset++, card.size);
        for(int i = 0; i < card.size; i++ ){
            EEPROM.get(offset++, card.data[i]);
        }
    }

    // read Q&A
    struct wolvesConfigQuestions &questions = app.config.questions;
    questions.len = NB_OF_QUESTIONS;
    for(int q = 0; q < questions.len; q++){
        struct wolvesConfigQuestion &question = questions.question[q];
        EEPROM.get(offset++, question.len); // nb questions
        for(int i = 0; i < question.len; i++ ){
            EEPROM.get(offset++, question.items[i]);
        }
    }
}

void App::saveConfig(struct wolvesConfig &config){

    EEPROM.write(EEPROM_ADDR_MUTEX, 1);

    // write chips
    int offset = EEPROM_ADDR_NB_CHIPS;

    struct wolvesConfigCards &cards = config.cards;
    EEPROM.put(offset++,cards.len);

    for(int c = 0; c < cards.len; c++){
        // read one rfuid address (size + data[size])
        struct rfidUid &card = cards.items[c];
        EEPROM.put(offset++, card.size);
        for(int i = 0; i < card.size; i++ ){
            EEPROM.put(offset++, card.data[i]);
        }
    }

    // write Q&A
    struct wolvesConfigQuestions &questions = config.questions;
    for(int q = 0; q < questions.len; q++){
        struct wolvesConfigQuestion &question = questions.question[q];
        EEPROM.put(offset++, question.len); // nb questions
        for(int i = 0; i < question.len; i++ ){
            EEPROM.put(offset++, question.items[i]);
        }
    }

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
