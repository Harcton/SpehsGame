#include "Enemy.h"
#include "Sentinel.h"


Sentinel::Sentinel(sf::RenderWindow&, Game*, std::vector<Object*>&, int behaviourLevel) : refVector(rVector), Object(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	dodging = false;
	repositioning = false;
	fleeing = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 600;
	closeRange = 350;
	maxTurnSpeedLimit = 0.015;
	maxSpeedLimit = 9;
	accelerationConstant;//?
	accelerationConstant;//?
	closeAngle;//set

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("enemy_base_purple.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);
}


Sentinel::~Sentinel()
{
}


bool Sentinel::update()
{
	//Counters
	dodgeCounter++;
	laserCounter++;
	repositionCounter++;
		//Behaviour counter!!
		//RotationDirection changes...


	AIupdate();
	HPMemory = components[0]->hp;

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
		xSpeed = -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed = -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		if (rotationDirection == true)
		{
			if (laserBChange == true && follow == true)//dunno mang..
			{
				follow = false;

				xSpeed = (-sin(angle))*accelerationConstant;
				ySpeed = (-cos(angle))*accelerationConstant;
				laserBChange = false;
				BCounter = 0;
				dodging = true;
				dodgeCounter = 60;
			}
			else
			{
				follow = true;

				xSpeed = (-sin(angle))*accelerationConstant + irandom(7, 13);
				ySpeed = (-cos(angle))*accelerationConstant + irandom(5, 10);
				BCounter = 0;
			}
		}
		else if (rotationDirection == false && BCounter > 80)
		{
			if (laserBChange == true && follow == true)
			{
				follow = false;

				xSpeed = (sin(angle))*accelerationConstant;
				ySpeed = (cos(angle))*accelerationConstant;
				laserBChange = false;
				BCounter = 0;
				dodging = true;
				dodgeCounter = 60;
			}
			else
			{
				follow = true;

				xSpeed = (sin(angle))*accelerationConstant + irandom(7, 13);
				ySpeed = (cos(angle))*accelerationConstant + irandom(5, 10);
				BCounter = 0;
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
		xSpeed = cos(2 * PI - angle)*maxSpeed;
		ySpeed = sin(2 * PI - angle)*maxSpeed;
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
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI / 2 && angle < PI) //2nd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
	}
	else if (angle >= PI && angle < PI*1.5)//3rd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (-cos(angle))*accelerationConstant;
		}
	}
	else //4th quarter
	{
		if (rotationDirection)
		{
			turnSpeed += turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= turnAccelerationConstant;
			xSpeed = (-sin(angle))*accelerationConstant;
			ySpeed = (cos(angle))*accelerationConstant;
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