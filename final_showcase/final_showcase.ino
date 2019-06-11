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

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int dis1 = duration*0.034/2; 
  
//  int dis1=SharpIR.distance();  // this returns the distance to the object you're measuring

  if (dis1 <= 300 && dis1 > 0 && lastdis > 300) {
    counter ++;
    yesman = true;
//    randNum = 1;
    randNum = random(0, 15);
    lcd.setCursor(5,3);
    lcd.print(dis1);
    lcd.setCursor(15,3);
    lcd.print(randNum);
    delay(50);
  } else {
    yesman = false;
  }

  lastdis = dis1;
  Serial.print(dis1);
  Serial.println("  distance"); 
  
  Serial.print("random num = ");
  Serial.println(randNum);

  if (yesman && randNum == 1) {
    for (pos = 179; pos > 65; pos -= 1) {
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(8);                     // waits 15ms for the servo to reach the position
    }
    delay(15);

    mp3.play(1, 30);     // Play the sound!
    delay(15);    // wait .15 seconds

    lcd.clear();
    lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("You are lucky!"); // Prints string "Distance" on the LCD
    lcd.setCursor(3,2); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Come to me! ");
    delay(5000);
    lcd.setCursor(3,0);
    lcd.print("Grab a snack!");
    lcd.setCursor(0,2);
    lcd.print("Share with friends!");
//    lcd.clear();
    
    for (int i = 15; i > 0; i--) {
      delay(500);
      lcd.setCursor(3,0);
      lcd.print("Grab a snack!");
      lcd.setCursor(0,2);
      lcd.print("Share with friends!");
      lcd.setCursor(2,3); // Sets the location at which subsequent text written to the LCD will be displayed
      lcd.print("Count Down: ");
      lcd.print(i, DEC);
      delay(1000);
      int dis2=SharpIR.distance();  // this returns the distance to the object you're measuring
      if (dis2 > 50) {
        break;
      }
      lcd.clear();
    }
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Thank you! ");
    lcd.setCursor(2,2);
    lcd.print("Have a great day!");
    for (pos = 65; pos < 179; pos += 1) {
      servo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(5);                     // waits 15ms for the servo to reach the position
    }
    delay(3000);
    lcd.clear();
  }
}
