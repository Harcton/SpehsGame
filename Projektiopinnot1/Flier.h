#ifndef FLIER_H
#define FLIER_H
//Included dependencies
#include "Commander.h"



class Flier : public Enemy
{
public:
	Flier(sf::RenderWindow&, Game*, int);
	~Flier();

	bool update();
	void AIupdate();
	void reposition();

private:
	bool repositioning;
	int repositionCounter;
	int laserCounter;
	int rotationCounter;
	int behaviourCounter;
	bool repositioningDirection;
	bool initiateAssault;
};

#endif