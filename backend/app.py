# Copyright (c) 2024 AkelMaister
# This software is licensed under the MIT License - see the LICENSE file for details

from fastapi import FastAPI, HTTPException
from fastapi.templating import Jinja2Templates
from fastapi.responses import HTMLResponse
from fastapi.staticfiles import StaticFiles
from fastapi import Request
from pydantic import BaseModel
from typing import List
from datetime import datetime
import os

app = FastAPI(title="Thermal Sensor API")

# Setup templates
templates = Jinja2Templates(directory="templates")

# In-memory storage for sensor data
sensor_data = []

class SensorReading(BaseModel):
    temperature_probe1: float
    temperature_probe2: float
    temperature_probe3: float

class SensorReadingWithTimestamp(SensorReading):
    timestamp: datetime

@app.post("/sensor/data", response_model=SensorReadingWithTimestamp)
async def collect_sensor_data(reading: SensorReading):
    """
    Endpoint to collect thermal sensor data from three probes
    """
    # Add timestamp to the reading
    reading_with_timestamp = SensorReadingWithTimestamp(
        temperature_probe1=reading.temperature_probe1,
        temperature_probe2=reading.temperature_probe2,
        temperature_probe3=reading.temperature_probe3,
        timestamp=datetime.now()
    )
    
    # Store the reading
    sensor_data.append(reading_with_timestamp)
    
    return reading_with_timestamp

@app.get("/sensor/data", response_model=List[SensorReadingWithTimestamp])
async def get_sensor_data():
    """
    Endpoint to retrieve all collected sensor data
    """
    return sensor_data

@app.get("/", response_class=HTMLResponse)
async def read_root(request: Request):
    """
    Serve the dashboard page
    """
    # Get the latest reading or return None if no data
    latest_data = sensor_data[-1] if sensor_data else None
    
    return templates.TemplateResponse("index.html", {"request": request, "latest_data": latest_data})

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000) 