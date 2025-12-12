#include <Servo.h>                   // Include servo library

// Ultrasonic sensor pins
const int trigPin = 8;              // Trigger pin of HC-SR04
const int echoPin = 9;              // Echo pin of HC-SR04

// Output indicators
const int buzzerPin = 11;           // Buzzer pin
const int ledRed = 7;               // Warning LED (Red)
const int ledGreen = 6;             // Normal LED (Green)

// Variables for ultrasonic measurement
long duration;                      // Echo pulse duration
int distance;                       // Converted distance in cm

Servo myServo;                      // Servo motor object


// ---------------------------------------------------------
// Setup
// ---------------------------------------------------------
void setup() {
  pinMode(trigPin, OUTPUT);         // Ultrasonic trigger as output
  pinMode(echoPin, INPUT);          // Ultrasonic echo as input
  pinMode(buzzerPin, OUTPUT);       // Buzzer output
  pinMode(ledRed, OUTPUT);          // Red LED output
  pinMode(ledGreen, OUTPUT);        // Green LED output

  // Default indicator states
  digitalWrite(ledGreen, HIGH);     // Green LED ON — safe
  digitalWrite(ledRed, LOW);        // Red LED OFF
  digitalWrite(buzzerPin, LOW);     // Buzzer OFF

  Serial.begin(9600);               // Serial communication for Processing radar
  myServo.attach(10);               // Attach servo to pin D10
}


// ---------------------------------------------------------
// Main Loop — Sweeps servo from 15° → 165° → 15°
// ---------------------------------------------------------
void loop() {

  // Sweep forward (left to right)
  for (int i = 15; i <= 165; i++) {

    distance = calculateDistance();   // Read current distance

    // Send angle + distance to Processing:  "45,23."
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");

    myServo.write(i);                 // Move servo to current angle

    // ----------------------------
    // STOP SERVO ONLY IF < 10 cm
    // ----------------------------
    if (distance < 10) {

      // Hold servo in place until object moves away
      while (calculateDistance() < 10) {
        myServo.write(i);             // Keep servo fixed
        digitalWrite(ledGreen, LOW);  // Green OFF
        digitalWrite(ledRed, HIGH);   // Red warning LED ON
        digitalWrite(buzzerPin, HIGH);// Buzzer ON
        delay(30);
      }
    }

    // Reset indicators when object is not dangerously close
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);

    delay(30);                        // Sweep speed
  }


  // Sweep backward (right to left)
  for (int i = 165; i >= 15; i--) {

    distance = calculateDistance();

    // Send "angle,distance." to Processing
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");

    myServo.write(i);

    // ----------------------------
    // STOP SERVO ONLY IF < 10 cm
    // ----------------------------
    if (distance < 10) {

      while (calculateDistance() < 10) {
        myServo.write(i);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, HIGH);
        digitalWrite(buzzerPin, HIGH);
        delay(30);
      }
    }

    // Normal mode
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, LOW);
    digitalWrite(buzzerPin, LOW);

    delay(30);
  }
}



// ---------------------------------------------------------
// Ultrasonic Distance Function
// Triggers sensor → Measures echo time → Returns cm
// ---------------------------------------------------------
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);       // Trigger pulse (10 µs)
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH); // Read echo duration
  distance = duration * 0.034 / 2;   // Convert to cm

  return distance;
}
