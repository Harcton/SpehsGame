#ifndef GAME_H
#define GAME_H

class Object;
class Background;
class Enemy;

class Debris;


class Game
{
public:
	Game(sf::RenderWindow&);
	~Game();

	void run();
	Object* playerObj;
	std::vector<Object*> objects;
	void updateBackgrounds();
	void updateObjects();
	sf::Event mEvent;

protected:
	sf::RenderWindow& mWindow;
	std::vector<Background*> backgrounds;

	//Iterators
	std::vector<Object*>::iterator obIt;
	std::vector<Background*>::iterator bgIt;
};

#endif