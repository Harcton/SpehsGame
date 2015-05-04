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
	//Upgrades
	int armor = 0;
	int angleModifier = 0; 
	int turret = 0; //Turret damage
	float engine = 0; //Acceleration increase
	int rotationDirection = 0;
	int repair = 0; //Repair amount
	int maxAngle = 0;
	float turnSpeed = 0;
	int capacity = 0; //magazine/charge capacity
	int rechargeSpeed = 0; //Reload & recharge rate
	float maxSpeed = 0; //bullet/engine speed
	int fireRateInterval = 0;
	bool hullMount = false;

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