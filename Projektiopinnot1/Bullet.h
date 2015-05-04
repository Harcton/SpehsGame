#ifndef BULLET_H
#define BULLET_H
//Included dependencies
#include "Object.h"



class Bullet : public Object
{
public:
	Bullet(Component*, sf::RenderWindow&, Game*, int, int, double, double);
	~Bullet();
	friend class Object;
	friend class Player;
	friend class Enemy;
	friend class Turret;

	bool update();

private:
	Component* master;
	double speed;
	bool canDamage = true;
	int damage = 15;

};

#endif