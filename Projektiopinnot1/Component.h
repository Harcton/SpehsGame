#ifndef COMPONENT_H
#define COMPONENT_H
//Forward declared dependencies
class Object;
//Included dependencies
#include "Sprite.h"
#include "Location.h"
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
	virtual void setLaserPointerColor(sf::Color&&){};
	virtual sf::RectangleShape& getChargeRectangle(){ return sf::RectangleShape(); }
	virtual sge::Sprite& getEngineFlameSpr(){ return sge::Sprite(); }
	virtual void drawEngineFlame(){}
	virtual void drawLaserPointer(){}
	virtual bool isThrusting(){ return false; }
	virtual void setBulletTexPtr(sf::Texture*){}
	virtual sf::Sprite& getMagazineSpr(){ return sf::Sprite(); }

	Object* master;
	Object* centerObj;
	int masterComponent;

	//offset compared to master origin
	double xOffset;
	double yOffset;
	double xOffsetOriginal;
	double yOffsetOriginal;

	double screenX;
	double screenY;

	component::Type type;
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
	Location location;
	//Turret 
	sf::Sprite turretPlatform;
	float turretPlatformAngle = -1;

	bool mouseAim;
	bool mouseAimRelativeToCenter;
	int id;
	std::vector<int> childComponents;
	int hp;
	int maxHp;
	int textureRadius;

	int gridLocationX;
	int gridLocationY;


	//Engine and turret
	int chargeConsumption;
	int charge;

	//Turret
	bool hasFired = false;
	bool reloading = false;

	//Engine
	int rotationDirection; // negative = CCW (!)
	bool holdToThrust = true;
	bool autoThrusting = false;
};

#endif