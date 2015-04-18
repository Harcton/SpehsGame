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

	dodging = false;
	snappingAngle = 0; //doesn't matter atm what this is...
	detectionDistance = SPAWN_RANGE;
}


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector, TypeOfAI tp, Enemy* fMaster) : refVector(rVector), Object(windowref, game)
{
	flierMaster = fMaster;
	typeOfEnemy = tp;
	dodging = false;
	snappingAngle = 0; //doesn't matter atm what this is...
	detectionDistance = SPAWN_RANGE;
	enemyInitialize();
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
		
		components.push_back(new Component(this, mGame->playerObj, 0, 0));
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_base_green.png"));
		components[components.size() - 1]->spr.setOrigin(50, 50);
		components.push_back(new Turret(this, centerObj, 0, 0));
		components[components.size()-2]->childComponents.push_back(components[components.size() - 1]->id);
	}
	else if (typeOfEnemy == et_bomber)
	{
		followingDistance = 300;
		actionDistance = 600;
		maxTurnSpeed = 0.01;
		maxSpeed = 2;

		components.push_back(new Component(this, mGame->playerObj, 0, 0));
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_base_purple.png"));
		components[components.size() - 1]->spr.setOrigin(50, 50);
	}
	else if (typeOfEnemy == et_flier)
	{
		followingDistance = 75;
		actionDistance = 85;
		maxTurnSpeed = 0.1;
		maxSpeed = 4;
		initiateFlierAssault = false;
		if (flipCoin)
			rotationDirection = false;

		components.push_back(new Component(this, mGame->playerObj, 0, 0));
		components[components.size() - 1]->spr.setTexture(RM.getTexture("Flier.png"));
		components[components.size() - 1]->spr.setOrigin(15, 15);
		components[components.size() - 1]->textureRadius = 30;
		components[components.size() - 1]->hp = 30;
	}
	else if (typeOfEnemy == et_laser)
	{
		followingDistance = 350;
		actionDistance = 600;
		maxTurnSpeed = 0.015;
		maxSpeed = 9;

		components.push_back(new Component(this, mGame->playerObj, 0, 0));
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_base_purple.png"));
		components[components.size() - 1]->spr.setOrigin(50, 50);
	}
	else if (typeOfEnemy == et_commander)
	{
		followingDistance = 1000;
		actionDistance = 2000;
		maxTurnSpeed = 0.003;
		maxSpeed = 2;
		initiateFlierAssault = false;

		//-9
		components.push_back(new Component(this, mGame->playerObj, -325, -100)); //Component 3 (REAR)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(0, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-8
		components.push_back(new Component(this, mGame->playerObj, -300, -50)); //Component 5 (R-REAR WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(400, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-7
		components.push_back(new Component(this, mGame->playerObj, -300, -150)); //Component 4 (L-REAR WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(200, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-6
		components.push_back(new Component(this, mGame->playerObj, -200, -50)); //Component 7 (R-CORE WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(700, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-5
		components.push_back(new Component(this, mGame->playerObj, -200, -150)); //Component 6 (L-CORE WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(500, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-4
		components.push_back(new Component(this, mGame->playerObj, -50, -50)); //Component 9 (R-FRONT WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(800, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-3
		components.push_back(new Component(this, mGame->playerObj, -50, -150)); //Component 8 (L-FRONT WING)
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(600, 300, 200, 200));
		components[components.size() - 1]->textureRadius = 100;
		//-2
		components.push_back(new Component(this, mGame->playerObj, -250, -100)); //Component 2 (CORE) //set correct coordinates for everyone...
		components[components.size() - 1]->spr.setOrigin(100, 100);
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
		components[components.size() - 1]->spr.setTextureRect(sf::IntRect(300, 0, 200, 200));
		components[components.size() - 1]->textureRadius = 100; 
		//-1
		components.push_back(new Component(this, mGame->playerObj, -100, -100)); //Component 1 (TANK) //(0,0)
		components[components.size() - 1]->spr.setOrigin(150, 150); //150
		components[components.size() - 1]->spr.setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
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
		
		components.push_back(new Turret(this, centerObj, -200, -150));
		components[components.size() - 5]->childComponents.push_back(components[components.size() - 1]->id);
		components.push_back(new Turret(this, centerObj, -200, -50));
		components[components.size() - 6]->childComponents.push_back(components[components.size() - 1]->id);
		
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

	tempHPMemory = this->components[0]->hp;

	updateComponents();
	
	return Object::update();
}


