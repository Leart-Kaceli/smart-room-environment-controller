# Smart Room Environmental Controller — System Design

## 1. System Overview

The Smart Room Environmental Controller is an Arduino-based prototype that monitors environmental conditions and automatically controls devices based on sensor readings.

The system is built around an Arduino UNO R4 WiFi and uses two main environmental inputs:

- Temperature
- Ambient light level

The Arduino continuously reads these sensors and uses predefined thresholds to determine whether the room lighting and cooling fan should be activated.

The system has three main outputs:

- LED representing automatic room lighting
- DC motor with fan representing an automatic cooling system
- 16x2 LCD displaying current system information

The purpose of the design is to demonstrate how sensors, control logic, and physical outputs can be integrated into an automated embedded system.


---

## 2. System Architecture

The overall system can be represented as:

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
                │          │          │
                ▼          ▼          ▼
            Automatic   Automatic    System
             Lighting    Cooling     Status
```

The system follows the general process:

```text
Environmental Conditions
          ↓
       Sensors
          ↓
    Arduino Inputs
          ↓
    Sensor Processing
          ↓
    Control Decisions
          ↓
   Arduino Outputs
          ↓
 LED / Fan / LCD
```


---

## 3. Inputs

### Temperature Sensor

The temperature sensor is connected to analog input:

`A0`

The Arduino reads the analog voltage produced by the temperature sensor and converts the reading into a temperature value.

The program first obtains the raw analog sensor value.

The reading is then converted into voltage:

```cpp
float voltage = (tempSensorValue / 1024.0) * 5.0;
```

The voltage is converted into degrees Celsius:

```cpp
float rawTempC = (voltage - 0.5) * 100;
```

The Celsius value is then converted into Fahrenheit:

```cpp
float rawTempF = (rawTempC * 9.0 / 5.0) + 32.0;
```

A calibration offset is currently applied:

```cpp
tempF = rawTempF - 10.0;
```

This offset was added after comparing the sensor output with a reference temperature and observing that the sensor reading was approximately 10°F higher.

The resulting `tempF` value is used by the automatic fan-control system.


### Light Sensor

The light sensor is connected to analog input:

`A1`

The Arduino reads the sensor using:

```cpp
analogRead(lightSensorPin);
```

This produces a numerical value representing the amount of light detected by the sensor.

The program compares this reading with a predefined light threshold to determine whether the room should be considered dark.


---

## 4. Processing

The Arduino UNO R4 WiFi acts as the central controller of the system.

During each program cycle, the Arduino performs five main operations:

```text
1. Read Sensors
        ↓
2. Control Lighting
        ↓
3. Control Fan
        ↓
4. Report Status Through Serial Monitor
        ↓
5. Update LCD
        ↓
Repeat
```

These operations are separated into functions in the Arduino program:

```cpp
readSensors();
controlLighting();
controlFan();
printStatus();
updateLCD();
```

Using separate functions makes the program easier to read, debug, modify, and expand.


---

## 5. Automatic Lighting Control

The automatic lighting system uses the light sensor connected to A1.

The current light threshold is:

```cpp
const int lightThreshold = 260;
```

The program determines whether the room is dark using:

```cpp
roomIsDark = (lightSensorValue < lightThreshold);
```

The LED is then controlled with:

```cpp
digitalWrite(LEDPin, roomIsDark ? HIGH : LOW);
```

The control logic is:

```text
Read Light Sensor
        ↓
Is reading below 260?
      /        \
    YES         NO
     ↓           ↓
 Room Dark    Room Bright
     ↓           ↓
 LED ON       LED OFF
```

The LED represents a room lighting system.

This allows the Arduino to respond automatically to changes in ambient light without requiring user input.


---

## 6. Automatic Fan Control

The fan system uses the temperature measurement from the sensor connected to A0.

The current fan threshold is:

```cpp
const float fanThresholdF = 65.0;
```

The program determines whether the room is above the selected threshold using:

```cpp
roomIsHot = (tempF > fanThresholdF);
```

The motor-control output is then set using:

```cpp
digitalWrite(motorPin, roomIsHot ? HIGH : LOW);
```

The control logic is:

```text
Read Temperature
        ↓
