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

//Ship core keys
#define key_turnRight 0.2
#define key_turnLeft 0.3
#define key_accelerate 0.4
#define key_reverse 0.5
#define key_zoomIn 0.6
#define key_zoomOut 0.7
//Component specific key values
#define KEYB_fire 0.0001
#define KEYB_thrust 0.0002
#define KEYB_reload 0.0003
#define KEYB_vertical 0.0004
#define KEYB_horizontal 0.0005
#define KEYB_right 0.0006
#define KEYB_left 0.0007


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
std::string getBoolAsString(bool b);










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
	AxisType wheelInput = noAxis;

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
extern bool directionalMovement;
extern int moveJoystickId;
extern int verticalMoveAxisPolarity;
extern int horizontalMoveAxisPolarity;
extern sf::Joystick::Axis horizontalMoveAxis;
extern sf::Joystick::Axis verticalMoveAxis;
extern bool starMap;
extern float radarRange;
extern float radarSpeed;
extern float movementSpeed;
extern float rotationSpeed;
bool testInput(MyKeys k, sf::Event&);


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
	bi_confNodeDirectionalMovement,
	bi_confBindVerticalMoveAxis,
	bi_confBindHorizontalMoveAxis,
	bi_confBindAccelerate,
	bi_confBindTurnRight,
	bi_confBindTurnLeft,
	bi_confBindReverse,
	bi_confBindZoomIn,
	bi_confBindZoomOut,

	bi_confBindFire,
	bi_confNodeHoldToFire,
	bi_confBindReload,
	bi_confNodeMouseAim,
	bi_confNodeMouseAimRelative,
	bi_confNodeDirectionalAim,
	bi_confBindVerticalAimAxis,
	bi_confBindHorizontalAimAxis,
	bi_confBindTurnTurretRight,
	bi_confBindTurnTurretLeft,

	bi_confBindThrust,
	bi_confNodeHoldToThrust,
};



#endif