#include "SerialMP3Player.h"

#define TX 6
#define RX 5

SerialMP3Player mp3(RX,TX);

void setup() {
  Serial.begin(9600);     // start serial interface
  mp3.begin(9600);        // start mp3-communication
  delay(500);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(500);             // wait for init
}

// the loop function runs over and over again forever
void loop() {
  mp3.play();     // Play "hello.mp3". You must hear "Hello World"
  delay(3000);    // wait 3 seconds
}
