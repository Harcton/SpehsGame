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

private:
	std::vector<Object*>& refVector;
	void enemyInitialize();
	void enemyAI();
	void bomberAI();
	void updateComponents();
	void updateAI();

	double playerDirection;
	bool follow;
	double distance;
	int detectionDistance;
	int followingDistance;
	double maxTurnSpeed;
	double snappingAngle;
	int timer;

	int typeOfEnemy;

	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;
};

#endif