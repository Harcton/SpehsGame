#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"
#include "Component.h"
#include "Bullet.h"
#include "Turret.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector, TypeOfAI tp) : refVector(rVector), Object(windowref, game)
{
	typeOfEnemy = tp;
	enemyInitialize();

	followingDistance = 500;
	detectionDistance = 1100;
	maxTurnSpeed = 0.07;
	maxSpeed = 3; //?
	snappingAngle = 0.15;
}


Enemy::~Enemy()
{//Enemy constructor CANNOT BE CALLED FROM Game.cpp! Game's objects's vector calls for object destructor instead...
	//std::cout << "\nEnemy deconstructor";
}


void Enemy::enemyInitialize()
{
	if (typeOfEnemy == et_standard)
	{
		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_green.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
		components.push_back(new Turret(this, centerObj, -50, -50));
		components[components.size()-2]->childComponents.push_back(components[components.size() - 1]->id);


		components.push_back(new Component(this, mGame->playerObj, -150, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_green.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	}
	else if (typeOfEnemy == et_bomber)
	{
		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	}
	else if (typeOfEnemy == et_laser)
	{
		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_purple.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	}
	else if (typeOfEnemy == et_commander)
	{
		maxTurnSpeed = 0.01;
		maxSpeed = 2;
		followingDistance = 700;
		snappingAngle = 0.005;

		components.push_back(new Component(this, mGame->playerObj, -50, -100));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(100, 0, 100, 100));
		components.push_back(new Component(this, mGame->playerObj, -150, -100));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(0, 0, 100, 100));
		components.push_back(new Component(this, mGame->playerObj, 50, -100));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(200, 0, 100, 100));
		components[components.size() - 3]->childComponents.push_back(components[components.size() - 1]->id);
		components[components.size() - 3]->childComponents.push_back(components[components.size() - 2]->id);

		components.push_back(new Component(this, mGame->playerObj, -50, 0));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(100, 100, 100, 100));
		components.push_back(new Component(this, mGame->playerObj, -150, 0));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(0, 100, 100, 100));
		components.push_back(new Component(this, mGame->playerObj, 50, 0));
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(200, 100, 100, 100));
		components[components.size() - 3]->childComponents.push_back(components[components.size() - 1]->id);
		components[components.size() - 3]->childComponents.push_back(components[components.size() - 2]->id);
	}
}


bool Enemy::update()
{
	//std::cout << this->xSpeed << std::endl;

	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	if (components.size() <= 0)
		return false;
	
	//limit turnSpeed
	if (turnSpeed > maxTurnSpeed)
		turnSpeed = maxTurnSpeed;
	else if (turnSpeed < -maxTurnSpeed)
		turnSpeed = -maxTurnSpeed;
	
	//Complex update
	double tempDistance = 0;
	int tempIndex = -1;

	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (i == 0)
		{
			tempDistance = getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
			tempIndex = i;
		}
		else if (tempDistance > getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y))
		{
			tempDistance = getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y);
			tempIndex = i;
		}
	}
	if (tempIndex >= 0)
	{
		nearestComponent = mGame->playerObj->components[tempIndex];
	}
	else
	{
		nearestComponent = components[0];
	}


	playerDirection = -1 * atan2(nearestComponent->y - y, nearestComponent->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);

	//update AI accordingly
	enemyAI();

	//limit speed
	//x/ySpeed NO BUENO
	if (this->xSpeed > maxSpeed)
		this->xSpeed = maxSpeed;
	else if (this->ySpeed > maxSpeed)
		this->ySpeed = maxSpeed;
	if (this->xSpeed < -maxSpeed)
		this->xSpeed = -maxSpeed;
	else if (this->ySpeed < -maxSpeed)
		this->ySpeed = -maxSpeed;

	updateComponents();
	
	return Object::update();
}


void Enemy::enemyAI()
{
	distance = getDistance(this->x, this->y, nearestComponent->x, nearestComponent->y);

	//"fix collision"
	if (distance < this->textureRadius + nearestComponent->textureRadius)
	{
		//somethingsomething..?
		if (typeOfEnemy == et_bomber)
		{
			explosion();
			this->hp = 0;
		}
	}

	if (distance > followingDistance && distance < detectionDistance)
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*(distance/700);
		ySpeed += sin(2 * PI - angle)*(distance/700);

		if (typeOfEnemy == et_standard || typeOfEnemy == et_laser)
		{
			if (timer == 8)
			{
				if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
				{
					if (typeOfEnemy == et_standard)
					{
						for (unsigned int i = 0; i < components.size(); i++)
							for (unsigned int k = 0; k < components[i]->types.size(); k++)
								if (components[i]->types[k] == component::turret)
									components[i]->fire();
					}
					if (typeOfEnemy == et_laser)
						fireMahLazors();
					timer = irandom(-10,0);
				}
			}
			else
				timer++;
		}
	}
	else if (distance < followingDistance)
	{
		if (typeOfEnemy != et_bomber)
		{
			follow = true;
			xSpeed -= (cos(2 * PI - angle)*(distance / 1200));
			ySpeed -= (sin(2 * PI - angle)*(distance / 1200));
		}
		else if (typeOfEnemy == et_bomber)
		{
			follow = true;
			xSpeed += (cos(2 * PI - angle)*(distance / 500));
			ySpeed += (sin(2 * PI - angle)*(distance / 500));
		}

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
			if (typeOfEnemy == et_commander)
				turnSpeed += maxTurnSpeed / 20;
			else
				turnSpeed += maxTurnSpeed/5;
		}
		else if (angle > playerDirection + snappingAngle)
		{
			if (typeOfEnemy == et_commander)
				turnSpeed -= maxTurnSpeed / 20;
			else
				turnSpeed -= maxTurnSpeed/5;
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
	//std::cout << components.size() << " "; // COUT

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


void Enemy::explosion()
{
	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y) < this->textureRadius * 2)
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
	std::cout << "\nremoving enemy child component...";
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->id == cid)
		{
		delete components[i];
		components.erase(components.begin() + i);
		}
}


void Enemy::fireMahLazors()
{
	//Lazors
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(this->screenX, this->screenY);
	line[1].position = sf::Vector2f(nearestComponent->screenX - 15 + irandom(0, 30), nearestComponent->screenY - 15 + irandom(0, 30));
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= 1;

	mWindow.draw(line);
}