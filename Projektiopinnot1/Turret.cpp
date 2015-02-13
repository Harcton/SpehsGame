#include "Main.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"

Turret::~Turret()
{
	while (!bullets.empty())
	{
		delete bullets.back();
		bullets.pop_back();
	}
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo)
{
	tex.loadFromFile("Texture/turret.png");
	spr.setTexture(tex);
	spr.setOrigin(20, 50);

	types.push_back(turret);
}


bool Turret::update()
{

	updateBullets();

	if (Component::update() == false)
		return false;


	return true;
}
void Turret::updateBullets()
{
	//Bullet update
	for (bulletIt = bullets.begin(); bulletIt != bullets.end();)
		if ((*bulletIt)->update() == false)
		{
		delete (*bulletIt);
		bulletIt = bullets.erase(bulletIt);
		}
		else
		{
			++bulletIt;
		}
	for (unsigned int i = 0; i < bullets.size(); i++)
		bullets[i]->draw();
}

void Turret::fire()
{
	bullets.push_back(new Bullet(this, this->master->mWindow, this->master->mGame, this->x, this->y, this->angle, 25));
}