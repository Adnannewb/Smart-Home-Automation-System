# ESP32 Smart Home Automation System

This project is an ESP32-based smart automation system that integrates multiple sensors and actuators to automate home utilities and provide security alerts.

## Features

- Automatic lighting control using LDR and PIR motion sensor
- Temperature-based fan control using DHT11 sensor
- Automatic water pump control using ultrasonic water level detection
- Vibration detection with instant Telegram alert
- WiFi-enabled system with secure Telegram notification support
- Blynk web and mobile Dashboard

## Components Used

- ESP32
- LDR (Light Dependent Resistor)
- PIR Motion Sensor
- DHT11 Temperature Sensor
- Ultrasonic Sensor (HC-SR04)
- Vibration Sensor(SW-420)
- 5V Relay Module
- WiFi Network
- Telegram Bot

## Working Logic

- Lights turn ON only when motion is detected and ambient light is low
- Fan operates based on temperature when motion is present
- Water pump automatically turns ON/OFF based on tank water level
- Any vibration triggers an instant Telegram alert
- For vibration sensor , we automate our phone to use its camera . It captures the photo when vibration detected and then send it to the given Gmail .It is a cheap replacement of CAM module . 
- System operates continuously with real-time sensor monitoring


## How to Use

1. Open the `.ino` file in Arduino IDE
2. Select ESP32 board and correct COM port
3. Update WiFi credentials and Telegram bot details if needed
4. Upload the code to ESP32
5. Monitor output via Serial Monitor

## Extras
We also use  Blynk dashboard to control the whole system manually and automatically . There is two mode in the system ,One is manual and automatic .In Automatic , all will be work automatically . And in manual , Light , fan ,and pump can be turned off/on manually using switch on Blynk dashboard .

- There will be project presentation video in that repo, that will give you demo of the project , which concept and editing is also done by me. Rather than that ,I mostly handle the whole project ,wiring and created the blynk dashboard fully and connect with it.   
