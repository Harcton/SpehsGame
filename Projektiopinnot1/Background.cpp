#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Background.h"





Background::~Background()
{
	/*std::cout << "\nDeconstructing background...";*/
}
Background::Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>& hw) : homeVector(hw), Object(windowref, game)
{
	/*std::cout << "\nConstructing background...";*/
	tex.loadFromFile("Texture/sky.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	angle = (irandom(0, 360)/double(360)) * (2*PI);
	scale = double(irandom(0, 500) + 100) / 100;

	hasCollisions = false;

}





bool Background::updateBackground()
{
	



	Object::update();

	return true;
}
