#ifndef GRIDDATA_H
#define GRIDDATA_H
//Included dependencies
#include "MyKeys.h" //includes <SFML/Window/Joystick.hpp>



class GridData
{
public:
	~GridData();
	GridData();
	friend class PlayerData;
	friend class Player;
	friend class Engine;
	friend class ShipEditor;

private:
	//Modifications
	int angleModifier = 0; 
	int rotationDirection = 0;
	bool hullMount = false;
	short int refund = 0;

	//////////////////
	//Upgrades (in "The bigger the better" format)
	short int armor = 0;
	short int capacity = 1; //magazine/charge capacity
	short int rechargeSpeed = 1; //Reload & recharge rate
	
	short int engine = 0; //Thrust strength

	short int turret = 0; //Turret damage
	short int maxAngle = 1;
	short int turnSpeed = 1;
	short int bulletSpeed = 1;
	short int recoilTime = 1;
	///////////////////

	//Structural
	bool core = false;
	bool childUp = false;
	bool childRight = false;
	bool childDown = false;
	bool childLeft = false;

	//Control scheme
	bool mouseAim = false;
	bool mouseAimRelativeToCenter = true;
	bool holdToFire = true;

	bool directionalAim = false;
	int directionalJoystickId = 0;
	float verticalAxisPolarity = 1;
	float horizontalAxisPolarity = 1;
	sf::Joystick::Axis horizontalAxis;
	sf::Joystick::Axis verticalAxis;

	int red = 255;
	int green = 255;
	int blue = 255;

	//MyKeys
	MyKeys turretFire;
	MyKeys turretReload;
	MyKeys turretLeft;
	MyKeys turretRight;
	MyKeys engineThrust;
};

#endif