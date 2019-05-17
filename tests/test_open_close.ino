
#include <Servo.h>

Servo servo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int const trigPin = 9;
int const echoPin = 10;


int pos = 0;    // variable to store the servo position

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  servo.attach(8);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  int duration, distance;
  digitalWrite(trigPin, HIGH); 
  delay(1);
  digitalWrite(trigPin, LOW);
  // Measure the pulse input in echo pin
  duration = pulseIn(echoPin, HIGH);
  // Distance is half the duration devided by 29.1 (from datasheet)
  distance = (duration/2) / 29.1;
  // if distance less than 0.5 meter and more than 0 (0 or less means over range) 
  if (distance <= 50 && distance >= 0) {
    servo.write(50);
      delay(3000);
  } else {
    servo.write(160);
  }
  // Waiting 60 ms won't hurt any one
  delay(60);
}
