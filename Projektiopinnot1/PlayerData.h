#ifndef PLAYERDATA_H
#define PLAYERDATA_H


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