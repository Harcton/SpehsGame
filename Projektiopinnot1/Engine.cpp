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
	sprites[sprites.size() - 1].setOrigin(67, 50);

	animatedSprites.push_back(sge::Sprite("engine_fire.png"));
	animatedSprites[0].setOrigin(167, 50);
	animatedSprites[0].setFrameSize(200, 100);
	animatedSprites[0].setTilesetSize(1, 1);
	animatedSprites[0].setVisibility(false);


	//Charge bar
	chargeBar.setSize(sf::Vector2f(15.0f, 5.0f));
	chargeBar.setFillColor(sf::Color(255, 110, 70, 160));
	chargeBar.setOrigin(0, 2);
	chargeBarCenter.setSize(sf::Vector2f(15.0f, 3.0f));
	chargeBarCenter.setFillColor(sf::Color(255, 240, 120, 200));
	chargeBarCenter.setOrigin(0, 1);


	types.push_back(component::engine);
	textureRadius = 20;

	thrustStrength = 0.0008;
	capacity = 200;			//thrust charge
	charge = capacity;
	rechargeAmount = 1;	//thrust recharge speed

	thrusting = false;
	autoThrusting = false;
}

//Power parameter 0-100
void Engine::thrust(float power)
{
	if (charge > 0)//()
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
					animatedSprites[0].setVisibility(false);
				}
				else
				{
					std::cout << " Enabling... ";
					autoThrusting = true;
					thrustButtonReleased = false;
					animatedSprites[0].setVisibility(true);
				}
				return;
			}
			return;//Dont advance for user pressed thrust events, handle them in engine::update -> thrust(-100)
		}

		
		thrusting = true;
		charge--;
		//animatedSprites[0].setVisibility(true);

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
		thrusting = false;
	}
}


bool Engine::alive()
{
	return Component::alive();
}
void Engine::update()
{
	
	if (testInput(master->data->grid[gridLocationX][gridLocationY]->engineThrust, master->mGame->mEvent) == false)
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
	chargeBarCenter.setScale(chargeBar.getScale());
	chargeBar.setRotation(360.0f - (angle)*(180.0f/PI));
	chargeBarCenter.setRotation(chargeBar.getRotation());
	chargeBar.setPosition(screenX, screenY);
	chargeBarCenter.setPosition(chargeBar.getPosition());

	return;
}

void Engine::draw()
{
	//Engine fire sprite visibility state
	if (thrusting)
		animatedSprites[0].setVisibility(true);
	else
		animatedSprites[0].setVisibility(false);

	Component::draw();

	master->mGame->mWindow.draw(chargeBar);
	master->mGame->mWindow.draw(chargeBarCenter);
}
