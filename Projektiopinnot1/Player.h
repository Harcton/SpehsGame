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

	//Movement
	void turnRight();
	void turnLeft();
	void accelerate();
	void reverse();


protected:
	sf::Event mEvent;
	std::vector<Component*> components;
	std::vector<Component*>::iterator componentIt;

	double mouseDirection;
	sf::Vector2i mousePosition;

};

#endif