#include <Arduino.h>
#include "LED.h"

void LED::begin() {
  pinMode(_red, OUTPUT);
  pinMode(_green, OUTPUT);
}

void LED::off() {
  digitalWrite(_red, LOW);
  digitalWrite(_green, LOW);
}

void LED::green() {
  digitalWrite(_red, LOW);
  digitalWrite(_green, HIGH);
}

void LED::red() {
  digitalWrite(_green, LOW);
  digitalWrite(_red, HIGH);
}
