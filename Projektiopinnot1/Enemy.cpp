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

	// IGNORE FOR NOW !
	//testing lazors
	/*sf::VertexArray line(sf::LinesStrip, 2);
	line[0].position = sf::Vector2f(this->x, this->y);
	line[1].position = sf::Vector2f(nearestComponent->x, nearestComponent->y);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	mWindow.draw(line);*/


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
				for (unsigned int i = 0; i < components.size(); i++)
					for (unsigned int k = 0; k < components[i]->types.size(); k++)
						if (components[i]->types[k] == ct_turret)
							components[i]->fire();
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
	std::cout << components.size() << " "; // COUT

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

	if (distance < this->textureRadius + nearestComponent->textureRadius + 10)
	{
		//explosion
		this->hp = 0;
		explosion();
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
	//ENEMY DON'T DIE WHEN DIED!!
	if (typeOfEnemy == 1)
	{
		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_green.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
		components[components.size() - 1]->createChild(-50, -50, ct_turret);
	}
	else if (typeOfEnemy == 2)
	{
		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
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


void Enemy::explosion()
{
	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y) < this->textureRadius*3)
		{
			mGame->playerObj->components[i]->hp -= 50;
		}
	}
}



void Enemy::checkBulletCollision(Bullet* b)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y, components[i]->x - b->x);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x, components[i]->y);
		b->checkCollisionRange = b->textureRadius + components[i]->textureRadius;

		if (b->checkCollisionDistance < b->checkCollisionRange)
		{
			if (b->canDamage == true)
			{
				components[i]->hp -= b->damage;
				b->canDamage = false;
				x += 6 * cos(angle);
				y += -6 * sin(angle);
			}

			//Bounce
			b->speed = b->speed*0.75;
			b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			b->xSpeed = cos(2 * PI - b->angle) * b->speed;
			b->ySpeed = sin(2 * PI - b->angle) * b->speed;
		}
	}
}


void Enemy::removeComponent(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->id == cid)
		{
		delete components[i];
		components.erase(components.begin() + i);
		}
}

