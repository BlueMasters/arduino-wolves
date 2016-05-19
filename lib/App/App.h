#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "RFIDUid.h"
#include "RGBLED.h"

#define APP_DEBUG

#define DI_GRANULARITY 100 // 1 unit means +100 ms
#define DF_GRANULARITY 25  // 1 unit means 25 ms
#define MAGIC 0x31424d41   //"B1MA"

#define NB_OF_QUESTIONS 3
#define MAX_CARDS 32
#define MAX_GOOD_ANSWERS 32


struct wolvesConfigCards {
    byte len;
    struct rfidUid items[MAX_CARDS];
};

struct wolvesConfigQuestion {
    byte len;
    byte items[MAX_GOOD_ANSWERS];
};

struct wolvesConfigQuestions {
    byte len;
    struct wolvesConfigQuestion question[NB_OF_QUESTIONS];
};

struct wolvesConfig {
    struct wolvesConfigCards cards;
    struct wolvesConfigQuestions questions;
};

enum globalMode {globmode_NORMAL, globmode_LEARN };
enum configMode {confmode_DI, confmode_DF, confmode_None};

class App {
public:
    App() : globalMode(globmode_NORMAL), configMode(confmode_None), emergency(false) {};
    uint8_t pinCode[4];
    enum globalMode globalMode;
    enum configMode configMode;
    bool emergency;
    struct wolvesConfig config;
    uint16_t DI; // Duration of Impuls (in ms)
    uint16_t DF; // Duration of Feedback (in ms)
    RGBLED statusLed;

    void loadApp(); // Load pinCode, config, DI and DF. // TODO Take granularity into account.
    void saveDI(uint16_t DI);
    void saveDF(uint16_t DF);
    void saveConfig(struct wolvesConfig& config);

};

extern App app;


#endif
