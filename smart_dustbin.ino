#include <Servo.h>

// -------- Pins & Constants --------
Servo sortingServo;
const int trigPin = 9;
const int echoPin = 10;
const int metalSensorPin = 2;
const int moistureSensorPin = A0;

// Motor Pins
int ENA = 5; int IN1 = 7; int IN2 = 8;
int ENB = 6; int IN3 = 11; int IN4 = 12;

void setup() {
  Serial.begin(9600);
  sortingServo.attach(4);
  sortingServo.write(90); // Neutral position

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(metalSensorPin, INPUT);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
}

void loop() {
  int distance = getDistance();

  if (distance > 30) {
    searchForTrash(); 
  } 
  else if (distance <= 30 && distance > 8) {
    moveForward(100); // Slow approach
  } 
  else {
    stopMoving();
    delay(500);
    collectAndSort();
  }
}

void collectAndSort() {
  Serial.println("Analyzing...");
  
  // NOTE: In a real build, you'd add a motor/arm move here 
  // to pull the trash onto the sensors.

  int metalValue = digitalRead(metalSensorPin);
  int moistureValue = analogRead(moistureSensorPin);

  // Logic Change: Metal sensors are usually Active LOW or HIGH 
  // depending on the module. Check your specific sensor.
  if (metalValue == HIGH) { 
    Serial.println("Metal detected");
    sortingServo.write(30); 
  } 
  else if (moistureValue > 400) { 
    Serial.println("Wet waste");
    sortingServo.write(90);
  } 
  else {
    Serial.println("Dry waste");
    sortingServo.write(150);
  }

  delay(2000); // Wait for flap to move
  // Add a 'push' action here if you have a pusher motor
  sortingServo.write(90); // Reset
}

// -------- Improved Movement --------

void searchForTrash() {
  // Rotate slowly to find something
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 120);   analogWrite(ENB, 120);
}

void moveForward(int speed) {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENA, speed); analogWrite(ENB, speed);
}

void stopMoving() {
  analogWrite(ENA, 0); analogWrite(ENB, 0);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}
