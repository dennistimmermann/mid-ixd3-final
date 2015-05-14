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
int led = 13;

TinyGPSPlus gps;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);

  Serial.begin(9600);
  Serial1.begin(9600);

}

// the loop routine runs over and over again forever:
void loop() {
	if(Serial1.available() > 0) {
		digitalWrite(led, HIGH);
		while(Serial1.available() > 0) {
			gps.encode(Serial1.read());
		}
		//Serial.println(gps.failedChecksum());
		Serial.print(gps.location.lat());
		Serial.print(" - ");
		Serial.println(gps.location.lng());
	}
	digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW

	//Serial.print(" - ");
	//Serial.println(gps.satellites.value());
	//delay(1000);
}
