#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "RFIDUid.h"
#include "RGBLED.h"

#define APP_DEBUG

#define DI_GRANULARITY 100 // 1 unit means +100 ms
#define DF_GRANULARITY 25  // 1 unit means 25 ms
#define MAGIC "B1M"

// --------------------------------

#define WOLVES_MAX_CARDS 32
#define WOLVES_MAX_QUESTIONS 3
#define WOLVES_MAX_GOOD_ANSWERS 32

struct wolvesConfig {
    struct {
        int len;
        struct rfidUid items[WOLVES_MAX_CARDS];
    } cards;
    struct {
        int len;
        struct {
            int len;
            int items[WOLVES_MAX_GOOD_ANSWERS];
        } question[WOLVES_MAX_QUESTIONS];
    } questions;
};

// --------------------------------

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
