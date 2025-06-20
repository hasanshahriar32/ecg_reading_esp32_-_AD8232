/*
 * Configuration File for ESP32 ECG Monitor
 * 
 * Contains all system constants, pin definitions, and settings
 */

#ifndef CONFIG_H
#define CONFIG_H

// ========== PIN DEFINITIONS ==========
const int ECG_PIN = 34;        // ADC pin for ECG signal (ADC1_CH6)
const int LO_PLUS_PIN = 32;    // Lead Off Detection positive
const int LO_MINUS_PIN = 33;   // Lead Off Detection negative
const int LED_PIN = 2;         // Built-in LED pin

// ========== COMMUNICATION SETTINGS ==========
const int SERIAL_BAUD_RATE = 115200;

// ========== WiFi CONFIGURATION ==========
// Update these with your network credentials
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const int WEB_SERVER_PORT = 80;

// ========== ECG SAMPLING SETTINGS ==========
const int SAMPLE_RATE = 500;                           // Hz - ECG sampling rate
const unsigned long SAMPLE_INTERVAL = 1000000 / SAMPLE_RATE; // microseconds

// ========== SIGNAL PROCESSING SETTINGS ==========
const int ECG_BUFFER_SIZE = 100;        // Buffer size for signal processing
const int HEARTBEAT_THRESHOLD = 2048;   // Threshold for heartbeat detection (0-4095)
const int BEAT_BUFFER_SIZE = 10;        // Number of heartbeat intervals to average

// ========== HEART RATE LIMITS ==========
const unsigned long MIN_BEAT_INTERVAL = 300;  // ms (200 BPM max)
const unsigned long MAX_BEAT_INTERVAL = 2000; // ms (30 BPM min)

// ========== ADC CONFIGURATION ==========
const int ADC_RESOLUTION = 12;          // 12-bit ADC (0-4095)
const adc_attenuation_t ADC_ATTENUATION = ADC_11db; // For 3.3V range

// ========== FILTER SETTINGS ==========
const int MOVING_AVERAGE_SIZE = 5;      // Moving average filter size

// ========== WEB UPDATE INTERVALS ==========
const unsigned long DATA_UPDATE_INTERVAL = 20;   // ms (50 Hz)
const unsigned long STATUS_UPDATE_INTERVAL = 1000; // ms (1 Hz)

// ========== SIGNAL QUALITY THRESHOLDS ==========
const int MIN_SIGNAL_QUALITY = 20;      // Minimum acceptable signal quality (%)
const int GOOD_SIGNAL_QUALITY = 70;     // Good signal quality threshold (%)

// ========== SYSTEM TIMEOUTS ==========
const unsigned long WIFI_TIMEOUT = 10000;       // ms - WiFi connection timeout
const unsigned long HEARTBEAT_TIMEOUT = 5000;   // ms - No heartbeat detection timeout

// ========== DEBUG SETTINGS ==========
const bool ENABLE_SERIAL_PLOTTING = true;       // Enable serial output for plotter
const bool ENABLE_DEBUG_MESSAGES = true;        // Enable debug messages
const int SERIAL_PLOT_SCALE_FACTOR = 10;        // Scale factor for heart rate in serial plot

// ========== HARDWARE SPECIFIC ==========
const float REFERENCE_VOLTAGE = 3.3;            // ESP32 reference voltage
const int ADC_MAX_VALUE = 4095;                 // 12-bit ADC maximum value

// ========== ERROR CODES ==========
enum ECGErrorCodes {
  ECG_OK = 0,
  ECG_ERROR_WIFI_FAILED = 1,
  ECG_ERROR_SENSOR_INIT = 2,
  ECG_ERROR_LEADS_DISCONNECTED = 3,
  ECG_ERROR_POOR_SIGNAL = 4,
  ECG_ERROR_NO_HEARTBEAT = 5
};

// ========== SYSTEM STATUS ==========
enum SystemStatus {
  STATUS_INITIALIZING = 0,
  STATUS_WIFI_CONNECTING = 1,
  STATUS_READY = 2,
  STATUS_MONITORING = 3,
  STATUS_ERROR = 4
};

#endif // CONFIG_H
