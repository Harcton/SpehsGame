#include "Main.h"
#include "Commander.h"
#include "Game.h"
#include "Turret.h"
#include "Flier.h"



Commander::Commander(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;
	state = state_spawned;
	metal = irandom(25, 35)*((enemyBehaviourLevel * 5 + 4) / 3);

	angle = playerDirection;
	fleeing = false;
	initiateFlierAssault = false;
	aggroRange = 2500;
	maxActionRange = 2300;
	closeRange = 1000;
	maxTurnSpeedLimit = 0.001;
	maxSpeedLimit = 0.1;
	accelerationConstant = 0.05;
	turnAccelerationConstant = 0.0003;
	closeAngle = 0.001;

	//Reserve memory for each component
	components.reserve(11);
	//-9
	components.push_back(new Component(this, mGame->playerObj, -325, -100)); //Component 3 (REAR)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(0, 300, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 80 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-8
	components.push_back(new Component(this, mGame->playerObj, -300, -50)); //Component 5 (R-REAR WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(400, 300, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 50 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-7
	components.push_back(new Component(this, mGame->playerObj, -300, -150)); //Component 4 (L-REAR WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(200, 300, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 50 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-6
	components.push_back(new Component(this, mGame->playerObj, -200, -50)); //Component 7 (R-CORE WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(700, 0, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 75 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-5
	components.push_back(new Component(this, mGame->playerObj, -200, -150)); //Component 6 (L-CORE WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(500, 0, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 75 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-4
	components.push_back(new Component(this, mGame->playerObj, -50, -50)); //Component 9 (R-FRONT WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(800, 300, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 50 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-3
	components.push_back(new Component(this, mGame->playerObj, -50, -150)); //Component 8 (L-FRONT WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(600, 300, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 50 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-2
	components.push_back(new Component(this, mGame->playerObj, -250, -100)); //Component 2 (CORE) //set correct coordinates for everyone...
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(300, 0, 200, 200));
	components.back()->textureRadius = 100;

	components.back()->hp = 100 + (enemyBehaviourLevel * 15);
	components.back()->maxHp = components.back()->hp;
	//-1
	components.push_back(new Component(this, mGame->playerObj, -100, -100)); //Component 1 (TANK) //(0,0)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.commanderTex);
	components.back()->sprites.back().setOrigin(150, 150);
	components.back()->sprites.back().setTextureRect(sf::IntRect(0, 0, 300, 300));
	components.back()->textureRadius = 150;

	components.back()->hp = 200 + (enemyBehaviourLevel * 20);
	components.back()->maxHp = components.back()->hp;

	components[components.size() - 2]->childComponents.push_back(components.back()->id); //TANK TO CORE
	components[components.size() - 2]->childComponents.push_back(components[components.size() - 6]->id); //R-CORE WING TO CORE
	components[components.size() - 2]->childComponents.push_back(components[components.size() - 5]->id); //L-CORE WING TO CORE
	components[components.size() - 2]->childComponents.push_back(components[components.size() - 9]->id); //REAR TO CORE
	components.back()->childComponents.push_back(components[components.size() - 4]->id); //R-FRONT WING TO TANK
	components.back()->childComponents.push_back(components[components.size() - 3]->id); //L-FRONT WING TO TANK
	components[components.size() - 5]->childComponents.push_back(components[components.size() - 7]->id); //L-REAR WING TO L-CORE WING
	components[components.size() - 6]->childComponents.push_back(components[components.size() - 8]->id); //R-REAR WING TO R-CORE WING

	components.push_back(new Turret(this, centerObj, -200, -150));
	components[components.size() - 5]->childComponents.push_back(components.back()->id);
	components.push_back(new Turret(this, centerObj, -200, -50));
	components[components.size() - 6]->childComponents.push_back(components.back()->id);
}


Commander::~Commander()
{
}


bool Commander::update()
{
	//When player is kill
	if (mGame->playerObj->components.size() == 0)
	{
		state = state_victory;
	}

	//Counters
	shootingCounter++;
	releaseFlier++;

	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}
	memoryState = state;

	return Enemy::update();
}


void Commander::AIupdate()//maybe not follow true all the time
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
		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
		return;
	}


	else
	{
		initiateFlierAssault = false;
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
		xSpeed = -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed = -(sin(2 * PI - angle))*accelerationConstant;
		if (shootingCounter >= 8)
		{
			if (angle < playerDirection + closeAngle && angle > -playerDirection - closeAngle)
				for (unsigned int i = 0; i < components.size(); i++)
					if (components[i]->type == component::turret)
						{
							components[i]->fire();
							shootingCounter = irandom(-30, -10);
						}
		}
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		state = state_active;

		if (stationDistance < 3200)
		{
			fleeing = true;
			return;
		}

		follow = true;
		xSpeed += (cos(angle))*accelerationConstant;
		ySpeed += (sin(angle))*accelerationConstant;

		if (releaseFlier > (90 / ((enemyBehaviourLevel + 10) / 10)) )
		{
			launchFliers();
			releaseFlier = 0;
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		state = state_detected;

		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
	{
		state = state_passive;

		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
	}
}


void Commander::launchFliers()
{
	if (mGame->objects.size() >= MAX_OBJECTS)
		return;

	mGame->objects.push_back(new Flier(mWindow, mGame, enemyBehaviourLevel));
	mGame->objects.back()->setLocation(x, y); //randomize a lil bit
}


void Commander::flee()
{
	follow = false;
	negFollow = true;
	xSpeed += (cos(2 * PI - angle))*accelerationConstant;
	ySpeed += (sin(2 * PI - angle))*accelerationConstant;
}