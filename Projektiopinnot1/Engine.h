#ifndef ENGINE_H
#define ENGINE_H
//Forward declared dependencies
class Object;
//Included dependencies
#include "Component.h"
#include <SFML/Graphics/RectangleShape.hpp>


class Engine : public Component
{
public:
	~Engine();
	Engine(Object*, double, double);
	friend class Player;

	bool alive();
	void update();
	void thrust(float);
	void draw();
	void drawEngineFlame();

	sf::RectangleShape& getChargeRectangle();
	sge::Sprite& getEngineFlameSpr();

private:
	bool thrustButtonReleased = true;
	bool thrusting = false;
	sf::RectangleShape chargeBar;
	sge::Sprite engineFlameSpr;

};

#endif