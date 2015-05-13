#include "Main.h"
#include "Object.h"
#include "Engine.h"
#include "Game.h"
#include "PlayerData.h"



Engine::~Engine()
{
}
Engine::Engine(Object* mstr, double xo, double yo) : Component(mstr, mstr, xo, yo)
{
	//Stats
	thrustStrength = 0.0006;
	capacity = 200;			//thrust charge
	rechargeAmount = 1;	//thrust recharge speed
	chargeConsumption = 1;




	thrusting = false;
	autoThrusting = false;



	engineFlameSpr.setTexture(RM.engineFireAnimation);
	engineFlameSpr.setVisibility(false);
	engineFlameSpr.setOrigin(167, 50);
	engineFlameSpr.setFrameSize(200, 100);
	engineFlameSpr.setTilesetSize(2, 3);
	engineFlameSpr.setFrameDuration(1);

	//Charge bar
	chargeBar.setSize(sf::Vector2f(15.0f, 4.0f));
	chargeBar.setFillColor(sf::Color(255, 110, 70, 160));
	chargeBar.setOrigin(0, 2);

	type = component::engine;
	textureRadius = 20;
	charge = capacity;
}

//Power parameter 0-100
void Engine::thrust(float power)
{
	if (charge >= chargeConsumption)//()
	{
		//Charge threshold
		if (thrusting == false && charge < capacity*0.5f)
		{
			thrusting = false;
			autoThrusting = false;
			return;
		}


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
					thrusting = false;
					thrustButtonReleased = false;
					engineFlameSpr.setVisibility(false);
				}
				else
				{
					std::cout << " Enabling... ";
					autoThrusting = true;
					thrustButtonReleased = false;
					engineFlameSpr.setVisibility(true);
				}
				return;
			}
			return;//Dont advance for user pressed thrust events, handle them in engine::update -> thrust(-100)
		}

		
		thrusting = true;
		charge -= chargeConsumption;

		if (rotationDirection == 0)
		{
			master->xSpeed += power*(cos(2 * PI - angle)*thrustStrength*(10.0 / (10 + master->getMass())));
			master->ySpeed += power*(sin(2 * PI - angle)*thrustStrength*(10.0 / (10 + master->getMass())));
			master->relativeSpeedX += power*(cos(2 * PI - angle)*thrustStrength*(10.0 / (10 + master->getMass())));
			master->relativeSpeedY += power*(sin(2 * PI - angle)*thrustStrength*(10.0 / (10 + master->getMass())));
		}
		else
		{
			master->turnSpeed += rotationDirection*thrustStrength*(10.0 / (10 + master->getMass()));
		}


	}
	else
	{//Ran out of charge
		autoThrusting = false;
		thrusting = false;
	}
}


bool Engine::alive()
{
	return Component::alive();
}
void Engine::update()
{
	
	if (testInput(master->dataPtr->grid[gridLocationX][gridLocationY].engineThrust, master->mGame->mEvent) == false)
		if (holdToThrust == false)
			thrustButtonReleased = true;
		else
			thrusting = false;

	if (autoThrusting == true)
		thrust(-100);

	if (charge < capacity && thrusting == false)
	{//Add charge if not thrusting
		charge += rechargeAmount;
		if (charge > capacity)
			charge = capacity;
	}


	Component::update();
	//Charge bar
	chargeBar.setScale(zoomFactor*resFactor*(float(charge) / capacity), zoomFactor*resFactor);
	chargeBar.setRotation(360.0f - (angle)*(180.0f/PI));
	chargeBar.setPosition(screenX, screenY);

	return;
}

void Engine::draw()
{

	Component::draw();

	master->mGame->mWindow.draw(chargeBar);
}
void Engine::drawEngineFlame()
{
	//Engine fire sprite visibility state
	if (thrusting)
	{
		engineFlameSpr.setVisibility(true);
		engineFlameSpr.setPosition(screenX, screenY);
		engineFlameSpr.setRotation(360 - (angle / PI) * 180);
		engineFlameSpr.setScale(resFactor*zoomFactor, resFactor*zoomFactor);
	}
	else
		engineFlameSpr.setVisibility(false);

	

	engineFlameSpr.draw(master->mGame->mWindow);
}
sge::Sprite& Engine::getEngineFlameSpr()
{
	return engineFlameSpr;
}

sf::RectangleShape& Engine::getChargeRectangle()
{
	return chargeBar;
}

bool Engine::isThrusting()
{
	return thrusting;
}
