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
	void notifyComponentDestruction(int){};

private:
	std::vector<Object*>& refVector;
	void enemyInitialize();
	void enemyAI();
	void updateComponents();
	void explosion();
	void fireMahLazors();

	Component* nearestComponent;
	double playerDirection;
	bool follow;
	double distance;
	int detectionDistance;
	int followingDistance;
	double maxTurnSpeed;
	double maxSpeed;
	double snappingAngle;
	int timer;
	
	TypeOfAI typeOfEnemy;

	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;
};

#endif