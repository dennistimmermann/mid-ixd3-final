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

#include "EffectsController.h"
#include "Com.h"
#include "Handoff.h"

EffectsController lights(&Serial3);
Com com(&Serial2, &lights);
Handoff handoff(&com, &lights);

elapsedMillis effectThrottle;
elapsedMillis handoffThrottle;
//Effects lights();


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  // while(!Serial) {
  //   ;
  // }
  pinMode(13, OUTPUT);
  randomSeed(1);
  Serial.begin(9600);
  lights.begin(9600);
  com.begin(9600);
  lights.setEffect(EFFECT_OFF);

}

// the loop routine runs over and over again forever:
void loop() {

  com.run();

  if(!com.isCharging()) {
    if(handoffThrottle > 2000) {
      handoffThrottle = 0;
      handoff.run();
    }
  }
	//digitalWrite(13, HIGH);
}
