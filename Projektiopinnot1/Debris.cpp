#include "Main.h"
#include "Game.h"
#include "Component.h"
#include "Debris.h"

Debris::~Debris()
{
}
Debris::Debris(sf::RenderWindow& windowref, Game* game) : Object(windowref, game)
{
	randomizeLook();
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, int cx, int cy) : Object(windowref, game, cx, cy)
{
	randomizeLook();
}

bool Debris::update()
{
	if (location.distanceFrom(centerObj->location) > DESPAWN_RANGE)
		return false;

	complexUpdate();

	if (nearestComponent != nullptr)
		distance = location.distanceFrom(nearestComponent->location);

	//limit speed
	if (this->xSpeed > maxSpeedLimit)
		this->xSpeed = maxSpeedLimit;
	else if (this->ySpeed > maxSpeedLimit)
		this->ySpeed = maxSpeedLimit;
	if (this->xSpeed < -maxSpeedLimit)
		this->xSpeed = -maxSpeedLimit;
	else if (this->ySpeed < -maxSpeedLimit)
		this->ySpeed = -maxSpeedLimit;

	hitTimer++;
	updateCollision();

	return Object::update();
}


void Debris::randomizeLook()
{
	spr.setTexture(RM.debrisTex);
	spr.setOrigin(50, 50);
	//spr.scale((float) (irandom(20, 5) / 10), (float) (irandom(20, 5) / 10));
	turnSpeed = (double)(irandom(-10, 10)/10) / double(360)*(2 * PI);
	textureRadius = 20;
	xSpeed = (double) (irandom(-30, 30) / 10);
	ySpeed = (double) (irandom(-30, 30) / 10);
	hp = 3;
	maxSpeedLimit = 12;
}


void Debris::updateCollision() //problems with components thingys...
{
	if (distance < this->textureRadius + nearestComponent->textureRadius)
	{
		if (hp > 1 && abs(this->xSpeed - centerObj->xSpeed) < 12 && abs(this->xSpeed - centerObj->ySpeed) < 12)
		{
			xSpeed = abs(this->xSpeed) + centerObj->xSpeed;
			ySpeed = abs(this->ySpeed) + centerObj->ySpeed;
			if (hitTimer > 60)
			{
				hp--;
				hitTimer = 0;
			}
		}
		else
		{
			//destroying animation
			hp--;
		}

	}
}


void Debris::complexUpdate()
{
	double tempDistance = 0;
	int tempIndex = -1;

	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (i == 0)
		{
			tempDistance = location.distanceFrom(centerObj->components[i]->location);
			tempIndex = i;
		}
		else if (tempDistance > location.distanceFrom(centerObj->components[i]->location))
		{
			tempDistance = location.distanceFrom(centerObj->components[i]->location);
			tempIndex = i;
		}
	}
	if (tempIndex >= 0)
	{
		nearestComponent = mGame->playerObj->components[tempIndex];
	}
	else
	{
		if (components.size() > 0)
			nearestComponent = components[0];
		else
			nearestComponent = nullptr;
	}
}