#include "RelayToggler.h"
#include "InterruptableThermistorTemperatureReader.h"
#include <Arduino.h>

InterruptableThermistorTemperatureReader interruptableThermistorTemperatureReader(A2, 2700, 3950, 25, 1000, 1);

void setup() {
  Serial.begin(9600);
  RelayToggler relayToggler(7, NORMALLY_OPEN);
  relayToggler.turnOn();
}


void loop() {
  float temperature = interruptableThermistorTemperatureReader.getTemperature();
  Serial.print("Temperature measured to be :");
  Serial.println(temperature);
  Serial.print("Currently in state: ");
  Serial.println(interruptableThermistorTemperatureReader.getState());
  delay(1000);
}