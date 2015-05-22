#include "Location.h"
#include "Main.h"

Location::~Location()
{
}
Location::Location()
{
	x = 0;
	y = 0;
}
Location::Location(float mx, float my)
{
	x = mx;
	y = my;
}
void Location::setLocation(float newX, float newY)
{
	x = newX;
	y = newY;
}
void Location::setLocation(Location& newLocation)
{
	x = newLocation.x;
	y = newLocation.y;
}

float Location::distanceFrom(const Location& other)
{
	float xDiff = abs(x - other.x);
	float yDiff = abs(y - other.y);

	return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

float Location::angleTowardsRad(Location& other)
{
	float result = atan2(y - other.y, x - other.x);
	if (result < 0)
		result = abs(result);
	else
		result = PI * 2 - result;
	return result;
}
float Location::angleTowardsDeg(Location& other)
{
	return angleTowardsRad(other)*(180/PI);
}


