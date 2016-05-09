#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "WolvesTypes.h"
#include "RGBLED.h"

#define DEBUG true
#define DI_GRANULARITY 100 // 1 unit means +100 ms
#define DF_GRANULARITY 25  // 1 unit means 25 ms

enum globalMode {globmode_NORMAL, globmode_LEARN };
enum configMode {confmode_DI, confmode_DF, confmode_None};

class App {
public:
    uint8_t pinCode[4+1];
    enum globalMode globalMode;
    enum configMode configMode;
    bool emergency;
    struct wolvesConfig config;
    uint8_t DI;
    uint8_t DF;
    RGBLED* statusLed;
};

extern App app;

#endif
