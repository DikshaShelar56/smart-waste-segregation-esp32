#include <ESP32Servo.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

#define MOISTURE_PIN 34
#define IR_PIN 27
#define SERVO_PIN 13

// HC-SR04 - BIN LEVEL MONITORING
#define TRIG_PIN 5
#define ECHO_PIN 18

// =====================================================
// MOISTURE CALIBRATION
// =====================================================

// Your actual calibration:
// DRY = approximately 703
// WET = approximately 1011
//
// Therefore:
// Below 850  = DRY
// 850 or more = WET

#define MOISTURE_THRESHOLD 1000

// =====================================================
// SERVO POSITIONS
// =====================================================

// Your physical setup:
// 40 degrees  = LEFT side  = WET
// 140 degrees = RIGHT side = DRY
// 90 degrees  = CENTER

#define CENTER_POSITION 90
#define WET_POSITION 40
#define DRY_POSITION 140

Servo sortingServo;

// =====================================================
// WASTE PROCESSING CONTROL
// =====================================================

// Prevents the same waste from being processed repeatedly
bool wasteProcessed = false;

// =====================================================
// MOISTURE READING FUNCTION
// =====================================================

int readMoisture() {

  long total = 0;

  // Take 10 readings for stability
  for (int i = 0; i < 10; i++) {

    total += analogRead(MOISTURE_PIN);

    delay(50);
  }

  int averageValue = total / 10;

  return averageValue;
}

// =====================================================
// ULTRASONIC DISTANCE FUNCTION
// =====================================================

float readDistance() {

  // Make sure trigger starts LOW
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Send ultrasonic pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read echo
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);

  // If no echo received
  if (duration == 0) {
    return -1;
  }

  // Calculate distance in cm
  float distance = duration * 0.0343 / 2;

  return distance;
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Sensor pins
  pinMode(MOISTURE_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  // Ultrasonic pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo
  sortingServo.attach(SERVO_PIN);

  // Start servo in center
  sortingServo.write(CENTER_POSITION);

  delay(1000);

  // ===================================================
  // STARTUP MESSAGE
  // ===================================================

  Serial.println();
  Serial.println("==========================================");
  Serial.println("     SMART WASTE SEGREGATION SYSTEM");
  Serial.println("==========================================");
  Serial.println();

  Serial.println("Moisture Calibration:");
  Serial.println("DRY  = below 850");
  Serial.println("WET  = 850 or above");
  Serial.println();

  Serial.println("Servo Positions:");
  Serial.println("WET  = 40 degrees");
  Serial.println("DRY  = 140 degrees");
  Serial.println("CENTER = 90 degrees");
  Serial.println();

  Serial.println("System Ready.");
  Serial.println("Waiting for waste...");
  Serial.println();
}

// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ===================================================
  // 1. READ IR SENSOR
  // ===================================================

  int irState = digitalRead(IR_PIN);


  // ===================================================
  // 2. NEW WASTE DETECTED
  // ===================================================

  // IR sensor is ACTIVE LOW
  // LOW = object detected
  // HIGH = no object

  if (irState == LOW && wasteProcessed == false) {

    // Mark this waste as being processed
    wasteProcessed = true;

    Serial.println();
    Serial.println("------------------------------------------");
    Serial.println("           WASTE DETECTED");
    Serial.println("------------------------------------------");

    // Give the waste time to settle on the
    // moisture sensor
    Serial.println("Waiting for waste to settle...");

    delay(1500);


    // =================================================
    // 3. READ MOISTURE
    // =================================================

    int moistureValue = readMoisture();

    Serial.print("Average Moisture Value: ");
    Serial.println(moistureValue);

    Serial.println();


    // =================================================
    // 4. CLASSIFY WASTE
    // =================================================

    // YOUR CALIBRATION:
    //
    // DRY ≈ 703
    // WET ≈ 1011
    //
    // Therefore:
    // >= 850 = WET
    // < 850  = DRY

if (moistureValue < MOISTURE_THRESHOLD) {

  Serial.println("Classification: WET");
  Serial.println("Moving flap toward WET bin...");

  sortingServo.write(WET_POSITION);

  delay(2500);

  Serial.println("Returning flap to CENTER...");
  sortingServo.write(CENTER_POSITION);

  delay(1000);

}
else {

  Serial.println("Classification: DRY");
  Serial.println("Moving flap toward DRY bin...");

  sortingServo.write(DRY_POSITION);

  delay(2500);

  Serial.println("Returning flap to CENTER...");
  sortingServo.write(CENTER_POSITION);

  delay(1000);
}


    // =================================================
    // 5. PROCESSING COMPLETE
    // =================================================

    Serial.println();
    Serial.println("Waste processing completed.");
    Serial.println("Remove the waste from the IR detection area.");
    Serial.println();

  }


  // ===================================================
  // 6. WAIT FOR WASTE TO BE REMOVED
  // ===================================================

  if (irState == HIGH && wasteProcessed == true) {

    // Allow the next waste item to be detected
    wasteProcessed = false;

    Serial.println("------------------------------------------");
    Serial.println("Waste removed.");
    Serial.println("System ready for next waste.");
    Serial.println("------------------------------------------");
    Serial.println();

    delay(1000);
  }


  // ===================================================
  // 7. ULTRASONIC BIN LEVEL MONITORING
  // ===================================================

  // Check bin distance every 3 seconds
  // This works independently of waste classification.

  static unsigned long lastDistanceCheck = 0;

  if (millis() - lastDistanceCheck >= 3000) {

    lastDistanceCheck = millis();

    float distance = readDistance();

    if (distance > 0) {

      Serial.print("Bin distance: ");
      Serial.print(distance);
      Serial.println(" cm");

    }
    else {

      Serial.println("Ultrasonic: No echo received.");

    }
  }


  // Small delay to keep the loop stable
  delay(100);
}