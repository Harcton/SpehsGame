#ifndef PLAYERDATA_H
#define PLAYERDATA_H
//Included dependencies
#include "GridData.h" //includes "MyKeys.h"



class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	friend class Player;

	GridData grid[EDITOR_WIDTH][EDITOR_HEIGHT];
	bool directionalMovement;
	int moveJoystickId;
	int verticalMoveAxisPolarity;
	int horizontalMoveAxisPolarity;
	int money = STARTING_MONEY;
	int spehsX = 0;
	int spehsY = 0;
	sf::Joystick::Axis horizontalMoveAxis;
	sf::Joystick::Axis verticalMoveAxis;

	MyKeys keyAccelerate;
	MyKeys keyReverse;
	MyKeys keyTurnRight;
	MyKeys keyTurnLeft;
	MyKeys keyZoomIn;
	MyKeys keyZoomOut;

	MyKeys keyGrid[EDITOR_WIDTH][EDITOR_HEIGHT][7];

private:

};

#endif