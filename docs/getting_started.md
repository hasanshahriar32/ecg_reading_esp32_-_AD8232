# Getting Started Guide

## Quick Start (5 minutes)

### 1. Hardware Setup
1. Connect AD8232 to ESP32 following the pin diagram
2. Attach electrodes to the 3.5mm jack on AD8232
3. Connect ESP32 to computer via USB

### 2. Software Setup
1. Install Arduino IDE
2. Add ESP32 board support
3. Install ArduinoJson library (for full version)
4. Open one of the provided sketch files

### 3. First Test
1. Start with `simple_ecg_monitor.ino` for basic functionality
2. Upload to ESP32
3. Open Serial Monitor (115200 baud)
4. Place electrodes on body and observe signal

## Which File to Use?

### For Beginners: `simple_ecg_monitor.ino`
- ✅ Easy to understand
- ✅ Serial output only
- ✅ Basic heartbeat detection
- ✅ No WiFi dependencies
- ✅ Great for learning and testing

### For Advanced Users: `ecg_monitor.ino`
- ✅ Web interface with real-time graphs
- ✅ Advanced signal processing
- ✅ Multiple output formats
- ✅ Remote monitoring capability
- ⚠️ Requires WiFi setup

## Library Dependencies

### For Simple Version
- No external libraries required
- Uses built-in ESP32 functions only

### For Full Version
```
ArduinoJson (by Benoit Blanchon)
```

Install via: Arduino IDE → Tools → Manage Libraries → Search "ArduinoJson"

## ESP32 Board Setup

### Arduino IDE Configuration
1. Open Arduino IDE
2. Go to File → Preferences
3. Add this URL to "Additional Board Manager URLs":
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Go to Tools → Board → Boards Manager
5. Search for "ESP32" and install "ESP32 by Espressif Systems"
6. Select your ESP32 board from Tools → Board → ESP32 Arduino

### Common ESP32 Boards
- **ESP32 Dev Module** (generic)
- **ESP32-WROOM-32**
- **ESP32-DevKitC**
- **DOIT ESP32 DEVKIT V1**

## Testing Your Setup

### 1. Basic Connection Test
```cpp
// Quick test code to verify ESP32 is working
void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("ESP32 is working!");
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
}
```

### 2. AD8232 Connection Test
Use the simple_ecg_monitor.ino and check:
- Power LED on AD8232 should light up
- Serial output should show values
- Touching electrodes should change readings

### 3. Electrode Test
1. Connect electrodes to your body
2. Check Serial Monitor for "Leads connected" message
3. Signal should show heart rhythm pattern

## Troubleshooting First Setup

### ESP32 Not Detected
- Try different USB cable
- Install USB drivers (CP210x or CH340)
- Press and hold BOOT button while uploading
- Check if correct COM port is selected

### No ECG Signal
- Verify all wiring connections
- Check electrode placement
- Ensure good skin contact
- Try different electrode positions

### Compilation Errors
- Verify ESP32 board package is installed
- Check if correct board is selected
- Install missing libraries
- Restart Arduino IDE

## Next Steps

1. **Start Simple**: Begin with `simple_ecg_monitor.ino`
2. **Test Thoroughly**: Verify all connections work
3. **Experiment**: Try different electrode placements
4. **Upgrade**: Move to full version with web interface
5. **Customize**: Modify code for your specific needs

## Safety Reminders

- 🔋 Use battery power when possible
- 🚫 Not for medical diagnosis
- 🧼 Clean electrodes before use
- ⚡ Avoid damaged components
- 👨‍⚕️ Consult healthcare professionals for medical concerns

Ready to start monitoring your heart rhythm! 💗
