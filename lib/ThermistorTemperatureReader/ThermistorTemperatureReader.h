#pragma once

class ThermistorTemperatureReader {
    private:
        int thermistorPin;
        int thermistorSeriesResistance;
        int thermistorBeta;
        float thermistorNominalCelciusTemperature;
        int thermistorNominalResistance;
        float getThermistorResistance();
    public:
        ThermistorTemperatureReader(
            int thermistorPin, 
            int thermistorSeriesResistance, 
            int thermistorBeta, 
            float thermistorNominalCelciusTemperature, 
            int thermistorNominalResistance);
        float getTemperature();
    
};