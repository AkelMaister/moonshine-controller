# NodeMCU PT100 Temperature Sensor Project

This project uses a NodeMCU ESP8266 to read temperatures from three PT100 resistance temperature detectors (RTDs) and send the data to a backend server via WiFi.

## Hardware Requirements

- NodeMCU ESP8266 board
- 3x PT100 temperature sensors
- 1x ADS1115 16-bit ADC module
- 3x Wheatstone bridge circuits:
  - 3x 100Ω precision resistors (R1)
  - 3x 100Ω precision resistors (R2)
  - 3x 100Ω precision resistors (R3)
- Jumper wires
- Power supply (USB or external 5V)
- Breadboard (optional, for testing)

## Software Requirements

1. Arduino IDE ([download](https://www.arduino.cc/en/software))
2. Required Libraries:
   - Adafruit_ADS1X15 (by Adafruit)
   - Wire (built-in)
   - ESP8266WiFi (comes with ESP8266 board support)
   - ArduinoJson (by Benoit Blanchon)

## Wiring Diagram

```
NodeMCU ESP8266  ADS1115 ADC       PT100 Sensors (3x)
+3.3V ----------+---- VDD
                |
D1(GPIO5) ------+---- SCL 
                |
D2(GPIO4) ------+---- SDA
                |
GND ------------+---- GND
                |
                +---- A0 ----+---- PT100 Sensor 1
                |            |
                +---- A1 ----+---- PT100 Sensor 2
                |            |
                +---- A2 ----+---- PT100 Sensor 3
                |
                +---- A3 (unused)

Wheatstone Bridge Circuit (for each PT100):
     VCC (3.3V)
        |
        |
    +---[R1]---+
    |          |
    |          |
[PT100]    [R2]
    |          |
    |          |
    +---[R3]---+
        |
        |
      GND
```

## PT100 Sensor Wiring

Each PT100 sensor has 2 or 3 wires:
- 2-wire PT100:
  - Wire 1: Connect to one side of the Wheatstone bridge
  - Wire 2: Connect to the other side of the Wheatstone bridge

- 3-wire PT100 (recommended):
  - Wire 1: Connect to one side of the Wheatstone bridge
  - Wire 2: Connect to the other side of the Wheatstone bridge
  - Wire 3: Connect to the same side as Wire 1 (for lead resistance compensation)

## Installation Instructions

1. Hardware Setup:
   - Connect ADS1115 to NodeMCU:
     - VDD -> 3.3V
     - GND -> GND
     - SCL -> D1 (GPIO5)
     - SDA -> D2 (GPIO4)
   - Connect each PT100 sensor to its own Wheatstone bridge circuit
   - Connect each bridge output to the corresponding ADC input (A0, A1, A2)

2. Software Setup:
   - Install Arduino IDE
   - Add ESP8266 board support:
     1. Open Arduino IDE
     2. Go to File > Preferences
     3. Add `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to Additional Board Manager URLs
     4. Go to Tools > Board > Boards Manager
     5. Search for "esp8266" and install

   - Install Required Libraries:
     1. Go to Tools > Manage Libraries
     2. Search and install:
        - "Adafruit ADS1X15"
        - "ArduinoJson"

3. Configuration:
   - Open `nodemcu_thermal_sensors.ino`
   - Update WiFi credentials:
     ```cpp
     const char* ssid = "YOUR_WIFI_SSID";
     const char* password = "YOUR_WIFI_PASSWORD";
     ```
   - Update server URL:
     ```cpp
     const char* serverUrl = "http://YOUR_SERVER_IP:5000/sensor/data";
     ```
   - Adjust Wheatstone bridge parameters if needed:
     ```cpp
     const float R1 = 100.0;  // Bridge resistor 1 (Ω)
     const float R2 = 100.0;  // Bridge resistor 2 (Ω)
     const float VREF = 3.3;  // Reference voltage (V)
     ```

## Usage

1. Upload the code:
   - Connect NodeMCU to computer via USB
   - Select correct board: Tools > Board > ESP8266 Boards > NodeMCU 1.0
   - Select correct port: Tools > Port > (Select your USB port)
   - Click Upload button

2. Monitor:
   - Open Serial Monitor (Tools > Serial Monitor)
   - Set baud rate to 115200
   - You should see:
     - WiFi connection status
     - ADC initialization
     - Temperature readings
     - Server communication status

## Troubleshooting

1. No temperature readings:
   - Check Wheatstone bridge connections
   - Verify PT100 sensor connections
   - Check ADC module connections
   - Verify power supply voltage

2. Inaccurate readings:
   - Calibrate Wheatstone bridge resistors
   - Check for loose connections
   - Verify PT100 sensor type (2-wire vs 3-wire)
   - Consider lead resistance compensation

3. WiFi connection issues:
   - Verify WiFi credentials
   - Ensure NodeMCU is within range of WiFi network
   - Check if network is 2.4GHz (NodeMCU doesn't support 5GHz)

4. Server communication errors:
   - Verify server URL is correct
   - Ensure server is running and accessible
   - Check if server accepts POST requests

## Data Format

The device sends JSON data to the server every 30 seconds:
```json
{
  "temperature_probe1": 25.6,
  "temperature_probe2": 26.1,
  "temperature_probe3": 24.8,
  "timestamp": 1234567890
}
```

## Power Considerations

- USB power is sufficient for testing
- For permanent installation:
  - Use a reliable 5V power supply
  - Consider adding a capacitor (100µF) between VDD and GND for stability
  - Use appropriate cable gauge for longer wire runs
  - Consider weather protection for outdoor installations
  - Use shielded cables for PT100 sensors to reduce noise 