# Hardware Setup

## Overview

The hardware section contains the components and wiring information required for the IoT-based Smart Waste Segregation System.

The system uses an ESP32 as the main controller. An IR sensor detects the presence of waste, the moisture sensor identifies whether the waste is wet or dry, and a servo motor moves the sorting flap toward the appropriate compartment. An HC-SR04 ultrasonic sensor is used to monitor the fill level of the bin.

## Main Hardware Components

| Component | Purpose |
|---|---|
| ESP32 Dev Module | Main microcontroller and system controller |
| FC-28 / YL-69 Moisture Sensor | Detects moisture level of waste |
| IR Obstacle Sensor | Detects the presence of incoming waste |
| SG90 Servo Motor | Moves the sorting flap |
| HC-SR04 Ultrasonic Sensor | Measures the distance to the waste and monitors bin fill level |
| MB102 Breadboard Power Supply | Provides 5V power for the servo and ultrasonic sensor |
| Breadboard | Used for temporary circuit connections |
| Jumper Wires | Used for electrical connections |
| 1kΩ Resistors | Used to create a voltage divider for the HC-SR04 ECHO signal |

## Sensor Roles

### IR Sensor

The IR sensor is used to detect when a waste item is placed into the system.

- `LOW` → Waste/object detected
- `HIGH` → No waste detected

The ESP32 uses this signal to start the waste classification process.

### Moisture Sensor

The moisture sensor is used to classify the waste.

The sensor's analog output is connected to GPIO 34 of the ESP32.

Based on the current calibration:

- Lower moisture reading → WET
- Higher moisture reading → DRY

The classification threshold used in the current implementation is defined in the ESP32 firmware.

### Servo Motor

The SG90 servo controls the sorting flap.

Current physical positions:

- `40°` → WET compartment
- `90°` → CENTER
- `140°` → DRY compartment

### HC-SR04 Ultrasonic Sensor

The ultrasonic sensor is used for bin-level monitoring.

It measures the distance between the sensor and the waste surface. A smaller distance indicates that the waste level is higher.

The ultrasonic sensor does **not** determine whether waste is wet or dry.

## Pin Configuration

| Component | Pin | ESP32 Connection |
|---|---|---|
| Moisture Sensor | AO | GPIO 34 |
| Moisture Sensor | VCC | 3.3V |
| Moisture Sensor | GND | GND |
| IR Sensor | DO | GPIO 27 |
| IR Sensor | VCC | 3.3V |
| IR Sensor | GND | GND |
| Servo | Signal | GPIO 13 |
| Servo | VCC | MB102 5V |
| Servo | GND | Common GND |
| HC-SR04 | TRIG | GPIO 5 |
| HC-SR04 | ECHO | GPIO 18 through voltage divider |
| HC-SR04 | VCC | MB102 5V |
| HC-SR04 | GND | Common GND |

## HC-SR04 Voltage Divider

The HC-SR04 operates with a 5V supply and its ECHO output can be approximately 5V. Since the ESP32 GPIO is a 3.3V logic input, a resistor voltage divider is used before connecting ECHO to GPIO 18.

Three 1kΩ resistors are used in the current setup.

The divider arrangement provides approximately:

```text
HC-SR04 ECHO
     |
    1kΩ
     |
     +------> ESP32 GPIO 18
     |
    1kΩ
     |
    1kΩ
     |
    GND
```

This reduces the ECHO voltage to a safer level for the ESP32 input.

## Power Configuration

The ESP32 is powered through USB from the computer.

The MB102 power supply provides 5V power to:

- SG90 servo motor
- HC-SR04 ultrasonic sensor

The grounds of the ESP32 and MB102 are connected together to provide a common ground.

### Important

The servo should not be powered directly from an ESP32 GPIO pin.

The HC-SR04 ECHO signal should not be connected directly to an ESP32 GPIO when the sensor is operating at 5V. The voltage divider is used for this purpose.

## Hardware Working Flow

```text
Waste
  ↓
IR Sensor
  ↓
Waste Detected
  ↓
Moisture Sensor
  ↓
ESP32 Classification
  ↓
 ┌───────────────┐
 │               │
WET             DRY
 │               │
 ↓               ↓
Servo 40°      Servo 140°
 │               │
 ↓               ↓
Wet Bin        Dry Bin
```

The ultrasonic sensor operates separately to monitor the distance inside the bin.

## Physical Prototype

The prototype consists of a two-compartment waste bin with the moisture sensor positioned in the waste detection area and a servo-controlled flap used to direct waste into the corresponding compartment.

The hardware setup is designed as a prototype and can be further improved with a permanent enclosure, protected sensor mounting, and a dedicated power supply arrangement.