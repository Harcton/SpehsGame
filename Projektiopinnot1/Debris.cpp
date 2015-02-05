#include "Main.h"
#include "Game.h"
#include "Object.h"
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
	mass = 100;

	tex.loadFromFile("Texture/debris1.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	turnSpeed = irandom(-6, 6) / double(360)*(2 * PI);
	textureRadius = 50;
}