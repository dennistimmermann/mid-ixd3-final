#include "TinyGPS++.h"
#include "Gps.h"
#include "utils.h"

Gps::Gps(HardwareSerial * ser) {
	serial = ser;
	lat = 0;
	lon = 0;
	timestamp = 0;
}

void Gps::begin(int baud) {
	serial->begin(baud);
	serial->println(PMTK_SET_NMEA_UPDATE_200_MILLIHERTZ);
}

void Gps::run() {
	if(serial->available() > 0) {
		digitalWrite(13, HIGH);
		while(serial->available() > 0) {
			tgps.encode(serial->read());
		}
		digitalWrite(13, LOW);
	}

	if(tgps.time.isValid() && tgps.time.isUpdated()) {
		timestamp = dateToUnix(tgps.date.year(), tgps.date.month(), tgps.date.day(), tgps.time.hour(), tgps.time.minute(), tgps.time.second());
		Serial.print("Timestamp: ");
		Serial.println(timestamp);
	}

	if(tgps.location.isValid() && tgps.location.isUpdated()) {
		lat = tgps.location.lat();
		lon = tgps.location.lng();

		Serial.print("Location: ");
		Serial.print(lat);
		Serial.print(" | ");
		Serial.println(lon);
	}

}
