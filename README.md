# Smart Room Environmental Controller

## Project Description

The Smart Room Environmental Controller is an Arduino-based system designed to monitor environmental conditions and automatically respond to changes in temperature and room brightness.

The system uses an Arduino UNO R4 WiFi to collect data from a temperature sensor and light sensor. Based on these readings, the Arduino automatically controls an LED that represents room lighting and a DC motor with a fan that represents a cooling system. A 16x2 LCD displays the current temperature and the status of the fan and lighting system.

I built this project to gain hands-on experience with electrical engineering concepts, including sensors, breadboard circuits, analog and digital signals, motor control, automated control logic, testing, and debugging.

The project demonstrates how environmental data can be used to automatically control devices without requiring constant user input.


## Features

- Real-time temperature monitoring
- Real-time light-level monitoring
- Automatic room lighting based on ambient light
- Automatic cooling fan based on temperature
- 16x2 LCD status display
- Serial Monitor output for sensor readings and system status
- Adjustable temperature and light thresholds
- Sensor testing and calibration


## Hardware Used

- Arduino UNO R4 WiFi
- Arduino Starter Kit R4 breadboard
- Temperature sensor
- Phototransistor/light sensor
- LED
- Resistors
- DC motor
- Fan blade
- MOSFET/transistor for motor control
- Protection diode
- 16x2 LCD
- Jumper wires
- 9V battery and battery connector for the motor circuit
- USB-C cable


## System Architecture

The system uses two environmental sensors as inputs. The Arduino processes their readings and automatically controls the appropriate outputs.

```text
Temperature Sensor ───────┐
                          │
                          ▼
                  ┌─────────────────┐
Light Sensor ────►│ Arduino UNO R4  │
                  │      WiFi       │
                  └────────┬────────┘
                           │
                ┌──────────┼──────────┐
                │          │          │
                ▼          ▼          ▼
               LED        Fan        LCD
          Room Lighting  Cooling    Status
                           System    Display
