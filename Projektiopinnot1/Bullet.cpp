#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"
#include "Bullet.h"


Bullet::~Bullet()
{
}
Bullet::Bullet(Component* mstr, sf::RenderWindow& w, Game* game, int x, int y, double angl, double spd) : Object(w, game, x, y)
{
	master = mstr;
	tex.loadFromFile("Texture/bullet1.png");
	spr.setTexture(tex);
	speed = spd;

	angle = angl;
	xSpeed = cos(2 * PI - angle)*speed + master->master->xSpeed;
	ySpeed = sin(2 * PI - angle)*speed + master->master->ySpeed;
}


bool Bullet::update()
{
	for (unsigned int i = 0; i < mGame->objects.size(); i++)
		if (mGame->objects[i] != master->master)
		{
		mGame->objects[i]->checkBulletCollision(this);
		}


	if (speed < 10)
		return false;
	
	return Object::update();
}
