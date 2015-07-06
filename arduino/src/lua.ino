/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:

#include "Effects.h"
#include "Com.h"
#include "Handoff.h"

Effects *lights = new Effects();
Com com(&Serial2);
Handoff handoff(&com);
//Effects lights();


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  // while(!Serial) {
  //   ;
  // }
  pinMode(13, OUTPUT);

  Serial.begin(9600);
  com.begin(9600);
  lights->setEffect(EFFECT_FLASH);

}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available()) {
    while(Serial.available()) {
      Serial.read();
    };
    Serial.write("simulating handoff");
    lights->setEffect(EFFECT_TWINKLE);
    handoff.doIt();
    //char data[] = "{\"lat\":52.286947,\"lon\":8.026087,\"timestamp\":\"1436118312\",\"bia\":\"666\"}";
    //com.http_post(data);
  }
  com.run();
	//digitalWrite(13, HIGH);
}
