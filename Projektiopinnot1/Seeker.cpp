#include "Main.h"
#include "Seeker.h"
#include "Game.h"



Seeker::Seeker(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;

	angle = playerDirection;
	explosionLimiter = false;
	dodging = false;
	aggroRange = 1500;
	maxActionRange = 700;
	closeRange = 150;
	maxTurnSpeedLimit = 0.03;
	maxSpeedLimit = 5;
	accelerationConstant = 0.25;
	turnAccelerationConstant = 0.003;
	closeAngle = 0.01;

	//Reserve memory for all of the components
	components.reserve(1);

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("Seeker.png"));
	components.back()->sprites.back().setOrigin(50, 50);

	//Initialize Animations
	components[0]->animatedSprites.push_back(sge::Sprite("seeker_active_animation.png"));
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(4, 2);
	components[0]->animatedSprites.back().setFrameDuration(1);

	components[0]->animatedSprites.push_back(sge::Sprite("seeker_passive_animation.png"));
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(4, 2);
	components[0]->animatedSprites.back().setFrameDuration(1);

	components[0]->animatedSprites.push_back(sge::Sprite("seeker_dodge_animation.png"));
	components[0]->animatedSprites.back().setVisibility(false);
	components[0]->animatedSprites.back().setOrigin(50, 50);
	components[0]->animatedSprites.back().setFrameSize(100, 100);
	components[0]->animatedSprites.back().setTilesetSize(5, 3);
	components[0]->animatedSprites.back().setFrameDuration(3);
}


Seeker::~Seeker()
{
}


bool Seeker::update()
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
		//nothing to see here just act normal
		return;
	}


	if (dodging)
	{
		state = state_dodging;
		if (state != memoryState)
		{
			animationHandler(anim_dodge);
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
						//wait for animation before destruction
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

		xSpeed += cos(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		ySpeed += sin(2 * PI - angle)*accelerationConstant*irandom(0.9, 1.1);
		turnSpeed += irandom(-1, 1)*turnAccelerationConstant;
	}
}


void Seeker::dodgeMove(const double tempXSpeed, const double tempYSpeed)
{
	follow = false;

	if (xSpeed != -tempXSpeed)
	{
		if (tempXSpeed > 0)
			xSpeed -= accelerationConstant * 4*enemyBehaviourLevel;
		else if (tempXSpeed < 0)
			xSpeed += accelerationConstant * 4*enemyBehaviourLevel;
	}
	if (ySpeed != -tempYSpeed)
	{
		if (tempYSpeed > 0)
			ySpeed -= accelerationConstant * 4*enemyBehaviourLevel;
		else if (tempYSpeed < 0)
			ySpeed += accelerationConstant * 4*enemyBehaviourLevel;
	}
	turnSpeed += turnAccelerationConstant * enemyBehaviourLevel;
}


void Seeker::animationHandler(AnimationID ID)
{
	if (!(components.size() > 0))
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
	case anim_dodge:
	{
		components[0]->animatedSprites[2].setVisibility(true);
		break;
	}
	default:
		break;
	}
}