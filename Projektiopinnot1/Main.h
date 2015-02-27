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
#define SPAWN_RANGE 500
#define DESPAWN_RANGE 2000

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern double resFactor;
extern double zoomFactor;



double getDistance(double, double, double, double);
int irandom(int min, int max);
bool flipCoin();
bool rollDie(unsigned int sides);









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
	double threshold = 20;
	//button sub
	int joystickButton = 0;
};
extern std::map<float, MyKeys> Keys;
bool testInput(MyKeys k);
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