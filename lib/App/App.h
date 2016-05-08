#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "WolvesTypes.h"

#define DEBUG true

enum globalMode {globmode_NORMAL, globmode_LEARN };
enum configMode {confmode_DI, confmode_DF, confmode_None};

class App {
public:
    int pinCode[4+1];
    enum globalMode globalMode;
    enum configMode configMode;
    bool emergency;
    struct wolvesConfig config;
    int DI;
    int DF;
};

extern App app;

#endif
