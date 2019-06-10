#include <SharpIR.h>
#include <LiquidCrystal_I2C.h>
#include<Servo.h>


#define ir A0
#define model 20150
// ir: the pin where your sensor is attached
// model: an int that determines your sensor:  1080 for GP2Y0A21Y
//                                            20150 for GP2Y0A02Y
//                                            (working distance range according to the datasheets)
// updated by Ahmad Nejrabi from Robojax.com
// get latest datasheets: http://www.sharp-world.com/products/device/lineup/selection/opto/haca/diagram.html
// model: an int that determines your sensor:
/*
 * GP2Y0A02YK0F --> "20150"
 GP2Y0A21YK --> "1080"
 GP2Y0A710K0F --> "100500"
  GP2YA41SK0F --> "430"
 */

Servo servo;

SharpIR SharpIR(ir, model);
int counter = 0;
int key = 2;
int lastdis = 5000;

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // lcd display
  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servo.attach(8);

}

void loop() {
  delay(100);   

  unsigned long startTime = millis();  // takes the time before the loop on the library begins

  int dis1=SharpIR.distance();  // this returns the distance to the object you're measuring

  if (dis1 <= 300 && dis1 > 0 && lastdis > 300) {
    counter ++;
  }
  
  lastdis = dis1;
  
  Serial.print(counter);
  Serial.println("  passing by");  // returns it to the serial monitor
  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
  lcd.print("Flow: "); // Prints string "Distance" on the LCD
  lcd.print(counter); // Prints the distance value from the sensor
  lcd.print(" people");
  delay(500);
  
  unsigned long calcTime=millis()-startTime;  // the following gives you the time taken to get the measurement
  Serial.print("Time taken (ms): ");
  Serial.println(calcTime);  

  if (counter % key == 1) {
//  if (true) {
    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Start testing distance "); // Prints string "Distance" on the LCD
    delay(500);
    
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
  
    Serial.print("Distance: ");
    Serial.println(distance);

    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Distance: "); // Prints string "Distance" on the LCD
    lcd.print(distance); // Prints the distance value from the sensor
    lcd.print(" cm");
    delay(500);
    
    // if distance less than 0.5 meter and more than 0 (0 or less means over range) 
    if (distance <= 50 && distance >= 0) {
      Serial.println("!!open!!");
      lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
      lcd.print("!!Open!!");
      servo.write(0);
      delay(500);
      lcd.clear();
    } else {
      lcd.setCursor(0,1);
      lcd.print("closing!..");
      servo.write(0);
      servo.write(180);
    }
    // Waiting 50 ms won't hurt any one
    delay(50);

  } else {
    servo.write(160);
    delay(50);
  }
}
