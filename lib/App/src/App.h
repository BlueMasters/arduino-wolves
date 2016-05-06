#ifndef APP__H
#define APP__H

#include <Arduino.h>

enum globalMode {globmode_NORMAL, globmode_LEARN };
enum configMode {confmode_DI, confmode_DF, confmode_None};

struct App {
  enum globalMode globalMode;
  enum configMode configMode;
  bool emergency;
};

#endif
