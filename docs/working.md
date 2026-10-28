# System Working

## 1. Introduction

The Smart Waste Segregation System automatically detects incoming waste, analyzes its moisture level, and directs the waste toward the appropriate wet or dry compartment.

The ESP32 coordinates the complete process by reading sensor inputs, making the classification decision, and controlling the servo motor.

---

## 2. Step-by-Step Working

### Step 1 — System Initialization

When the ESP32 is powered on:

- The serial communication is initialized at 115200 baud.
- Sensor pins are configured.
- The servo motor is attached.
- The servo moves to the center position of 90°.
- The system displays the startup information in the Serial Monitor.

The system then enters the waiting state.

```text
ESP32 ON
   ↓
Initialize Sensors
   ↓
Initialize Servo
   ↓
Servo → 90°
   ↓
System Ready
```

---

### Step 2 — Waste Detection

The IR sensor continuously checks whether a waste item has entered the detection area.

The IR sensor is active-low:

```text
LOW  → Waste detected
HIGH → No waste detected
```

When the IR sensor detects an object, the ESP32 starts the classification process.

---

### Step 3 — Waste Settling

After detecting waste, the ESP32 waits for approximately 1.5 seconds.

This allows the waste to settle onto the moisture sensor so that the moisture reading can be obtained more reliably.

```text
Waste Detected
      ↓
Wait 1.5 seconds
      ↓
Read Moisture
```

---

### Step 4 — Moisture Measurement

The ESP32 reads the analog output of the moisture sensor connected to GPIO 34.

Instead of relying on a single reading, the program takes 10 readings.

Each reading is separated by a short delay.

```text
Reading 1
Reading 2
Reading 3
   .
   .
   .
Reading 10
   ↓
Calculate Average
```

The average is calculated using:

```text
Average = (Reading1 + Reading2 + ... + Reading10) / 10
```

This averaging helps reduce short-term fluctuations in the sensor reading.

---

### Step 5 — Wet/Dry Classification

The calculated average moisture value is compared with the threshold defined in the firmware.

The current firmware contains:

```cpp
#define MOISTURE_THRESHOLD 1000
```

The classification logic is:

```text
Moisture value < threshold
        ↓
      WET

Moisture value ≥ threshold
        ↓
      DRY
```

The threshold can be adjusted after calibration because moisture sensor readings depend on the physical sensor setup and the material being tested.

---

### Step 6 — Servo Movement

Once the classification is completed, the ESP32 controls the SG90 servo motor.

The current physical configuration is:

```text
                 CENTER
                   90°
                    │
             ┌──────┴──────┐
             │             │
           40°           140°
             │             │
            WET           DRY
```

Therefore:

```text
WET → Servo moves to 40°
DRY → Servo moves to 140°
```

The flap remains in the selected position for approximately 2.5 seconds.

---

### Step 7 — Waste Falls Into the Correct Compartment

The servo-operated flap physically directs the waste toward the selected compartment.

```text
              Waste
                ↓
         Moisture Sensor
                ↓
             ESP32
                ↓
        ┌───────┴───────┐
        │               │
       WET             DRY
        ↓               ↓
     40° Servo       140° Servo
        ↓               ↓
    Wet Compartment  Dry Compartment
```

---

### Step 8 — Servo Returns to Center

After the waste has been directed into the selected compartment, the servo returns to 90°.

```text
Selected Position
       ↓
   Wait 2.5 sec
       ↓
Servo → 90°
```

This prepares the flap for the next waste item.

---

### Step 9 — Waste Removal Detection

After processing, the system waits until the waste is no longer detected by the IR sensor.

When the IR sensor changes back to:

```text
HIGH → No object detected
```

the ESP32 resets its processing state.

The system is then ready to process the next waste item.

This prevents the same waste item from being classified repeatedly while it remains in the detection area.

---

## 3. Ultrasonic Bin-Level Monitoring

The HC-SR04 is used separately from the wet/dry classification system.

It measures the distance between the ultrasonic sensor and the waste surface.

The measurement is taken periodically and displayed in the Serial Monitor.

Example:

```text
Bin distance: 65.26 cm
Bin distance: 63.58 cm
Bin distance: 63.97 cm
```

