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
	void addFromGrid(int,int);

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

	bool controller;
	double mouseDirection;
	sf::Vector2i mousePosition;
	PlayerData* data;

};

#endif