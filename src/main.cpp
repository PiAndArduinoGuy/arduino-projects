
#include "AutoTemperatureToggler.h"
#include <Arduino.h>

AutoTemperatureToggler autoTemperatureToggler(7, NORMALLY_OPEN, A2, 2700, 3950, 25, 1000, 1, A1, 2);

void setup() {
  Serial.begin(9600);
}


void loop() {
  autoTemperatureToggler.performManualAndAutonomousRelayToggling();
}