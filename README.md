# Real-Time Air Quality Monitoring with ESP32 and OLED

A compact and interactive IoT project using ESP32 to monitor real-time air quality, temperature, and humidity. The system provides visual and auditory feedback with an OLED display, LEDs, and a buzzer.

## 🚀 Features
- **Real-Time Air Quality Monitoring**: Displays air quality levels (Good, Moderate, Poor) with corresponding LED indicators.
- **Real-Time Temperature and Humidity Monitoring**: Captures and displays real-time data from a DHT22 sensor.
- **OLED Graphical Display**: View live data and historical air quality trends on a 128x64 OLED display.
- **Auditory Feedback**:
  - Tik-Tok sound for good air quality.
  - Beeping alert for poor air quality.
- **Persistent Data Storage**: Air quality history is stored in EEPROM for visualization.

## 🛠️ Components Used
- **ESP32** Microcontroller
- **DHT22** Temperature and Humidity Sensor
- **128x64 OLED Display** (I2C)
- **LEDs** (Green, Yellow, Red)
- **Buzzer**
- Push Button

## ⚙️ How It Works
1. **Air Quality Measurement**: Simulated air quality levels are evaluated and categorized as Good, Moderate, or Poor.
2. **Temperature & Humidity Monitoring**: Data is fetched from the DHT22 sensor.
3. **Graph Display**: Historical air quality data is displayed on the OLED screen.
4. **Auditory Alerts**:
   - Tik-Tok tone for Good air quality.
   - Continuous beep for Poor air quality.


## 🛠️ Setup Instructions
1. Connect components as per the circuit diagram provided.
2. Upload the code to the ESP32 using Arduino IDE.
3. Ensure you have the necessary libraries installed:
   - Adafruit_SSD1306
   - DHT
   - EEPROM

## 🖥️ Wokwi Simulation : https://wokwi.com/projects/417786259029098497
