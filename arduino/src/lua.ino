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

int charge = 10;
int last_charge = 0;

elapsedMillis counter;

Effects *lights = new Effects();
int touchReading = 0;
bool touched = false;
int touchStep = 0;
bool stepped = false;

int colorCounter = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  // while(!Serial) {
  //   ;
  // }
  randomSeed(1);
  Serial.begin(9600);
  lights->setEffect(EFFECT_TWINKLE);
  lights->charge = 30;
}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial.available() > 0) {
    while(Serial.available() > 0) {
      Serial.read();
    }
    lights->setEffect(EFFECT_FLASH);
    //lights->charge = min(lights->charge +10, 100);
  }

  touchReading = touchRead(23);
  if(touchReading > 4000 && touched == false) {
    touched = true;
    stepped = false;
    touchStep = (touchStep+1)%5;
  } else if(touchReading < 2000 && touched == true) {
    touched = false;
  }

  if(stepped == false) {
    stepped = true;
    if(touchStep == 0) {
      // lights->colorIndex = (lights->colorIndex+1)%5;
      lights->energy = 10;
      lights->setEffect(EFFECT_TWINKLE);
    }
    if(touchStep == 1) {
      lights->colorIndex = (lights->colorIndex+1)%5;
      lights->energy = 100;
      lights->setEffect(EFFECT_FLASH);
    }
    if(touchStep == 2) {
      lights->energy = 25;
      lights->setEffect(EFFECT_TWINKLE);
    }
    if(touchStep == 3) {
      lights->colorIndex = (lights->colorIndex+1)%5;
      lights->energy = 100;
      lights->setEffect(EFFECT_FLASH);
    }
    if(touchStep == 4) {
      lights->energy = 100;
      counter = 0;
      lights->setEffect(EFFECT_FLASH);
    }
  }

  if(touchStep == 4) {
    lights->energy = max(100-(counter/250), 5);
  }

  Serial.println(touchReading);
	lights->run();
	//digitalWrite(13, HIGH);
}
