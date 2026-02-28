#include <Servo.h>

// Create servo object
Servo sortingServo;

// Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;

// Metal Sensor Pin
const int metalSensorPin = 2;

// Moisture Sensor Pin
const int moistureSensorPin = A0;

// Variables
long duration;
int distance;

void setup() {
  Serial.begin(9600);

  // Attach servo to pin 6
  sortingServo.attach(6);

  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(metalSensorPin, INPUT);

  // Start servo at default position
  sortingServo.write(0);

  Serial.println("Smart Dustbin System Started...");
}

void loop() {

  // -------- Detect Object Using Ultrasonic --------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // If object is within 15 cm
  if (distance > 0 && distance < 15) {

    Serial.println("Trash Detected!");

    int metalValue = digitalRead(metalSensorPin);
    int moistureValue = analogRead(moistureSensorPin);

    // -------- Waste Classification --------
    if (metalValue == HIGH) {
      Serial.println("Metal Waste");
      sortingServo.write(30);     // Metal bin
    }
    else if (moistureValue > 500) {
      Serial.println("Wet Waste");
      sortingServo.write(90);     // Organic bin
    }
    else {
      Serial.println("Dry Waste");
      sortingServo.write(150);    // Plastic/Paper bin
    }

    delay(3000);  // Wait before resetting
    sortingServo.write(0);  // Return to default
    Serial.println("System Reset");
  }

  delay(500);
}
