#include "RelayToggler.h"
#include "ThermistorTemperatureReader.h"
#include <Arduino.h>

ThermistorTemperatureReader thermistorTemperatureReader(A2, 2700, 3950, 25, 1000);

void setup() {
  Serial.begin(9600);
  RelayToggler relayToggler(7, NORMALLY_OPEN);
  relayToggler.turnOn();
}

void loop() {
  float temperature = thermistorTemperatureReader.getTemperature();
  Serial.print("Temperature measured to be :");
  Serial.println(temperature);
}