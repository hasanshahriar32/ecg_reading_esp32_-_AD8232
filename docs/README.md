# ESP32 ECG Monitor with AD8232

This project creates a real-time ECG monitoring system using ESP32 and AD8232 ECG sensor module.

## Features

- ✅ Real-time ECG signal acquisition
- ✅ Web-based visualization interface  
- ✅ Heart rate calculation
- ✅ Lead-off detection
- ✅ Signal quality assessment
- ✅ Serial output for Arduino IDE Serial Plotter
- ✅ Built-in LED heartbeat indicator

## Hardware Requirements

- ESP32 Development Board
- AD8232 ECG Sensor Module
- ECG electrodes (3 pieces)
- Jumper wires
- Breadboard (optional)

## Wiring Diagram

```
AD8232 Module    ESP32 Pin
-----------     ---------
VCC             3.3V
GND             GND
OUTPUT          GPIO34 (ADC1_CH6)
LO+             GPIO32
LO-             GPIO33

Additional:
Built-in LED    GPIO2 (for heartbeat indication)
```

## Pin Configuration

| AD8232 Pin | ESP32 Pin | Description |
|------------|-----------|-------------|
| VCC | 3.3V | Power supply |
| GND | GND | Ground |
| OUTPUT | GPIO34 | ECG signal output |
| LO+ | GPIO32 | Lead-off detection positive |
| LO- | GPIO33 | Lead-off detection negative |

## Setup Instructions

### 1. Hardware Setup
1. Connect the AD8232 module to ESP32 according to the wiring diagram
2. Attach ECG electrodes:
   - **RA (Right Arm)**: Right arm or right side of chest
   - **LA (Left Arm)**: Left arm or left side of chest  
   - **RL (Right Leg)**: Right leg or lower right abdomen (reference/ground)

### 2. Software Setup
1. Install Arduino IDE
2. Install ESP32 board package in Arduino IDE
3. Install required libraries:
   ```
   - ArduinoJson (by Benoit Blanchon)
   ```
4. Open `ecg_monitor.ino` in Arduino IDE
5. Update WiFi credentials:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```
6. Upload the code to ESP32

### 3. Usage
1. Open Serial Monitor (115200 baud) to see debug output and IP address
2. Open web browser and navigate to the ESP32's IP address
3. View real-time ECG waveform and vital statistics
4. Use Arduino IDE Serial Plotter for additional visualization

## Web Interface Features

The web interface provides:
- **Real-time ECG waveform** with smooth plotting
- **Heart rate display** in BPM
- **Signal quality indicator** (0-100%)
- **Lead connection status** with visual indicator
- **Responsive design** for mobile and desktop

## Serial Output Format

The serial output provides comma-separated values suitable for Arduino Serial Plotter:
```
ECG_Value, Threshold, HeartRate_Scaled
```

## Configuration Options

### Sampling Rate
```cpp
const int SAMPLE_RATE = 500;   // 500 Hz (adjustable)
```

### Heartbeat Detection Threshold
```cpp
const int THRESHOLD = 2048;    // Adjust based on signal amplitude
```

### Buffer Sizes
```cpp
const int BUFFER_SIZE = 100;           // Signal processing buffer
const int BEAT_BUFFER_SIZE = 10;       // Heartbeat interval buffer
```

## Troubleshooting

### Common Issues

1. **No Signal Detected**
   - Check electrode connections
   - Ensure good skin contact (clean skin, remove hair if necessary)
   - Verify wiring connections
   - Check if leads are properly connected (LO+ and LO- should be LOW)

2. **Noisy Signal**
   - Ensure stable power supply
   - Keep away from AC power lines
   - Use shorter wires
   - Ensure good electrode contact
   - Try different electrode positions

3. **Incorrect Heart Rate**
   - Adjust the `THRESHOLD` value based on your signal amplitude
   - Ensure electrodes are placed correctly
   - Check for motion artifacts

4. **WiFi Connection Issues**
   - Verify WiFi credentials
   - Check WiFi signal strength
   - Try different WiFi channels

### Signal Quality Tips

- **Electrode Placement**: Follow standard ECG lead placement
- **Skin Preparation**: Clean with alcohol, remove oils/lotions
- **Stay Still**: Minimize movement during monitoring
- **Environment**: Avoid areas with electrical interference

## Technical Specifications

- **Sampling Rate**: 500 Hz (configurable)
- **ADC Resolution**: 12-bit (0-4095)
- **Input Voltage Range**: 0-3.3V
- **Heart Rate Range**: 30-200 BPM
- **Web Server**: Real-time updates at 50 Hz
- **Status Updates**: 1 Hz

## API Endpoints

The ESP32 serves the following endpoints:

- `GET /` - Main web interface
- `GET /data` - Current ECG data (JSON)
- `GET /status` - System status and vital signs (JSON)

### Data Format
```json
{
  "ecgValue": 2048,
  "timestamp": 123456
}
```

### Status Format
```json
{
  "heartRate": 72,
  "signalQuality": 85,
  "leadsConnected": true,
  "sampleRate": 500,
  "uptime": 123456
}
```

## Safety Considerations

⚠️ **IMPORTANT SAFETY NOTICE** ⚠️

This project is for **educational and experimental purposes only**. It is **NOT** intended for medical diagnosis or clinical use.

- This device is not medically certified
- Do not rely on this for medical decisions
- Consult healthcare professionals for medical concerns
- Ensure proper electrical isolation
- Use only battery-powered ESP32 for safety

## License

This project is open source and available under the MIT License.

## Contributing

Feel free to contribute improvements, bug fixes, or additional features through pull requests.

## Support

For issues and questions, please check the troubleshooting section or create an issue in the project repository.
