#include <SharpIR.h>
#include <LiquidCrystal_I2C.h>


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

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // lcd display
  lcd.init();
  lcd.backlight();
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


//  lcd.setCursor(0,0); // Sets the location at which subsequent text written to the LCD will be displayed
//  lcd.print("Flow: "); // Prints string "Distance" on the LCD
//  lcd.print(counter); // Prints the distance value from the sensor
//  lcd.print(" passed by");

  Serial.print(counter);
  Serial.println("  passing by");  // returns it to the serial monitor

  if (counter % 3 == 2) {
    lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("You are lucky!"); // Prints string "Distance" on the LCD
    delay(3000);
    lcd.setCursor(3,0); // Sets the location at which subsequent text written to the LCD will be displayed
    lcd.print("Come to me! ");
    lcd.print("Grab a candy! "); 
    delay(3000);
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
    lcd.print("Thank you! Have a great day! :)"); 
    delay(3000);
    lcd.clear();
    counter ++;
  }
  
  unsigned long calcTime=millis()-startTime;  // the following gives you the time taken to get the measurement
  Serial.print("Time taken (ms): ");
  Serial.println(calcTime);  

  lcd.clear();
}
