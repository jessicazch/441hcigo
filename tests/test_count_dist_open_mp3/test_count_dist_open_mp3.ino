#include <SharpIR.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "SerialMP3Player.h"

#define TX 6
#define RX 5

SerialMP3Player mp3(RX,TX);

Servo servo;

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

SharpIR SharpIR(ir, model);
int counter = 0;
int lastdis = 5000;
int pos = 0;

LiquidCrystal_I2C lcd(0x27,16,3);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(8);

  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init

  // lcd display
  lcd.init();
  lcd.backlight();
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.write(0);
  for (pos = 0; pos < 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    Serial.println("test sweeping...");
    lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("test sweeping...");
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 20; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println("sweeping 2");
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}

void loop() {
  delay(100);   

  unsigned long startTime = millis();  // takes the time before the loop on the library begins

  int dis1=SharpIR.distance();  // this returns the distance to the object you're measuring

  if (dis1 <= 50 && dis1 > 0 && lastdis > 50) {
    counter ++;
  }

  lastdis = dis1;
  Serial.print(dis1);
  Serial.println("  distance"); 

  Serial.print(counter);
  Serial.println("  passing by");  // returns it to the serial monitor

  if (counter % 3 == 2) {
    servo.write(160);
    delay(15);
    
    lcd.setCursor(3,3);
    lcd.print("Open!");

    mp3.play();     // Play the sound!
    delay(15);    // wait .15 seconds

    unsigned long startTime = millis();

    while ((millis()-startTime) <= 3000) {
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
      delay(15);
  
      if (distance <= 50 && distance >= 0) {
  //      lcd.setCursor(3,3);
  //      lcd.print("Open!");
        servo.write(160);
  //      delay(15);
    
        lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
        lcd.print("You are lucky!"); // Prints string "Distance" on the LCD
        delay(3000);
        lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
        lcd.print("Come to me! ");
        lcd.setCursor(3,1);
        lcd.print("Grab a candy! "); 
        lcd.clear();
        
        for (int i = 3; i > 0; i--) {
          delay(500);
          lcd.setCursor(2,0); // Sets the location at which subsequent text written to the LCD will be displayed
          lcd.print("Count Down: ");
          lcd.print(i, DEC);
          delay(1000);
          lcd.clear();
        }
        lcd.setCursor(2,0);
        lcd.print("Thank you! ");
        lcd.setCursor(2,1);
        lcd.print("Have a great day!");
        lcd.setCursor(0,3);
        lcd.print("(closing)");
        servo.write(0);
        delay(3000);
        lcd.clear();
        counter ++;
        break;
      } 
//      else {
//        servo.write(180);
//        delay(20);
//      }

    }
    lcd.setCursor(2,0);
    lcd.print("Bye bye!");
    servo.write(50);
    delay(50);
  }
  lcd.clear();
}
