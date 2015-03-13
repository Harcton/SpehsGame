#ifndef ENEMY_H
#define ENEMY_H

class Object;
class Component;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&);
	~Enemy();

	bool update();
	void complexUpdate();
	void checkBulletCollision(Bullet*);
	void removeComponent(int);

private:
	std::vector<Object*>& refVector;
	void enemyInitialize();
	void enemyAI();
	void bomberAI();
	void updateComponents();
	void updateAI();
	void explosion();

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

	int typeOfEnemy;

	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;
};

#endif