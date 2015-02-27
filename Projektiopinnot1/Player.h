#ifndef PLAYER_H
#define PLAYER_H


class Component;

class Player : public Object
{
public:
	~Player();
	Player(sf::RenderWindow&, Game*, int, int);

	bool update();
	void updateComponents();

	//Handle input
	void turnRight(double);
	void turnLeft(double);
	void accelerate(double);
	void reverse(double);
	void zoomIn(double);
	void zoomOut(double);

protected:
	sf::Event mEvent;
	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;

	bool controller;
	double mouseDirection;
	sf::Vector2i mousePosition;

};

#endif