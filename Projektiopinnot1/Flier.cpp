#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"
#include "Flier.h"
#include "Commander.h"
#include "Component.h"


Flier::Flier(sf::RenderWindow& windowref, Game* game, int behaviourLevel, Commander* master) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	fleetMaster = master;

	repositioning = false;
	initiateAssault = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 90;
	closeRange = 75;
	maxTurnSpeedLimit = 0.05;
	maxSpeedLimit = 4;
	accelerationConstant = 0.4;
	turnAccelerationConstant = 0.005;
	closeAngle = 0.003;

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("Flier.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);
	components[components.size() - 1]->textureRadius = 30;
	components[components.size() - 1]->maxHp = 30;
}


Flier::~Flier()
{
}


bool Flier::update()
{
	//Counters
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
		rotationDirection = irandom(0, 1);
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
	if (components.size()>0)
	{
		HPMemory = components[0]->hp;
	}

	return Enemy::update();
}


void Flier::AIupdate()//change behaviour when not fighting
{
	if (fleetMaster->initiateFlierAssault == true)
	{
		this->initiateAssault = true;
	}

	if (repositioning)
	{
		reposition();
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed += -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed += -(sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		follow = true;
		if (rotationDirection == true) //rotation direction to be fixed?
		{
			xSpeed += (-sin(angle))*accelerationConstant;
			ySpeed += (-cos(angle))*accelerationConstant;
		}
		else if (rotationDirection == false)
		{
			xSpeed += (sin(angle))*accelerationConstant;
			ySpeed += (cos(angle))*accelerationConstant;
		}

		if (laserCounter >= 10)
		{
			if (angle < playerDirection + closeAngle || angle > -playerDirection - closeRange)
			{
				shootLaser(1);
				laserCounter = irandom(-12, -8);
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		if (initiateAssault == false)
		{
			follow = false;
			//Follow the Commander ship!
			//Not actually following !!!??!
			if (angle >= 0 && angle < PI / 2) //1st quarter
			{
				/*if (flierMaster->angle < PI && flierMaster->angle > angle)
				{
				turnSpeed += maxTurnSpeed;
				}
				else if (flierMaster->angle < angle || flierMaster->angle > PI*1.5)
				{
				turnSpeed -= maxTurnSpeed;
				}
				else if (flierMaster->angle < angle + PI)
				{
				turnSpeed += maxTurnSpeed;
				}
				else
				{
				turnSpeed -= maxTurnSpeed;
				}*/
			}
			else if (angle >= PI / 2 && angle < PI) //2nd quarter
			{
				/*if (flierMaster->angle < PI*1.5 && flierMaster->angle > angle)
				{
				turnSpeed += maxTurnSpeed;
				}
				else if (flierMaster->angle < angle)
				{
				turnSpeed -= maxTurnSpeed;
				}
				else if (flierMaster->angle > angle + PI)
				{
				turnSpeed -= maxTurnSpeed;
				}
				else
				{
				turnSpeed += maxTurnSpeed;
				}*/
			}
			else if (angle >= PI && angle < PI*1.5)//3rd quarter
			{
				/*if (flierMaster->angle > angle)
				{
				turnSpeed += maxTurnSpeed;
				}
				else if (flierMaster->angle < angle && flierMaster->angle > PI / 2)
				{
				turnSpeed -= maxTurnSpeed;
				}
				else if (flierMaster->angle < angle - PI)
				{
				turnSpeed += maxTurnSpeed;
				}
				else
				{
				turnSpeed -= maxTurnSpeed;
				}*/
			}
			else //4th quarter
			{
				/*if (flierMaster->angle > angle || flierMaster->angle < PI / 2)
				{
				turnSpeed += maxTurnSpeed;
				}
				else if (flierMaster->angle > PI && flierMaster->angle < angle)
				{
				turnSpeed -= maxTurnSpeed;
				}
				else if (flierMaster->angle < angle - PI)
				{
				turnSpeed += maxTurnSpeed;
				}
				else
				{
				turnSpeed -= maxTurnSpeed;
				}*/
			}
			xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
			ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
			turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
		}
		else if (initiateAssault == true)
		{
			follow = true;
			xSpeed += cos(2 * PI - angle)*accelerationConstant;
			ySpeed += sin(2 * PI - angle)*accelerationConstant;
		}
	}
	else //Passive state
	{
		follow = false;
		initiateAssault = false;
		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
	}
}


void Flier::reposition()
{
	//TBD
}