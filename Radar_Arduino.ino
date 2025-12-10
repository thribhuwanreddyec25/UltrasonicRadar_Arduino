#include <Servo.h>

const int trigPin = 8;      // Ultrasonic trigger pin
const int echoPin = 9;      // Ultrasonic echo pin
const int buzzerPin = 11;   // Buzzer output pin
const int ledRed = 7;       // Red LED pin
const int ledGreen = 6;     // Green LED pin

// Variables for ultrasonic timing and distance
long duration;
int distance;

Servo myServo;  // Servo motor object

void setup() {
  pinMode(trigPin, OUTPUT);     // Set trigger pin as output
  pinMode(echoPin, INPUT);      // Set echo pin as input
  pinMode(buzzerPin, OUTPUT);   // Buzzer as output
  pinMode(ledRed, OUTPUT);      // Red LED output
  pinMode(ledGreen, OUTPUT);    // Green LED output

  // Default LED/buzzer states
  digitalWrite(ledGreen, HIGH);
  digitalWrite(ledRed, LOW);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(9600);           // Start serial communication
  myServo.attach(10);           // Attach servo to pin 10
}

void loop() {

  // Sweep from 15° to 165°
  for (int i = 15; i <= 165; i++) {

    distance = calculateDistance();   // Read ultrasonic distance

    // Send angle and distance to serial monitor/Processing
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");

    int servoPos = i;
    myServo.write(servoPos);          // Move servo to current angle

    // Object detected within 40 cm
    if (distance < 40) {
      while (calculateDistance() < 40) {
        myServo.write(servoPos);      // Hold servo at same angle
        digitalWrite(ledGreen, LOW);  // Green OFF
        digitalWrite(ledRed, HIGH);   // Red ON
        digitalWrite(buzzerPin, HIGH);// Buzzer ON
        delay(30);
      }
    }

    // Reset indicator states
    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledRed, LOW);

    delay(30);
  }

  // Sweep back from 165° to 15°
  for (int i = 165; i > 15; i--) {

    distance = calculateDistance();

    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");

    int servoPos = i;
    myServo.write(servoPos);

    if (distance < 40) {
      while (calculateDistance() < 40) {
        myServo.write(servoPos);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, HIGH);
        digitalWrite(buzzerPin, HIGH);
        delay(30);
      }
    }

    digitalWrite(ledGreen, HIGH);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledRed, LOW);

    delay(30);
  }
}

// ------------------------------------------------------------
// Ultrasonic distance calculation function
// ------------------------------------------------------------
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Trigger the ultrasonic sensor with 10µs pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);   // Read echo time

  distance = duration * 0.034 / 2;     // Convert to cm

  return distance;
}
