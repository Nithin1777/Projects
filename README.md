# ESP32 CarBot - Bluetooth Controlled Robot Car

A wireless robot car controlled via Bluetooth using an ESP32 microcontroller and L298N motor driver. Compatible with the [HC-05 Bluetooth Controller](https://play.google.com/store/apps/details?id=com.giristudio.hc05.bluetooth.arduino.control) Android app.

##  Features

- **Bluetooth Control**: Wirelessly control your robot car from your smartphone
- **Multi-directional Movement**: Forward, backward, left, right, and curved movements
- **Variable Speed Control**: 10 speed levels (0-9) for precise control
- **LED Status Indicator**: Visual feedback for connection status
- **Serial Monitor Support**: Debug and control via USB serial connection
- **Optimized Turning**: Enhanced turning performance with minimum speed thresholds

##  Compatible App

This project is designed to work with the **Arduino Bluetooth Controller** app:
- **Download**: [Play Store Link](https://play.google.com/store/apps/details?id=com.giristudio.hc05.bluetooth.arduino.control)
- **Device Name**: `ESP32_CarBot`

##  Hardware Requirements

### Components Needed:
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- **L298N Motor Driver Module**
- **2x DC Geared Motors** (with wheels)
- **Battery Pack** (7.4V Li-Po or 6x AA batteries recommended)
- **LED** (for status indication, wired to the onboard LED by defualt)
- **Jumper Wires**
- **Chassis/Frame** for mounting components

### Wiring Diagram

#### ESP32 to L298N Motor Driver:
| ESP32 Pin | L298N Pin | Function |
|-----------|-----------|----------|
| GPIO 12   | IN1       | Left Motor Direction 1 |
| GPIO 13   | IN2       | Left Motor Direction 2 |
| GPIO 14   | ENA       | Left Motor Speed (PWM) |
| GPIO 27   | IN3       | Right Motor Direction 1 |
| GPIO 26   | IN4       | Right Motor Direction 2 |
| GPIO 25   | ENB       | Right Motor Speed (PWM) |
| GND       | GND       | Ground |

#### Additional Connections(if using external LED):
| ESP32 Pin | Component | Function |
|-----------|-----------|----------|
| GPIO 2    | LED (+)   | Status LED |
| GND       | LED (-)   | LED Ground (via 220Ω resistor) |

#### L298N to Motors:
- **OUT1 & OUT2**: Connect to Left Motor
- **OUT3 & OUT4**: Connect to Right Motor
- **12V**: Connect to battery positive
- **GND**: Connect to battery negative
- **5V**: Can be used to power ESP32 (if battery > 7V)

##  Software Setup

### Prerequisites:
1. **Arduino IDE** (version 1.8.0 or later)
2. **ESP32 Board Package** installed in Arduino IDE

### Installation Steps:

1. **Install ESP32 Board Package**:
   - Open Arduino IDE
   - Go to File → Preferences
   - Add this URL to "Additional Boards Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to Tools → Board → Boards Manager
   - Search for "ESP32" and install the package

2. **Upload the Code**:
   - Connect ESP32 to computer via USB
   - Select the correct board: Tools → Board → ESP32 Dev Module
   - Select the correct port: Tools → Port → (your ESP32 port)
   - Upload the provided code

## 📋 Control Commands

The robot responds to these single-character commands:

### Movement Commands:
| Command | Action |
|---------|--------|
| `F` | Move Forward |
| `B` | Move Backward |
| `L` | Turn Left (pivot) |
| `R` | Turn Right (pivot) |
| `G` | Forward Left (curve) |
| `H` | Forward Right (curve) |
| `S` | Stop |

### Speed Control:
| Command | Speed Level | PWM Value |
|---------|-------------|-----------|
| `0` | Stop | 0 |
| `1` | Level 1 | 50 |
| `2` | Level 2 | 75 |
| `3` | Level 3 | 100 |
| `4` | Level 4 | 125 |
| `5` | Level 5 | 150 |
| `6` | Level 6 | 175 |
| `7` | Level 7 | 200 |
| `8` | Level 8 | 225 |
| `9` | Maximum | 255 |

## 🔄 How to Use

### Pairing with Smartphone:

1. **Power on the ESP32** - The onboard LED will blink indicating it's ready to pair
2. **Enable Bluetooth** on your smartphone
3. **Scan for devices** - Look for "ESP32_CarBot"
4. **Pair the device** (no PIN required)
5. **Open the HC-05 Bluetooth Controller app**
6. **Connect to ESP32_CarBot**
7. **LED will turn solid** indicating successful connection

### Controlling the Robot:

- Use the directional buttons in the app for movement
- Use the speed slider or number buttons (0-9) for speed control
- The 'S' button or releasing controls will stop the robot

## 🛠️ Troubleshooting

### Common Issues:

**LED keeps blinking (won't connect)**:
- Ensure Bluetooth is enabled on your phone
- Check if the device is already paired with another device
- Try restarting the ESP32

**Motors don't move**:
- Check battery voltage (should be > 6V)
- Verify all wiring connections
- Ensure L298N module is powered
- Check motor connections to L298N outputs

**Weak turning performance**:
- The code includes minimum turning speed (100 PWM)
- Ensure adequate battery power
- Check for mechanical obstructions

**Serial Monitor shows "Failed to attach PWM"**:
- This indicates a PWM configuration issue
- Try different GPIO pins or restart the ESP32

## ⚙️ Customization

### Modify Speed Settings:
Change the speed values in the `processCommand()` function:
```cpp
case '1': valSpeed = 50; break;   // Adjust these values
case '2': valSpeed = 75; break;   // as needed for your motors
```

### Change Bluetooth Device Name:
Modify this line in `setup()`:
```cpp
SerialBT.begin("ESP32_CarBot"); // Change name here
```

### Adjust Turning Behavior:
Modify `minTurnSpeed` variable:
```cpp
int minTurnSpeed = 100; // Increase for more aggressive turns
```

## 📊 Technical Specifications

- **Microcontroller**: ESP32 (240MHz dual-core)
- **Bluetooth**: Classic Bluetooth 2.0/2.1
- **PWM Frequency**: 5kHz
- **PWM Resolution**: 8-bit (0-255)
- **Operating Voltage**: 3.3V (ESP32), 5-12V (Motors)
- **Communication**: Serial (115200 baud)

## 🤝 Contributing

Feel free to submit issues, fork the repository, and create pull requests for any improvements.

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

## 📞 Support

For questions or support:
- Open an issue in this repository
- Check the troubleshooting section above
- Refer to the HC-05 Bluetooth Controller app documentation

---


