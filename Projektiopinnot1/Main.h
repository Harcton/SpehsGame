#ifndef MAIN_H
#define MAIN_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <math.h>
#include <cmath>
#include <time.h>
#include <map>


#define PI 3.14159265
#define SPAWN_RANGE 1800
#define DESPAWN_RANGE 2200

#define EDITOR_WIDTH 41
#define EDITOR_HEIGHT 41

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern double resFactor;
extern double zoomFactor;

//global textures
extern sf::Texture skeletonTex;
extern sf::Texture editorTurretTex;
extern sf::Texture editorEngineTex;
extern sf::Texture commanderShipTex;



double getDistance(double, double, double, double);
int irandom(int min, int max);
bool flipCoin();
bool rollDie(unsigned int sides);
float limitZoom();









//Global controll settings
enum InputType
{
	keyboardInput,
	mouseInput,
	joystickInput
};
enum AxisType
{
	negativeAxis,
	noAxis,
	positiveAxis
};
struct MyKeys
{
	//Define type, common for all keys
	InputType inputType;

	//Mouse
	sf::Mouse::Button mouseButton;

	//Keyboard
	sf::Keyboard::Key keyCode;
	
	//Joystick common
	AxisType axisType = noAxis;
	int joystickIndex = 0;
	//axis sub
	sf::Joystick::Axis joystickAxis;
	float threshold = 20;
	//button sub
	int joystickButton = 0;
};
extern std::map<float, MyKeys> coreKeys;
extern std::map<float, MyKeys> componentKeys;
bool testInput(MyKeys k);


enum ButtonId
{
	bi_false,
	bi_true,
	bi_actionUpgradeArmor,
	bi_actionTurret,
	bi_actionEngine,
	bi_actionConfiguration,
	bi_actionScrap,

	bi_confExit,
	bi_confBindAccelerate,
	bi_confBindReverse,
	bi_confBindTurnRight,
	bi_confBindTurnLeft,
	bi_confBindZoomIn,
	bi_confBindZoomOut,
};

#define key_shoot 0.1
#define key_turnRight 0.2
#define key_turnLeft 0.3
#define key_accelerate 0.4
#define key_reverse 0.5
#define key_zoomIn 0.6
#define key_zoomOut 0.7
//In order to have infinte turrets there must be infinite definitions...
#define key_turnTurret1Right 1
#define key_fireTurret1 1.1
#define key_turnTurret1Left -1
//...


#endif