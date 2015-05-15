#include "Main.h"
#include "Game.h"
#include "Background.h"





Background::~Background()
{
	/*std::cout << "\nDeconstructing background...";*/
}
Background::Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>& hw) : homeVector(hw), Object(windowref, game)
{
	/*std::cout << "\nConstructing background...";*/
	spr.setTexture(RM.sky1Tex);
	spr.setOrigin(50, 50);
	angle = (irandom(0, 360)/double(360)) * (2*PI);
	scale = double(irandom(250, 650) + 100) / 100;
	turnSpeed = irandom(-1, 1)*0.1 / double(360);

	hasCollisionChecks = false;


	//Randomize x/y
	float tempLocator = irandom(0, 359) * (PI / 180.0f);

	x = centerObj->x + (DESPAWN_RANGE) * cos(tempLocator);
	y = centerObj->y + (DESPAWN_RANGE) * sin(tempLocator);
	/*if (flipCoin())
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	if (flipCoin())
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);*/

}





bool Background::updateBackground()
{
	distanceFromPlayer = abs(getDistance(this->x, this->y, mGame->playerObj->x, mGame->playerObj->y)/4 - DESPAWN_RANGE/4);
	if (distanceFromPlayer > 255)
		distanceFromPlayer = 255;

	//std::cout << distanceFromPlayer << std::endl;

	spr.setColor(sf::Color(100, 100, 100, distanceFromPlayer));

	return Object::update();
}
