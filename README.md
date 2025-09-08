# IoT Smart Plant Monitoring and Irrigation System

An IoT-based project for automated plant monitoring and irrigation.  
The system uses a soil moisture sensor and an ESP8266 microcontroller to monitor soil conditions and control a water pump. Sensor readings and pump status are logged to Firebase, enabling real-time monitoring and threshold adjustments via a mobile application.

---

## Project Info
**Date:** April 2025  

---

## Features

### Soil Monitoring
- Reads soil moisture values from an analog sensor.  
- Sends live sensor data to Firebase in real-time.  

### Automated Irrigation
- Water pump connected to ESP8266 (GPIO D1).  
- Pump automatically turns ON/OFF based on moisture threshold.  
- Moisture threshold can be updated remotely via Firebase.  

### Cloud Communication
- ESP8266 connects to Firebase Realtime Database over Wi-Fi.  
- Stores moisture values and pump status for cloud-based access.  

### Mobile Application 
- Displays real-time soil moisture values and pump status.  
- Allows remote threshold adjustment for irrigation control.  

---

## Tech Stack
- **Hardware:** ESP8266, Soil Moisture Sensor, Water Pump  
- **Cloud:** Firebase Realtime Database  
- **Domain:** IoT, Smart Agriculture  
