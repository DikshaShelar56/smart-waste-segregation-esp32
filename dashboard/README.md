# Dashboard and Monitoring

## 1. Overview

The current Smart Waste Segregation System uses the ESP32 Serial Monitor as the primary software monitoring interface.

The Serial Monitor displays sensor readings, classification results, servo actions, and ultrasonic bin-distance measurements during system operation.

A dedicated web or mobile dashboard is not part of the current implemented prototype.

---

## 2. Current Monitoring Interface

The ESP32 sends system information through serial communication at:

```text
115200 baud
```

The Serial Monitor can be opened through the Arduino IDE after uploading the firmware to the ESP32.

---

## 3. Information Displayed

The current monitoring output includes:

- System startup status
- Waste detection status
- Average moisture value
- Wet/dry classification
- Servo movement status
- Waste processing completion
- Waste removal status
- HC-SR04 bin distance

Example:

```text
==========================================
     SMART WASTE SEGREGATION SYSTEM
==========================================

System Ready.

WASTE DETECTED
Waiting for waste to settle...

Average Moisture Value: 730

Classification: WET
Moving flap toward WET bin...

Returning flap to CENTER...

Waste processing completed.

Bin distance: 65.26 cm
```

---

## 4. Monitoring Architecture

```text
                  ┌─────────────────┐
                  │      ESP32      │
                  │                 │
                  │ Sensor Reading  │
                  │ Decision Making │
                  │ Servo Control   │
                  └────────┬────────┘
                           │
                    Serial Communication
                           │
                           ▼
                  ┌─────────────────┐
                  │ Arduino IDE     │
                  │ Serial Monitor  │
                  └─────────────────┘
```

---

## 5. Moisture Monitoring

The Serial Monitor displays the averaged moisture reading:

```text
Average Moisture Value: <value>
```

The firmware takes multiple analog readings and calculates their average before classification.

This allows the user to observe the actual sensor value during testing and calibration.

The threshold can be adjusted based on readings obtained from the physical prototype.

---

## 6. Waste Classification Monitoring

The Serial Monitor displays the classification result:

```text
Classification: WET
```

or:

```text
Classification: DRY
```

It also displays the corresponding servo action:

```text
Moving flap toward WET bin...
```

or:

```text
Moving flap toward DRY bin...
```

This makes the complete classification process visible during demonstrations and debugging.

---

## 7. Bin-Level Monitoring

The HC-SR04 periodically measures the distance between the ultrasonic sensor and the waste surface.

The result is displayed as:

```text
Bin distance: XX.XX cm
```

The distance can be used as the basis for a future bin-fill indicator.

```text
Larger distance
      ↓
More available space

Smaller distance
      ↓
Higher waste level
```

---

## 8. Future Dashboard

Although a dedicated dashboard is not currently implemented, the existing ESP32 monitoring data can be extended into an IoT dashboard.

A future dashboard could display:

### Waste Classification

```text
Wet Waste:  ✓
Dry Waste:  ✓
```

### Moisture Reading

```text
Current Moisture Value: XXXX
```

### Bin Fill Level

```text
Distance: XX cm
Status: Normal
```

### System Status

```text
System: ONLINE
Sensor: READY
Servo: CENTER
```

---

## 9. Possible IoT Dashboard Architecture

A future version could use MQTT or another IoT communication protocol to transmit data from the ESP32 to a dashboard.

```text
             ┌─────────────────┐
             │     Sensors     │
             └────────┬────────┘
                      │
                      ▼
             ┌─────────────────┐
             │      ESP32      │
             └────────┬────────┘
                      │
                 Wi-Fi / MQTT
                      │
                      ▼
             ┌─────────────────┐
             │ MQTT Broker     │
             └────────┬────────┘
                      │
                      ▼
             ┌─────────────────┐
             │ IoT Dashboard   │
             └─────────────────┘
```

The dashboard could then display real-time information such as:

- Waste classification
- Moisture readings
- Bin fill level
- System status
- Number of waste items processed
- Historical sensor readings

---

## 10. Current vs Future Implementation

| Feature | Current Prototype | Future Enhancement |
|---|---|---|
| ESP32 control | Implemented | — |
| Moisture reading | Implemented | Graph / history |
| Waste classification | Implemented | Statistics |
| Servo control | Implemented | — |
| IR detection | Implemented | Event logging |
| Bin distance | Implemented | Fill percentage |
| Serial monitoring | Implemented | — |
| Web dashboard | Not implemented | Possible |
| Mobile dashboard | Not implemented | Possible |
| MQTT communication | Not implemented in this prototype | Possible |
| Cloud storage | Not implemented | Possible |

---

## 11. Purpose of the Dashboard Section

The dashboard section documents the monitoring interface and provides a clear path for future IoT expansion.

The current prototype demonstrates the complete sensing and sorting functionality locally through the ESP32 and Serial Monitor.

A future implementation can extend this architecture to provide remote monitoring and real-time visualization.