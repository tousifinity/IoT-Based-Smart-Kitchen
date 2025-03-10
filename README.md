# IoT Based Smart Kitchen

An ESP8266-based smart kitchen monitoring and automation system with real-time air quality, temperature, humidity, fire detection, and motion sensing, accessible via a web dashboard.

## Features
- **Air Quality Monitoring**: Detects air pollution levels using MQ135 sensor.
- **Temperature & Humidity Monitoring**: Uses DHT11 sensor.
- **Fire Detection**: Alerts and activates a buzzer if a flame is detected.
- **Motion Detection**: PIR sensor to detect human presence.
- **Remote Control**: Control fan and light via an ESP-DASH web interface.
- **OLED Display**: Real-time data visualization.
- **WiFi Access Point**: Connects as an independent WiFi network.

## Hardware Requirements
- ESP8266 (NodeMCU)
- MQ135 Air Quality Sensor
- DHT11 Temperature & Humidity Sensor
- Flame Sensor
- PIR Motion Sensor
- OLED Display (SSD1306)
- Relay Module (for fan and light control)
- Buzzer

## Software Requirements
- Arduino IDE
- ESP8266WiFi Library
- ESPAsyncWebServer Library
- ESPAsyncTCP Library
- ESPDash Library
- Adafruit_Sensor Library
- Adafruit_GFX Library
- Adafruit_SSD1306 Library
- DHT Sensor Library

## Installation & Setup
1. Clone the repository:
   ```sh
   git clone https://github.com/your-username/iot-based-smart-kitchen.git
   cd iot-smart-kitchen
   ```
2. Open the project in Arduino IDE.
3. Install the required libraries (use Arduino Library Manager).
4. Connect the ESP8266 and upload the code.
5. Connect to the WiFi network `IoT Smart Kitchen` with password `1234567890`.
6. Open the browser and navigate to the ESP-DASH web interface.

## Usage
- The system continuously monitors air quality, temperature, humidity, fire, and motion.
- If poor air quality or fire is detected, the buzzer and fan will be activated.
- Users can remotely turn the fan and light on/off through the web dashboard.
- The OLED display provides real-time data updates.


