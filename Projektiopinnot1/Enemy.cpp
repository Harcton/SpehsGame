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

	detectionDistance = SPAWN_RANGE;
}


Enemy::~Enemy()
{//Enemy constructor CANNOT BE CALLED FROM Game.cpp! Game's objects's vector calls for object destructor instead...
	//std::cout << "\nEnemy deconstructor";
}


void Enemy::enemyInitialize()
{
	if (typeOfEnemy == et_standard)
	{
		followingDistance = 500;
		actionDistance = 1000;
		maxTurnSpeed = 0.014;
		maxSpeed = 3;

		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_green.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
		components.push_back(new Turret(this, centerObj, 0, 0));
		components[components.size()-2]->childComponents.push_back(components[components.size() - 1]->id);
	}
	else if (typeOfEnemy == et_bomber)
	{
		followingDistance = 600;
		actionDistance = SPAWN_RANGE - 500;
		maxTurnSpeed = 0.01;
		maxSpeed = 2;

		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	}
	else if (typeOfEnemy == et_laser)
	{
		followingDistance = 400;
		actionDistance = SPAWN_RANGE - 1000;
		maxTurnSpeed = 0.015;
		maxSpeed = 9;

		components.push_back(new Component(this, mGame->playerObj, -50, -50));
		components[components.size() - 1]->tex.loadFromFile("Texture/enemy_base_purple.png");
		components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	}
	else if (typeOfEnemy == et_commander)
	{
		followingDistance = 1000;
		actionDistance = SPAWN_RANGE - 200;
		maxTurnSpeed = 0.001;
		maxSpeed = 1; 

		//-9
		components.push_back(new Component(this, mGame->playerObj, -325, -100)); //Component 3 (REAR)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(0, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-8
		components.push_back(new Component(this, mGame->playerObj, -300, -50)); //Component 5 (R-REAR WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(400, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-7
		components.push_back(new Component(this, mGame->playerObj, -300, -150)); //Component 4 (L-REAR WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(200, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-6
		components.push_back(new Component(this, mGame->playerObj, -200, -50)); //Component 7 (R-CORE WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(700, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-5
		components.push_back(new Component(this, mGame->playerObj, -200, -150)); //Component 6 (L-CORE WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(500, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-4
		components.push_back(new Component(this, mGame->playerObj, -50, -50)); //Component 9 (R-FRONT WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(800, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-3
		components.push_back(new Component(this, mGame->playerObj, -50, -150)); //Component 8 (L-FRONT WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(600, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-2
		components.push_back(new Component(this, mGame->playerObj, -250, -100)); //Component 2 (CORE) //set correct coordinates for everyone...
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(300, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100; 
		//-1
		components.push_back(new Component(this, mGame->playerObj, -150, -150)); //Component 1 (TANK) //(0,0)
		components[components.size() - 1]->spr.setOrigin(100, 100); //150
		components[components.size() - 1]->spr.setTexture(commanderShipTex);
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(0, 0, 300, 300));
		components[components.size() - 1]->textureRadius = 150;

		components[components.size() - 2]->childComponents.push_back(components[components.size() - 1]->id); //TANK TO CORE
		components[components.size() - 2]->childComponents.push_back(components[components.size() - 6]->id); //R-CORE WING TO CORE
		components[components.size() - 2]->childComponents.push_back(components[components.size() - 5]->id); //L-CORE WING TO CORE
		components[components.size() - 2]->childComponents.push_back(components[components.size() - 9]->id); //REAR TO CORE
		components[components.size() - 1]->childComponents.push_back(components[components.size() - 4]->id); //R-FRONT WING TO TANK
		components[components.size() - 1]->childComponents.push_back(components[components.size() - 3]->id); //L-FRONT WING TO TANK
		components[components.size() - 5]->childComponents.push_back(components[components.size() - 7]->id); //L-REAR WING TO L-CORE WING
		components[components.size() - 6]->childComponents.push_back(components[components.size() - 8]->id); //R-REAR WING TO R-CORE WING

		
		/*components.push_back(new Component(this, mGame->playerObj, -50, -100));
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
		components[components.size() - 3]->childComponents.push_back(components[components.size() - 2]->id);*/
		
	}
}


bool Enemy::update()
{
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

	//limit speed
	if (this->xSpeed > maxSpeed)
		this->xSpeed = maxSpeed;
	else if (this->ySpeed > maxSpeed)
		this->ySpeed = maxSpeed;
	if (this->xSpeed < -maxSpeed)
		this->xSpeed = -maxSpeed;
	else if (this->ySpeed < -maxSpeed)
		this->ySpeed = -maxSpeed;

	//update AI accordingly
	enemyAI();

	updateComponents();
	
	return Object::update();
}


