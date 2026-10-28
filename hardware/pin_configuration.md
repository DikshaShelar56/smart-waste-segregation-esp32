# Hardware Pin Configuration

## ESP32 Pin Mapping

The following pin configuration is used in the final Smart Waste Segregation System.

| Hardware Component | Component Pin | ESP32 / Power Connection |
|---|---|---|
| Moisture Sensor | AO | GPIO 34 |
| Moisture Sensor | VCC | 3.3V |
| Moisture Sensor | GND | GND |
| IR Sensor | DO | GPIO 27 |
| IR Sensor | VCC | 3.3V |
| IR Sensor | GND | GND |
| SG90 Servo | Signal | GPIO 13 |
| SG90 Servo | VCC | MB102 5V |
| SG90 Servo | GND | Common GND |
| HC-SR04 | TRIG | GPIO 5 |
| HC-SR04 | ECHO | GPIO 18 through voltage divider |
| HC-SR04 | VCC | MB102 5V |
| HC-SR04 | GND | Common GND |

## Pin Summary

```text
ESP32
│
├── GPIO 34  → Moisture Sensor AO
│
├── GPIO 27  → IR Sensor DO
│
├── GPIO 13  → Servo Signal
│
├── GPIO 5   → HC-SR04 TRIG
│
└── GPIO 18  ← HC-SR04 ECHO
               through voltage divider
```

## Power Connections

### ESP32

The ESP32 is powered through USB from the computer.

```text
Computer USB
     ↓
   ESP32
```

### MB102 Power Supply

The MB102 supplies 5V power to the components that require it.

```text
MB102 5V
   ├── HC-SR04 VCC
   └── Servo VCC

MB102 GND
   ├── HC-SR04 GND
   └── Common Ground
```

The ESP32 GND is connected to the same common ground.

## Moisture Sensor

```text
Moisture Sensor       ESP32

VCC  ----------------> 3.3V
GND  ----------------> GND
AO   ----------------> GPIO 34
```

The analog output is read by the ESP32 using `analogRead()`.

## IR Sensor

```text
IR Sensor             ESP32

VCC  ----------------> 3.3V
GND  ----------------> GND
DO   ----------------> GPIO 27
```

The IR sensor is configured as an active-low detection input:

```text
LOW  → Object detected
HIGH → No object detected
```

## SG90 Servo

```text
SG90 Servo            Connection

Signal  -------------> GPIO 13
VCC     -------------> MB102 5V
GND     -------------> Common GND
```

Servo positions used by the project:

```text
40°  → Wet compartment
90°  → Center
140° → Dry compartment
```

## HC-SR04

```text
HC-SR04               Connection

VCC   ---------------> MB102 5V
GND   ---------------> Common GND
TRIG  ---------------> ESP32 GPIO 5
ECHO  ---------------> Voltage Divider → GPIO 18
```

## HC-SR04 ECHO Voltage Divider

The ECHO output is reduced before reaching the ESP32.

Current resistor arrangement:

```text
HC-SR04 ECHO
     |
    1kΩ
     |
     +──────────────> GPIO 18
     |
    1kΩ
     |
    1kΩ
     |
    GND
```

The divider is used because the HC-SR04 is powered from 5V while the ESP32 uses 3.3V GPIO logic.

## Common Ground

All components must share a common ground.

```text
ESP32 GND
    │
    ├── Moisture Sensor GND
    ├── IR Sensor GND
    ├── Servo GND
    ├── HC-SR04 GND
    └── MB102 GND
```

## Complete Connection Overview

```text
                         ┌─────────────────────┐
                         │        ESP32        │
                         │                     │
 Moisture AO ───────────►│ GPIO 34             │
 IR DO ─────────────────►│ GPIO 27             │
 Servo Signal ──────────►│ GPIO 13             │
 HC-SR04 TRIG ──────────►│ GPIO 5              │
 HC-SR04 ECHO ──────────►│ GPIO 18             │
                         │                     │
                         │ GND ────────────────┼──── Common GND
                         └─────────────────────┘
                                  │
                                  │ USB Power
                                  ▼
                              Computer


        ┌──────────────────── MB102 ────────────────────┐
        │                                                │
        │ 5V ────────────────┬───────────────┐           │
        │                    │               │           │
        │                    ▼               ▼           │
        │                 HC-SR04          Servo         │
        │                                                │
        │ GND ──────────────────────────────────────────┼── Common GND
        └────────────────────────────────────────────────┘
```

## Functional Pin Mapping

| GPIO | Function |
|---:|---|
| GPIO 34 | Moisture sensor analog reading |
| GPIO 27 | IR waste detection |
| GPIO 13 | Servo control |
| GPIO 5 | Ultrasonic trigger |
| GPIO 18 | Ultrasonic echo |

## Safety / Wiring Notes

- Keep the ESP32 and MB102 grounds connected together.
- Power the servo from the external 5V supply rather than an ESP32 GPIO.
- Do not connect the HC-SR04 ECHO directly to the ESP32 when using the sensor at 5V.
- Use the voltage divider on the ECHO line.
- Keep the moisture sensor and its wiring protected from excessive liquid exposure.
- Verify all connections before powering the system.