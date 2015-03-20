#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"


int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double resFactor = WINDOW_HEIGHT/1080.0;
double zoomFactor = 1;
//Global controll settings
std::map<float, MyKeys> Keys;

//Global textures
sf::Texture skeletonTex;
sf::Texture commanderShipTex;


void main()
{

	skeletonTex.loadFromFile("Texture/skeleton.png");
	commanderShipTex.loadFromFile("Texture/enemy_commander_ship.png");

	//Randomize randomization...
	time_t t;
	srand((unsigned)time(&t));


	sf::RenderWindow mWindow{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wrench Space"}; //, sf::Style::Fullscreen
	mWindow.setFramerateLimit(60);
	mWindow.setMouseCursorVisible(true);


	Game game(mWindow);
	game.run();
}




//Returns a random integer between min and max
int irandom(int min, int max)
{
	return (min + (rand() % (int)(max - min + 1)));
}
//Return true with a chance of 50%
bool flipCoin()
{
	switch (irandom(0, 1))
	{
	default:
		return true;
	case 1:
		return  false;
	}
}
//Returns true with a chance of 1/sides
bool rollDie(unsigned int sides)
{
	switch (irandom(0, sides - 1))
	{
	case 0:
		return true;
	default:
		return false;
	}

}
double getDistance(double x1, double y1, double x2, double y2)
{
	int xDiff = abs(x1 - x2);
	int yDiff = abs(y1 - y2);

	return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

bool testInput(MyKeys k)
{
	//Mouse
	if (k.inputType == mouseInput && sf::Mouse::isButtonPressed(k.mouseButton))
		return (true);
	else if (k.inputType == mouseInput)
		return false;
	
	//Keyboard
	if (k.inputType == keyboardInput && sf::Keyboard::isKeyPressed(k.keyCode))
		return (true);
	else if (k.inputType == keyboardInput)
		return false;
	
	//Joystick
	if (sf::Joystick::isConnected(k.joystickIndex) == false)
		return false;
	if (k.inputType == joystickInput && ((k.axisType == positiveAxis && sf::Joystick::getAxisPosition(k.joystickIndex, k.joystickAxis) > k.threshold) || (k.axisType == negativeAxis && sf::Joystick::getAxisPosition(k.joystickIndex, k.joystickAxis) < -k.threshold) || (k.axisType == noAxis && sf::Joystick::isButtonPressed(k.joystickIndex, k.joystickButton))))
		return true;


	return (false);
}