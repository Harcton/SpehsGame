#ifndef COMMANDER_H
#define COMMANDER_H
//Included dependencies
#include "Enemy.h"



class Commander : public Enemy
{
public:
	Commander(sf::RenderWindow&, Game*, int);
	~Commander();

	bool update();
	void AIupdate();
	void flee();
	void launchFliers();

private:
	bool fleeing;
	bool initiateFlierAssault;
	int shootingCounter;
	int rotationCounter;
	int releaseFlier;
};

#endif