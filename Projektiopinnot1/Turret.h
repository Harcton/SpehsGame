#ifndef TURRET_H
#define TURRET_H

class Object;
class Bullet;
class Player;

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
	void updateBullets();
	//void fixAngle();



private:
	std::vector<Bullet*>::iterator bulletIt;
	int canFireTimer;
	int magazine;

	double turretMinAngle;
	double turretMaxAngle;
	double masterAngle;



};

#endif