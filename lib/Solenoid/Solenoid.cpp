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

#include <Arduino.h>
#include "Solenoid.h"
#include "App.h"

// Give time for the disk to fall out of the RFID sensor field
#define SOLENOID_WAITING_TIME 1000

void Solenoid::begin() {
  _state = SOLENOID_IDLE;
}

void Solenoid::fire(long t) {
  _state = SOLENOID_FIRED;
  _timestamp = t;
  digitalWrite(_impulsePin, HIGH);
}

void Solenoid::tick() {
  long now = millis();

  switch (_state) {
  case SOLENOID_IDLE:
    if (app.emergency) {
      fire(now);
      _led.green();
    } else if (_sensor.rfidSensorStatus() == VALID_CARD) {
      fire(now);
      _led.green();
    } else if (_sensor.rfidSensorStatus() == INVALID_CARD) {
      fire(now);
      _led.red();
    } else {
      digitalWrite(_impulsePin, LOW);
      _led.off();
    }
    break;

  case SOLENOID_FIRED:
    if (now - _timestamp > app.DI) {
      _timestamp = now;
      digitalWrite(_impulsePin, LOW);
      _led.off();
      _state = SOLENOID_WAITING;
    } break;

  case SOLENOID_WAITING:
    if (now - _timestamp > SOLENOID_WAITING_TIME) {
      _state = SOLENOID_IDLE;
    } break;

  default: {
    _state = SOLENOID_IDLE;
    break;
  }
  }
};
