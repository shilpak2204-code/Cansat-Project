CanSat Project

Title: Atmospheric Data Acquisition and Telemetry CanSat

## Overview

This project presents a low-cost CanSat system designed to collect real-time atmospheric data during flight and transmit it wirelessly to a ground station. The system uses ESP32-based transmitter and receiver modules with NRF24L01 communication to monitor environmental conditions and flight parameters.

## Features

- Real-time measurement of atmospheric parameters:

  - Temperature
  - Humidity
  - Pressure
  - Altitude
  - Acceleration (X, Y, Z)
- Wireless data transmission using NRF24L01 modules.
- Live sensor data displayed on an OLED display at the ground station.
- Sensor data is also transmitted to a mobile phone using Serial Bluetooth for wireless monitoring.
- Integration with the ThingSpeak cloud platform for real-time data logging and visualization.
- Tilt detection using the accelerometer; an LED blinks whenever excessive tilt or turbulence is detected.
- GPS-based live location tracking. The receiver sends the CanSat's live location to a mobile device, enabling real-time tracking during the mission.
- Reliable telemetry communication between the CanSat transmitter and the ground station receiver.

## Hardware Components

- ESP32 Development Board
- NRF24L01 Wireless Transceiver
- BMP280 Pressure and Altitude Sensor
- DHT22 Temperature and Humidity Sensor
- ADXL345 Accelerometer
- GPS Module (Neo-6M)
- OLED Display
- Bluetooth Module (Serial Bluetooth)
- LED Indicator
- Lithium-ion Battery

## Software and Technologies

- Arduino IDE
- Embedded C/C++
- Git & GitHub
- ThingSpeak Cloud Platform
- Serial Bluetooth Communication

## Working Principle

The transmitter continuously collects atmospheric data from the onboard sensors and sends it wirelessly through the NRF24L01 module. The receiver processes the received data and displays it on the OLED screen while simultaneously transmitting the sensor readings to a mobile phone via Serial Bluetooth. The collected data is uploaded to the ThingSpeak cloud platform for remote monitoring and analysis. If the accelerometer detects excessive tilt or turbulence, the receiver activates an LED as a visual alert. The GPS module provides the CanSat's location, and the receiver sends the live location to the mobile device for real-time tracking throughout the mission.

## Applications

- Atmospheric Monitoring
- Environmental Data Collection
- Educational CanSat Missions
- IoT-Based Telemetry Systems
- Remote Sensing and Research
- Student Aerospace Projects

## Future Enhancements

- Automatic parachute deployment mechanism.
- SD card data logging.
- Battery health monitoring.
- Web dashboard for live telemetry.
- Enhanced telemetry range and communication reliability.


Project Team
- Bindu A R
- K Shilpa
- Leekshitha N

License
This project is for educational purposes.