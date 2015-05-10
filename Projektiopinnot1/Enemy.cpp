#include "Main.h"
#include "Enemy.h"
#include "Game.h"
#include "Player.h"



Enemy::Enemy(sf::RenderWindow& windowref, Game* game) : Object(windowref, game)
{
	metal = 1;
}


Enemy::~Enemy()
{
}


bool Enemy::update()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;
	if (components.size() <= 0)
	{//All components destroyed, chance to drop metal
		int dropAmount = irandom(0, metal);
		if (dropAmount > 0)
			centerObj->dataPtr->money += dropAmount;
		return false;
	}
	

	complexUpdate();
	updateComponents();


	distance = getDistance(this->x, this->y, nearestComponent->x, nearestComponent->y);
	playerDirection = -1 * atan2(nearestComponent->y - y, nearestComponent->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);

	//limit turnSpeed
	if (turnSpeed > maxTurnSpeedLimit)
		turnSpeed = maxTurnSpeedLimit;
	else if (turnSpeed < -maxTurnSpeedLimit)
		turnSpeed = -maxTurnSpeedLimit;

	//limit speed
	if (this->xSpeed > maxSpeedLimit)
		this->xSpeed = maxSpeedLimit;
	else if (this->ySpeed > maxSpeedLimit)
		this->ySpeed = maxSpeedLimit;
	if (this->xSpeed < -maxSpeedLimit)
		this->xSpeed = -maxSpeedLimit;
	else if (this->ySpeed < -maxSpeedLimit)
		this->ySpeed = -maxSpeedLimit;
	
	//follow
	if (follow == true) //set angle to match playerdirection
	{
		if (angle >= 0 && angle < PI / 2) //1st quarter
		{
			if (playerDirection < PI && playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle || playerDirection > PI*1.5)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle + PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
		else if (angle >= PI / 2 && angle < PI) //2nd quarter
		{
			if (playerDirection < PI*1.5 && playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection > angle + PI)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else
			{
				turnSpeed += turnAccelerationConstant;
			}
		}
		else if (angle >= PI && angle < PI*1.5)//3rd quarter
		{
			if (playerDirection > angle)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection < angle && playerDirection > PI / 2)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
		else //4th quarter
		{
			if (playerDirection > angle || playerDirection < PI / 2)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else if (playerDirection > PI && playerDirection < angle)
			{
				turnSpeed -= turnAccelerationConstant;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += turnAccelerationConstant;
			}
			else
			{
				turnSpeed -= turnAccelerationConstant;
			}
		}
	}
	else
	{
		if (turnSpeed > 0.001)
		{
			turnSpeed = turnSpeed*0.9;
		}
	}

	return Object::update();
}


void Enemy::notifyComponentDestruction(Component* component)
{
	//Erase child id from it's parents' memory
	for (unsigned int i = 0; i < components.size(); i++)
		for (unsigned int c = 0; c < components[i]->childComponents.size(); c++)
			if (components[i]->childComponents[c] == component->id)
			{
				components[i]->childComponents.erase(components[i]->childComponents.begin() + c);
			}
}





void Enemy::complexUpdate()
{
	double tempDistance = 0;
	int tempIndex = -1;

	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (i == 0)
		{
			tempDistance = getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
			tempIndex = i;
		}
		else if (tempDistance > getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y))
		{
			tempDistance = getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
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


void Enemy::explosion(int dmg, float explosionRadius)
{
	if (components.size() < 1)
		return;//Prevents fatal vector out of range error happening (components[0])
	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (getDistance(this->components[0]->x, this->components[0]->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y) 
			< this->components[0]->textureRadius * explosionRadius)
		{
			mGame->playerObj->components[i]->hp -= dmg;
		}
	}
}


void Enemy::shootLaser(int dmg)
{
	if (components.size() < 1)
		return;
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(this->screenX + resFactor*zoomFactor, this->screenY + resFactor*zoomFactor);
	line[1].position = sf::Vector2f(nearestComponent->screenX + (irandom(-15, 15))*resFactor*zoomFactor, nearestComponent->screenY + (irandom(-15, 15))*resFactor*zoomFactor);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= dmg;

	mWindow.draw(line);
}