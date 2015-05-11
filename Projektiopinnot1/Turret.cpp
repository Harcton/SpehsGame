#include "Main.h"
#include "Turret.h"
#include "Object.h"
#include "Game.h"


Turret::~Turret()
{//Cannot be called, see player/enemy destructor
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo), laserPointer(sf::Lines, 2)
{
	//Default stats
	maxAngle = PI/7;
	turningSpeed = PI/240;
	capacity = 5;			// magazine/thrust charge
	rechargeInterval = 30;	//Reload/thrust recharge speed
	maxSpeed = 11;			//max bullet speed
	damage = 15;			//Turret damage
	fireRateInterval = 60;



	bulletTexPtr = &RM.bullet1Tex;
	types.push_back(component::turret);
	mouseAim = true;
	textureRadius = 20;	
	canFireTimer = 0;
	magazine = capacity;
	laserPointer[0].color = sf::Color(195, 40, 0, 220);
	laserPointer[1].color = sf::Color(195, 40, 0, 220);
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
	if (maxAngle != PI)
	{
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
	}
	



	//Update magazine
	if (reloading == false)
	{
		magazineSpr.setOrigin(30 + int(20.0f*(magazine / float(capacity))), 50);
		magazineSpr.setColor(sf::Color(255, 255, 255, 255));
	}
	else
	{
		magazineSpr.setOrigin(50, 50);
		magazineSpr.setColor(sf::Color(100, 100, 160, 150));
	}
	magazineSpr.setPosition(screenX, screenY);
	magazineSpr.setRotation(360 - ((angle) / PI) * 180);
	magazineSpr.setScale(resFactor*zoomFactor, resFactor*zoomFactor);

	if (maxSpeed == 50)
	{
		laserPointer[0].position = sf::Vector2f(screenX, screenY);
		laserPointer[1].position = sf::Vector2f(screenX + cos(angle)*(WINDOW_WIDTH+WINDOW_HEIGHT), screenY - sin(angle)*(WINDOW_WIDTH+WINDOW_HEIGHT));
		master->mGame->mWindow.draw(laserPointer);
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
			canFireTimer = capacity*rechargeInterval;
		}
		return;
	}
	
	if (master->mGame->bullets.size() >= MAX_BULLETS)
		return;

	magazine--;
	canFireTimer = fireRateInterval;
	master->mGame->bullets.push_back(new Object(master->mGame, master, x, y, 2 * PI - angle, maxSpeed, damage));
	master->mGame->bullets.back()->spr.setTexture(*bulletTexPtr);
	hasFired = true;
}
void Turret::reload()
{
	reloading = true;
	canFireTimer = capacity*rechargeInterval;
}

//VIRTUAL GETTERS
void Turret::setLaserPointerColor(sf::Color&& c)
{
	laserPointer[0].color = c;
	laserPointer[1].color = c;
}
