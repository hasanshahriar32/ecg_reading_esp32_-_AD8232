# API Reference

## ECGSensor Class

### Constructor
```cpp
ECGSensor(int ecgPin, int loPlusPin, int loMinusPin)
```
Creates an ECG sensor instance with specified pin assignments.

### Methods

#### `bool begin()`
Initializes the ECG sensor and configures ADC settings.
- **Returns**: `true` if initialization successful, `false` otherwise

#### `bool areLeadsConnected()`
Checks if ECG electrodes are properly connected.
- **Returns**: `true` if leads connected, `false` if disconnected

#### `int readValue()`
Reads ECG value if sample interval has elapsed.
- **Returns**: ADC value (0-4095) or -1 if no new sample

#### `bool isTimeForSample()`
Checks if it's time for the next sample based on configured rate.
- **Returns**: `true` if time for new sample

#### `int getRawValue()`
Gets immediate ADC reading without timing checks.
- **Returns**: Raw ADC value (0-4095)

#### `float getVoltage(int adcValue)`
Converts ADC value to voltage.
- **Parameters**: `adcValue` - ADC reading to convert
- **Returns**: Voltage value in volts

---

## SignalProcessor Class

### Constructor
```cpp
SignalProcessor()
```
Creates a signal processor instance.

### Methods

#### `bool begin()`
Initializes signal processing buffers and parameters.
- **Returns**: `true` if initialization successful

#### `void processSample(int ecgValue)`
Processes a new ECG sample through filtering and analysis.
- **Parameters**: `ecgValue` - Raw ECG value to process

#### `int getHeartRate()`
Gets the current calculated heart rate.
- **Returns**: Heart rate in beats per minute (BPM)

#### `int getSignalQuality()`
Gets the current signal quality assessment.
- **Returns**: Quality percentage (0-100)

#### `int getFilteredValue()`
Gets the last filtered ECG value.
- **Returns**: Filtered ADC value

#### `bool isHeartbeatDetected()`
Checks if a heartbeat was detected in the last sample.
- **Returns**: `true` if heartbeat detected (resets after reading)

#### `void reset()`
Resets all processing buffers and state.

#### `bool isSignalGoodQuality()`
Checks if signal quality meets minimum threshold.
- **Returns**: `true` if signal quality is acceptable

---

## ECGWebServer Class

### Constructor
```cpp
ECGWebServer()
```
Creates a web server instance.

### Methods

#### `bool begin()`
Initializes WiFi connection and starts web server.
- **Returns**: `true` if server started successfully

#### `void handleClient()`
Processes incoming web client requests. Call in main loop.

#### `void updateECGData(int ecgValue, int heartRate, int signalQuality)`
Updates current ECG data for web interface.
- **Parameters**: 
  - `ecgValue` - Current ECG reading
  - `heartRate` - Current heart rate
  - `signalQuality` - Current signal quality

#### `void updateLeadStatus(bool connected)`
Updates electrode connection status.
- **Parameters**: `connected` - Lead connection state

#### `bool isWiFiConnected()`
Gets WiFi connection status.
- **Returns**: `true` if connected to WiFi

#### `String getIPAddress()`
Gets the current IP address.
- **Returns**: IP address string or "Not connected"

---

## Configuration Constants

### Pin Definitions
```cpp
const int ECG_PIN = 34;        // ECG signal input
const int LO_PLUS_PIN = 32;    // Lead-off detection +
const int LO_MINUS_PIN = 33;   // Lead-off detection -
const int LED_PIN = 2;         // Status LED
```

### Sampling Settings
```cpp
const int SAMPLE_RATE = 500;                    // Hz
const unsigned long SAMPLE_INTERVAL = 2000;    // microseconds
```

### Processing Parameters
```cpp
const int ECG_BUFFER_SIZE = 100;        // Signal buffer size
const int HEARTBEAT_THRESHOLD = 2048;   // Beat detection threshold
const int BEAT_BUFFER_SIZE = 10;        // Heart rate averaging
```

### WiFi Configuration
```cpp
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";
const int WEB_SERVER_PORT = 80;
```

---

## REST API Endpoints

### GET /
Returns the main web interface HTML page.

### GET /data
Returns current ECG data in JSON format.

**Response:**
```json
{
  "ecgValue": 2048,
  "timestamp": 123456,
  "lastUpdate": 123450
}
```

### GET /status
Returns system status and vital signs.

**Response:**
```json
{
  "heartRate": 72,
  "signalQuality": 85,
  "leadsConnected": true,
  "sampleRate": 500,
  "uptime": 123456,
  "wifiConnected": true,
  "ipAddress": "192.168.1.100",
  "rssi": -45,
  "freeHeap": 200000
}
```

---

## Error Codes

```cpp
enum ECGErrorCodes {
  ECG_OK = 0,
  ECG_ERROR_WIFI_FAILED = 1,
  ECG_ERROR_SENSOR_INIT = 2,
  ECG_ERROR_LEADS_DISCONNECTED = 3,
  ECG_ERROR_POOR_SIGNAL = 4,
  ECG_ERROR_NO_HEARTBEAT = 5
};
```

---

## System Status

```cpp
enum SystemStatus {
  STATUS_INITIALIZING = 0,
  STATUS_WIFI_CONNECTING = 1,
  STATUS_READY = 2,
  STATUS_MONITORING = 3,
  STATUS_ERROR = 4
};
```

---

## Usage Examples

### Basic ECG Reading
```cpp
#include "src/sensors/ecg_sensor.h"
#include "src/config/config.h"

ECGSensor sensor(ECG_PIN, LO_PLUS_PIN, LO_MINUS_PIN);

void setup() {
  Serial.begin(115200);
  sensor.begin();
}

void loop() {
  if (sensor.isTimeForSample()) {
    int ecgValue = sensor.readValue();
    if (ecgValue >= 0) {
      Serial.println(ecgValue);
    }
  }
}
```

### Signal Processing
```cpp
#include "src/processing/signal_processor.h"

SignalProcessor processor;

void setup() {
  processor.begin();
}

void loop() {
  // Process ECG sample
  processor.processSample(ecgValue);
  
  // Get results
  int heartRate = processor.getHeartRate();
  int quality = processor.getSignalQuality();
  
  if (processor.isHeartbeatDetected()) {
    Serial.println("Heartbeat detected!");
  }
}
```

### Web Server Setup
```cpp
#include "src/web/web_server.h"

ECGWebServer webServer;

void setup() {
  webServer.begin();
}

void loop() {
  webServer.handleClient();
  
  // Update data
  webServer.updateECGData(ecgValue, heartRate, quality);
}
```
