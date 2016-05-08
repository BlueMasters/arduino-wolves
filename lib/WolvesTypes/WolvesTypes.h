#ifndef WOLVES_TYPES__H
#define WOLVES_TYPES__H

#include <Arduino.h>
#define WOLVES_MAX_CARDS 32
#define WOLVES_MAX_QUESTIONS 3
#define WOLVES_MAX_GOOD_ANSWERS 32

struct rfidUid {
    byte size;
    byte data[10];

    void set(byte ns, byte* nd){
      size = ns;
      for(int i = 0; i < size; i++){
        data[i] = nd[i];
      }
    }

    bool equals(struct rfidUid * other){
      if (size != other->size) return false;
      for (int i = 0; i < size; i++) {
        if (data[i] != other->data[i]) return false;
      }
      return true;
    }

    void dump(){
      // TODO
    }
};

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

#endif
