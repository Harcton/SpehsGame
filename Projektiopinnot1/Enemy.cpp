#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"
#include "Component.h"
#include "Bullet.h"
#include "Turret.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector) : refVector(rVector), Object(windowref, game)
{
	typeOfEnemy = irandom(1, 2); //remove randomness later
	enemyInitialize();

	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;

	followingDistance = 500;
	detectionDistance = 1100;
	maxTurnSpeed = 0.07;
	maxSpeed = 4;
	snappingAngle = 0.2;
	complexUpdateTimer = 0;
	complexUpdate();
}


Enemy::~Enemy()
{
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
}


bool Enemy::update()
{	
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	playerDirection = -1 * atan2(nearestComponent->y - y, nearestComponent->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);
	
	//limit speed
	//x/ySpeed no bueno
	if (this->xSpeed > maxSpeed)
		this->xSpeed = maxSpeed;
	else if (this->ySpeed > maxSpeed)
		this->ySpeed = maxSpeed;
	if (this->xSpeed < -maxSpeed)
		this->xSpeed = -maxSpeed;
	else if (this->ySpeed < -maxSpeed)
		this->ySpeed = -maxSpeed;

	std::cout << this->xSpeed+this->ySpeed << std::endl;


	//limit turnSpeed
	if (turnSpeed > maxTurnSpeed)
		turnSpeed = maxTurnSpeed;
	else if (turnSpeed < -maxTurnSpeed)
		turnSpeed = -maxTurnSpeed;
	
	//Complex update
	complexUpdateTimer--;
	if (complexUpdateTimer <= 0)
		complexUpdate();

	//update AI accordingly
	updateAI();

	updateComponents();
	
	return Object::update();
}


void Enemy::enemyAI()
{

	distance = getDistance(x, y, nearestComponent->x, nearestComponent->y);

	//"fix collision"
	if (distance < this->textureRadius + nearestComponent->textureRadius)
	{
		//somethingsomething..?
		std::cout << "oops!" << std::endl;
	}

	if (distance > followingDistance && distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance/700);
		ySpeed += sin(2 * PI - angle)*(distance/700);

		if (timer == 6)
		{
			if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
			{
				components[0]->fire();
				timer = 0;
			}
		}
		else
			timer++;
	}
	else if (distance < followingDistance)
	{
		follow = true;
		if (xSpeed < 0)
			xSpeed -= 0.2;
		else if (xSpeed > 0)
			xSpeed += 0.2;
		if (ySpeed < 0)
			ySpeed -= 0.2;
		else if (ySpeed > 0)
			ySpeed += 0.2;
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
		if (angle < playerDirection - snappingAngle)
		{
			turnSpeed += maxTurnSpeed/4;
		}
		else if (angle > playerDirection + snappingAngle)
		{
			turnSpeed -= maxTurnSpeed/4;
		}

		if (angle / playerDirection > 1.1 || angle / playerDirection < 0.9)
		{
			
		}
	}
	else
	{
		if (turnSpeed > 0.005)
		{
			turnSpeed = turnSpeed*0.9;
			//?
		}
	}

}


void Enemy::updateComponents()
{
	for (componentIt = components.begin(); componentIt != components.end();)
		if ((*componentIt)->update() == false)
		{
		delete (*componentIt);
		componentIt = components.erase(componentIt);
		}
		else
		{
			++componentIt;
		}
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}


void Enemy::bomberAI()
{
	distance = getDistance(x, y, nearestComponent->x, nearestComponent->y);

	//"fix collision"
	if (distance < this->textureRadius + nearestComponent->textureRadius + 10)
	{
		//explosion?
		this->hp = 0;
		std::cout << "boom!" << std::endl;
	}

	if (distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance / 700);
		ySpeed += sin(2 * PI - angle)*(distance / 700);				
	}
	else if (distance > detectionDistance)
	{
		follow = false;
		xSpeed = xSpeed*0.99;
		ySpeed = ySpeed*0.99;
	}


	if (follow == true)
	{
		if (angle < playerDirection - snappingAngle)
		{
			turnSpeed += maxTurnSpeed / 4;
		}
		else if (angle > playerDirection + snappingAngle)
		{
			turnSpeed -= maxTurnSpeed / 4;
		}

		if (angle / playerDirection > 1.1 || angle / playerDirection < 0.9)
		{

		}
	}
	else
	{
		if (turnSpeed > 0.005)
		{
			turnSpeed = turnSpeed*0.9;
			turnSpeed = -turnSpeed;
			//?
		}
	}

}


void Enemy::updateAI()
{
	if (typeOfEnemy == 1)
		enemyAI();
	else if (typeOfEnemy == 2)
		bomberAI();
}


void Enemy::enemyInitialize()
{
	if (typeOfEnemy == 1)
	{
		tex.loadFromFile("Texture/enemy_base_green.png");
		components.push_back(new Turret(this, centerObj, 0, 0));
	}
	else if (typeOfEnemy == 2)
	{
		tex.loadFromFile("Texture/enemy_base.png");
	}
}


void Enemy::complexUpdate()
{
	double tempDistance = 0;
	int tempIndex = -1;


	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (i == 0)
		{
			tempDistance = getDistance(x, y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
			tempIndex = i;
		}
		else if (tempDistance > getDistance(x, y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y))
		{
			tempDistance = getDistance(x, y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
			tempIndex = i;
		}
	}

	if (tempIndex != -1)
		nearestComponent = mGame->playerObj->components[tempIndex];
	else
	{
		//What if player dies???????
	}

	complexUpdateTimer = 10 + irandom(0,10); //?
}