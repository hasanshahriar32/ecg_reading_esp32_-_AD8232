/*
 * Simple ESP32 ECG Monitor with Serial Output
 * 
 * This is a simplified version that focuses on basic ECG reading
 * and serial output for easy monitoring and debugging.
 * 
 * Hardware Connections:
 * AD8232 -> ESP32
 * VCC    -> 3.3V
 * GND    -> GND
 * OUTPUT -> GPIO34 (ADC1_CH6)
 * LO+    -> GPIO32 (Lead Off Detection +)
 * LO-    -> GPIO33 (Lead Off Detection -)
 * LED    -> GPIO2 (Built-in LED)
 */

// Pin definitions
const int ECG_PIN = 34;        // ADC pin for ECG signal
const int LO_PLUS_PIN = 32;    // Lead Off Detection +
const int LO_MINUS_PIN = 33;   // Lead Off Detection -
const int LED_PIN = 2;         // Built-in LED

// Sampling and processing variables
const int SAMPLE_RATE = 500;   // 500 Hz sampling rate
unsigned long lastSampleTime = 0;
const unsigned long sampleInterval = 1000000 / SAMPLE_RATE; // microseconds

// Heartbeat detection
const int THRESHOLD = 2048;    // Adjust based on your signal
bool lastBeatState = false;
unsigned long lastBeatTime = 0;
int heartRate = 0;

// Moving average filter
const int FILTER_SIZE = 5;
int filterBuffer[FILTER_SIZE];
int filterIndex = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(LO_PLUS_PIN, INPUT);
  pinMode(LO_MINUS_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Configure ADC
  analogReadResolution(12); // 12-bit resolution (0-4095)
  analogSetAttenuation(ADC_11db); // For 3.3V range
  
  // Initialize filter buffer
  for (int i = 0; i < FILTER_SIZE; i++) {
    filterBuffer[i] = 0;
  }
  
  Serial.println("=== ESP32 ECG Monitor ===");
  Serial.println("Connect electrodes and start monitoring");
  Serial.println("Output format: ECG_Value, Filtered_Value, Heart_Rate");
  Serial.println("========================================");
  
  delay(2000); // Allow time for setup
}

void loop() {
  // Check if it's time for next sample
  if (micros() - lastSampleTime >= sampleInterval) {
    readAndProcessECG();
    lastSampleTime = micros();
  }
}

void readAndProcessECG() {
  // Check lead connections
  bool loPlus = digitalRead(LO_PLUS_PIN);
  bool loMinus = digitalRead(LO_MINUS_PIN);
  
  if (loPlus || loMinus) {
    Serial.println("! Leads not connected - check electrode placement");
    digitalWrite(LED_PIN, LOW);
    return;
  }
  
  // Read ECG signal
  int rawECG = analogRead(ECG_PIN);
  
  // Apply simple moving average filter
  int filteredECG = applyMovingAverage(rawECG);
  
  // Detect heartbeat
  detectHeartbeat(filteredECG);
  
  // Output data (CSV format for Serial Plotter)
  Serial.print(rawECG);
  Serial.print(",");
  Serial.print(filteredECG);
  Serial.print(",");
  Serial.print(THRESHOLD);
  Serial.print(",");
  Serial.print(heartRate * 20); // Scale for visualization
  Serial.println();
}

int applyMovingAverage(int newValue) {
  // Add new value to filter buffer
  filterBuffer[filterIndex] = newValue;
  filterIndex = (filterIndex + 1) % FILTER_SIZE;
  
  // Calculate average
  long sum = 0;
  for (int i = 0; i < FILTER_SIZE; i++) {
    sum += filterBuffer[i];
  }
  
  return sum / FILTER_SIZE;
}

void detectHeartbeat(int ecgValue) {
  bool currentBeatState = ecgValue > THRESHOLD;
  
  // Detect rising edge (potential heartbeat)
  if (currentBeatState && !lastBeatState) {
    unsigned long currentTime = millis();
    
    if (lastBeatTime > 0) {
      unsigned long interval = currentTime - lastBeatTime;
      
      // Valid heartbeat interval (30-200 BPM)
      if (interval > 300 && interval < 2000) {
        // Calculate heart rate
        heartRate = 60000 / interval;
        
        // Flash LED to indicate heartbeat
        digitalWrite(LED_PIN, HIGH);
        
        // Print heartbeat info
        Serial.print("💓 BEAT detected! Heart Rate: ");
        Serial.print(heartRate);
        Serial.print(" BPM, Interval: ");
        Serial.print(interval);
        Serial.println(" ms");
        
        delay(50); // Brief LED flash
        digitalWrite(LED_PIN, LOW);
      }
    }
    
    lastBeatTime = currentTime;
  }
  
  lastBeatState = currentBeatState;
}
