#ifndef TURRET_H
#define TURRET_H

class Object;
class Bullet;

class Turret : public Component
{
public:
	~Turret();
	Turret(Object*, Object*, double, double);
	friend class Player;
	
	void fire();
	void reload();
	bool update();
	void updateBullets();
	//void fixAngle();



private:
	std::vector<Bullet*>::iterator bulletIt;
	int canFireTimer;
	int magazine;




};

#endif