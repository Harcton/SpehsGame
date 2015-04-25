#include "Main.h"
#include "GridData.h"


GridData::~GridData()
{
}
GridData::GridData()
{

	//Default key bindings:
	turretFire.inputType = mouseInput;
	turretFire.mouseButton = sf::Mouse::Left;

	turretReload.inputType = keyboardInput;
	turretReload.keyCode = sf::Keyboard::R;

	turretRight.inputType = keyboardInput;
	turretRight.keyCode = sf::Keyboard::E;
	turretLeft.inputType = keyboardInput;
	turretLeft.keyCode = sf::Keyboard::Q;

	engineThrust.inputType = keyboardInput;
	engineThrust.keyCode = sf::Keyboard::Space;
	//engineThrust.inputType = joystickInput;
	//engineThrust.joystickIndex = 0;
	//engineThrust.axisType = negativeAxis;
	//engineThrust.joystickAxis = sf::Joystick::Z;	

	horizontalAxis = sf::Joystick::Axis::X;
	verticalAxis = sf::Joystick::Axis::Y;


}


