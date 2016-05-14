#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "WolvesTypes.h"
#include "RGBLED.h"

#define APP_DEBUG

#define DI_GRANULARITY 100 // 1 unit means +100 ms
#define DF_GRANULARITY 25  // 1 unit means 25 ms
#define MAGIC "B1M"

enum globalMode {globmode_NORMAL, globmode_LEARN };
enum configMode {confmode_DI, confmode_DF, confmode_None};

class App {
public:
    uint8_t pinCode[4];
    enum globalMode globalMode;
    enum configMode configMode;
    bool emergency;
    struct wolvesConfig config;
    uint8_t DI; // TODO use a more descriptive name or comment
    uint8_t DF; // TODO use a more descriptive name or comment
    RGBLED statusLed;
    void readConfig();
};

extern App app;

#endif
