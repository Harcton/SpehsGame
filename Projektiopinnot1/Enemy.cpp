#include "Main.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Game.h"



Enemy::Enemy(sf::RenderWindow& windowref, Game* game) : Object(windowref, game)
{
}


Enemy::~Enemy()
{
}


bool Enemy::update()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;
	if (components.size() <= 0)
		return false;
	

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


void Enemy::updateComponents()
{
	componentIt = components.begin();
	while (componentIt != components.end())
	{
		if ((*componentIt)->alive() == false)
		{
			Component* temp_componentPointer = (*componentIt);
			components.erase(componentIt);
			delete temp_componentPointer;
			componentIt = components.begin();
		}
		else
			componentIt++;
	}

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->update();
}

void Enemy::draw()
{
	Object::draw();
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}

void Enemy::notifyComponentDestruction(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		for (unsigned int c = 0; c < components[i]->childComponents.size(); c++)
		{
			if (components[i]->childComponents[c] == cid)
			{
				components[i]->childComponents.erase(components[i]->childComponents.begin() + c);
			}
		}

	}
}


void Enemy::checkBulletCollision(Bullet* b)
{
	float temp_coordinateModifier = resFactor*zoomFactor*textureRadius;
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y - temp_coordinateModifier, components[i]->x - b->x - temp_coordinateModifier);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x - temp_coordinateModifier, components[i]->y - temp_coordinateModifier);
		b->checkCollisionRange = b->textureRadius + components[i]->textureRadius;

		if (b->checkCollisionDistance < b->checkCollisionRange)
		{
			if (b->canDamage == true)
			{
				components[i]->hp -= b->damage;
				b->canDamage = false;
				x += 6 * cos(angle);
				y += -6 * sin(angle);
			}

			//Bounce
			b->speed = b->speed*0.75;
			b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			b->xSpeed = cos(2 * PI - b->angle) * b->speed;
			b->ySpeed = sin(2 * PI - b->angle) * b->speed;
		}
	}
}


void Enemy::removeComponent(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->id == cid)
		{
			components[i]->hp = -999; //?
		}
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
		nearestComponent = components[0];
	}
}


void Enemy::explosion(int dmg, float explosionRadius)
{
	if (components.size() < 1)
		return;//Prevents fatal vector out of range error happening inside the for loop
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
	line[0].position = sf::Vector2f(this->screenX + components[0]->xOffset*resFactor*zoomFactor, this->screenY + components[0]->yOffset*resFactor*zoomFactor);
	line[1].position = sf::Vector2f(nearestComponent->screenX + (irandom(-15, 15))*resFactor*zoomFactor, nearestComponent->screenY + (irandom(-15, 15))*resFactor*zoomFactor);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= dmg;

	mWindow.draw(line);
}