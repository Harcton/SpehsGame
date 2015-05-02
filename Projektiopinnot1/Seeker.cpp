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
	state = state_spawned;

	explosionLimiter = false;
	dodging = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 700;
	closeRange = 150;
	maxTurnSpeedLimit = 0.03;
	maxSpeedLimit = 6;
	accelerationConstant = 0.25;
	turnAccelerationConstant = 0.003;
	closeAngle = 0.01;

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("Seeker.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);

	//Initialize Animations
	components[0]->animatedSprites.push_back(sge::Sprite("seeker_active_animation.png"));
	components[0]->animatedSprites[components.size() - 1].setVisibility(false);
	components[0]->animatedSprites[components.size() - 1].setOrigin(50, 50);
	components[0]->animatedSprites[components.size() - 1].setFrameSize(100, 100);
	components[0]->animatedSprites[components.size() - 1].setTilesetSize(4, 2);
	components[0]->animatedSprites[components.size() - 1].setFrameDuration(1);

	components[0]->animatedSprites.push_back(sge::Sprite("seeker_passive_animation.png"));
	components[0]->animatedSprites[components.size() - 1].setVisibility(false);
	components[0]->animatedSprites[components.size() - 1].setOrigin(50, 50);
	components[0]->animatedSprites[components.size() - 1].setFrameSize(100, 100);
	components[0]->animatedSprites[components.size() - 1].setTilesetSize(4, 2);
	components[0]->animatedSprites[components.size() - 1].setFrameDuration(1);
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
	activationCounter--;
	
	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}	
	memoryState = state;

	return Enemy::update();
}


void Seeker::AIupdate()//maybe not follow true all the time
{
	if (dodging)
	{
		state = state_dodging;
		if (state != memoryState)
		{
			//dodging animation
		}

		dodgeMove(xSpeed, ySpeed);
	}
	else if (distance < closeRange) //Close state
	{
		state = state_closeRange;
		if (state != memoryState)
		{
			animationHandler(anim_active);
		}

		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (nearestComponent != nullptr)
		{
			if (distance < this->textureRadius + nearestComponent->textureRadius && explosionLimiter == false) //Contact
				{
					state = state_contact;
					if (state != memoryState)
					{
						//explosion animation
						//wait animation before destruction
					}

					explosion(50, 1.5);
					this->hp = 0;
					explosionLimiter = true;
				}
		}		
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		state = state_active;
		if (state != memoryState)
		{
			animationHandler(anim_active);
		}

		follow = true;
		xSpeed += (cos(2 * PI - angle))*accelerationConstant;
		ySpeed += (sin(2 * PI - angle))*accelerationConstant;

		if (components.size() > 0)
		{
			if (HPMemory > this->components[0]->hp)
			{
				dodging = true;
				dodgeCounter = 50;
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		if (activationCounter < 0)
			state = state_detected;
		if (state != memoryState)
		{	
			animationHandler(anim_active);	
			if (memoryState == state_passive)
			{
				state = state_activated;
				activationCounter = 40;//change accordingly!
				//activation animation
			}		
		}

		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
		if (components.size() > 0)
		{
			if (HPMemory > this->components[0]->hp)
			{
				dodging = true;
				dodgeCounter = 35;
			}
		}
	}
	else //Passive state
	{
		state = state_passive;
		if (state != memoryState)
		{
			animationHandler(anim_passive);
			//deactivation(dodge) animation?
		}

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


void Seeker::dodgeMove(const double tempXSpeed, const double tempYSpeed)
{
	follow = false;

	if (xSpeed != -tempXSpeed)
	{
		if (tempXSpeed > 0)
			xSpeed -= accelerationConstant * 2.5*enemyBehaviourLevel;
		else if (tempXSpeed < 0)
			xSpeed += accelerationConstant * 2.5*enemyBehaviourLevel;
	}
	if (ySpeed != -tempYSpeed)
	{
		if (tempYSpeed > 0)
			ySpeed -= accelerationConstant * 2.5*enemyBehaviourLevel;
		else if (tempYSpeed < 0)
			ySpeed += accelerationConstant * 2.5*enemyBehaviourLevel;
	}
	turnSpeed += turnAccelerationConstant * enemyBehaviourLevel;
}


void Seeker::animationHandler(AnimationID ID)
{
	if (!components.size() > 0)
		return;

	for (unsigned int i = 0; i < components[0]->animatedSprites.size(); i++)
	{
		components[0]->animatedSprites[i].setVisibility(false);
	}

	switch (ID)
	{
	case anim_active:
	{
		components[0]->animatedSprites[0].setVisibility(true);
		break;
	}
	case anim_passive:
	{
		components[0]->animatedSprites[1].setVisibility(true);//no workerino??
		break;
	}
	default:
		break;
	}
}