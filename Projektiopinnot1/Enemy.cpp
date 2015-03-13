#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"
#include "Component.h"
#include "Bullet.h"
#include "Turret.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector) : refVector(rVector), Object(windowref, game)
{
	typeOfEnemy = irandom(1, 2); //remove randomness later
	enemyInitialize();

	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;

	followingDistance = 250;
	detectionDistance = 600;
	maxTurnSpeed = 0.07;
	snappingAngle = 0.2;

}


Enemy::~Enemy()
{
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
}


bool Enemy::update()
{	
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	playerDirection = -1 * atan2(mGame->playerObj->y - y, mGame->playerObj->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);
	
	//limit speed
	if (xSpeed > 5)
		xSpeed = 5;
	else if (ySpeed > 5)
		ySpeed = 5;
	if (xSpeed < -5)
		xSpeed = -5;
	else if (ySpeed < -5)
		ySpeed = -5;


	//limit turnSpeed
	if (turnSpeed > maxTurnSpeed)
		turnSpeed = maxTurnSpeed;
	else if (turnSpeed < -maxTurnSpeed)
		turnSpeed = -maxTurnSpeed;
	
	//update AI accordingly
	updateAI();

	updateComponents();
	
	return Object::update();
}


void Enemy::enemyAI()
{
	distance = getDistance(x, y, mGame->playerObj->x, mGame->playerObj->y);

	//"fix collision"
	if (distance < this->textureRadius + mGame->playerObj->textureRadius)
	{
		//somethingsomething..?
		std::cout << "oops!" << std::endl;
	}

	if (distance > followingDistance && distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance/700);
		ySpeed += sin(2 * PI - angle)*(distance/700);

		if (timer == 6)
		{
			if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
			{
				components[0]->fire();
				timer = 0;
			}
		}
		else
			timer++;
	}
	else if (distance < followingDistance)
	{
		follow = true;
		if (xSpeed < 0)
			xSpeed -= 0.2;
		else if (xSpeed > 0)
			xSpeed += 0.2;
		if (ySpeed < 0)
			ySpeed -= 0.2;
		else if (ySpeed > 0)
			ySpeed += 0.2;
	}
	else if (distance > detectionDistance)
	{
		follow = false;
		xSpeed = xSpeed*0.96;
		ySpeed = ySpeed*0.96;
		if (xSpeed > -0.01 && xSpeed < 0.01)
		{
			xSpeed = 0;
		}
		if (ySpeed > -0.01 && ySpeed < 0.01)
		{
			ySpeed = 0;
		}
	}


	if (follow == true)
	{
		if (angle < playerDirection - snappingAngle)
		{
			turnSpeed += maxTurnSpeed/4;
		}
		else if (angle > playerDirection + snappingAngle)
		{
			turnSpeed -= maxTurnSpeed/4;
		}

		if (angle / playerDirection > 1.1 || angle / playerDirection < 0.9)
		{
			
		}
	}
	else
	{
		if (turnSpeed > 0.005)
		{
			turnSpeed = turnSpeed*0.9;
			//?
		}
	}

}


void Enemy::updateComponents()
{
	for (componentIt = components.begin(); componentIt != components.end();)
		if ((*componentIt)->update() == false)
		{
		delete (*componentIt);
		componentIt = components.erase(componentIt);
		}
		else
		{
			++componentIt;
		}
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}


void Enemy::bomberAI()
{
	distance = getDistance(x, y, mGame->playerObj->x, mGame->playerObj->y);

	//"fix collision"
	if (distance < this->textureRadius + mGame->playerObj->textureRadius + 10)
	{
		//explosion?
		this->hp = 0;
		std::cout << "boom!" << std::endl;
	}

	if (distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance / 700);
		ySpeed += sin(2 * PI - angle)*(distance / 700);				
	}
	else if (distance > detectionDistance)
	{
		follow = false;
		xSpeed = xSpeed*0.99;
		ySpeed = ySpeed*0.99;
	}


	if (follow == true)
	{
		if (angle < playerDirection - snappingAngle)
		{
			turnSpeed += maxTurnSpeed / 4;
		}
		else if (angle > playerDirection + snappingAngle)
		{
			turnSpeed -= maxTurnSpeed / 4;
		}

		if (angle / playerDirection > 1.1 || angle / playerDirection < 0.9)
		{

		}
	}
	else
	{
		if (turnSpeed > 0.005)
		{
			turnSpeed = turnSpeed*0.9;
			turnSpeed = -turnSpeed;
			//?
		}
	}

}


void Enemy::updateAI()
{
	if (typeOfEnemy == 1)
		enemyAI();
	else if (typeOfEnemy == 2)
		bomberAI();
}


void Enemy::enemyInitialize()
{
	if (typeOfEnemy == 1)
	{
		tex.loadFromFile("Texture/enemy_base_green.png");
		components.push_back(new Turret(this, centerObj, 0, 0));
	}
	else if (typeOfEnemy == 2)
	{
		tex.loadFromFile("Texture/enemy_base.png");
	}
}