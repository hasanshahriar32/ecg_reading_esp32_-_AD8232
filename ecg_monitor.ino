/*
 * ESP32 ECG Monitor - Main Application
 * 
 * Advanced ECG monitoring system with web interface
 * Author: Your Name
 * Date: June 2025
 * 
 * This is the main Arduino sketch that ties together all components
 * of the ECG monitoring system.
 */

#include "src/config/config.h"
#include "src/sensors/ecg_sensor.h"
#include "src/processing/signal_processor.h"
#include "src/web/web_server.h"

// Global objects
ECGSensor ecgSensor(ECG_PIN, LO_PLUS_PIN, LO_MINUS_PIN);
SignalProcessor signalProcessor;
ECGWebServer webServer;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("=== ESP32 ECG Monitor Starting ===");
  
  // Initialize LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize ECG sensor
  ecgSensor.begin();
  Serial.println("✓ ECG sensor initialized");
  
  // Initialize signal processor
  signalProcessor.begin();
  Serial.println("✓ Signal processor initialized");
  
  // Initialize web server
  webServer.begin();
  Serial.println("✓ Web server initialized");
  
  Serial.println("=== System Ready ===");
  Serial.println("Place electrodes and start monitoring!");
}

void loop() {
  // Handle web server requests
  webServer.handleClient();
  
  // Check if it's time for next ECG sample
  if (ecgSensor.isTimeForSample()) {
    // Read ECG data
    int ecgValue = ecgSensor.readValue();
    
    if (ecgSensor.areLeadsConnected()) {
      // Process the signal
      signalProcessor.processSample(ecgValue);
      
      // Update web server data
      webServer.updateECGData(ecgValue, signalProcessor.getHeartRate(), 
                              signalProcessor.getSignalQuality());
      
      // Handle heartbeat LED
      if (signalProcessor.isHeartbeatDetected()) {
        digitalWrite(LED_PIN, HIGH);
        delay(50);
        digitalWrite(LED_PIN, LOW);
      }
      
      // Serial output for debugging/plotting
      Serial.print(ecgValue);
      Serial.print(",");
      Serial.print(signalProcessor.getThreshold());
      Serial.print(",");
      Serial.println(signalProcessor.getHeartRate() * 10);
    } else {
      Serial.println("! Leads disconnected");
      digitalWrite(LED_PIN, LOW);
    }
  }
  
  // Small delay to prevent watchdog timeout
  delay(1);
}
