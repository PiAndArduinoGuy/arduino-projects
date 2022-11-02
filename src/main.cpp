#include<math.h>
#include<Arduino.h>

class InterruptableTemperatureReader
{
private:
  int _temperaturePin;
  int static _interruptPin;
  bool static _interrupted;
  int _readingsToCapture;
  int _interval;
  void static temperatureReaderIntrruption() {
    if(digitalRead(_interruptPin) == 1) {
        _interrupted = true;
    } else {
       _interrupted = false; 
    }
  };
public:
  InterruptableTemperatureReader(int temperaturePin, int interruptPin, int interval, int readingsToCapture);
  float getAverageTemperatureOverInterval();
  float getSingleTemperatureReading();
};


InterruptableTemperatureReader::InterruptableTemperatureReader(int temperaturePin, int interruptPin, int interval, int readingsToCapture)
{
  attachInterrupt(1, &temperatureReaderIntrruption, CHANGE);
}





const int OVERRIDE_CONTROL_BUTTON_PIN = 2;
const int OVERRIDE_ON_LED_PIN = 11;
const int OVERRIDE_OFF_LED_PIN = 12;
const int AUTO_LED_PIN = 13;
const int RELAY_PIN = 7;
const int TEMPERATURE_SET_PIN = A1;
const int THERMISTOR_PIN = A2;
const int AUTO_OR_OVERRIDE_MODE_PIN = 3;

// thermistor constants
const int THERMISTOR_KNOWN_SERIES_RESISTOR_RESISTANCE = 2700;
const int THERMISTOR_BETA = 3950;
const int THERMISTOR_NOMINAL_TEMPERATURE_CELCIUS = 25;
const int THERMISTOR_NOMINAL_RESISTANCE = 1000;

// auto mode variables
const int AUTO_MODE_CHECK_DELAY = 10000; // 10 secs 
const int NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE = 5;
uint32_t currentTime;
uint32_t autoModeTime;
float capturedTemperatureTotal = 0;
int numberOfTemperatureReadingsCaptured = 0;

// FSM variables 
enum states {
  AUTO,
  OVERRIDE,
  NONE
};
volatile states currentState = AUTO;
states previousState = NONE;
int overrideControl = 0;

int autoOrOverrideState;
int buttonPressesCount = 0;

void resetOutputs(){
  digitalWrite(OVERRIDE_OFF_LED_PIN, LOW);
  digitalWrite(OVERRIDE_ON_LED_PIN, LOW);
  digitalWrite(AUTO_LED_PIN, LOW);
  digitalWrite(RELAY_PIN,HIGH);
}

void toggleStateInOverrideFor(int buttonPresses){
  if (buttonPressesCount % 2 == 1){ // off override
     Serial.println("Override off requested");
     digitalWrite(OVERRIDE_OFF_LED_PIN, HIGH);
     digitalWrite(OVERRIDE_ON_LED_PIN, LOW);
     digitalWrite(RELAY_PIN, HIGH);
  } else { // on override 
     Serial.println("Override on requested");
     digitalWrite(OVERRIDE_ON_LED_PIN, HIGH);
     digitalWrite(OVERRIDE_OFF_LED_PIN, LOW);
     digitalWrite(RELAY_PIN, LOW);
  }
}

void toggleStateInAutoFor(int temperature, int turnOnTemperature){
  if (temperature >= turnOnTemperature){
      Serial.println("Automatic mode determined that the pump should be turned on.");
      digitalWrite(RELAY_PIN, LOW);
    } else {
      Serial.println("Automatic mode determined that the pump should be turned off.");
      digitalWrite(RELAY_PIN, HIGH);
    }
}

