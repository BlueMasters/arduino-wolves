#ifndef RFIDUID__H
#define RFIDUID__H

#include <Arduino.h>

struct rfidUid {
    byte size;
    byte data[10];

    void set(byte ns, byte* nd){
        size = ns;
        for(int i = 0; i < size; i++) {
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

#endif
