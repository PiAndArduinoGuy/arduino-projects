#include "RelayToggler.h"
#include <Arduino.h>

RelayToggler::RelayToggler(int relayPinVal, RelayType relayTypeVal): relayPin{relayPinVal}, relayType{relayTypeVal}{
    pinMode(relayPin, OUTPUT);
};

void RelayToggler::turnOn(){
    Serial.println("Relay will be toggled to on.");
    if (relayType == NORMALLY_OPEN){
        digitalWrite(relayPin, LOW);
    } else {
      digitalWrite(relayPin, HIGH);  
    }
};

void RelayToggler::turnOff(){
    Serial.println("Relay will be toggled to off.");
    if (relayType == NORMALLY_OPEN){
        digitalWrite(relayPin, HIGH);
    } else {
      digitalWrite(relayPin, LOW);  
    }
};