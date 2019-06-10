#include<Servo.h>
Servo servo;
int const trigPin = 9;
int const echoPin = 10;

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  servo.attach(8);
  Serial.print("helloworld ");

}

void loop() {
  Serial.print("loop ");
  int duration, distance;
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;

  Serial.print(distance);
  Serial.println("  distance"); 
  
  // if distance less than 0.5 meter and more than 0 (0 or less means over range) 
  if (distance <= 50 && distance >= 0) {
    Serial.println("!!open!!!");
    servo.write(50);
      delay(2000);
  } else {
    
    servo.write(160);
  }
  // Waiting 60 ms won't hurt any one
  delay(50);
}
