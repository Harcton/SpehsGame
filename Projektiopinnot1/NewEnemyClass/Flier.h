#ifndef FLIER_H
#define FLIER_H

class Flier : public Enemy
{
public:
	Flier(sf::RenderWindow&, Game*, std::vector<Object*>&, int, Commander*);
	~Flier();

	bool update();
	void AIupdate();
	void reposition();

private:
	bool repositioning;
	int laserCounter;
	int rotationCounter;
	bool initiateAssault;
	Commander* fleetMaster;
};

#endif