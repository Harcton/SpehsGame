#ifndef PLAYERDATA_H
#define PLAYERDATA_H


class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	friend class Player;

	GridData* grid[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::map<float, MyKeys> coreKeys;
	std::map<float, MyKeys> componentKeys;
	bool directionalMovement;
	int moveJoystickId;
	int verticalMoveAxisPolarity;
	int horizontalMoveAxisPolarity;
	sf::Joystick::Axis horizontalMoveAxis;
	sf::Joystick::Axis verticalMoveAxis;

private:

};

#endif