int getThermistorResistance(){
  float resistance;
 
  float thermistorAnalogVoltage = analogRead(THERMISTOR_PIN);
 
  Serial.print("Analog reading "); 
  Serial.println(thermistorAnalogVoltage);
 
  // convert the value to resistance
  resistance = (1023 / thermistorAnalogVoltage)  - 1;     // (1023/ADC - 1) 
  resistance = THERMISTOR_KNOWN_SERIES_RESISTOR_RESISTANCE / resistance;  // 2.7K / (1023/ADC - 1)
  Serial.print("Thermistor resistance "); 
  Serial.println(resistance);
  return resistance;
}
int getTemperature(){
   float thermistorResistance = getThermistorResistance();
   float steinhart;
   steinhart = thermistorResistance / THERMISTOR_NOMINAL_RESISTANCE;     // (R/Ro)
   steinhart = log(steinhart);                                           // ln(R/Ro)
   steinhart /= THERMISTOR_BETA;                                         // 1/B * ln(R/Ro)
   steinhart += 1.0 / (THERMISTOR_NOMINAL_TEMPERATURE_CELCIUS + 273.15); // + (1/To)
   steinhart = 1.0 / steinhart;                                          // Invert
   steinhart -= 273.15;                                                  // convert absolute temp to C
  
   Serial.print("Temperature "); 
   Serial.print(steinhart);
   Serial.println(" *C");
   return steinhart;
}

int setTurnOnTemperature(){
  int digitalVoltage = analogRead(TEMPERATURE_SET_PIN);
  Serial.print("Voltage for temperature set pin: ");
  Serial.println(digitalVoltage);
  int turnOnTemperature = 0.025 * digitalVoltage + 25; // temperature between 25 and 50 degrees maps 0 to 1023 to value between 25 and 50
  Serial.print("Turn on temperature set to ");
  Serial.println(turnOnTemperature);
  return turnOnTemperature;
}

void toggleStateAutonomously(){
  int turnOnTemperature = setTurnOnTemperature();
  int temperature = getTemperature();
  numberOfTemperatureReadingsCaptured += 1;
  capturedTemperatureTotal += temperature;
  Serial.print("Total readings taken: ");
  Serial.println(numberOfTemperatureReadingsCaptured);
  if (numberOfTemperatureReadingsCaptured == NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE){
    Serial.print("Will perform toggle based on average over ");
    Serial.print(NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE);
    Serial.println(" readings");
    float averageTemperature = capturedTemperatureTotal / NUMBER_OF_TEMPERATURE_READINGS_TO_CAPTURE ;
    Serial.print("Average temperature of ");
    Serial.print(averageTemperature);
    Serial.println(" *C");
    toggleStateInAutoFor(averageTemperature, turnOnTemperature);
    numberOfTemperatureReadingsCaptured = 0;
    capturedTemperatureTotal = 0;
  }
}

void toggleAutoAndOverrideState(){
  Serial.println("Toggle detected");
  if(digitalRead(AUTO_OR_OVERRIDE_MODE_PIN) == 1) {
    currentState = OVERRIDE;
  } else {
    currentState = AUTO;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(OVERRIDE_ON_LED_PIN,OUTPUT);
  pinMode(OVERRIDE_OFF_LED_PIN,OUTPUT);
  pinMode(AUTO_LED_PIN,OUTPUT);
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(OVERRIDE_CONTROL_BUTTON_PIN,INPUT);
  pinMode(AUTO_OR_OVERRIDE_MODE_PIN,INPUT);
  attachInterrupt(1, toggleAutoAndOverrideState, CHANGE);
  resetOutputs();
}

void loop() {
  switch(currentState){
    case AUTO:
      Serial.println("Auto mode.");
      currentState = AUTO;
      if (previousState != currentState){
        previousState = currentState;
        autoModeTime = millis();
      }
      digitalWrite(OVERRIDE_ON_LED_PIN, LOW);
      digitalWrite(OVERRIDE_OFF_LED_PIN, LOW);
      digitalWrite(AUTO_LED_PIN, HIGH);
      currentTime = millis();
      if (currentTime >= autoModeTime + AUTO_MODE_CHECK_DELAY){
        toggleStateAutonomously();
        autoModeTime = currentTime;
      }
      break;
    
    case OVERRIDE:
      Serial.println("Override mode.");
      digitalWrite(AUTO_LED_PIN, LOW);
      overrideControl = digitalRead(OVERRIDE_CONTROL_BUTTON_PIN);
      if (overrideControl == HIGH){
        Serial.print("Button pressed.");
        buttonPressesCount += 1;  
        Serial.print("Number of presses up until now is ");
        Serial.println(buttonPressesCount);
        toggleStateInOverrideFor(buttonPressesCount);
      } 
      break;
    
  }
  delay(250);
}
