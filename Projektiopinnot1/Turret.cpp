#include "Main.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"

Turret::~Turret()
{//Cannot be called, see player/enemy destructor
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo)
{
	tex.loadFromFile("Texture/turret.png");
	spr.setTexture(tex);
	spr.setOrigin(20, 50);
	maxAngle = PI/3;
	turningSpeed = PI/40;

	types.push_back(component::turret);
	mouseAim = true;
	textureRadius = 20;
	
	canFireTimer = 0;

	capacity = 10;			// magazine/thrust charge
	magazine = capacity;
	rechargeInterval = 120;	//Reload/thrust recharge speed
	maxSpeed = 25;			//max bullet speed
	damage = 15;			//Turret damage
	fireRateInterval = 20;
}


bool Turret::update()
{
	updateBullets();

	canFireTimer--;	
	if (reloading == true && canFireTimer < 1)
	{//Reload complete
		reloading = false;
		magazine = capacity;
		hasFired = false;
	}

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
	if (canFireTimer > 0)
		return;
	if (magazine < 1)
	{
		if (reloading == false)
		{
			reloading = true;
			canFireTimer = rechargeInterval;
		}
		return;
	}
	
	magazine--;
	canFireTimer = fireRateInterval;
	bullets.push_back(new Bullet(this, this->master->mWindow, this->master->mGame, this->x, this->y, this->angle, maxSpeed));
	bullets[bullets.size() - 1]->damage = damage;
	hasFired = true;
}
void Turret::reload()
{
	reloading = true;
	canFireTimer = rechargeInterval;
}

