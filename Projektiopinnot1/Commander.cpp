#include "Main.h"
#include "Commander.h"
#include "Game.h"
#include "Turret.h"
#include "Flier.h"



Commander::Commander(sf::RenderWindow& windowref, Game* game, int behaviourLevel) : Enemy(windowref, game)
{
	enemyBehaviourLevel = behaviourLevel;

	fleeing = false;
	initiateFlierAssault = false;
	aggroRange = SPAWN_RANGE;
	maxActionRange = 2300;
	closeRange = 1000;
	maxTurnSpeedLimit = 0.001;
	maxSpeedLimit = 0.1;
	accelerationConstant = 0.01;
	turnAccelerationConstant = 0.0002;
	closeAngle = 0.001;

	//-9
	components.push_back(new Component(this, mGame->playerObj, -325, -100)); //Component 3 (REAR)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(0, 300, 200, 200));
	components.back()->textureRadius = 100;
	//-8
	components.push_back(new Component(this, mGame->playerObj, -300, -50)); //Component 5 (R-REAR WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(400, 300, 200, 200));
	components.back()->textureRadius = 100;
	//-7
	components.push_back(new Component(this, mGame->playerObj, -300, -150)); //Component 4 (L-REAR WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(200, 300, 200, 200));
	components.back()->textureRadius = 100;
	//-6
	components.push_back(new Component(this, mGame->playerObj, -200, -50)); //Component 7 (R-CORE WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(700, 0, 200, 200));
	components.back()->textureRadius = 100;
	//-5
	components.push_back(new Component(this, mGame->playerObj, -200, -150)); //Component 6 (L-CORE WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(500, 0, 200, 200));
	components.back()->textureRadius = 100;
	//-4
	components.push_back(new Component(this, mGame->playerObj, -50, -50)); //Component 9 (R-FRONT WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(800, 300, 200, 200));
	components.back()->textureRadius = 100;
	//-3
	components.push_back(new Component(this, mGame->playerObj, -50, -150)); //Component 8 (L-FRONT WING)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(600, 300, 200, 200));
	components.back()->textureRadius = 100;
	//-2
	components.push_back(new Component(this, mGame->playerObj, -250, -100)); //Component 2 (CORE) //set correct coordinates for everyone...
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(100, 100);
	components.back()->sprites.back().setTextureRect(sf::IntRect(300, 0, 200, 200));
	components.back()->textureRadius = 100;
	//-1
	components.push_back(new Component(this, mGame->playerObj, -100, -100)); //Component 1 (TANK) //(0,0)
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.getTexture("enemy_commander_ship_upgraded.png"));
	components.back()->sprites.back().setOrigin(150, 150);
	components.back()->sprites.back().setTextureRect(sf::IntRect(0, 0, 300, 300));
	components.back()->textureRadius = 150;

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
	//Counters
	shootingCounter++;
	flierAttackCounter++;
	releaseFlier++;

	AIupdate();
	if (components.size() > 0)
	{
		HPMemory = components[0]->hp;
	}	

	return Enemy::update();
}


void Commander::AIupdate()//maybe not follow true all the time
{
	if (flierAttackCounter >= 600)
	{
		initiateFlierAssault = true;
		flierAttackCounter = 0;
		fliersFollowing = 0;
	}
	else
	{
		initiateFlierAssault = false;
	}

	if (fleeing)
	{
		//
	}
	else if (distance < closeRange) //Close state
	{
		follow = true;
		xSpeed = -(cos(2 * PI - angle))*accelerationConstant;
		ySpeed = -(sin(2 * PI - angle))*accelerationConstant;
		if (shootingCounter >= 8)
		{
			if (angle < playerDirection + closeAngle && angle > -playerDirection - closeAngle)
				for (unsigned int i = 0; i < components.size(); i++)
					for (unsigned int k = 0; k < components[i]->types.size(); k++)
						if (components[i]->types[k] == component::turret)
						{
							components[i]->fire();
							shootingCounter = irandom(-30, -10);
						}
		}
	}
	else if (distance > closeRange && distance < maxActionRange) //Active state
	{
		follow = true;
		xSpeed += (cos(angle))*accelerationConstant;
		ySpeed += (sin(angle))*accelerationConstant;

		if (releaseFlier > 60)
		{
			if (fliersFollowing < 6)
			{
				launchFliers();
				releaseFlier = 0;
			}
		}
	}
	else if (distance > maxActionRange && distance < aggroRange) //Detection state
	{
		follow = true;
		xSpeed += cos(2 * PI - angle)*accelerationConstant;
		ySpeed += sin(2 * PI - angle)*accelerationConstant;
	}
	else //Passive state
	{
		follow = false;
		xSpeed += cos(angle)*accelerationConstant;
		ySpeed += sin(angle)*accelerationConstant;
	}
}


void Commander::launchFliers()
{
	mGame->objects.push_back(new Flier(mWindow, mGame, 1/*, this*/));
	mGame->objects.back()->setLocation(x, y); //randomize a lil bit
	fliersFollowing++;
}