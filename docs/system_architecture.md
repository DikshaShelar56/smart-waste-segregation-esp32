# System Architecture

## 1. Project Overview

The Smart Waste Segregation System is an IoT-based prototype designed to automatically separate waste into wet and dry categories.

The ESP32 acts as the central controller. The system detects the presence of waste using an IR sensor, determines its moisture condition using a moisture sensor, and controls a servo-operated flap to direct the waste into the appropriate compartment.

An HC-SR04 ultrasonic sensor is additionally used to monitor the level of waste inside the bin.

---

## 2. High-Level Architecture

```text
                    ┌─────────────────────┐
                    │      WASTE ITEM     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    IR SENSOR        │
                    │ Waste Detection     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │       ESP32         │
                    │   Main Controller   │
                    └──────────┬──────────┘
                               │
                     ┌─────────┴─────────┐
                     │                   │
                     ▼                   ▼
          ┌──────────────────┐   ┌──────────────────┐
          │ Moisture Sensor  │   │  HC-SR04         │
          │ Wet/Dry Detection│   │ Bin Level        │
          └────────┬─────────┘   │ Monitoring       │
                   │             └──────────────────┘
                   ▼
          ┌──────────────────┐
          │ Classification   │
          │     Logic        │
          └────────┬─────────┘
                   │
                   ▼
          ┌──────────────────┐
          │   Servo Motor    │
          │ Sorting Flap     │
          └────────┬─────────┘
                   │
             ┌─────┴─────┐
             │           │
             ▼           ▼
       ┌──────────┐ ┌──────────┐
       │ WET BIN  │ │ DRY BIN  │
       └──────────┘ └──────────┘
```

---

## 3. Major System Blocks

### 3.1 Input Layer

The input layer consists of sensors that collect information from the physical environment.

It includes:

- IR obstacle sensor
- Moisture sensor
- HC-SR04 ultrasonic sensor

The IR sensor detects the arrival of waste, while the moisture sensor provides the data used for wet/dry classification.

The ultrasonic sensor operates independently for bin-level monitoring.

---

### 3.2 Processing Layer

The ESP32 is responsible for processing the sensor data.

Its main responsibilities are:

1. Detect waste using the IR sensor.
2. Wait for the waste to settle.
3. Read the moisture sensor.
4. Calculate an average moisture value.
5. Compare the value with the configured threshold.
6. Classify the waste.
7. Control the servo motor.
8. Monitor the ultrasonic sensor.
9. Display system information through the Serial Monitor.

---

### 3.3 Actuation Layer

The SG90 servo motor acts as the mechanical actuator.

The servo controls the sorting flap according to the classification.

```text
WET  → 40°
CENTER → 90°
DRY  → 140°
```

The flap physically directs the waste toward the corresponding compartment.

---

### 3.4 Monitoring Layer

The HC-SR04 ultrasonic sensor provides distance measurements.

The measured distance represents the available space between the sensor and the waste surface.

Conceptually:

```text
Larger distance → More empty space
Smaller distance → More waste / higher fill level
```

The current implementation reports this distance through the Serial Monitor.

---

## 4. Data Flow

The complete data flow is:

```text
Waste placed
     ↓
IR detects object
     ↓
ESP32 starts processing
     ↓
Waste settles on moisture sensor
     ↓
10 analog moisture readings collected
     ↓
Average moisture value calculated
     ↓
Threshold comparison
     ↓
Wet / Dry classification
     ↓
Servo moves sorting flap
     ↓
Waste enters appropriate compartment
     ↓
Servo returns to center
     ↓
System waits for waste removal
     ↓
Next waste can be processed
```

At the same time, the ultrasonic sensor periodically measures the bin distance.

---

## 5. Control Logic

The IR sensor uses active-low detection:

```text
IR = LOW
    ↓
Waste detected

IR = HIGH
    ↓
No waste detected
```

After detecting waste, the ESP32 takes multiple moisture readings and calculates their average.

The current firmware uses a moisture threshold to determine the classification.

The servo then moves to the corresponding position.

---

## 6. System Architecture Summary

The system follows a sensor → controller → actuator architecture.

```text
Sensors
   ↓
ESP32
   ↓
Decision / Classification
   ↓
Servo Actuator
   ↓
Physical Waste Segregation
```

The ultrasonic sensor adds an additional monitoring function for the bin's fill level.

This architecture allows the project to combine sensing, decision-making, actuation, and monitoring within a single ESP32-based system.