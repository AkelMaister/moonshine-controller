# Thermal Sensor API

A simple FastAPI application that provides endpoints for collecting and retrieving thermal sensor data from three probes, including a web dashboard for real-time monitoring.

## Setup

1. Create a virtual environment (recommended):
```bash
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate
```

2. Install dependencies:
```bash
pip install -r requirements.txt
```

3. Run the application:
```bash
python main.py
```

The API will be available at `http://localhost:8000`

## Features

### Web Dashboard
Access the real-time dashboard at `http://localhost:8000/` to view the latest sensor readings from all three probes.
The dashboard automatically refreshes every 30 seconds, or you can manually refresh using the refresh button.

### API Endpoints

#### 1. POST /sensor/data
Collects new sensor readings from three temperature probes.

Request body:
```json
{
    "temperature_probe1": 25.5,
    "temperature_probe2": 26.2,
    "temperature_probe3": 24.8
}
```

You can test this endpoint using curl:
```bash
curl -X POST http://localhost:8000/sensor/data \
     -H "Content-Type: application/json" \
     -d "{\"temperature_probe1\": 25.5, \"temperature_probe2\": 26.2, \"temperature_probe3\": 24.8}"
```

#### 2. GET /sensor/data
Retrieves all collected sensor readings.

You can also test this endpoint using curl:
```bash
curl http://localhost:8000/sensor/data
```

## API Documentation

Once the server is running, you can access:
- Interactive API docs (Swagger UI): `http://localhost:8000/docs`
- Alternative API docs (ReDoc): `http://localhost:8000/redoc` 