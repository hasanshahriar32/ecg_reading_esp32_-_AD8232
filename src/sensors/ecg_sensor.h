/*
 * ECG Sensor Class Header
 * 
 * Handles all ECG sensor operations including reading values,
 * checking lead connections, and managing ADC settings
 */

#ifndef ECG_SENSOR_H
#define ECG_SENSOR_H

#include <Arduino.h>

class ECGSensor {
private:
  int ecgPin;
  int loPlus;
  int loMinus;
  unsigned long lastSampleTime;
  bool initialized;
  
public:
  // Constructor
  ECGSensor(int ecgPin, int loPlusPin, int loMinusPin);
  
  // Initialize the sensor
  bool begin();
  
  // Check if leads are properly connected
  bool areLeadsConnected();
  
  // Read current ECG value
  int readValue();
  
  // Check if it's time for next sample
  bool isTimeForSample();
  
  // Get raw ADC value without timing check
  int getRawValue();
  
  // Convert ADC value to voltage
  float getVoltage(int adcValue);
  
  // Check sensor status
  bool isInitialized() { return initialized; }
  
  // Reset sampling timer
  void resetSampleTimer();
};
