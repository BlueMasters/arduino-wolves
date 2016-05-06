#ifndef LED__H
#define LED__H

#include <Arduino.h>

class LED {

public:
  LED(int red, int green) : _red(red), _green(green) {};
  void begin();
  void off();
  void red();
  void green();

private:
  int _red;
  int _green;
};


#endif
