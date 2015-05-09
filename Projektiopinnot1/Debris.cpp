#include "Main.h"
#include "Debris.h"

Debris::~Debris()
{
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& hw) : homeVector(hw), Object(windowref, game)
{
	randomizeLook();
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& hw, int cx, int cy) : homeVector(hw), Object(windowref, game, cx, cy)
{
	randomizeLook();
}

bool Debris::updateDebris()
{
	

	return Object::update();
}


void Debris::randomizeLook()
{
	spr.setTexture(RM.debris1Tex);
	spr.setOrigin(50, 50);
	turnSpeed = irandom(-6, 6) / double(360)*(2 * PI);
	textureRadius = 50;
}