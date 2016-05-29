/****************************************************************************
 * Copyright 2016 BlueMasters
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
 
#ifndef APP__H
#define APP__H

#include <Arduino.h>
#include "RFIDUid.h"
#include "StatusLed.h"

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

class App {
public:
    App() : globalMode(globmode_NORMAL), emergency(false) {};
    String pinCode;
    enum globalMode globalMode;
    bool emergency;
    struct wolvesConfig config;
    uint16_t DI; // Duration of Impuls (in ms)
    uint16_t DF; // Duration of Feedback (in ms)
    StatusLed statusLed;
    void dump();
    void dumpConfig(struct wolvesConfig& config);

    void loadApp(); // Load pinCode, config, DI and DF. // TODO Take granularity into account.
    void saveDI(uint16_t DI);
    void saveDF(uint16_t DF);
    void saveConfig(struct wolvesConfig& config);

};

extern App app;


#endif
