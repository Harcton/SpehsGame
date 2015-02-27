#ifndef COMPONENT_H
#define COMPONENT_H



class Object;
class Bullet;

enum componentType
{
	hull,
	turret,
};


class Component
{
public:
	~Component();
	Component(Object*, Object*, double, double);
	friend class Player;

	virtual bool update();
	void draw();

	//Virtual functions
	virtual void fire(){}
	//virtual void fixAngle(){}


	Object* master;
	Object* centerObj;

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
	sf::Texture tex;
	sf::Sprite spr;

	bool mouseAim;
	
};

#endif