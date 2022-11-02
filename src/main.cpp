#include "RelayToggler.h"
#include <Arduino.h>

void setup() {
  RelayToggler relayToggler(7, NORMALLY_OPEN);
  relayToggler.turnOn();
}

void loop() {
  // put your main code here, to run repeatedly:
}