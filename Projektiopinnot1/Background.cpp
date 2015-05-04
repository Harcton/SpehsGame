#include "Main.h"
#include "Background.h"





Background::~Background()
{
	/*std::cout << "\nDeconstructing background...";*/
}
Background::Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>& hw) : homeVector(hw), Object(windowref, game)
{
	/*std::cout << "\nConstructing background...";*/
	spr.setTexture(RM.getTexture("sky.png"));
	spr.setOrigin(50, 50);
	angle = (irandom(0, 360)/double(360)) * (2*PI);
	scale = double(irandom(400, 600) + 100) / 100;
	turnSpeed = irandom(-1, 1)*0.1 / double(360);

	hasCollisionChecks = false;


	//Randomize x/y
	if (flipCoin())
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	if (flipCoin())
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);

}





bool Background::updateBackground()
{
	return Object::update();
}
