#include "ThermistorTemperatureReader.h"
#include <Arduino.h>

ThermistorTemperatureReader::ThermistorTemperatureReader(
    int thermistorPinVal, 
    int thermistorSeriesResistanceVal, 
    int thermistorBetaVal, 
    float thermistorNominalCelciusTemperatureVal, 
    int thermistorNominalResistanceVal) : 
        thermistorPin{thermistorPinVal}, 
        thermistorSeriesResistance{thermistorSeriesResistanceVal},
        thermistorBeta{thermistorBetaVal},
        thermistorNominalCelciusTemperature{thermistorNominalCelciusTemperatureVal},
        thermistorNominalResistance{thermistorNominalResistanceVal}{};

float ThermistorTemperatureReader::getThermistorResistance(){
  float thermistorAnalogVoltage = analogRead(thermistorPin);
 
  float resistance = -1;
  resistance = (1023.0 / thermistorAnalogVoltage)  - 1.0; 
  resistance = thermistorSeriesResistance / resistance;  
  return resistance; 
}

float ThermistorTemperatureReader::getTemperature(){
    float resistance = getThermistorResistance();
    float temperature = 0.0;
    temperature = resistance / thermistorNominalResistance;               // (R/Ro)
    temperature = log(temperature);                                       // ln(R/Ro)
    temperature /= thermistorBeta;                                        // 1/B * ln(R/Ro)
    temperature += 1.0 / (thermistorNominalCelciusTemperature + 273.15);  // + (1/To)
    temperature = 1.0 / temperature;                                      // Invert
    temperature -= 273.15;                                                // convert absolute temp to C
    Serial.print("Temperature measured to be ");
    Serial.println(temperature);
    return temperature;
}

