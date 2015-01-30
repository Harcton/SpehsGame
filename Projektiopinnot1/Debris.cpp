#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Debris.h"

Debris::~Debris()
{
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, std::vector<Debris*>& hw) : homeVector(hw), Object(windowref, game)
{
	randomizeLook();
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, std::vector<Debris*>& hw, int cx, int cy) : homeVector(hw), Object(windowref, game, cx, cy)
{
	randomizeLook();
}

bool Debris::updateDebris()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	Object::update();
}


void Debris::randomizeLook()
{
	mass = 10;

	tex.loadFromFile("Texture/debris1.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	turnSpeed = irandom(-6, 6) / double(360)*(2 * PI);
	textureRadius = 50;
}