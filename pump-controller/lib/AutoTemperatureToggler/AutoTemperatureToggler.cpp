#include "AutoTemperatureToggler.h"

// TODO: These need to be initalized in constructor - no need to have them statically defined.
// TODO: Single responsiblity says we should define 2 new classes - automatic pump toggler and manual pump toggler
//       Rename this class to an applicable name that will then have automatic and manual pump toggler, looking at current state 
//       to determine what toggler is needed when (the job of this class is to look at state and call appropriate toggler!)
// TODO: Huge constructors need to be refactored to make use of builder design pattern 
int AutoTemperatureToggler::NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE = 5;
int AutoTemperatureToggler::AUTO_MODE_CHECK_DELAY = 20000;
int AutoTemperatureToggler::BUTTON_PRESSES = 0;
uint32_t AutoTemperatureToggler::currentTime {0};
uint32_t AutoTemperatureToggler::autoModeTime {0};

AutoTemperatureToggler::AutoTemperatureToggler(
    int relayPinVal, 
    RelayType relayTypeVal, 
    int thermistorPinVal, 
    int thermistorSeriesResistanceVal, 
    int thermistorBetaVal, 
    float thermistorNominalCelciusTemperatureVal, 
    int thermistorNominalResistanceVal,
    int interruptPinVal,
    int temperatureSetPinVal,
    int overrideControlPinVal
): interruptableThermistorTemperatureReader{InterruptableThermistorTemperatureReader(
        thermistorPinVal, 
        thermistorSeriesResistanceVal, 
        thermistorBetaVal, 
        thermistorNominalCelciusTemperatureVal, 
        thermistorNominalResistanceVal, 
        interruptPinVal)}, 
    relayToggler{RelayToggler(relayPinVal, relayTypeVal)},
    numberOfTemperatureReadingsCaptured{0},
    totalTemperature {0},
    temperatureSetPin{temperatureSetPinVal},
    overrideControlPin{overrideControlPinVal}
    {};

void AutoTemperatureToggler::performAutoRelayToggling(){
  float temperature = interruptableThermistorTemperatureReader.getTemperature();
  numberOfTemperatureReadingsCaptured += 1;
  totalTemperature += temperature;
  if (numberOfTemperatureReadingsCaptured == NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE){
    Serial.print(NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE);
    Serial.println(" have been measured, toggling of relay will now be determined.");
    float averageTemperature = totalTemperature / NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE ;
    if (averageTemperature >= getTurnOnTemperature()){
      relayToggler.turnOn();
    } else {
     relayToggler.turnOff();
    };
    numberOfTemperatureReadingsCaptured = 0;
    totalTemperature = 0;
  }
}

float AutoTemperatureToggler::getTurnOnTemperature(){
    int temperatureSetDigitalVoltage = analogRead(temperatureSetPin);
    int turnOnTemperature = 0.025 * temperatureSetDigitalVoltage + 25; // temperature between 25 and 50 degrees maps 0 to 1023 to value between 25 and 50
    Serial.print("Turn  on temperature was set to ");
    Serial.println(turnOnTemperature);
    return turnOnTemperature;
}

State AutoTemperatureToggler::getState(){
    return interruptableThermistorTemperatureReader.getState();
}

void AutoTemperatureToggler::performManualRelayToggling(){
    if (BUTTON_PRESSES % 2 == 1){
        relayToggler.turnOff();
    } else { 
        relayToggler.turnOn();
    }
}

void AutoTemperatureToggler::performManualAndAutonomousRelayToggling(){
    switch(getState()){
        case UNINTERRUPTED:
            Serial.println("Auto mode.");
            if (autoModeTime == 0){
                autoModeTime = millis(); 
            }
            currentTime = millis();
            if (currentTime >= autoModeTime + AUTO_MODE_CHECK_DELAY){
                Serial.println("Will perform auto toggling.");
                performAutoRelayToggling();
                autoModeTime = currentTime;
            }
            break;
        
        case INTERRUPTED:
            Serial.println("Override mode.");            
            int overrideControl {digitalRead(overrideControlPin)};
            if (overrideControl == HIGH){
                BUTTON_PRESSES += 1;      
                performManualRelayToggling();
            } 
            break;
        
  }
}



