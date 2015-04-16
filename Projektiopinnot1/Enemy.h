#ifndef ENEMY_H
#define ENEMY_H

enum TypeOfAI
{
	et_standard,
	et_laser,
	et_bomber,
	et_commander
};

class Object;
class Component;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&, TypeOfAI);
	~Enemy();

	bool update();
	void checkBulletCollision(Bullet*);
	void removeComponent(int); 
	void notifyComponentDestruction(int);

private:
	std::vector<Object*>& refVector;
	void enemyInitialize();
	void enemyAI();
	void updateComponents();
	void explosion();
	void fireMahLazors();
	void launchFliers();
	void dodgeMove();

	Component* nearestComponent;
	double playerDirection;
	bool follow;
	double distance;

	int detectionDistance;	//aggro range
	int followingDistance;	//close range
	int actionDistance;		//action range

	double maxTurnSpeed; //more like normal turningspeed...
	double maxSpeed;
	double snappingAngle; //?
	bool rotationDirection; //true CW, false CCW

	bool laserBChange = false;
	int BCounter;
	int explosionTimer;
	int laserCounter;
	int timer;
	int tempHPMemory;
	
	TypeOfAI typeOfEnemy;

	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;
};

#endif