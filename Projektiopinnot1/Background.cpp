#include "Main.h"
#include "Game.h"
#include "Background.h"





Background::~Background()
{
	/*std::cout << "\nDeconstructing background...";*/
}
Background::Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>& hw) : homeVector(hw), Object(windowref, game)
{
	spr.setOrigin(50, 50);
	angle = (irandom(0, 360)/double(360)) * (2*PI);
	switch (irandom(0, 5))
	{
	case 0:
	case 1:
	case 2:
		spr.setTexture(RM.sky1Tex);
		turnSpeed = (1 - irandom(0, 1)*2)*0.1 / double(360);
		scale = double(irandom(250, 650) + 100) / 100;
		break;
	case 3:
	case 4:
		spr.setTexture(RM.sky2Tex);
		turnSpeed = (1 - irandom(0, 1) * 2)*0.01 / double(360);
		scale = double(irandom(200, 450) + 100) / 100;
		break;
	case 5:
		spr.setTexture(RM.sky3Tex);
		turnSpeed = (1 - irandom(0, 1) * 2)*0.01 / double(360);
		scale = double(irandom(200, 450) + 100) / 100;
		break;
	}

	hasCollisionChecks = false;


	//Randomize x/y
	float tempLocator = irandom(0, 359) * (PI / 180.0f);

	location.x = centerObj->getLocation().x + (DESPAWN_RANGE) * cos(tempLocator);
	location.y = centerObj->getLocation().y + (DESPAWN_RANGE) * sin(tempLocator);
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
	distanceFromPlayer = abs(location.distanceFrom(centerObj->getLocation())/4 - DESPAWN_RANGE/4);
	if (distanceFromPlayer > 255)
		distanceFromPlayer = 255;

	//std::cout << distanceFromPlayer << std::endl;

	spr.setColor(sf::Color(100, 100, 100, distanceFromPlayer));

	return Object::update();
}
