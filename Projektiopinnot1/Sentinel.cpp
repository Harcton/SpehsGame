#include "Main.h"
#include "Sentinel.h"
#include "Game.h"



Sentinel::Sentinel(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;
	metal = irandom(8, 14) + enemyBehaviourLevel;

	angle = playerDirection;
	std::cout << angle << " " << playerDirection << std::endl;
	dodging = false;
	repositioning = false;
	fleeing = false;
	aggroRange = 1850;
	maxActionRange = 600;
	closeRange = 350;
	maxTurnSpeedLimit = 0.05;
	maxSpeedLimit = 8;
	accelerationConstant = 0.8;
	turnAccelerationConstant = 0.005;
	closeAngle = 0.002;

	//Reserve memory for all of the components
	components.reserve(1);
	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.sentinelTex);
	components.back()->sprites.back().setOrigin(130, 75);
	components.back()->textureRadius = 50;
}


Sentinel::~Sentinel()
{
}


bool Sentinel::update()
{
	//When player is kill
	if (mGame->playerObj->components.size() == 0)
	{
		state = state_victory;
	}

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
	memoryState = state;

	return Enemy::update();
}


void Sentinel::AIupdate()
{
	//Special cases
	if (state == state_spawned)
	{
		xSpeed = (cos(2 * PI - angle))*(maxSpeedLimit / 3);
		ySpeed = (sin(2 * PI - angle))*(maxSpeedLimit / 3);
		state = state_passive;
		return;
	}
	if (state == state_victory)
	{
		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
		return;
	}


	if (dodging)
	{
		state = state_dodging;

		dodgeMove();
	}
	else if (repositioning)
	{
		state = state_repositioning;

		//
	}
	else if (fleeing)
	{
		state = state_fleeing;

		flee();
	}
	else if (distance < closeRange) //Close state
	{
		state = state_closeRange;

		follow = true;
		xSpeed += -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed += -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		state = state_active;

		if (stationDistance < 2000)
		{
			fleeing = true;
			return;
		}

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
				shootLaser(2+enemyBehaviourLevel);
				laserCounter = irandom(-25, -15);
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		state = state_detected;

		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
	{
		state = state_passive;

		follow = false;

		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
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
	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;
}