#include "Main.h"
#include "GridData.h"
#include "PlayerData.h"


PlayerData::~PlayerData()
{
	std::cout << "\nPlayerData destructor";
	//free grid element memory
	//for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
	//	for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
	//		delete grid[ex][ey];
}
PlayerData::PlayerData()
{
	//Initialize grid elements
	for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
			grid[ex][ey] = GridData();
		}

	//Initialize controls
	directionalMovement = false;
	moveJoystickId = 0;
	verticalMoveAxisPolarity = 1;
	horizontalMoveAxisPolarity = 1;
	horizontalMoveAxis = sf::Joystick::Axis::X;
	verticalMoveAxis = sf::Joystick::Axis::Y;
	//Keyboard default setup
	keyTurnRight.inputType = keyboardInput;
	keyTurnRight.keyCode = sf::Keyboard::D;

	keyTurnLeft.inputType = keyboardInput;
	keyTurnLeft.keyCode = sf::Keyboard::A;

	keyReverse.inputType = keyboardInput;
	keyReverse.keyCode = sf::Keyboard::S;

	keyAccelerate.inputType = keyboardInput;
	keyAccelerate.keyCode = sf::Keyboard::W;

	keyZoomIn.inputType = keyboardInput;
	keyZoomIn.keyCode = sf::Keyboard::Period;

	keyZoomOut.inputType = keyboardInput;
	keyZoomOut.keyCode = sf::Keyboard::Comma;

	//Default ship
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].armor = 1;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].core = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].childUp = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].childDown = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].childLeft = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)].childRight = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0) - 1].armor = 1;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0) + 1].armor = 1;
	grid[int(EDITOR_WIDTH / 2.0) - 1][int(EDITOR_HEIGHT / 2.0)].armor = 1;
	grid[int(EDITOR_WIDTH / 2.0) + 1][int(EDITOR_HEIGHT / 2.0)].armor = 1;

}


