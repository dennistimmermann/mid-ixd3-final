#include "Com.h"
#include <Adafruit_FONA.h>

#define URL "http://38bab1d1.ngrok.com/data"

#define FONA_RST 4
#define FONA_CHARGE 20
#define FONA_KEY 5
#define FONA_PS 6

Adafruit_FONA gsm = Adafruit_FONA(FONA_RST);
uint16_t statuscode;
int16_t length;
int8_t gpsFix = 0;

char PIN[] = {'2','5','4','1'};

Com::Com(HardwareSerial * ser) {
	serial = ser;

	pinMode(FONA_PS, INPUT);
	pinMode(FONA_KEY, OUTPUT);
	pinMode(FONA_CHARGE, INPUT_PULLUP);

	digitalWrite(FONA_KEY, HIGH);
	position = {0,0,0,0,0,0};
	charging = false;
}

void Com::begin(int baud) {

	serial->begin(baud);
	if (!gsm.begin(*serial)) {
	    Serial.println(F("Couldn't find GSM"));
	    //while (1);
	}
	Serial.println(F("FONA is connected"));
	Serial.println(". enabling GPS");
	if(!gsm.enableGPS(true)) {
		Serial.println("! failed to turn on GPS");
	}


}

void Com::http_post(char* data) {
	Serial.println("enabling gprs");
	Serial.println("current state is:");
	Serial.write(gsm.GPRSstate());
	if (!gsm.enableGPRS(true)) {
		Serial.println(F("Failed to turn on"));
	}
	if (!gsm.HTTP_POST_start(URL, F("application/json"), (uint8_t *) data, strlen(data), &statuscode, (uint16_t *)&length)) {
		Serial.println("Failed!");
  	}
	while (length > 0) {
		while (gsm.available()) {
			char c = gsm.read();
			Serial.write(c);
			length--;
			if (! length) break;
		}
	}
	Serial.println(F("\n****"));
	gsm.HTTP_POST_end();
}

void Com::queue(char* data) {

}

void Com::unlock() {
	Serial.println(". unlocking SIM...");
	if(!gsm.unlockSIM(PIN)) {
		Serial.println("... pin failed");
	} else {
		Serial.println("... pin ok");
		Serial.println(". enabling network time");
		if(!gsm.enableNetworkTimeSync(true)) {
			Serial.println("! failed to enable time sync");
		}
	}
}

void Com::send() {

}

bool Com::connect() {
	int netState = gsm.getNetworkStatus();
	if(netState != 1) {
		Serial.println("! GSM not connected");
	}
	if(netState == 0) {
		unlock();
		return false;
	}

	if(gsm.GPRSstate() == 0) {
		Serial.println("! GPRS not attached");
		if(!gsm.enableGPRS(true)) {
			Serial.println("! failed to enable GPRS");
		}
	}
}

void Com::checkGPS() {
	int8_t gpsStatus = gsm.GPSstatus();
	if(gpsFix != gpsStatus) {
		Serial.print(". new GPS state: ");
		Serial.println(gpsStatus);
		gpsFix = gpsStatus;
		//flash;
	}
	if(gpsStatus >= 2) {
		Serial.println(". updating GPS position...");
		if(!gsm.getGPS(&position.lat, &position.lon, &position.speed, &position.heading, &position.altitude)) {
			Serial.println("... failed");
		}
		Serial.print(position.lat);
		Serial.print(" - ");
		Serial.println(position.lon);
	}
}

void Com::run() {
	if(networkStatusTimer>5000) {
		networkStatusTimer = 0;
		connect();
		Serial.print("Power state: ");
		Serial.println(digitalRead(FONA_PS));

		Serial.print("Charger state: ");
		Serial.println(analogRead(FONA_CHARGE));
	}

	if(updatePositionTimer>10000) {
		updatePositionTimer = 0;
		checkGPS();
	}

	if(analogRead(FONA_CHARGE) < 1000) {
		digitalWrite(13, LOW);
		charging = false;
	} else {
		digitalWrite(13, HIGH);
		charging = true;
	}
}
