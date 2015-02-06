#ifndef COMPONENT_H
#define COMPONENT_H



class Object;


class Component
{
public:
	~Component();
	Component(Object*, Object*, double, double);

	virtual bool update();
	void draw();

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



protected:
	sf::Texture tex;
	sf::Sprite spr;
	
	double angle;
};

#endif