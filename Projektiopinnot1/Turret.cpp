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
	//capacity				// magazine/thrust charge
	//rechargeInterval		//Reload/thrust recharge speed
	maxSpeed = 11;			//max bullet speed
	//damage				//Turret damage
	fireRateInterval = 30;
	chargeConsumption = 1;



	bulletTexPtr = &RM.bullet1Tex;
	type = component::turret;
	mouseAim = true;
	textureRadius = 0;	
	canFireTimer = 0;
	charge = capacity; //magazine
	laserPointer[0].color = sf::Color(195, 40, 0, 220);
	laserPointer[1].color = sf::Color(195, 40, 0, 220);
	hp = 99999999;
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
		charge = capacity;
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
		magazineSpr.setOrigin(30 + int(20.0f*(charge / float(capacity))), 50);
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
	}


	Component::update();
	return;
}

void Turret::draw()
{
	master->mGame->mWindow.draw(magazineSpr);
	Component::draw();
}

void Turret::drawLaserPointer()
{
	master->mGame->mWindow.draw(laserPointer);
}

void Turret::fire()
{
	if (canFireTimer > 0)
		return;
	if (charge < chargeConsumption)
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

	if (master == centerObj)
	{
		//Very weird sound atm??
		RM.turretFireSound.play();
	}

	charge -= chargeConsumption;
	canFireTimer = fireRateInterval;
	master->mGame->bullets.push_back(new Object(master->mGame, master, location.x, location.y, 2 * PI - angle, maxSpeed, damage));
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

void Turret::setBulletTexPtr(sf::Texture* texPtr)
{
	bulletTexPtr = texPtr;
}

sf::Sprite& Turret::getMagazineSpr()
{
	return magazineSpr;
}