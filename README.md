# Smart Home Interface System

## Overview
The **Smart Home Interface System** is a proof-of-concept project designed to automate and integrate various household devices through a web-based interface. This project allows users to control and monitor home appliances remotely via an ESP8266-based web server.

## Features
- **Remote Control via Web Interface**: Turn devices on/off, adjust settings, and monitor status.
- **Multi-Device Integration**: Compatible with various smart devices and sensors.
- **Custom Hardware Setup**: Uses ESP8266, Arduino, motors, relays, and sensors.
- **Energy-Efficient Design**: Optimized power consumption with external battery support.
- **Scalability**: Can be expanded with additional devices and modules.

## System Architecture
- **ESP8266**: Hosts the web server and manages communication.
- **Arduino UNO**: Controls DC motors and acts as a power distributor.
- **Sensors & Actuators**:
  - Temperature sensor (for monitoring room temperature).
  - Servo motor (for door simulation).
  - DC motor (for fan simulation).
  - LED (for lighting simulation).
- **Power Supply**:
  - 9V battery module for additional power.
  - Breadboard-based connections.

## Setup Instructions
### Hardware Requirements
- ESP8266 board
- Arduino UNO Rev3
- Servo motor
- DC motor
- Temperature sensor
- LCD display
- LED
- Relay module
- 9V battery
- Breadboard and jumper wires

### Software Requirements
- **Arduino IDE** (for programming ESP8266 and Arduino)
- **ESP8266WiFi** and **WebServer** libraries
- **Temperature sensor libraries** (depending on sensor model)

## Usage
1. **Turn on the system** and open the web interface.
2. **Monitor temperature** displayed on the LCD screen.
3. **Control devices** via the interface:
   - Toggle LED for lighting simulation.
   - Open/close the servo motor-based door.
   - Start/stop the DC motor (fan simulation).
4. **Expand the system** by adding new components to the ESP8266 setup.

## Challenges & Limitations
- **Latency Issues**: Delay in web interface response (~10-20 seconds).
- **Power Management**: Initial underestimation of current requirements led to additional battery integration.
- **Communication between ESP8266 & Arduino**: Required workaround with relays.
- **Hardware Constraints**: Limited GPIO pins necessitated additional circuit optimizations.

## Future Improvements
- Implementing **MQTT protocol** for faster communication.
- Reducing latency by **optimizing ESP8266's request handling**.
- Expanding compatibility with **IoT ecosystems** like Home Assistant.
- Adding **voice control integration** (Google Assistant, Alexa).

## Contributors
- **Stefan-Cristian Marcu**  -- https://github.com/Stefan2810
- **Sorin-Ioan-Alexandru Bîrchi**  -- https://github.com/SorinAlexB
- **Silvia-Georgiana Dumitrache**  -- https://github.com/silviadum


