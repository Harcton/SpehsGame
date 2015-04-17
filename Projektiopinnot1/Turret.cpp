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
	spr.setTexture(RM.getTexture("turret.png"));
	spr.setOrigin(20, 50);
	maxAngle = PI/3;
	turningSpeed = PI/40;

	types.push_back(component::turret);
	mouseAim = true;
	textureRadius = 20;
	
	canFireTimer = 0;

	capacity = 20;			// magazine/thrust charge
	magazine = capacity;
	rechargeInterval = 120;	//Reload/thrust recharge speed
	maxSpeed = 25;			//max bullet speed
	damage = 15;			//Turret damage
	fireRateInterval = 5;
}


bool Turret::alive()
{
	return Component::alive();
}
void Turret::update()
{
	updateBullets();

	canFireTimer--;	
	if (reloading == true && canFireTimer < 1)
	{//Reload complete
		reloading = false;
		magazine = capacity;
		hasFired = false;
	}

	//Fix angle
	turretMinAngle = master->angle + angleModifier - maxAngle;
	turretMaxAngle = master->angle + angleModifier + maxAngle;
	if (turretMinAngle < 0)
		turretMinAngle += 2 * PI;	
	if (turretMinAngle > 2 * PI)
		turretMinAngle -= 2 * PI;
	if (turretMaxAngle > 2 * PI)
		turretMaxAngle -= 2 * PI;
	if (turretMaxAngle < 0)
		turretMaxAngle += 2 * PI;

	masterAngle = master->angle + angleModifier;
	if (masterAngle > 2 * PI)
		masterAngle -= 2 * PI;


	if (turretMaxAngle > turretMinAngle)
	{
		if (angle > turretMaxAngle || angle < turretMinAngle)
		{//If angle is out of range
			if (masterAngle < PI)
			{//master angle < 180
				if (angle < masterAngle + PI && angle > turretMaxAngle)
					angle = turretMaxAngle;
				else
					angle = turretMinAngle;
			}
			else
			{//master angle > 180
				if (angle > masterAngle - PI && angle < turretMinAngle)
					angle = turretMinAngle;
				else
					angle = turretMaxAngle;
			}
		}
	}
	else
	{//0-angle is interfering
		if (angle > turretMaxAngle && angle < turretMinAngle)
		{//If angle is out of range
			if (masterAngle < PI)
			{//master angle < 180
				if (angle < masterAngle + PI)
					angle = turretMaxAngle;
				else
					angle = turretMinAngle;
			}
			else
			{//master angle > 180
				if (angle > masterAngle - PI)
					angle = turretMinAngle;
				else
					angle = turretMaxAngle;
			}
		}
	}



	Component::update();
	return;
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

