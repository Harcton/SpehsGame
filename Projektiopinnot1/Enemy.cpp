#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector) : refVector(rVector), Object(windowref, game)
{
	tex.loadFromFile("Texture/enemy_base.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;

	followingDistance = 250;
	detectionDistance = 600;
	maxTurnSpeed = 0.07;
	snappingAngle = 0.5;
}


Enemy::~Enemy()
{
		
}


bool Enemy::update()
{	
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	playerDirection = -1 * atan2(mGame->playerObj->y - y, mGame->playerObj->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);
	
	//limit the speed
	if (xSpeed > 5)
	{
		xSpeed = 5;
	}
	else if (ySpeed > 5)
	{
		ySpeed = 5;
	}
	if (xSpeed < -5)
	{
		xSpeed = -5;
	}
	else if (ySpeed < -5)
	{
		ySpeed = -5;
	}


	//limit turnSpeed
	if (turnSpeed > maxTurnSpeed)
	{
		turnSpeed = maxTurnSpeed;
	}
	else if (turnSpeed < -maxTurnSpeed)
	{
		turnSpeed = -maxTurnSpeed;
	}
		
	enemyAI();
	
	return Object::update();
}


void Enemy::enemyAI()
{
	distance = getDistance(x, y, mGame->playerObj->x, mGame->playerObj->y);

	if (distance > followingDistance && distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance/700);
		ySpeed += sin(2 * PI - angle)*(distance/700);
	}
	else if (distance < followingDistance)
	{
		follow = true;
		xSpeed -= -xSpeed*0.01;
		ySpeed -= -xSpeed*0.01;
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
		if (angle < playerDirection-snappingAngle)
		{
			turnSpeed += maxTurnSpeed/3;
		}
		else if (angle > playerDirection+snappingAngle)
		{
			turnSpeed -= maxTurnSpeed/3;
		}
	}
	else
	{
		if (turnSpeed > 0.005)
		{
			turnSpeed = turnSpeed*0.9;
			angle -= turnSpeed;
			
		}
	}
}