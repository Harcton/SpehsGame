#ifndef ENEMY_H
#define ENEMY_H

enum TypeOfAI
{
	et_standard,
	et_laser,
	et_bomber
};

class Object;
class Component;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&, TypeOfAI);
	~Enemy();

	bool update();
	void complexUpdate();
	void checkBulletCollision(Bullet*);
	void removeComponent(int);

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

	int complexUpdateTimer;
	int complexIndex;

	TypeOfAI typeOfEnemy;

	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;
};

#endif