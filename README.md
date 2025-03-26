# Thermal Sensor Dashboard Project

A real-time temperature monitoring system using PT100 sensors, NodeMCU, and a web dashboard.

## Project Structure

```
.
├── backend/          # Flask application
│   ├── app.py        # Main application
│   ├── requirements.txt
│   ├── README.md
│   └── templates/    # HTML templates
│       └── index.html
│
└── nodemcu/          # NodeMCU firmware
    ├── nodemcu_thermal_sensors.ino
    └── README.md
```

## Features

- Real-time temperature monitoring from 3 PT100 sensors
- Web-based dashboard with live updates
- Temperature threshold alerts
- Historical data tracking
- Mobile-responsive design

## Quick Start

1. Set up the NodeMCU:
   - Follow instructions in `nodemcu/README.md`
   - Upload firmware to NodeMCU
   - Configure WiFi credentials

2. Start the application:
   ```bash
   cd backend
   pip install -r requirements.txt
   python app.py
   ```

3. Access the dashboard:
   - Open browser and navigate to `http://localhost:8000`
   - Configure temperature thresholds
   - Monitor sensor data

## Hardware Requirements

- NodeMCU ESP8266
- 3x PT100 temperature sensors
- ADS1115 16-bit ADC module
- Wheatstone bridge components
- Power supply

## Software Requirements

- Python 3.7+
- Arduino IDE
- Required Python packages (see `backend/requirements.txt`)
- Required Arduino libraries (see `nodemcu/README.md`)

## Documentation

- [NodeMCU Setup Guide](nodemcu/README.md)
- [Application Documentation](backend/README.md)

## License

MIT License 