void Enemy::enemyAI()
{
	distance = getDistance(this->x, this->y, nearestComponent->x, nearestComponent->y);

	//update Counters
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
		laserBChange = true;
	}

	BCounter++;
	explosionTimer++;
	dodgeCounter--;
	flierAttackCounter++;
	
	//attack?
	if (typeOfEnemy == et_flier)
	{
		if (flierMaster->initiateFlierAssault == true)
		{
			this->initiateFlierAssault = true;
		}
	}
	if (typeOfEnemy == et_commander)
	{
		if (flierAttackCounter >= 500)
		{
			initiateFlierAssault = true;
			flierAttackCounter = 0;
			fliersFollowing = 0;
		}
		else
		{
			initiateFlierAssault = false;
		}
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
		if (typeOfEnemy == et_laser || typeOfEnemy == et_standard || typeOfEnemy == et_flier)
		{
			follow = true;
			xSpeed = -(cos(2 * PI - angle))*maxSpeed;
			ySpeed = -(sin(2 * PI - angle))*maxSpeed;
		}
		else if (typeOfEnemy == et_commander)
		{
			follow = true;
			xSpeed = -(cos(2 * PI - angle))*(distance / 1000)*maxSpeed;
			ySpeed = -(sin(2 * PI - angle))*(distance / 1000)*maxSpeed;
			if (timer >= 8)
			{
				if (angle < playerDirection|| angle > -playerDirection) //fix this thing?
				for (unsigned int i = 0; i < components.size(); i++)
					for (unsigned int k = 0; k < components[i]->types.size(); k++)
						if (components[i]->types[k] == component::turret)
						{
							components[i]->fire();
							timer = irandom(-30, -10);
						}
			}
			else
				timer++;
		}
		else
		{
			follow = true;
			xSpeed = (cos(2 * PI - angle))*maxSpeed;
			ySpeed = (sin(2 * PI - angle))*maxSpeed;
		}

	}
	//action distance
	else if (distance > followingDistance && distance < actionDistance)
	{
		if (typeOfEnemy == et_laser)
		{
			if (rotationDirection == true && BCounter > 80)
			{
				if (laserBChange == true && follow == true)
				{
					follow = false;

					xSpeed = (-sin(angle))*maxSpeed;
					ySpeed = (-cos(angle))*maxSpeed;
					laserBChange = false;
					BCounter = 0;
					dodging = true;
					dodgeCounter = 60;
				}
				else
				{
					follow = true;

					xSpeed = (-sin(angle))*maxSpeed + irandom(7, 13);
					ySpeed = (-cos(angle))*maxSpeed + irandom(5, 10);
					BCounter = 0;
				}
			}
			else if (rotationDirection == false && BCounter > 80)
			{
				if (laserBChange == true && follow == true)
				{
					follow = false;

					xSpeed = (sin(angle))*maxSpeed;
					ySpeed = (cos(angle))*maxSpeed;
					laserBChange = false;
					BCounter = 0;
					dodging = true;
					dodgeCounter = 60;
				}
				else
				{
					follow = true;

					xSpeed = (sin(angle))*maxSpeed + irandom(7, 13);
					ySpeed = (cos(angle))*maxSpeed + irandom(5, 10);
					BCounter = 0;
				}
			}
			if (dodging == true && dodgeCounter > 0)
			{
				dodgeMove();
			}
			else
				dodging = false;
		}
		else if (typeOfEnemy == et_flier)
		{			
			follow = true;
			if (rotationDirection == true) //rotation direction to be fixed?
			{				
				xSpeed += (-sin(angle)) + irandom(-2, 2);
				ySpeed += (-cos(angle)) + irandom(-2, 2);
			}
			else if (rotationDirection == false)
			{
				xSpeed += (sin(angle)) + irandom(-2, 2);
				ySpeed += (cos(angle)) + irandom(-2, 2);
			}
		}
		else if (typeOfEnemy == et_standard)
		{
			if (tempHPMemory > this->components[0]->hp && BCounter > 120)
			{
				follow = false;
				BCounter = 0;
				dodgeMove(); //NO WORKERINO YET
			}
			else
			{
				xSpeed += (cos(2 * PI - angle));
				ySpeed += (sin(2 * PI - angle));
						
				follow = true;
			}
		}
		else if (typeOfEnemy == et_commander)
		{
			follow = false;
			xSpeed = (cos(2 * PI - angle));
			ySpeed = (sin(2 * PI - angle));
			if (angle < playerDirection + PI / 2)
			{
				turnSpeed += maxTurnSpeed;
			}
			else if (angle > playerDirection - PI / 2)
			{
				turnSpeed -= maxTurnSpeed;
			}

		}
		else if (typeOfEnemy == et_bomber)
		{
			if (dodging == false)
			{
				follow = true;
				xSpeed = (cos(2 * PI - angle))*maxSpeed;
				ySpeed = (sin(2 * PI - angle))*maxSpeed;
				
				if (tempHPMemory > this->components[0]->hp)
				{
					dodging = true;
					dodgeCounter = 40;
				}
			}
			else if (dodging == true && dodgeCounter > 0)
			{
				dodgeMove();
			}
			else
			{
				dodging = false;
			}
		}

		if (typeOfEnemy == et_standard || typeOfEnemy == et_laser || typeOfEnemy == et_commander || typeOfEnemy == et_flier)
		{
			if (timer >= 8)
			{
				if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
				{
					if (typeOfEnemy == et_standard)
					{
						for (unsigned int i = 0; i < components.size(); i++)
							for (unsigned int k = 0; k < components[i]->types.size(); k++)
								if (components[i]->types[k] == component::turret)
								{
									components[i]->fire();
									timer = irandom(-20, -10);
								}
					}
					if (typeOfEnemy == et_laser && follow == true && dodging == false)
					{
						if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
						{
							fireMahLazors(3);
							laserCounter++;
							timer = irandom(-25, -15);
						}
					}
					if (typeOfEnemy == et_flier)
					{
						if (angle < playerDirection + snappingAngle || angle > -playerDirection - snappingAngle)
						{
							fireMahLazors(irandom(0,1));
							timer = 3;							
						}
					}
				}
				if (typeOfEnemy == et_commander)
				{
					if (fliersFollowing < 6)
					{
						launchFliers();
						timer = irandom(-60, -30);
					}
				}
			}
			else
				timer++;
		}
	}
	//too far
	else if (distance > actionDistance && distance < detectionDistance)
	{
		if (typeOfEnemy == et_flier)
		{
			if (initiateFlierAssault == false)
			{
				//Follow the Commander ship!
				//Not actually following !!!??!
				if (angle >= 0 && angle < PI / 2) //1st quarter
				{
					if (flierMaster->angle < PI && flierMaster->angle > angle)
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
					}
				}
				else if (angle >= PI / 2 && angle < PI) //2nd quarter
				{
					if (flierMaster->angle < PI*1.5 && flierMaster->angle > angle)
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
					}
				}
				else if (angle >= PI && angle < PI*1.5)//3rd quarter
				{
					if (flierMaster->angle > angle)
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
					}
				}
				else //4th quarter
				{
					if (flierMaster->angle > angle || flierMaster->angle < PI / 2)
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
					}
				}
				xSpeed = cos(2 * PI - angle)*maxSpeed;
				ySpeed = sin(2 * PI - angle)*maxSpeed;
			}
			else if (initiateFlierAssault == true)
			{
				follow = true;
				xSpeed = cos(2 * PI - angle)*maxSpeed;
				ySpeed = sin(2 * PI - angle)*maxSpeed;
			}
		}
		else
		{
			follow = true;
			xSpeed = cos(2 * PI - angle)*maxSpeed;
			ySpeed = sin(2 * PI - angle)*maxSpeed;
		}
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

	componentIt = components.begin();
	while (componentIt != components.end())
	{
		if ((*componentIt)->alive() == false)
		{
			Component* temp_componentPointer = (*componentIt);
			components.erase(componentIt);
			delete temp_componentPointer;
			componentIt = components.begin();
		}
		else
			componentIt++;
	}

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->update();
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}


