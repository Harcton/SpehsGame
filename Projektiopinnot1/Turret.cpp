#include "Main.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"

Turret::~Turret()
{
	std::cout << "turretDeconstructor ";
	while (!bullets.empty())
	{
		delete bullets.back();
		bullets.pop_back();
	}
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo)
{
	std::cout << "turretConstructor ";
	tex.loadFromFile("Texture/turret.png");
	spr.setTexture(tex);
	spr.setOrigin(20, 50);
	maxAngle = PI/3;
	turningSpeed = PI/40;

	types.push_back(ct_turret);
	mouseAim = true;
	textureRadius = 20;
}


bool Turret::update()
{
	std::cout << "turretUpdate ";
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

	//Draw bullets
	for (unsigned int i = 0; i < bullets.size(); i++)
		bullets[i]->draw();
}

void Turret::fire()
{
	bullets.push_back(new Bullet(this, this->master->mWindow, this->master->mGame, this->x, this->y, this->angle, 25));
}


