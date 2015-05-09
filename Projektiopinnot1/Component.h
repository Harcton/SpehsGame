#ifndef COMPONENT_H
#define COMPONENT_H
//Forward declared dependencies
class Object;
//Included dependencies
#include "Sprite.h"
#include <SFML/Graphics/CircleShape.hpp>


namespace component
{


	enum Type
	{
		hull,
		turret,
		engine,
	};

	enum DestructorMode
	{
		wreck,
		quit,
	};

}


static int currentId = 0;


class Component
{
public:
	~Component();
	Component(Object*, Object*, double, double);
	Component(Object*, Object*, double, double, int, int);
	Component(const Component& other);
	Component& operator=(Component);
	Component(Component&& other);
	Component& operator=(Component&&);

	friend class Game;
	friend class Object;
	friend class Player;
	friend class Enemy;
	friend class Seeker;
	friend class Sentinel;
	friend class Sentry;
	friend class Flier;
	friend class Commander;

	virtual void update();
	virtual bool alive();
	virtual void draw();
	void createChild(double, double, component::Type);

	//Virtual functions
	virtual void fire(){}
	virtual void reload(){}
	virtual void thrust(float){}
	//virtual void fixAngle(){}


	Object* master;
	Object* centerObj;
	int masterComponent;

	//offset compared to master origin
	double xOffset;
	double yOffset;
	double xOffsetOriginal;
	double yOffsetOriginal;
	
	// actual coordinates in space 
	double x;
	double y;

	double screenX;
	double screenY;

	std::vector<component::Type> types;
	double angle;
	double angleModifier = 0;
	double turningSpeed;
	double maxAngle;

	//Turret/Engine
	int capacity; // magazine/thrust charge
	int rechargeInterval; //Reload/thrust recharge speed
	float maxSpeed; //max bullet speed/ max engine speed

	//Turret
	int damage; //Turret damage
	int fireRateInterval;

	//Engine
	float thrustStrength; //thrust strength
	int rechargeAmount;

protected:
	std::vector<sf::Sprite> sprites;
	std::vector<sge::Sprite> animatedSprites;
	component::DestructorMode destructorMode = component::wreck;
	//Turret 
	sf::Sprite turretPlatform;
	float turretPlatformAngle = -1;
	sf::Sprite magazineSpr;

	bool mouseAim;
	bool mouseAimRelativeToCenter;
	int id;
	std::vector<int> childComponents;
	int hp;
	int maxHp;
	int textureRadius;

	int gridLocationX;
	int gridLocationY;

	//Turret
	bool hasFired = false;
	bool reloading = false;

	//Engine
	int charge;
	int rotationDirection; // negative = CCW (!)
	bool holdToThrust = true;
	bool autoThrusting = false;
};

#endif