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

#include "TinyGPS++.h"
#include "Gps.h"

#include <stack>

using namespace std;
stack<int> Queue;

int led = 13;

Gps gps(&Serial1);

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  gps.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
	gps.run();
	//digitalWrite(13, HIGH);
}

extern "C"{
  int _getpid(){ return -1;}
  int _kill(int pid, int sig){ return -1; }
}
