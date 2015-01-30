#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Debris.h"

Debris::~Debris()
{
}
Debris::Debris(sf::RenderWindow& windowref, Game* game) : Object(windowref, game)
{
	randomizeLook();
}
Debris::Debris(sf::RenderWindow& windowref, Game* game, int cx, int cy) : Object(windowref, game, cx, cy)
{
	randomizeLook();
}




void Debris::randomizeLook()
{
	tex.loadFromFile("Texture/debris1.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	turnSpeed = irandom(-6, 6) / double(360)*(2 * PI);
	textureRadius = 50;
}