#ifndef PLAYER_H
#define PLAYER_H



class Player : public Object
{
public:
	~Player();
	Player(sf::RenderWindow&, Game*, bool, int, int);

	void update();

	//Movement
	void turnRight();
	void turnLeft();
	void accelerate();
	void reverse();


protected:
	sf::Event mEvent;


};

#endif