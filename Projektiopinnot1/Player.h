#ifndef PLAYER_H
#define PLAYER_H


class Component;
class PlayerData;

class Player : public Object
{
public:
	~Player();
	Player(sf::RenderWindow&, Game*, int, int);

	void loadPlayerData();

	bool update();
	void updateComponents();
	void checkBulletCollision(Bullet*);
	void removeComponent(int);
	void notifyComponentDestruction(int);
	void addFromGrid(int, int);
	void editShip();
	void loadKeybindings();
	void calculateCenterOfMass();

	//Handle input
	void turnRight(double);
	void turnLeft(double);
	void accelerate(double);
	void reverse(double);
	void zoomIn(double);
	void zoomOut(double);

protected:
	sf::Event mEvent;
	std::vector<Component*>::iterator componentIt;
	float joystickDirection = 0;

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
};

#endif

