/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* by Dejan Nedelkovski,
* www.HowToMechatronics.com
*
*/

#include <LiquidCrystal.h> // includes the LiquidCrystal Library
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

Servo servo;

//LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int trigPin = 9;
const int echoPin = 10;

int pos = 0;

long duration;
int distanceCm, distanceInch;

void setup() {
  //lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  lcd.init();
  lcd.init();
  lcd.backlight();

  servo.attach(8);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.println("setting up!!");

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println("test sweeping...");
    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("test sweeping...");
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void loop() {
//  for (pos = 0; pos <= 180; pos += 10) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    Serial.println("test sweeping... 1");
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 180; pos >= 0; pos -= 10) { // goes from 180 degrees to 0 degrees
//    servo.write(pos);              // tell servo to go to position in variable 'pos'
//    Serial.println("test sweeping... 2");
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distanceCm= duration*0.034/2;
  
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Distance: "); // Prints string "Distance" on the LCD
  lcd.print(distanceCm); // Prints the distance value from the sensor
  lcd.print(" cm  ");
  delay(10);

  if (distanceCm <= 50 && distanceCm >= 0) {
    lcd.setCursor(0,1);
    lcd.print("sweeping 1");
    servo.write(180);
//    for (pos = 0; pos <= 180; pos += 60) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//      servo.write(pos);              // tell servo to go to position in variable 'pos'
//      Serial.println("sweeping 1");
//      delay(15);                       // waits 15ms for the servo to reach the position
//    }
  } else {
    lcd.setCursor(0,1);
    lcd.print("sweeping 2");
    servo.write(0);
  }
  delay(60);
}
