#include "ThermistorTemperatureReader.h"
#include <Arduino.h>

#pragma once

enum State {INTERRUPTED, UNINTERRUPTED};

class InterruptableThermistorTemperatureReader : public ThermistorTemperatureReader {
    public:
        InterruptableThermistorTemperatureReader(
            int thermistorPin, 
            int thermistorSeriesResistance, 
            int thermistorBeta, 
            float thermistorNominalCelciusTemperature, 
            int thermistorNominalResistance,
            int interruptPin
        );
        static State getState();
    private:
        int interrupPin;
        static State state;
        // needs to be static - used by the attachInterrupt Arduino method that requires it to be statically defined
        static void performInterruptAction(){
            if (InterruptableThermistorTemperatureReader::state == INTERRUPTED){
                InterruptableThermistorTemperatureReader::state = UNINTERRUPTED;
            } else {
                InterruptableThermistorTemperatureReader::state = INTERRUPTED;
            }
            
        };
};