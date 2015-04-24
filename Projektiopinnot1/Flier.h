#ifndef FLIER_H
#define FLIER_H

class Commander;

class Flier : public Enemy
{
public:
	Flier(sf::RenderWindow&, Game*, int, Commander*);
	~Flier();

	bool update();
	void AIupdate();
	void reposition();

private:
	bool repositioning;
	int repositionCounter;
	int laserCounter;
	int rotationCounter;
	bool initiateAssault;
	Commander* fleetMaster;
};

#endif