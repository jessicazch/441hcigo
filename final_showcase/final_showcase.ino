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

SharpIR SharpIR(ir, model);
int counter = 0;
int lastdis = 5000;
int pos = 0;
boolean yesman = false;
long randNum;

LiquidCrystal_I2C lcd(0x27,16,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const int trigPin = 9;
const int echoPin = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  randomSeed(analogRead(0));
  
  servo.attach(8);

  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init

  // lcd display
  lcd.init();
  lcd.backlight();

  lcd.setCursor(5,0);
  lcd.print("WELCOME TO");
  lcd.setCursor(3,1);
  lcd.print("SNACKOVERFLOW");
  lcd.setCursor(7,2);
  lcd.print("TO-GO");
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.write(179);
}

void loop() {
  delay(100);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("WELCOME TO");
  lcd.setCursor(3,1);
  lcd.print("SNACKOVERFLOW");
  lcd.setCursor(7,2);
  lcd.print("TO-GO");
  delay(15);

  unsigned long startTime = millis();  // takes the time before the loop on the library begins

  int dis1=SharpIR.distance();  // this returns the distance to the object you're measuring

  if (dis1 <= 50 && dis1 > 0 && lastdis > 50) {
    counter ++;
    yesman = true;
//    randNum = 1;
    randNum = random(0, 2);
    lcd.setCursor(5,3);
    lcd.print(randNum);
    delay(50);
  } else {
    yesman = false;
  }

  lastdis = dis1;
  Serial.print(dis1);
  Serial.println("  distance"); 

  Serial.print(counter);
  Serial.println("  passing by");  // returns it to the serial monitor

  Serial.print(randNum);
  Serial.println("random num = ");

  if (yesman && randNum == 1) {
    for (pos = 180; pos > 70; pos -= 1) {
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(8);                     // waits 15ms for the servo to reach the position
    }
//    servo.write(80); // open
    delay(15);

    mp3.play(1, 30);     // Play the sound!
    delay(15);    // wait .15 seconds

    lcd.clear();
    lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("You are lucky!"); // Prints string "Distance" on the LCD
    delay(3000);
    lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Come to me! ");
    lcd.setCursor(3,1);
    lcd.print("Grab a snack!");
    lcd.setCursor(0,2);
    lcd.print("Share with friends!");
    lcd.clear();
    
    for (int i = 20; i > 0; i--) {
      delay(500);
      lcd.setCursor(2,0); // Sets the location at which subsequent text written to the LCD will be displayed
      lcd.print("Count Down: ");
      lcd.print(i, DEC);
      delay(1000);
      int dis2=SharpIR.distance();  // this returns the distance to the object you're measuring
      if (dis2 > 100) {
        break;
      }
      lcd.clear();
    }
    lcd.setCursor(2,0);
    lcd.print("Thank you! ");
    lcd.setCursor(2,2);
    lcd.print("Have a great day!");
    for (pos = 60; pos < 180; pos += 1) {
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                     // waits 15ms for the servo to reach the position
    }
    delay(3000);
    lcd.clear();
  }
}
