#ifndef COMPONENT_H
#define COMPONENT_H
#include "Sprite.h"

namespace component
{


	enum Type
	{
		hull,
		turret,
		engine,
	};

}


class Object;
class Bullet;
static int currentId = 0;


class Component
{
public:
	~Component();
	Component(Object*, Object*, double, double);
	Component(Object*, Object*, double, double, int, int);
	friend class Player;
	friend class Enemy;
	friend class Seeker;
	friend class Sentinel;
	friend class Sentry;
	friend class Flier;
	friend class Commander;

	virtual void update();
	virtual bool alive();
	void draw();
	void createChild(double, double, component::Type);

	//Virtual functions
	virtual void fire(){}
	virtual void reload(){};
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
	std::vector<Bullet*> bullets;
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
	float thrust; //thrust strength

protected:
	std::vector<sf::Sprite> sprites;
	std::vector<sge::Sprite> animatedSprites;

	bool mouseAim;
	bool mouseAimRelativeToCenter;
	int id;
	std::vector<int> childComponents;
	int hp;
	int maxHp;
	int textureRadius;

	int gridLocationX;
	int gridLocationY;

	bool hasFired = false;
	bool reloading = false;


	//DEBUG
	bool drawCollisionCircle;
	sf::CircleShape collisionCircle;
};

#endif