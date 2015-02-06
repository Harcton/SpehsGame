#ifndef ENEMY_H
#define ENEMY_H

class Object;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&);
	~Enemy();

	bool update();
	void enemyAI();

private:
	std::vector<Object*>& refVector;
	double playerDirection;
	bool follow;
	double distance;
	int detectionDistance;
	int followingDistance;
	double maxTurnSpeed;
	double snappingAngle;
};

#endif