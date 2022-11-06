#include "RelayToggler.h"
#include "InterruptableThermistorTemperatureReader.h"

#pragma once 
class AutoTemperatureToggler { 
    private:
        RelayToggler relayToggler;
        InterruptableThermistorTemperatureReader interruptableThermistorTemperatureReader;  
        int numberOfTemperatureReadingsCaptured;   
        float totalTemperature;
        int temperatureSetPin;
        int overrideControlPin;

        static int NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE;
        static int AUTO_MODE_CHECK_DELAY;
        static int BUTTON_PRESSES;

        float getTurnOnTemperature();
        void performAutoRelayToggling();
        void performManualRelayToggling();
        State getState();
    public:
        AutoTemperatureToggler(
            int relayPin, 
            RelayType relayType, 
            int thermistorPin, 
            int thermistorSeriesResistance, 
            int thermistorBeta, 
            float thermistorNominalCelciusTemperature, 
            int thermistorNominalResistance,
            int interruptPin,
            int temperatureSetPin,
            int overrideControlPin);
        void performManualAndAutonomousRelayToggling();
    
};