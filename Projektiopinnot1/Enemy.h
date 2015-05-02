#ifndef ENEMY_H
#define ENEMY_H

//ENEMY STATES
//-Passive
//-Detected
//-Active
//-Close range
//-Dodging
//-Repositioning
//-Fleeing
//
//-Destroyed
//-Fled
//-Contact

class Object;
class Component;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*);
	~Enemy();

	virtual bool update();
	virtual void updateComponents();
	virtual void checkBulletCollision(Bullet*);
	virtual void removeComponent(int);
	virtual void notifyComponentDestruction(int);
	virtual void complexUpdate();

	virtual void explosion(int, float);
	virtual void shootLaser(int);

protected:
	//Object
	bool rotationDirection;
	double closeAngle;
	double maxSpeedLimit;
	double maxTurnSpeedLimit;
	double accelerationConstant;
	double turnAccelerationConstant;
	std::vector<Component*>::iterator componentIt;
	
	//Ranges
	int aggroRange;
	int maxActionRange;
	int closeRange;

	//AI
	bool follow;
	int HPMemory;
	int enemyBehaviourLevel; //1: defensive 2: passive 3: aggressive
	double distance;
	double playerDirection;
	Component* nearestComponent;
};

#endif