#ifndef SENTINEL_H
#define SENTINEL_H
//Included dependencies
#include "Enemy.h"



class Sentinel : public Enemy
{
public:
	Sentinel(sf::RenderWindow&, Game*, int);
	~Sentinel();

	bool update();
	void AIupdate();
	void dodgeMove();
	void reposition();
	void flee();

private:
	bool dodging;
	bool repositioning;
	bool fleeing;
	int dodgeCounter;
	int repositionCounter;
	int laserCounter;
	int rotationCounter;
};

#endif