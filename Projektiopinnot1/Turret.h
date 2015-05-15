#ifndef TURRET_H
#define TURRET_H
//Forward declared dependencies
class Object;
//Included dependencies
#include "Component.h"



class Turret : public Component
{
public:
	~Turret();
	Turret(Object*, Object*, double, double);
	friend class Player;
	
	void fire();
	void reload();
	void update();
	bool alive();
	void draw();
	//void fixAngle();

	void drawLaserPointer();
	//Virtual functions
	void setLaserPointerColor(sf::Color&& c);
	void setBulletTexPtr(sf::Texture*);
	sf::Sprite& getMagazineSpr();


private:
	int canFireTimer;
	sf::VertexArray laserPointer;
	sf::Sprite magazineSpr;
	sf::Texture* bulletTexPtr;

	double turretMinAngle;
	double turretMaxAngle;
	double masterAngle;



};

#endif