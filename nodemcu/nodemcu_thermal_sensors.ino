/*
 * Copyright (c) 2024 AkelMaister
 * This software is licensed under the MIT License - see the LICENSE file for details
 */

#include <Wire.h>
#include <Adafruit_ADS1X15.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Backend server details
const char* serverUrl = "http://YOUR_SERVER_IP:8000/sensor/data";

// Create ADC object
Adafruit_ADS1115 ads;

// PT100 parameters
const float R0 = 100.0;  // PT100 resistance at 0°C
const float A = 3.9083e-3;  // PT100 coefficient A
const float B = -5.775e-7;  // PT100 coefficient B
const float C = -4.183e-12; // PT100 coefficient C

// Wheatstone bridge parameters
const float R1 = 100.0;  // Bridge resistor 1 (Ω)
const float R2 = 100.0;  // Bridge resistor 2 (Ω)
const float VREF = 3.3;  // Reference voltage (V)

// Store sensor locations for identification
const char* sensorLocations[] = {
    "Top Shelf",    // probe1
    "Middle Shelf", // probe2
    "Bottom Shelf"  // probe3
};

// Last time we sent data
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 30000; // 30 seconds

// Function to convert ADC reading to temperature
float adcToTemperature(int16_t adc) {
    // Convert ADC reading to voltage
    float voltage = (adc * VREF) / 32767.0;
    
    // Calculate PT100 resistance using Wheatstone bridge equation
    float Rpt100 = R1 * (1 + voltage/VREF) / (1 - voltage/VREF);
    
    // Convert resistance to temperature using PT100 equation
    float temp = (-A + sqrt(A*A - 4*B*(1 - Rpt100/R0))) / (2*B);
    
    return temp;
}

void setup() {
    Serial.begin(115200);
    
    // Initialize WiFi
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize ADC
    if (!ads.begin()) {
        Serial.println("Failed to initialize ADS1115!");
        while(1);
    }
    
    // Set ADC gain to 1 (for 0-3.3V range)
    ads.setGain(GAIN_ONE);
    
    Serial.println("ADS1115 initialized successfully");
}

void loop() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastSendTime >= sendInterval) {
        // Read ADC values for each sensor
        int16_t adc1 = ads.readADC_SingleEnded(0);  // A0
        int16_t adc2 = ads.readADC_SingleEnded(1);  // A1
        int16_t adc3 = ads.readADC_SingleEnded(2);  // A2
        
        // Convert to temperature
        float temp1 = adcToTemperature(adc1);
        float temp2 = adcToTemperature(adc2);
        float temp3 = adcToTemperature(adc3);
        
        // Print readings to Serial for debugging
        Serial.println("\nCurrent Temperatures:");
        Serial.print(sensorLocations[0]); Serial.print(": "); Serial.print(temp1); Serial.println("°C");
        Serial.print(sensorLocations[1]); Serial.print(": "); Serial.print(temp2); Serial.println("°C");
        Serial.print(sensorLocations[2]); Serial.print(": "); Serial.print(temp3); Serial.println("°C");
        
        // Create JSON payload
        StaticJsonDocument<200> doc;
        doc["temperature_probe1"] = temp1;
        doc["temperature_probe2"] = temp2;
        doc["temperature_probe3"] = temp3;
        doc["timestamp"] = currentTime;
        
        String jsonString;
        serializeJson(doc, jsonString);
        
        // Send data to server
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            WiFiClient client;
            
            http.begin(client, serverUrl);
            http.addHeader("Content-Type", "application/json");
            
            int httpResponseCode = http.POST(jsonString);
            
            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.println("HTTP Response code: " + String(httpResponseCode));
            } else {
                Serial.println("Error on sending POST: " + String(httpResponseCode));
            }
            
            http.end();
        } else {
            Serial.println("WiFi Disconnected");
        }
        
        lastSendTime = currentTime;
    }
    
    delay(1000);
} 