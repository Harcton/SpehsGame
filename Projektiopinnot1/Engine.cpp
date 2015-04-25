#include "Main.h"
#include "Object.h"
#include "Player.h"
#include "Engine.h"


Engine::~Engine()
{
}
Engine::Engine(Object* mstr, double xo, double yo) : Component(mstr, mstr, xo, yo)
{
	std::cout << "  Engine construcotr  ";
	sprites.push_back(sf::Sprite());
	sprites[sprites.size() - 1].setTexture(RM.getTexture("engine.png"));
	sprites[sprites.size() - 1].setOrigin(70, 50);

	types.push_back(component::engine);
	textureRadius = 20;

	thrustStrength = 0.1;
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
		//temp_distance = getDistance(x, y, master->x, master->y);
		//temp_centerDirection = -1 * atan2(master->y - y, master->x - x);
		//temp_angleDifference = abs(temp_centerDirection - angle);
		//while (temp_angleDifference > PI)
		//	temp_angleDifference -= PI;
		//if (temp_angleDifference > PI / 2)
		//	temp_angleDifference = PI - temp_angleDifference;

		//std::cout << "\n1: " << temp_angleDifference;

		//master->xSpeed += pow(cos(temp_angleDifference), 2)*(power / 100.0f)*thrustStrength*cos(2 * PI - angle);
		//master->ySpeed += pow(cos(temp_angleDifference), 2)*(power / 100.0f)*thrustStrength*sin(2 * PI - angle);
		//master->turnSpeed += (abs(temp_angleDifference)/temp_angleDifference)*(pow(sin(temp_angleDifference), 2) * (power/100.0f)*thrustStrength)/temp_distance;
		
		/*std::cout << "\nFf " << forwardFactor;
		std::cout << "\nRf " << rotationFactor;
		std::cout << "\nCd " << coreDistance;
		std::cout << "\n\nxSpeed += " << forwardFactor*(power / 100.0f)*thrustStrength*cos(2 * PI - angle);
		std::cout << "\nySpeed += " << forwardFactor*(power / 100.0f)*thrustStrength*sin(2 * PI - angle);
		std::cout << "\nturnSpeed += " << (rotationFactor * (power / 100.0f)*thrustStrength) / coreDistance;*/
		
		charge--;
		master->xSpeed += forwardFactor*(power / 100.0f)*thrustStrength*cos(2 * PI - angle);
		master->ySpeed += forwardFactor*(power / 100.0f)*thrustStrength*sin(2 * PI - angle);
		if (rotationFactor > 0)
			master->turnSpeed +=  (rotationFactor * (power / 100.0f)*thrustStrength) / (coreDistance + 5);
		else if (rotationFactor < 0)
			master->turnSpeed += (rotationFactor * (power / 100.0f)*thrustStrength) / (coreDistance + 5);
	}
}


bool Engine::alive()
{
	return Component::alive();
}
void Engine::update()
{
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
