#include "Location.cpp"

class Person {
	double bia;
	long t;
	Location * location;
public:
	Person(double,long);
};

Person::Person(double _bia, long _t) {
	bia = _bia;
	t = _t;
	Location l(0,0,0);
	location = &l;
}