void Enemy::enemyAI()
{
	distance = getDistance(this->x, this->y, nearestComponent->x, nearestComponent->y);

	//update laserCounter
	if (laserCounter <= 2)
	{
		rotationDirection = true;
	}
	else if (laserCounter > 2 && laserCounter < 6)
	{
		rotationDirection = false;
	}
	else if (laserCounter >= 6)
	{
		laserCounter = irandom(-5,1);
	}

	//literally inside the player
	if (distance < this->textureRadius + nearestComponent->textureRadius)
	{
		if (typeOfEnemy == et_bomber)
		{
			explosion();
			this->hp = 0;
		}
	}

	//too close
	if (distance < followingDistance)
	{
		if (typeOfEnemy != et_bomber)
		{
			follow = true;
			xSpeed = -(cos(2 * PI - angle))*maxSpeed;
			ySpeed = -(sin(2 * PI - angle))*maxSpeed;
		}
		else
		{
			follow = true;
			xSpeed += (cos(2 * PI - angle));
			ySpeed += (sin(2 * PI - angle));
		}

	}
	//action distance
	else if (distance > followingDistance && distance < actionDistance)
	{
		follow = true;
		if (typeOfEnemy == et_laser)
		{
			if (rotationDirection == true)
			{
				xSpeed = (-sin(angle))*maxSpeed;
				ySpeed = (-cos(angle))*maxSpeed;
			}
			else if (rotationDirection == false)
			{
				xSpeed = (sin(angle))*maxSpeed;
				ySpeed = (cos(angle))*maxSpeed;
			}

			//blink?
		}
		else if (typeOfEnemy == et_standard)
		{
			xSpeed += (cos(2 * PI - angle));
			ySpeed += (sin(2 * PI - angle));
		}
		else if (typeOfEnemy == et_commander)
		{
			xSpeed += (cos(2 * PI - angle));
			ySpeed += (sin(2 * PI - angle));
		}
		else //bomber
		{
			xSpeed += (cos(2 * PI - angle));
			ySpeed += (sin(2 * PI - angle));
		}

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
					laserCounter++;
				}
			}
			else
				timer++;
		}
	}
	//too far
	else if (distance > actionDistance && distance < detectionDistance)
	{
		follow = true;
		xSpeed = cos(2 * PI - angle)*maxSpeed;
		ySpeed = sin(2 * PI - angle)*maxSpeed;
	}
	//outside of range
	else
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


	if (follow == true) //set angle to match playerdirection
	{
		if (angle >= 0 && angle < PI/2) //1st quarter
		{
			if (playerDirection < PI && playerDirection > angle)
			{
				turnSpeed += maxTurnSpeed;
			}
			else if (playerDirection < angle || playerDirection > PI*1.5)
			{
				turnSpeed -= maxTurnSpeed;
			}
			else if (playerDirection < angle+PI)
			{
				turnSpeed += maxTurnSpeed;
			}
			else
			{
				turnSpeed -= maxTurnSpeed;
			}
		}
		else if (angle >= PI/2 && angle < PI) //2nd quarter
		{
			if (playerDirection < PI*1.5 && playerDirection > angle)
			{
				turnSpeed += maxTurnSpeed;
			}
			else if (playerDirection < angle)
			{
				turnSpeed -= maxTurnSpeed;
			}
			else if (playerDirection > angle + PI)
			{
				turnSpeed -= maxTurnSpeed;
			}
			else
			{
				turnSpeed += maxTurnSpeed;
			}
		}
		else if (angle >= PI && angle < PI*1.5)//3rd quarter
		{
			if (playerDirection > angle)
			{
				turnSpeed += maxTurnSpeed;
			}
			else if (playerDirection < angle && playerDirection > PI/2)
			{
				turnSpeed -= maxTurnSpeed;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += maxTurnSpeed;
			}
			else
			{
				turnSpeed -= maxTurnSpeed;
			}
		}
		else //4th quarter
		{
			if (playerDirection > angle || playerDirection < PI/2)
			{
				turnSpeed += maxTurnSpeed;
			}
			else if (playerDirection > PI && playerDirection < angle)
			{
				turnSpeed -= maxTurnSpeed;
			}
			else if (playerDirection < angle - PI)
			{
				turnSpeed += maxTurnSpeed;
			}
			else
			{
				turnSpeed -= maxTurnSpeed;
			}
		}
	}
	else
	{
		if (turnSpeed > 0.001)
		{
			turnSpeed = turnSpeed*0.9;
		}
	}

}


void Enemy::updateComponents()
{
	//std::cout << components.size() << " "; // COUT

	//for (componentIt = components.begin(); componentIt < components.end();)
	//{
	//	if ((*componentIt)->update() == false)
	//	{
	//		delete (*componentIt);
	//		componentIt++;
	//		components.erase(componentIt - 1);
	//	}
	//	else
	//		componentIt++;
	//}

	/*for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->update() == false)
		{
			int temp_originalSize = components.size();
			delete components[i];
			temp_originalSize -= components.size();
			components.erase(components.begin() + i);
			i--;
		}
	}*/

	for (componentIt = components.begin(); componentIt != components.end();)
	{
		if ((*componentIt)->update() == false)
		{
			std::vector<int> temp_ids;
			for (unsigned int i = 0; i < components.size(); i++)
				temp_ids.push_back(components[i]->id);






			delete (*componentIt);



			componentIt = components.erase(componentIt);
		}
		else
		{
			++componentIt;
		}
	}

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}


void Enemy::explosion()
{
	for (unsigned int i = 0; i < mGame->playerObj->components.size(); i++)
	{
		if (getDistance(this->x, this->y, mGame->playerObj->components[i]->x, mGame->playerObj->components[i]->y) < components[0]->textureRadius * 2)
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


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x + components[i]->textureRadius, components[i]->y + components[i]->textureRadius);
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
		return;
		}
}


void Enemy::notifyComponentDestruction(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		for (unsigned int c = 0; c < components[i]->childComponents.size(); c++)
		{
			if (components[i]->childComponents[c] == cid)
			{
				components[i]->childComponents.erase(components[i]->childComponents.begin() + c);
			}
		}

	}
}


void Enemy::fireMahLazors()
{
	//Lazors
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(this->screenX, this->screenY);
	line[1].position = sf::Vector2f(nearestComponent->screenX - (15 + irandom(0, 30))*resFactor*zoomFactor, nearestComponent->screenY - (15 + irandom(0, 30))*resFactor*zoomFactor);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= 1;

	mWindow.draw(line);
}