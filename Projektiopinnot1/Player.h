#ifndef PLAYER_H
#define PLAYER_H
//Forward declared dependencies
class Game;
class Component;
//Included dependencies
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include "Object.h"
#include "PlayerData.h"
#include "Sprite.h"



class Player : public Object
{
public:
	~Player();
	Player(sf::RenderWindow&, Game*, int, int);
	friend class Engine;

	void loadPlayerData();
	void applyPlayerData();

	bool update();
	void notifyComponentDestruction(Component*);
	void addFromGrid(int, int);
	void editShip();
	void loadKeybindings();
	void calculateCenterOfMass();
	void reloadSkeletonSprites();
	void setTurretLooks(int, int);
	void setTurretStats(int, int);
	void setEngineLooks(int, int);
	void setEngineStats(int, int);

	//Handle input
	void turnRight(double);
	void turnLeft(double);
	void accelerate(double);
	void reverse(double);
	void zoomIn(double);
	void zoomOut(double);

	//Virtual methods
	int getMass();


protected:
	sf::Event mEvent;
	float joystickDirection = 0;
	PlayerData data;//Only for player objects


	bool controller;
	double mouseDirection;
	sf::Vector2i mousePosition;

	int coreX = (1 - EDITOR_WIDTH) / 2;
	int coreY = (1 - EDITOR_HEIGHT) / 2;

	int shipMass = 0;
	int shipWidth = 0;
	int shipHeight = 0;

	float preferredTurnSpeed;
	//Temporal variables to be used in frequent loops
	float temp_angleVar;
	float temp_accelerationPower;
	
	double xAcceleration;
	double yAcceleration;
	double xSpeed1;
	double ySpeed1;
	double xAccVariable;
	double yAccVariable;
};

#endif

