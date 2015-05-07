#include "Main.h"
#include "Turret.h"
#include "Object.h"
#include "Game.h"


Turret::~Turret()
{//Cannot be called, see player/enemy destructor
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo)
{
	sprites.push_back(sf::Sprite());
	sprites.back().setTexture(RM.getTexture("turret.png"));
	sprites.back().setOrigin(20, 50);
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
	
	if (master->mGame->bullets.size() >= MAX_BULLETS)
		return;

	magazine--;
	canFireTimer = fireRateInterval;
	master->mGame->bullets.push_back(new Object(master->mGame, master, x, y, 2 * PI - angle, 15.0f, damage));
	hasFired = true;
}
void Turret::reload()
{
	reloading = true;
	canFireTimer = rechargeInterval;
}

