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

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&);
	~Enemy();

	virtual bool update();
	virtual bool updateComponents();
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
	double accelerationConstant;//fix these constant problems (pun intented)
	double turnAccelerationConstant;
	std::vector<Component*>::iterator componentIt;
	
	//Ranges
	int aggroRange;
	int maxActionRange;
	int closeRange;

	//AI
	bool follow;
	int HPMemory;
	int enemyBehaviourLevel; //1-10, 1=defensive 10=aggressive
	double distance;
	double playerDirection;
	Component* nearestComponent;
};

#endif