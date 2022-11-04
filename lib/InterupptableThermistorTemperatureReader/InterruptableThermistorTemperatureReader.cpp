#include "InterruptableThermistorTemperatureReader.h"
#include <Arduino.h>

State InterruptableThermistorTemperatureReader::state = UNINTERRUPTED;



InterruptableThermistorTemperatureReader::InterruptableThermistorTemperatureReader(
    int thermistorPinVal, 
    int thermistorSeriesResistanceVal, 
    int thermistorBetaVal, 
    float thermistorNominalCelciusTemperatureVal, 
    int thermistorNominalResistanceVal,
    int interruptPinVal) : 
        ThermistorTemperatureReader{
            thermistorPinVal, 
            thermistorSeriesResistanceVal, 
            thermistorBetaVal, 
            thermistorNominalCelciusTemperatureVal, 
            thermistorNominalResistanceVal}, 
        interrupPin{interruptPinVal} {
            attachInterrupt(interrupPin, InterruptableThermistorTemperatureReader::performInterruptAction, CHANGE);
        
            
};

State InterruptableThermistorTemperatureReader::getState(){
    return state;
}