Convert Reading to °F
        ↓
Is temperature above 65°F?
       /          \
     YES           NO
      ↓             ↓
   Fan ON        Fan OFF
```

The motor represents an automatic cooling fan.

The Arduino digital output is used as the control signal for the motor-control circuit rather than directly powering the motor.


---

## 7. Motor-Control Circuit

The DC motor requires more power than should be supplied directly through an Arduino digital I/O pin.

For this reason, a separate motor-control circuit is used.

The circuit includes:

- DC motor
- Fan blade
- Motor-control transistor/MOSFET
- Protection diode
- External motor power source
- Arduino control signal

Arduino pin D6 provides the control signal.

Conceptually, the system works as:

```text
Arduino D6
    │
    ▼
Motor-Control
Transistor/MOSFET
    │
    ▼
DC Motor + Fan
```

The transistor/MOSFET allows the Arduino's low-power digital control signal to control the higher-current motor circuit.

The protection diode helps protect the circuit from voltage generated by the motor when its magnetic field collapses as the motor is switched off.


---

## 8. LCD Status Display

A 16x2 LCD is used to display system information.

The LCD operates in 4-bit mode.

This means four LCD data lines are used:

- D4
- D5
- D6
- D7

The LCD also uses:

- RS
- Enable

The LCD is initialized using the Arduino `LiquidCrystal` library:

```cpp
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 9, 2, 3, 4, 5);
```

The constructor follows:

```text
LiquidCrystal(RS, E, D4, D5, D6, D7)
```

Therefore:

```text
LCD RS → Arduino D7
LCD E  → Arduino D9
LCD D4 → Arduino D2
LCD D5 → Arduino D3
LCD D6 → Arduino D4
LCD D7 → Arduino D5
```

The LCD displays information such as:

```text
Temp: 72.4F
Fan:OFF Lt:ON
```

This allows the current system status to be viewed without relying entirely on the Arduino IDE Serial Monitor.


---

## 9. Pin Assignments

The current Arduino pin assignments are:

| Component | Arduino Pin | Type |
|---|---|---|
| Temperature Sensor | A0 | Analog Input |
| Light Sensor | A1 | Analog Input |
| LCD D4 | D2 | Digital Output |
| LCD D5 | D3 | Digital Output |
| LCD D6 | D4 | Digital Output |
| LCD D7 | D5 | Digital Output |
| Motor Control | D6 | Digital Output |
| LCD RS | D7 | Digital Output |
| LED | D8 | Digital Output |
| LCD Enable | D9 | Digital Output |

This pin arrangement allows all major components to operate without sharing Arduino signal pins.


---

## 10. Current Thresholds

The current control values are:

| Parameter | Current Value |
|---|---:|
| Light Threshold | 260 |
| Fan Temperature Threshold | 65°F |
| Temperature Calibration Offset | -10°F |

These values are defined near the beginning of the program so they can easily be modified during future calibration and testing.


---

## 11. Serial Monitor

The Serial Monitor is used as a debugging and testing interface.

The program reports:

- Temperature in Fahrenheit
- Light sensor reading
- Room-light status
- Fan status

Example:

```text
Temperature in Fahrenheit: 72.4
Light Level: 235
Room Light: ON
Fan: ON
```

The Serial Monitor was especially useful during development because it allowed sensor readings and control decisions to be observed directly.

This made it easier to determine whether a problem originated from the sensor readings, software logic, or physical output circuit.


---

## 12. Software Structure

The main Arduino program is divided into several functions.

### `setup()`

The setup function:

- Initializes the LCD
- Starts Serial communication
- Configures the LED as an output
- Configures the motor-control pin as an output
- Displays the startup message


### `loop()`

The main loop repeatedly calls:

```cpp
readSensors();
controlLighting();
controlFan();
printStatus();
updateLCD();
```

A short delay is included before the process repeats.


### `readSensors()`

This function:

- Reads the light sensor
- Reads the temperature sensor
- Converts the temperature sensor reading
- Applies the temperature calibration offset


### `controlLighting()`

This function:

- Compares the light reading with the light threshold
- Determines whether the room is dark
- Turns the LED on or off


### `controlFan()`

This function:

- Compares the measured temperature with the fan threshold
- Determines whether cooling is required
- Turns the motor-control output on or off


### `printStatus()`

This function sends system information to the Serial Monitor for debugging and testing.


### `updateLCD()`

This function displays the current temperature, fan status, and lighting status on the 16x2 LCD.


---

## 13. Design Decisions

### Automatic Control Instead of Manual Control

The final design operates automatically rather than including a manual/automatic mode switch.

The purpose of the project is to demonstrate an environmental control system that responds directly to sensor readings.

Removing the manual-control system also reduced unnecessary hardware and allowed the project to focus on sensor-based automation.


### Separate Sensors for Temperature and Light

Temperature and ambient light are measured independently because they control different outputs.

The temperature sensor controls the fan.

The light sensor controls the LED.

This creates two independent automatic control systems running simultaneously on one Arduino.


### Separate Motor-Control Circuit

The motor is not driven directly from an Arduino digital pin.

Instead, the Arduino provides a control signal to a transistor/MOSFET circuit.

This design allows the microcontroller to control the motor while keeping the motor's power requirements separate from the Arduino's digital output.


### LCD in 4-Bit Mode

The LCD is operated using four data lines instead of all eight available LCD data lines.

This reduces the number of Arduino pins required while still allowing the LCD to display all necessary information.


### Modular Software Design

The program was separated into individual functions rather than placing all logic inside `loop()`.

This makes it easier to:

- Understand the program
- Debug individual systems
- Modify thresholds
- Add future sensors
- Add additional outputs
- Expand the project later


---

## 14. System Flow

The complete system operates according to the following sequence:

```text
START
  │
  ▼
