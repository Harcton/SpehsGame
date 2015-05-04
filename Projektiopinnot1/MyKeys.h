#ifndef MYKEYS_H
#define MYKEYS_H
//Included dependencies
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>



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

#endif