The general relationship is:

```text
Large distance
     ↓
More empty space

Small distance
     ↓
Higher waste level
```

The ultrasonic sensor therefore provides information about how full the bin is.

It does **not** determine whether the waste is wet or dry.

---

## 4. Complete System Flow

```text
                 START
                   │
                   ▼
          Initialize ESP32
                   │
                   ▼
          Initialize Sensors
                   │
                   ▼
           Servo → CENTER
                   │
                   ▼
          Wait for Waste
                   │
                   ▼
            IR Detection
                   │
             Object Found?
              /          \
            NO            YES
            │              │
            │              ▼
            │       Wait 1.5 sec
            │              │
            │              ▼
            │       Read Moisture
            │              │
            │              ▼
            │        Average 10 Reads
            │              │
            │              ▼
            │       Compare Threshold
            │              │
            │        ┌─────┴─────┐
            │        │           │
            │       WET         DRY
            │        │           │
            │        ▼           ▼
            │      Servo 40°   Servo 140°
            │        │           │
            │        └─────┬─────┘
            │              │
            │              ▼
            │        Waste Sorted
            │              │
            │              ▼
            │        Servo → 90°
            │              │
            │              ▼
            │       Wait for removal
            │              │
            └──────────────┘
                   │
                   ▼
             Next Waste
```

The HC-SR04 operates alongside this process to periodically measure bin distance.

---

## 5. Software Processing Sequence

The firmware follows this sequence:

1. Read IR sensor.
2. Detect incoming waste.
3. Prevent duplicate processing.
4. Wait for waste to settle.
5. Read the moisture sensor 10 times.
6. Calculate the average moisture value.
7. Compare the average with the configured threshold.
8. Classify the waste as wet or dry.
9. Move the servo to the corresponding position.
10. Wait for the waste to pass into the selected compartment.
11. Return the servo to the center position.
12. Wait for the IR sensor to indicate that the waste has been removed.
13. Reset the processing state.
14. Continue ultrasonic bin-level monitoring.
15. Wait for the next waste item.

---

## 6. Example Processing

For example, if the measured average moisture value is below the configured threshold:

```text
Average Moisture Value
        ↓
Below Threshold
        ↓
Classification = WET
        ↓
Servo = 40°
        ↓
Waste → Wet Compartment
```

If the measured average is at or above the threshold:

```text
Average Moisture Value
        ↓
At/Above Threshold
        ↓
Classification = DRY
        ↓
Servo = 140°
        ↓
Waste → Dry Compartment
```

The threshold should be calibrated according to the actual sensor readings obtained from the prototype.

---

## 7. System State Control

The variable:

```cpp
bool wasteProcessed = false;
```

is used to prevent the same waste item from being processed repeatedly.

When new waste is detected:

```text
wasteProcessed = false
        ↓
IR detects waste
        ↓
wasteProcessed = true
```

The system then completes the sorting operation.

After the waste leaves the IR detection area:

```text
IR = HIGH
        ↓
wasteProcessed = false
        ↓
Ready for next waste
```

This provides basic state management for the sorting process.

---

## 8. Output and Monitoring

The ESP32 sends useful information to the Arduino IDE Serial Monitor.

The output includes:

- System startup information
- Waste detection status
- Average moisture value
- Wet/dry classification
- Servo movement status
- Waste processing completion
- Waste removal status
- Ultrasonic bin distance

Example:

```text
WASTE DETECTED
Waiting for waste to settle...
Average Moisture Value: 730

Classification: WET
Moving flap toward WET bin...

Returning flap to CENTER...

Waste processing completed.
Remove the waste from the IR detection area.

Bin distance: 65.26 cm
```

---

## 9. Overall Working Summary

The project combines three sensing functions:

```text
IR Sensor
    ↓
Detects incoming waste

Moisture Sensor
    ↓
Classifies wet/dry condition

HC-SR04
    ↓
Monitors bin level
```

The ESP32 processes these inputs and controls the servo:

```text
Sensors
   ↓
ESP32
   ↓
Decision
   ↓
Servo
   ↓
Waste Segregation
```

This creates an automated prototype for separating waste into wet and dry compartments while also providing basic bin-level monitoring.