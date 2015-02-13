#ifndef BULLET_H
#define BULLET_H


class Game;
class Component;

class Bullet : public Object
{
public:
	Bullet(Component*, sf::RenderWindow&, Game*, int, int, double, double);
	~Bullet();

	bool update();

private:
	Component* master;
	double speed;
	bool canDamage = true;
	int damage = 5;

};

#endif