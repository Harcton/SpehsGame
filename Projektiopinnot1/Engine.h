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
	bool thrustButtonReleased = true;

};

#endif