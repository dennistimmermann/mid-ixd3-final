#include "Adafruit_FONA.h"
#include "Gprs.h"

#define URL "http://71ae2c86.ngrok.com/data"

#define FONA_RST 4

Adafruit_FONA gsm = Adafruit_FONA(FONA_RST);
uint16_t statuscode;
int16_t length;

Gprs::Gprs(HardwareSerial * ser) {
	serial = ser;
}

void Gprs::begin(int baud) {
	serial->begin(baud);
	if (!gsm.begin(*serial)) {
	    Serial.println(F("Couldn't find GSM"));
	    while (1);
	}
	Serial.println(F("GSM is OK"));
}

void Gprs::http_post(char* data) {
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

void Gprs::queue(char* data) {

}

void Gprs::send() {

}
