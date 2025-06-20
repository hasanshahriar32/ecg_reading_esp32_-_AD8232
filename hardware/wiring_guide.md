# Circuit Diagram and Wiring Guide

## AD8232 ECG Module to ESP32 Connections

```
AD8232 ECG Module                ESP32 Dev Board
┌─────────────────┐             ┌─────────────────┐
│                 │             │                 │
│  VCC  ●─────────┼─────────────┼─● 3.3V          │
│  GND  ●─────────┼─────────────┼─● GND           │
│  OUTPUT ●───────┼─────────────┼─● GPIO34 (ADC)  │
│  LO+  ●─────────┼─────────────┼─● GPIO32        │
│  LO-  ●─────────┼─────────────┼─● GPIO33        │
│                 │             │                 │
│  3.5mm Jack     │             │  Built-in LED   │
│  for Electrodes │             │  GPIO2 ●        │
└─────────────────┘             └─────────────────┘
```

## Electrode Placement

### Standard 3-Lead ECG Configuration

```
        RA (Red)    LA (Yellow)
           ●           ●
            \         /
             \       /
              \     /
               \   /
                \ /
                 ●
            RL (Green)
```

**Electrode Positions:**
- **RA (Right Arm/Red)**: Right arm or right side of chest below collarbone
- **LA (Left Arm/Yellow)**: Left arm or left side of chest below collarbone  
- **RL (Right Leg/Green)**: Right leg or lower right side of torso (reference electrode)

### Alternative Chest Placement
For better signal quality, you can also place electrodes on the chest:

```
     LA ●                    ● RA
        |                    |
        |                    |
        |                    |
        |                    |
        └────────● RL────────┘
               (lower chest)
```

## Components List

### Required Components
- [ ] ESP32 Development Board
- [ ] AD8232 ECG Sensor Module
- [ ] 3x ECG Electrodes (disposable gel electrodes recommended)
- [ ] Jumper wires (Male-to-Female)
- [ ] Breadboard (optional)
- [ ] USB cable for ESP32 programming

### Optional Components
- [ ] 3.3V battery pack for portable operation
- [ ] Enclosure/case for protection
- [ ] LED for external heartbeat indication
- [ ] Buzzer for audio heartbeat indication

## Wiring Details

### Power Connections
```cpp
AD8232 VCC  →  ESP32 3.3V  (Red wire)
AD8232 GND  →  ESP32 GND   (Black wire)
```

### Signal Connections
```cpp
AD8232 OUTPUT  →  ESP32 GPIO34  (Yellow wire)  // ECG signal
AD8232 LO+     →  ESP32 GPIO32  (Blue wire)    // Lead-off detect +
AD8232 LO-     →  ESP32 GPIO33  (Green wire)   // Lead-off detect -
```

### Built-in LED
```cpp
ESP32 GPIO2  →  Built-in LED (usually on-board)
```

## Important Notes

### Power Requirements
- **Voltage**: 3.3V (DO NOT use 5V - it may damage the AD8232)
- **Current**: ~1mA typical consumption
- **Source**: Use ESP32's 3.3V output or external 3.3V supply

### Signal Quality Tips
1. **Clean Skin**: Use alcohol swab to clean electrode placement areas
2. **Remove Hair**: Shave or remove hair from electrode areas if necessary
3. **Firm Contact**: Ensure electrodes make good contact with skin
4. **Stay Still**: Minimize movement during monitoring
5. **Avoid Interference**: Keep away from AC power sources and WiFi routers

### Safety Considerations
⚠️ **IMPORTANT**: 
- Use battery-powered ESP32 for safety (avoid USB power during body contact)
- This is for educational purposes only - not for medical diagnosis
- Ensure proper electrical isolation
- Never use damaged or wet electrodes

## Troubleshooting

### Common Wiring Issues
| Problem | Possible Cause | Solution |
|---------|---------------|----------|
| No signal | Wrong pin connections | Double-check GPIO pin numbers |
| Weak signal | Poor electrode contact | Clean skin, replace electrodes |
| Noisy signal | Interference or loose wires | Use shorter, shielded wires |
| Lead-off always triggered | LO pins not connected | Verify GPIO32 and GPIO33 connections |

### Testing the Circuit
1. Connect circuit without electrodes first
2. Check power supply (3.3V at AD8232 VCC pin)
3. Verify all connections with multimeter
4. Test with simple electrode touch test
5. Upload and run the simple_ecg_monitor.ino first

## PCB Layout (Advanced)
For permanent installation, consider creating a custom PCB with:
- Proper ground planes
- Filtered power supply
- ESD protection
- Compact layout to minimize noise

This completes your ESP32 ECG monitoring setup! 🫀
