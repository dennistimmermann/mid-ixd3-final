#include "utils.h"

tm now = {0, 0, 0, 1, 0, 0}; //0,0,0

long dateToUnix(int year, int month, int day, int hour, int minute, int second) {
	now.tm_year = year-1900;	//tm starts at 1900
	now.tm_mon = month-1;		//tm_mon starts at 0
	now.tm_mday = day;
	now.tm_hour = hour;
	now.tm_min = minute;
	now.tm_sec = second;

	return (long) mktime(&now);
}
