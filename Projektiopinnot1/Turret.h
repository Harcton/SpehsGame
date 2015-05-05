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
	//void fixAngle();



private:
	int canFireTimer;
	int magazine;

	double turretMinAngle;
	double turretMaxAngle;
	double masterAngle;



};

#endif