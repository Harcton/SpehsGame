#ifndef LOCATION_H
#define LOCATION_H


class Location
{
public:
	~Location();
	Location();
	Location(float, float);
	void setLocation(float, float);
	void setLocation(Location&);
	float x;
	float y;
	float distanceFrom(const Location&);
	float angleTowardsDeg(Location&);
	float angleTowardsRad(Location&);
};

#endif