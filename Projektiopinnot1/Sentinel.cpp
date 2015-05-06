#include "Main.h"
#include "Sentinel.h"
#include "Game.h"



Sentinel::Sentinel(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	dodging = false;
	repositioning = false;
	fleeing = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 600;
	closeRange = 350;
	maxTurnSpeedLimit = 0.05;
	maxSpeedLimit = 6;
	accelerationConstant = 0.8;
	turnAccelerationConstant = 0.005;
	closeAngle = 0.002;

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("Sentinel.png"));
	components.back()->sprites.back().setOrigin(130, 75);
}


Sentinel::~Sentinel()
{
}


bool Sentinel::update()
{
	//Counters
	dodgeCounter--;
	if (dodgeCounter < 0)
	{
		dodging = false;
	}

	laserCounter++;

	rotationCounter += irandom(0, 2);
	if (rotationCounter <= 20)
	{
		rotationDirection = true;
	}
	else if (rotationCounter > 20 && rotationCounter < 60)
	{
		rotationDirection = false;
	}
	else if (rotationCounter > 60 && rotationCounter < 100)
	{
		rotationDirection = irandom(0,1);
	}
	else if (rotationCounter > 100)
	{
		rotationCounter = 0;
	}

	repositionCounter++;
	if (repositionCounter < 0)
	{
		repositioning = false;
	}

	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}

	return Enemy::update();
}


void Sentinel::AIupdate()
{
	if (dodging)
	{
		dodgeMove();
	}
	else if (repositioning)
	{
		//
	}
	else if (fleeing)
	{
		//
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed += -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed += -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		if (rotationDirection == true)
		{
			if (components.size() > 0)
			{
				if (HPMemory > components[0]->hp && follow == true)
				{
					follow = false;
					dodging = true;
					dodgeCounter = 60;
				}
			}
			
			else
			{
				follow = true;

				xSpeed += (-sin(angle))*accelerationConstant;
				ySpeed += (-cos(angle))*accelerationConstant;
			}
		}
		else if (rotationDirection == false)
		{
			if (components.size() > 0)
			{
				if (HPMemory > components[0]->hp && follow == true)
				{
					follow = false;
					dodging = true;
					dodgeCounter = 60;
				}
			}
			else
			{
				follow = true;

				xSpeed += (sin(angle))*accelerationConstant;
				ySpeed += (cos(angle))*accelerationConstant;
			}
		}

		if (laserCounter >= 10)
		{
			if (angle < playerDirection + closeAngle || angle > -playerDirection - closeRange)
			{
				shootLaser(3);
				laserCounter = irandom(-25, -15);
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
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
}


void Sentinel::dodgeMove()
{//work very much in progress over here
	//check player acceleration
	if (angle >= 0 && angle < PI / 2) //1st quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed += (sin(angle))*accelerationConstant;
			ySpeed += (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed += (sin(angle))*accelerationConstant;
			ySpeed += (cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI / 2 && angle < PI) //2nd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed += (sin(angle))*accelerationConstant;
			ySpeed += (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed += (sin(angle))*accelerationConstant;
			ySpeed += (-cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI && angle < PI*1.5)//3rd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed += (-sin(angle))*accelerationConstant;
			ySpeed += (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed += (-sin(angle))*accelerationConstant;
			ySpeed += (-cos(angle))*accelerationConstant;
		}
	}
	else //4th quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed += (-sin(angle))*accelerationConstant;
			ySpeed += (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed += (-sin(angle))*accelerationConstant;
			ySpeed += (cos(angle))*accelerationConstant;
		}
	}
}


void Sentinel::reposition()
{
	//TBD
}


void Sentinel::flee()
{
	//TBD
}