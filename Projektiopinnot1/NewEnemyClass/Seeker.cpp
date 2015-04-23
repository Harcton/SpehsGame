#include "Enemy.h"
#include "Seeker.h"


Seeker::Seeker(sf::RenderWindow&, Game*, std::vector<Object*>&, int behaviourLevel) : refVector(rVector), Object(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	dodging = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 600;
	closeRange = 300;
	maxTurnSpeedLimit = 0.01;
	maxSpeedLimit = 4;
	accelerationConstant;//?
	accelerationConstant;//?
	closeAngle;//set

	components.push_back(new Component(this, mGame->playerObj, 0, 0));
	components[components.size() - 1]->sprites.push_back(sf::Sprite());
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("enemy_base.png"));
	components[components.size() - 1]->sprites[components[components.size() - 1]->sprites.size() - 1].setOrigin(50, 50);
}


Seeker::~Seeker()
{
}


bool Seeker::update()
{
	//Counters
	dodgeCounter++;
	//Behaviour counter!!
	//RotationDirection changes...


	AIupdate();
	HPMemory = components[0]->hp;

	return Enemy::update();
}


void Seeker::AIupdate()//maybe not follow true all the time
{
	if (dodging)
	{
		dodgeMove();
	}
	else if (distance < this->textureRadius + nearestComponent->textureRadius) //Contact
	{
		explosion(50, 2);
		this->hp = 0;
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed = (cos(2 * PI - angle))*accelerationConstant;
		ySpeed = (sin(2 * PI - angle))*accelerationConstant;
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		follow = true;
		xSpeed = (cos(2 * PI - angle))*accelerationConstant;
		ySpeed = (sin(2 * PI - angle))*accelerationConstant;

		if (HPMemory > this->components[0]->hp)
		{
			dodging = true;
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		follow = true;
		xSpeed = cos(2 * PI - angle)*accelerationConstant;
		ySpeed = sin(2 * PI - angle)*accelerationConstant;
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
			turnSpeed += maxTurnSpeed;
			xSpeed = (sin(angle))*maxSpeed;
			ySpeed = (cos(angle))*maxSpeed;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= maxTurnSpeed;
			xSpeed = (sin(angle))*maxSpeed;
			ySpeed = (cos(angle))*maxSpeed;
		}
	}
	else if (angle >= PI / 2 && angle < PI) //2nd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += maxTurnSpeed;
			xSpeed = (sin(angle))*maxSpeed;
			ySpeed = (-cos(angle))*maxSpeed;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= maxTurnSpeed;
			xSpeed = (sin(angle))*maxSpeed;
			ySpeed = (-cos(angle))*maxSpeed;
		}
	}
	else if (angle >= PI && angle < PI*1.5)//3rd quarter
	{
		if (rotationDirection)
		{
			turnSpeed += maxTurnSpeed;
			xSpeed = (-sin(angle))*maxSpeed;
			ySpeed = (-cos(angle))*maxSpeed;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= maxTurnSpeed;
			xSpeed = (-sin(angle))*maxSpeed;
			ySpeed = (-cos(angle))*maxSpeed;
		}
	}
	else //4th quarter
	{
		if (rotationDirection)
		{
			turnSpeed += maxTurnSpeed;
			xSpeed = (-sin(angle))*maxSpeed;
			ySpeed = (cos(angle))*maxSpeed;
		}
		else if (!rotationDirection)
		{
			turnSpeed -= maxTurnSpeed;
			xSpeed = (-sin(angle))*maxSpeed;
			ySpeed = (cos(angle))*maxSpeed;
		}
	}
}