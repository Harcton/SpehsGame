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
		collisionCheckAngle = -1 * atan2(mGame->objects[i]->y - y, mGame->objects[i]->x - x);
		if (collisionCheckAngle < 0)
			collisionCheckAngle = ((2 * PI) + collisionCheckAngle);


		checkCollisionDistance = getDistance(x, y, mGame->objects[i]->x, mGame->objects[i]->y);
		checkCollisionRange = textureRadius + mGame->objects[i]->textureRadius;

		if (checkCollisionDistance < checkCollisionRange)
		{
			if (canDamage == true)
			{
				mGame->objects[i]->getHpRef() -= damage;
				canDamage = false;
				mGame->objects[i]->x += 2 * cos(angle);
				mGame->objects[i]->y += -2*sin(angle);
			}
				
			speed = speed*0.75;
			if (speed < 3)
				return false;

			angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			xSpeed = cos(2 * PI - angle) * speed;
			ySpeed = sin(2 * PI - angle) * speed;
		}
		}


	
	return Object::update();
}
