#ifndef COMMANDER_H
#define COMMANDER_H

class Commander : public Enemy
{
public:
	Commander(sf::RenderWindow&, Game*, std::vector<Object*>&, int);
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
	int fliersFollowing;
	int flierAttackCounter;
};

#endif