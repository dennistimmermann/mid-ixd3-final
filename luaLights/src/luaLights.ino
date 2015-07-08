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

Effects lights;
byte b[8];
int i;
bool complete;

elapsedMillis blinkTimer;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  // while(!Serial) {
  //   ;
  // }
  pinMode(13, OUTPUT);
  randomSeed(1);
  complete = false;
  i = 0;
  // Serial.begin(9600);
  // com.begin(9600);
  lights.setEffect(EFFECT_TWINKLE);
  Serial.begin(9600);
  Serial3.begin(9600);

}

// the loop routine runs over and over again forever:
void loop() {
  if(Serial3.available()) {
    //digitalWrite(13, HIGH);
    while(Serial3.available()) {
      byte in = Serial3.read();
      if(in == 's') {
        i = 0;
      }
      if(in == '\n') {
        complete = true;
      }
      b[i] = in;
      i++;
    };


    // //Serial.write(b-48);
    // //lights.setEffect(b-48);
    // Serial.println(b[0]);
    // Serial.println(b[1]);
    // Serial.println(b[2]);
    // Serial.println(b[3]);
  }
  if(complete) {
    Serial.println(b[0]);
    Serial.println(b[1]);
    Serial.println(b[2]);
    Serial.println(b[3]);
    Serial.println(b[4]);
    Serial.println(b[5]);

    complete = false;
    if(b[0] == 's') {
      if(b[1] == 0) {
        lights.setTargetColor(b[2], b[3], b[4]);
      } else if(b[1] == 1) {
        lights.setEffect(b[2]);
      } else if(b[1] == 2) {
        lights.setEnergy(b[2]);
      } else if(b[1] == 3) {
        lights.setCharge(b[2]);
      }
    }
  }

  lights.run();

  if(blinkTimer < 500) {
    digitalWrite(13, LOW);
  } else if(blinkTimer < 1000) {
    digitalWrite(13, HIGH);
  } else {
    blinkTimer = 0;
  }

}