void Enemy::explosion() //I guess not with componentships
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
	float temp_coordinateModifier = resFactor*zoomFactor*textureRadius;
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y - temp_coordinateModifier, components[i]->x - b->x - temp_coordinateModifier);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x - temp_coordinateModifier, components[i]->y - temp_coordinateModifier);
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
		components[i]->hp = -999;
		//Component* temp_componentPointer = components[i];
		//components.erase(components.begin() + i);
		//delete temp_componentPointer;
		//return;
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


void Enemy::fireMahLazors(int dmg)
{
	//Lazors
	sf::VertexArray line(sf::Lines, 2);
	line[0].position = sf::Vector2f(this->screenX + components[0]->xOffset*resFactor*zoomFactor, this->screenY + components[0]->yOffset*resFactor*zoomFactor);
	line[1].position = sf::Vector2f(nearestComponent->screenX + (irandom(-15, 15))*resFactor*zoomFactor, nearestComponent->screenY + (irandom(-15, 15))*resFactor*zoomFactor);
	line[0].color = sf::Color::Red;
	line[1].color = sf::Color::Red;
	nearestComponent->hp -= dmg;

	mWindow.draw(line);
}


void Enemy::dodgeMove() //NO WORK
{
	std::cout << "dodge!" << std::endl;
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


void Enemy::launchFliers()
{
	mGame->objects.push_back(new Enemy(mWindow, mGame, mGame->objects, et_flier, this));
	mGame->objects[mGame->objects.size() - 1]->setLocation(x, y); //randomize a lil bit
	fliersFollowing++;
}