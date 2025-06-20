/*
 * Signal Processor Class Header
 * 
 * Handles ECG signal processing including filtering, heartbeat detection,
 * heart rate calculation, and signal quality assessment
 */

#ifndef SIGNAL_PROCESSOR_H
#define SIGNAL_PROCESSOR_H

#include <Arduino.h>

class SignalProcessor {
private:
  // Signal buffers
  int ecgBuffer[100];  // Will use ECG_BUFFER_SIZE from config
  int bufferIndex;
  
  // Moving average filter
  int filterBuffer[5];  // Will use MOVING_AVERAGE_SIZE from config
  int filterIndex;
  
  // Heartbeat detection
  bool lastBeatState;
  unsigned long lastBeatTime;
  unsigned long beatIntervals[10];  // Will use BEAT_BUFFER_SIZE from config
  int beatIntervalIndex;
  bool heartbeatDetected;
  
  // Calculated values
  int currentHeartRate;
  int signalQuality;
  int filteredValue;
  
  // Internal methods
  int applyMovingAverage(int newValue);
  void detectHeartbeat(int ecgValue);
  void calculateHeartRate();
  void calculateSignalQuality();
  bool isValidHeartbeatInterval(unsigned long interval);
  
public:
  // Constructor
  SignalProcessor();
  
  // Initialize the processor
  bool begin();
  
  // Process a new ECG sample
  void processSample(int ecgValue);
  
  // Getters
  int getHeartRate() { return currentHeartRate; }
  int getSignalQuality() { return signalQuality; }
  int getFilteredValue() { return filteredValue; }
  int getThreshold() { return 2048; }  // Will use HEARTBEAT_THRESHOLD from config
  bool isHeartbeatDetected();
  
  // Get statistical information
  int getMeanValue();
  int getVariance();
  
  // Reset processor state
  void reset();
  
  // Check if signal is good quality
  bool isSignalGoodQuality();
};
