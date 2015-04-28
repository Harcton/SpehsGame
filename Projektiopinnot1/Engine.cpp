#include "Main.h"
#include "Object.h"
#include "Player.h"
#include "Engine.h"
#include "GridData.h"
#include "PlayerData.h"
#include "Game.h"
#include "Sprite.h"


Engine::~Engine()
{
}
Engine::Engine(Object* mstr, double xo, double yo) : Component(mstr, mstr, xo, yo)
{
	std::cout << "  Engine construcotr  ";
	sprites.push_back(sf::Sprite());
	sprites[sprites.size() - 1].setTexture(RM.getTexture("engine.png"));
	sprites[sprites.size() - 1].setOrigin(70, 50);

	animatedSprites.push_back(sge::Sprite("engine_fire.png"));
	animatedSprites[0].setOrigin(170, 50);
	//animatedSprites[0].setTextureRect();


	types.push_back(component::engine);
	textureRadius = 20;

	thrustStrength = 0.0008;
	capacity = 20;			//thrust charge
	charge = capacity;
	rechargeInterval = 10;	//thrust recharge speed
	rechargeTimer = 0;
}

//Power parameter 0-100
void Engine::thrust(float power)
{
	if (true)//(charge > 0)
	{
		//Handle hold to thrust
		if (power == -100)
		{//Automatic thrust using on/off thrust
			power = 100;
		}
		else if (holdToThrust == false)
		{//Player is holding the thrust key -> handle on/off state
			if (thrustButtonReleased == true)//This is the second time (->switch off) the player is pressing the button
			{//Player wishes to cancel thrusting
				std::cout << " Press->";
				if (autoThrusting)
				{
					std::cout << " Disabling... ";
					autoThrusting = false;
					thrustButtonReleased = false;
				}
				else
				{
					std::cout << " Enabling... ";
					autoThrusting = true;
					thrustButtonReleased = false;
				}
				return;
			}
			return;//Dont advance for user pressed thrust events, handle them in engine::update -> thrust(-100)
		}


		charge--;
		if (rotationDirection == 0)
		{
			master->xSpeed += power*(cos(2 * PI - angle)*thrustStrength);
			master->ySpeed += power*(sin(2 * PI - angle)*thrustStrength);
			master->relativeSpeedX += power*(cos(2 * PI - angle)*thrustStrength);
			master->relativeSpeedY += power*(sin(2 * PI - angle)*thrustStrength);
		}
		else
		{
			master->turnSpeed += rotationDirection*thrustStrength;
		}


	}
	else
	{//Ran out of charge
		autoThrusting = false;
	}
}


bool Engine::alive()
{
	return Component::alive();
}
void Engine::update()
{
	//Hold to thrust feature
	if (holdToThrust == false)
		if (testInput(master->data->grid[gridLocationX][gridLocationY]->engineThrust, master->mGame->mEvent) == false)
			thrustButtonReleased = true;
	if (autoThrusting == true)
		thrust(-100);

	if (rechargeTimer <= 0 && charge < capacity)
	{//Reset timer and add charge
		rechargeTimer = rechargeInterval;
		charge++;
	}
	else
		rechargeTimer--;

	Component::update();
	return;
}
