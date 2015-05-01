#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"
#include "Seeker.h"
#include "Component.h"


//Explosion Bugged!
Seeker::Seeker(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	explosionLimiter = false;
	dodging = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 600;
	closeRange = 300;
	maxTurnSpeedLimit = 0.03;
	maxSpeedLimit = 6;
	accelerationConstant = 0.25;
	turnAccelerationConstant = 0.003;
	closeAngle = 0.5;

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("Seeker.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);

	components[components.size() - 1]->animatedSprites.push_back(sge::Sprite("seeker_active_animation.png"));
	components[components.size() - 1]->animatedSprites[0].setVisibility(false);
	components[components.size() - 1]->animatedSprites[0].setOrigin(50, 50);
	components[components.size() - 1]->animatedSprites[0].setFrameSize(100, 100);
	components[components.size() - 1]->animatedSprites[0].setTilesetSize(4, 2);
	components[components.size() - 1]->animatedSprites[0].setFrameDuration(1);
}


Seeker::~Seeker()
{
}


bool Seeker::update()
{
	//Counters
	dodgeCounter--;
	if (dodgeCounter < 0)
	{
		dodging = false;
	}

	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}	

	return Enemy::update();
}


void Seeker::AIupdate()//maybe not follow true all the time
{
	if (dodging)
	{
		dodgeMove();
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (nearestComponent != nullptr)
		{
			if (distance < this->textureRadius + nearestComponent->textureRadius && explosionLimiter == false) //Contact
				{
					explosion(50, 1.5);
					this->hp = 0;
					explosionLimiter = true;
				}
		}		
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (components.size() > 0)
		{
			if (HPMemory > this->components[0]->hp)
			{
				dodging = true;
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
		components[components.size() - 1]->animatedSprites[0].setVisibility(true);
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


void Seeker::dodgeMove()
{//work very much in progress over here
	//set speed almost opposite of what it was(sidestep) and start acceleration back as normal
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