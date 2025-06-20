/*
 * Signal Processor Class Implementation
 * 
 * Advanced ECG signal processing with filtering and heartbeat detection
 */

#include "signal_processor.h"
#include "../config/config.h"

SignalProcessor::SignalProcessor() {
  bufferIndex = 0;
  filterIndex = 0;
  lastBeatState = false;
  lastBeatTime = 0;
  beatIntervalIndex = 0;
  heartbeatDetected = false;
  currentHeartRate = 0;
  signalQuality = 0;
  filteredValue = 0;
}

bool SignalProcessor::begin() {
  // Initialize buffers
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    ecgBuffer[i] = 0;
  }
  
  for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
    filterBuffer[i] = 0;
  }
  
  for (int i = 0; i < BEAT_BUFFER_SIZE; i++) {
    beatIntervals[i] = 0;
  }
  
  Serial.println("Signal processor initialized");
  Serial.print("Buffer size: ");
  Serial.println(ECG_BUFFER_SIZE);
  Serial.print("Filter size: ");
  Serial.println(MOVING_AVERAGE_SIZE);
  Serial.print("Heartbeat threshold: ");
  Serial.println(HEARTBEAT_THRESHOLD);
  
  return true;
}

void SignalProcessor::processSample(int ecgValue) {
  // Store raw value in buffer
  ecgBuffer[bufferIndex] = ecgValue;
  bufferIndex = (bufferIndex + 1) % ECG_BUFFER_SIZE;
  
  // Apply filtering
  filteredValue = applyMovingAverage(ecgValue);
  
  // Detect heartbeat
  detectHeartbeat(filteredValue);
  
  // Calculate signal quality
  calculateSignalQuality();
}

int SignalProcessor::applyMovingAverage(int newValue) {
  // Add new value to filter buffer
  filterBuffer[filterIndex] = newValue;
  filterIndex = (filterIndex + 1) % MOVING_AVERAGE_SIZE;
  
  // Calculate average
  long sum = 0;
  for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
    sum += filterBuffer[i];
  }
  
  return sum / MOVING_AVERAGE_SIZE;
}

void SignalProcessor::detectHeartbeat(int ecgValue) {
  bool currentBeatState = ecgValue > HEARTBEAT_THRESHOLD;
  heartbeatDetected = false;
  
  // Detect rising edge (potential heartbeat)
  if (currentBeatState && !lastBeatState) {
    unsigned long currentTime = millis();
    
    if (lastBeatTime > 0) {
      unsigned long interval = currentTime - lastBeatTime;
      
      // Check if interval is valid
      if (isValidHeartbeatInterval(interval)) {
        // Store interval
        beatIntervals[beatIntervalIndex] = interval;
        beatIntervalIndex = (beatIntervalIndex + 1) % BEAT_BUFFER_SIZE;
        
        // Calculate new heart rate
        calculateHeartRate();
        
        // Set heartbeat detected flag
        heartbeatDetected = true;
        
        if (ENABLE_DEBUG_MESSAGES) {
          Serial.print("💓 Heartbeat detected! BPM: ");
          Serial.print(currentHeartRate);
          Serial.print(", Interval: ");
          Serial.print(interval);
          Serial.println("ms");
        }
      }
    }
    
    lastBeatTime = currentTime;
  }
  
  lastBeatState = currentBeatState;
}

void SignalProcessor::calculateHeartRate() {
  unsigned long totalInterval = 0;
  int validIntervals = 0;
  
  // Sum all valid intervals
  for (int i = 0; i < BEAT_BUFFER_SIZE; i++) {
    if (beatIntervals[i] > 0) {
      totalInterval += beatIntervals[i];
      validIntervals++;
    }
  }
  
  if (validIntervals > 0) {
    unsigned long avgInterval = totalInterval / validIntervals;
    currentHeartRate = 60000 / avgInterval; // Convert to BPM
    
    // Clamp to reasonable values
    if (currentHeartRate < 30) currentHeartRate = 30;
    if (currentHeartRate > 200) currentHeartRate = 200;
  }
}

void SignalProcessor::calculateSignalQuality() {
  if (bufferIndex < ECG_BUFFER_SIZE) {
    signalQuality = 0;
    return;
  }
  
  // Calculate variance-based signal quality
  long sum = 0;
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    sum += ecgBuffer[i];
  }
  int mean = sum / ECG_BUFFER_SIZE;
  
  long variance = 0;
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    variance += (ecgBuffer[i] - mean) * (ecgBuffer[i] - mean);
  }
  variance /= ECG_BUFFER_SIZE;
  
  // Convert variance to quality percentage (0-100)
  // Higher variance generally indicates better ECG signal
  signalQuality = constrain(map(variance, 0, 1000000, 0, 100), 0, 100);
  
  // Additional quality checks
  if (currentHeartRate < 30 || currentHeartRate > 200) {
    signalQuality = max(0, signalQuality - 30);
  }
}

bool SignalProcessor::isValidHeartbeatInterval(unsigned long interval) {
  return (interval >= MIN_BEAT_INTERVAL && interval <= MAX_BEAT_INTERVAL);
}

bool SignalProcessor::isHeartbeatDetected() {
  bool detected = heartbeatDetected;
  heartbeatDetected = false;  // Reset flag after reading
  return detected;
}

int SignalProcessor::getMeanValue() {
  if (bufferIndex < ECG_BUFFER_SIZE) return 0;
  
  long sum = 0;
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    sum += ecgBuffer[i];
  }
  return sum / ECG_BUFFER_SIZE;
}

int SignalProcessor::getVariance() {
  if (bufferIndex < ECG_BUFFER_SIZE) return 0;
  
  int mean = getMeanValue();
  long variance = 0;
  
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    variance += (ecgBuffer[i] - mean) * (ecgBuffer[i] - mean);
  }
  
  return variance / ECG_BUFFER_SIZE;
}

void SignalProcessor::reset() {
  // Reset all buffers and state
  for (int i = 0; i < ECG_BUFFER_SIZE; i++) {
    ecgBuffer[i] = 0;
  }
  
  for (int i = 0; i < MOVING_AVERAGE_SIZE; i++) {
    filterBuffer[i] = 0;
  }
  
  for (int i = 0; i < BEAT_BUFFER_SIZE; i++) {
    beatIntervals[i] = 0;
  }
  
  bufferIndex = 0;
  filterIndex = 0;
  beatIntervalIndex = 0;
  lastBeatTime = 0;
  currentHeartRate = 0;
  signalQuality = 0;
  heartbeatDetected = false;
  
  Serial.println("Signal processor reset");
}

bool SignalProcessor::isSignalGoodQuality() {
  return (signalQuality >= GOOD_SIGNAL_QUALITY);
}
