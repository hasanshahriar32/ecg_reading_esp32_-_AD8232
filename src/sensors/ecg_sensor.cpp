/*
 * ECG Sensor Class Implementation
 * 
 * Handles AD8232 ECG sensor operations
 */

#include "ecg_sensor.h"
#include "../config/config.h"

ECGSensor::ECGSensor(int ecgPin, int loPlusPin, int loMinusPin) {
  this->ecgPin = ecgPin;
  this->loPlus = loPlusPin;
  this->loMinus = loMinusPin;
  this->lastSampleTime = 0;
  this->initialized = false;
}

bool ECGSensor::begin() {
  // Configure pins
  pinMode(loPlus, INPUT);
  pinMode(loMinus, INPUT);
  
  // Configure ADC
  analogReadResolution(ADC_RESOLUTION);
  analogSetAttenuation(ADC_ATTENUATION);
  
  // Test ADC reading
  int testRead = analogRead(ecgPin);
  if (testRead < 0) {
    Serial.println("ERROR: ECG sensor initialization failed");
    return false;
  }
  
  initialized = true;
  resetSampleTimer();
  
  Serial.println("ECG sensor initialized successfully");
  Serial.print("ADC resolution: ");
  Serial.print(ADC_RESOLUTION);
  Serial.println(" bits");
  Serial.print("Reference voltage: ");
  Serial.print(REFERENCE_VOLTAGE);
  Serial.println("V");
  
  return true;
}

bool ECGSensor::areLeadsConnected() {
  if (!initialized) return false;
  
  bool loPlusState = digitalRead(loPlus);
  bool loMinusState = digitalRead(loMinus);
  
  // Leads are connected when both LO+ and LO- are LOW
  return !(loPlusState || loMinusState);
}

int ECGSensor::readValue() {
  if (!initialized) return 0;
  
  if (isTimeForSample()) {
    lastSampleTime = micros();
    return analogRead(ecgPin);
  }
  
  return -1; // Indicates no new sample
}

bool ECGSensor::isTimeForSample() {
  if (!initialized) return false;
  
  return (micros() - lastSampleTime) >= SAMPLE_INTERVAL;
}

int ECGSensor::getRawValue() {
  if (!initialized) return 0;
  
  return analogRead(ecgPin);
}

float ECGSensor::getVoltage(int adcValue) {
  return (float)adcValue * REFERENCE_VOLTAGE / ADC_MAX_VALUE;
}

void ECGSensor::resetSampleTimer() {
  lastSampleTime = micros();
}
