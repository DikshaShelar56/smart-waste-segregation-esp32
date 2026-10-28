# Hardware Components

## 1. ESP32 Dev Module

### Purpose

The ESP32 is the main controller of the Smart Waste Segregation System.

It receives data from the sensors, processes the readings, classifies the waste, and controls the servo motor.

### Main Responsibilities

- Read the IR sensor.
- Read the moisture sensor.
- Process sensor readings.
- Classify waste as wet or dry.
- Control the SG90 servo.
- Read the HC-SR04 ultrasonic sensor.
- Display system status through the Serial Monitor.

### Connections

```text
GPIO 34 → Moisture Sensor AO
GPIO 27 → IR Sensor DO
GPIO 13 → Servo Signal
GPIO 5  → HC-SR04 TRIG
GPIO 18 → HC-SR04 ECHO through voltage divider
```

---

## 2. FC-28 / YL-69 Moisture Sensor

### Purpose

The moisture sensor is used to determine the moisture condition of the waste.

The analog output is connected to GPIO 34 of the ESP32.

The sensor works based on the electrical characteristics of the material contacting its probes.

### Connection

```text
VCC → ESP32 3.3V
GND → ESP32 GND
AO  → ESP32 GPIO 34
```

### Role in the System

```text
Waste
  ↓
Moisture Sensor
  ↓
Analog Reading
  ↓
ESP32
  ↓
Threshold Comparison
  ↓
WET / DRY
```

The threshold is calibrated experimentally because readings can vary depending on the material, contact with the probes, and physical setup.

---

## 3. IR Obstacle Sensor

### Purpose

The IR sensor detects whether waste has entered the sensing area.

It acts as the trigger for the waste-processing cycle.

### Connection

```text
VCC → ESP32 3.3V
GND → ESP32 GND
DO  → ESP32 GPIO 27
```

### Detection Logic

The sensor is used as an active-low input:

```text
LOW
 ↓
Object detected

HIGH
 ↓
No object detected
```

When the sensor detects an object, the ESP32 starts the moisture measurement and classification process.

---

## 4. SG90 Servo Motor

### Purpose

The SG90 servo is responsible for physically moving the sorting flap.

The servo converts the classification decision made by the ESP32 into mechanical movement.

### Connection

```text
Signal → ESP32 GPIO 13
VCC    → MB102 5V
GND    → Common GND
```

### Positions

| Servo Angle | Function |
|---:|---|
| 40° | Wet compartment |
| 90° | Center / neutral |
| 140° | Dry compartment |

### Operation

```text
ESP32 Classification
       ↓
 ┌─────┴─────┐
 │           │
WET         DRY
 │           │
 ▼           ▼
40°        140°
 │           │
 └─────┬─────┘
       ↓
   Sorting Flap
```

After approximately 2.5 seconds, the servo returns to the center position.

---

## 5. HC-SR04 Ultrasonic Sensor

### Purpose

The HC-SR04 is used for monitoring the approximate fill level of the waste bin.

It measures the distance between the sensor and the surface of the waste.

### Connection

```text
VCC  → MB102 5V
GND  → Common GND
TRIG → ESP32 GPIO 5
ECHO → ESP32 GPIO 18 through voltage divider
```

### Working Principle

The sensor sends an ultrasonic pulse through the TRIG pin.

The ECHO pin receives the reflected signal.

The ESP32 measures the time taken for the echo and calculates the distance.

The firmware uses:

```text
Distance = Duration × 0.0343 / 2
```

The distance is expressed in centimetres.

### Interpretation

```text
Large Distance
      ↓
More empty space

Small Distance
      ↓
More waste / higher fill level
```

The HC-SR04 is **not used for wet/dry classification**.

---

## 6. MB102 Breadboard Power Supply

### Purpose

The MB102 power supply module provides the external 5V supply required by the servo and ultrasonic sensor.

### Connections

```text
MB102 5V
   ├── SG90 Servo VCC
   └── HC-SR04 VCC

MB102 GND
   └── Common Ground
```

The ESP32 itself is powered through USB.

---

## 7. Breadboard

### Purpose

The breadboard provides a convenient platform for connecting the sensors, ESP32, power rails, resistors, and jumper wires during prototyping.

It allows the circuit to be assembled without permanently soldering the components.

---

## 8. Jumper Wires

### Purpose

Jumper wires are used to establish electrical connections between:

- ESP32
- Sensors
- Servo
- MB102
- Breadboard
- Voltage-divider resistors

Different jumper-wire connections are used for power, ground, signals, and sensor outputs.

---

## 9. 1kΩ Resistors

### Purpose

Three 1kΩ resistors are used to create the voltage-divider circuit for the HC-SR04 ECHO signal.

The divider reduces the voltage before the signal reaches ESP32 GPIO 18.

### Arrangement

```text
HC-SR04 ECHO
      │
     1kΩ
      │
      ├────────── GPIO 18
      │
     1kΩ
      │
     1kΩ
      │
     GND
```

This protects the ESP32 input from the higher-voltage ECHO signal.

---

## 10. Two-Compartment Waste Bin

### Purpose

The physical bin provides separate collection areas for wet and dry waste.

The servo-operated flap determines which compartment receives the waste.

```text
             Sorting Flap
                  │
           ┌──────┴──────┐
           │             │
        WET SIDE       DRY SIDE
         40°            140°
```

The physical prototype is constructed using a two-compartment cardboard dustbin.

---

## 11. Component Interaction

The components work together as follows:

```text
              ┌──────────────┐
              │   IR Sensor  │
              │ Waste Detect │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │    ESP32     │
              │  Controller  │
              └──────┬───────┘
                     │
              ┌──────▼───────┐
              │   Moisture   │
              │    Sensor    │
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │ Classification│
              └──────┬───────┘
                     │
              ┌──────▼───────┐
              │ SG90 Servo   │
              │ Sorting Flap │
              └──────┬───────┘
                     │
              ┌──────┴───────┐
              ▼              ▼
         ┌─────────┐    ┌─────────┐
         │ WET BIN │    │ DRY BIN │
         └─────────┘    └─────────┘

              ┌──────────────┐
              │   HC-SR04    │
              │ Bin Monitoring│
              └──────┬───────┘
                     │
                     ▼
                  ESP32
```

---

## 12. Complete Component Summary

| Component | Input / Output | Main Function |
|---|---|---|
| ESP32 | Controller | Processes all sensor data and controls the system |
| FC-28 / YL-69 | Analog input | Moisture sensing |
| IR Sensor | Digital input | Waste detection |
| SG90 Servo | PWM output | Sorting flap movement |
| HC-SR04 | Digital I/O | Bin-level distance measurement |
| MB102 | Power | 5V external supply |
| 1kΩ Resistors | Passive | HC-SR04 ECHO voltage divider |
| Breadboard | Interconnection | Prototype circuit assembly |
| Jumper Wires | Interconnection | Component wiring |
| Two-compartment bin | Mechanical | Separate wet and dry collection |

---

## 13. Overall Role of Each Component

The complete system can be summarized as:

```text
IR Sensor
   ↓
"Is waste present?"

Moisture Sensor
   ↓
"Is the waste wet or dry?"

ESP32
   ↓
"What action should be taken?"

Servo
   ↓
"Move the waste to the correct compartment."

HC-SR04
   ↓
"How much space is left in the bin?"
```

Together, these components form the sensing, processing, actuation, and monitoring layers of the Smart Waste Segregation System.