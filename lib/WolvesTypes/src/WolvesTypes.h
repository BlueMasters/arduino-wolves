#ifndef WOLVES_TYPES__H
#define WOLVES_TYPES__H

#include <Arduino.h>

struct rfid_uid {
    byte size;
    byte data[10];
};


#endif
