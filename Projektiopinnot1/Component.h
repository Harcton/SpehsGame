#ifndef COMPONENT_H
#define COMPONENT_H
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

	virtual bool update();
	void draw();
	void createChild(double, double, component::Type);

	//Virtual functions
	virtual void fire(){}
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
	double turningSpeed;
	double maxAngle;

protected:
	sf::Sprite spr;
	sf::Texture tex;

	bool mouseAim;
	bool mouseAimRelativeToCenter;
	int id;
	std::vector<int> childComponents;
	int hp;
	int textureRadius;

	int gridLocationX;
	int gridLocationY;
};

#endif