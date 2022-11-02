#include "RelayToggler.h"
#include <Arduino.h>

RelayToggler::RelayToggler(int relayPinVal, RelayType relayTypeVal): relayPin{relayPinVal}, relayType{relayTypeVal}{
    pinMode(relayPin, OUTPUT);
};

void RelayToggler::turnOn(){
    if (relayType == NORMALLY_OPEN){
        digitalWrite(relayPin, LOW);
    } else {
      digitalWrite(relayPin, HIGH);  
    }
};

void RelayToggler::turnOff(){
    if (relayType == NORMALLY_OPEN){
        digitalWrite(relayPin, HIGH);
    } else {
      digitalWrite(relayPin, LOW);  
    }
};