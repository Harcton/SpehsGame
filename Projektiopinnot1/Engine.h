#ifndef ENGINE_H
#define ENGINE_H
#include "Component.h"

class Engine : public Component
{
public:
	~Engine();
	Engine(Object*, double, double);
	friend class Player;

	bool alive();
	void update();
	void thrust(float);

private:
	int charge;
	int rechargeTimer;
	//float MaxSpeed in Component.cpp



	//Temps memory
	float temp_distance;
	float temp_centerDirection;
	float temp_angleDifference;
};

#endif