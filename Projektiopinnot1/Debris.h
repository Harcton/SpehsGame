#ifndef DEBRIS_H
#define DEBRIS_H
//Included dependencies
#include "Object.h"

class Debris : public Object
{
public:
	~Debris();
	Debris(sf::RenderWindow& windowref, Game* game);
	Debris(sf::RenderWindow& windowref, Game* game, int cx, int cy);
	virtual bool update();

	void randomizeLook();

protected:
	void complexUpdate();
	void updateCollision();

	int hitTimer = 0;
	double maxSpeedLimit;
	double distance;
	Component* nearestComponent;
};

#endif