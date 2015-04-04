#ifndef GRIDDATA_H
#define GRIDDATA_H

struct MyKeys;

class GridData
{
public:
	~GridData();
	GridData();
	friend class PlayerData;
	friend class Player;
	friend class ShipEditor;

private:
	int armor = 0;
	int rotation = 0; 
	int turret = 0; //Turret damage
	float engine = 0; //Acceleration increase
	int repair = 0; //Repair amount
	int maxAngle = 0;
	float turnSpeed = 0;
	int capacity = 0; //magazine/charge capacity
	float rechargeSpeed = 0; //Reload & recharge rate
	float maxSpeed = 0; //bullet/engine speed
	float recoilTime = 0;


	bool hullMount = false;

	bool childUp = false;
	bool childRight = false;
	bool childDown = false;
	bool childLeft = false;

	bool core = false;
	bool mouseAim = false;
	bool mouseAimRelativeToCenter = true;

	bool directionalTargetting = false;
	sf::Joystick::Axis horizontalAxis;
	sf::Joystick::Axis verticalAxis;

	MyKeys turretLeft;
	MyKeys turretRight;
	MyKeys turretFire;
	MyKeys engineThrust;
};

#endif