Initialize Arduino, LCD, and outputs
  │
  ▼
Read temperature sensor
  │
  ▼
Read light sensor
  │
  ├─────────────────────────────┐
  │                             │
  ▼                             ▼
Compare temperature         Compare light
with fan threshold          with light threshold
  │                             │
  ▼                             ▼
Control fan                 Control LED
  │                             │
  └──────────────┬──────────────┘
                 │
                 ▼
        Print status to Serial
                 │
                 ▼
            Update LCD
                 │
                 ▼
              Repeat
```


---

## 15. Future Design Improvements

The current design provides the foundation for a larger Smart Room system.

Possible future improvements include:

- WiFi-based remote monitoring
- Web dashboard
- Humidity sensor
- Motion/occupancy detection
- Air-quality monitoring
- Environmental data logging
- Improved temperature calibration
- Automatic blinds using a servo motor
- Additional room-control outputs
- Physical enclosure or model room
- Mobile-friendly monitoring interface

Because the project uses the Arduino UNO R4 WiFi, future versions could also use its networking capabilities to send environmental readings to a web interface.


---

## 16. Conclusion

The Smart Room Environmental Controller combines sensing, processing, and physical control into a single embedded system.

The temperature and light sensors provide environmental inputs to the Arduino UNO R4 WiFi. The Arduino processes these readings and automatically controls an LED and cooling fan while reporting system information through both the Serial Monitor and LCD.

The project demonstrates several fundamental electrical and computer engineering concepts, including:

- Analog sensor measurement
- Digital output control
- Sensor calibration
- Threshold-based automation
- Motor control
- LCD communication
- Breadboard circuit construction
- Hardware/software integration
- Modular programming
- Testing and debugging

The modular design also provides a foundation for expanding the system with additional sensors, wireless communication, data logging, and more advanced automation in future versions.
