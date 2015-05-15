#include "Main.h"
#include "Flier.h"
#include "Game.h"



Flier::Flier(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;
	metal = ((enemyBehaviourLevel + 4) / 4);

	initiateAssault = false;
	repositioning = false;
	aggroRange = 2500;
	maxActionRange = 90;
	closeRange = 75;
	maxTurnSpeedLimit = 0.06;
	maxSpeedLimit = 3;
	accelerationConstant = 0.5;
	turnAccelerationConstant = 0.005;
	closeAngle = 0.003;

	//Reserve memory for all of the components
	components.reserve(1);
	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	/*
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.flierTex);
	components.back()->sprites.back().setOrigin(15, 15);
	*/
	components.back()->textureRadius = 30;

	components.back()->hp = 25 + (enemyBehaviourLevel * 5);
	components.back()->maxHp = components.back()->hp;

	//animations
	components[0]->animatedSprites.push_back(RM.flierTex);
	components[0]->animatedSprites.back().setOrigin(15, 15);
	components[0]->animatedSprites.back().setFrameSize(30, 30);
	components[0]->animatedSprites.back().setTilesetSize(2, 2);
	components[0]->animatedSprites.back().setFrameDuration(2);
}


Flier::~Flier()
{
}


bool Flier::update()
{
	//When player is kill
	if (mGame->playerObj->components.size() == 0)
	{
		state = state_victory;
	}

	//Counters
	laserCounter++;
	behaviourCounter++;

	rotationCounter += irandom(1, 2);
	if (rotationCounter <= 20)
	{
		if (flipCoin)
			rotationDirection = true;
		else
			rotationDirection = false;
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
	memoryState = state;

	return Enemy::update();
}


void Flier::AIupdate()
{
	//Special cases
	if (state == state_victory)
	{
		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
		turnSpeed -= turnAccelerationConstant / 2;
		return;
	}

	if (behaviourCounter > 200)
	{
		initiateAssault = true;
	}

	if (repositioning)
	{
		state = state_repositioning;

		reposition();
	}
	if (fleeing)
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
		if (stationDistance < 3000)
		{
			fleeing = true;
			return;
		}
		state = state_active;

		follow = true;
		if (rotationDirection == true)
		{
			xSpeed += (cos(angle))*accelerationConstant;
			ySpeed += (sin(angle))*accelerationConstant;
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
				shootLaser(enemyBehaviourLevel);
				laserCounter = irandom(-12, -8);
			}
		}

		if (behaviourCounter % 5000)
		{
			repositioning = true;
			repositionCounter = -60;
			if (flipCoin)
				repositioningDirection = true;
			else
				repositioningDirection = false;
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		if (stationDistance < 3000)
		{
			fleeing = true;
			return;
		}
		state = state_detected;

		if (initiateAssault == false)
		{
			follow = false;			
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
		state = state_passive;

		follow = false;
		initiateAssault = false;
		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
	}
}


void Flier::reposition()
{
	follow = false;
	if (repositioningDirection == true)
		angle = playerDirection - PI / 2;
	else
		angle = playerDirection + PI / 2;

	xSpeed += cos(2 * PI - angle)*accelerationConstant;
	ySpeed += sin(2 * PI - angle)*accelerationConstant;
}

void Flier::flee()
{
	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;
}