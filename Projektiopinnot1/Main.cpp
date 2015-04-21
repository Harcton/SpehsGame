#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"
#include "GridData.h"
#include "ResourceManager.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;
double resFactor = WINDOW_HEIGHT/1080.0;
double zoomFactor = 1;
//Global control settings
bool directionalMovement = false;
int moveJoystickId = 0;
int verticalMoveAxisPolarity = 1;
int horizontalMoveAxisPolarity = 1;
sf::Joystick::Axis horizontalMoveAxis = sf::Joystick::Axis::X;
sf::Joystick::Axis verticalMoveAxis = sf::Joystick::Axis::Y;
std::map<float, MyKeys> coreKeys;
std::map<float, MyKeys> componentKeys;

bool starMap = false;
float radarRange = 0;
float radarSpeed = 0;
float movementSpeed = 0.0004;
float rotationSpeed = PI / 180000;

//Global textures
ResourceManager RM;
void initializeResourceManager(ResourceManager&);


void main()
{
	initializeResourceManager(RM);
	

	//Randomize randomization...
	time_t t;
	srand((unsigned)time(&t));


	sf::RenderWindow mWindow{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wrench Space"}; //, sf::Style::Fullscreen
	mWindow.setFramerateLimit(60);
	mWindow.setMouseCursorVisible(true);


	Game game(mWindow);
	game.run();
}



void initializeResourceManager(ResourceManager& rm)
{
//TEXTURES
	//PLAYER
	rm.loadTexture("Texture/Player/skeleton.png");
	rm.loadTexture("Texture/Player/turret.png");

	//ENEMY
	rm.loadTexture("Texture/Enemy/enemy_commander_ship_upgraded.png");
	rm.loadTexture("Texture/Enemy/Flier.png");
	rm.loadTexture("Texture/Enemy/enemy_base_purple.png");
	rm.loadTexture("Texture/Enemy/enemy_base_green.png");
	rm.loadTexture("Texture/Enemy/enemy_base.png");

	//MENU
	rm.loadTexture("Texture/Menu/inheritanceArrow.png");
	rm.loadTexture("Texture/Menu/xButton.png");
	rm.loadTexture("Texture/Menu/circleSlider.png");
	rm.loadTexture("Texture/Menu/slider1.png");
	rm.loadTexture("Texture/Menu/editorTurret.png");
	
	//EFFECTS

	//BACKGROUNDS
	rm.loadTexture("Texture/Background/sky.png");

	//MISC
	rm.loadTexture("Texture/Misc/bullet1.png");
	rm.loadTexture("Texture/Misc/debris1.png");



//SOUND BUFFERS
	//coming soon



	std::cout << "\nResourceManager intialized";
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

bool testInput(MyKeys k, sf::Event& mEvent)
{
	//Mouse
	if (k.inputType == mouseInput && sf::Mouse::isButtonPressed(k.mouseButton) && k.wheelInput == noAxis)
		return true;
	else if (k.inputType == mouseInput && k.wheelInput != noAxis)
	{
		if (mEvent.type == sf::Event::MouseWheelMoved)
			if ((mEvent.mouseWheel.delta > 0 && k.wheelInput == positiveAxis) || (mEvent.mouseWheel.delta < 0 && k.wheelInput == negativeAxis))
			{
				mEvent.mouseWheel.delta = 0;
				return true;
			}
		return false;		
	}
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

//Returns the amount cut-out from the zoomFactor
float limitZoom()
{
	float cutValue = 0;
	if (zoomFactor < 0.2)
	{
		cutValue += 0.2 - zoomFactor;
		zoomFactor = 0.2;
	}
	else if (zoomFactor > 4)
	{
		cutValue += zoomFactor - 4;
		zoomFactor = 4;
	}
	return cutValue;
}


std::string getBoolAsString(bool b)
{
	if (b)
		return "True";
	else
		return "False";
}