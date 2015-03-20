#ifndef COMPONENT_H
#define COMPONENT_H

enum componentType
{
	ct_hull,
	ct_turret
};


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
	void createChild(double, double, componentType);

	//Virtual functions
	virtual void fire(){}
	//virtual void fixAngle(){}


	Object* master;
	Object* centerObj;
	int masterComponent;

	//offset compared to master origin
	double xOffset;
	double yOffset;
	
	// actual coordinates in space 
	double x;
	double y;

	double screenX;
	double screenY;

	std::vector<componentType> types;
	std::vector<Bullet*> bullets;
	double angle;
	double turningSpeed;
	double maxAngle;

protected:
	sf::Sprite spr;
	sf::Texture tex;

	bool mouseAim;
	int id;
	std::vector<int> childComponents;
	int hp;
	int textureRadius;

	int gridLocationX;
	int gridLocationY;
};

#endif