#ifndef GAME_H
#define GAME_H

class Object;
class Background;
class Enemy;
class Player;

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
	void demo();
	std::string intToString(int);
	sf::Event mEvent;

	sf::RenderWindow& mWindow;
	bool keepRunning = true;
protected:
	std::vector<Background*> backgrounds;

	std::vector<sf::Sprite> elements;

	//Iterators
	std::vector<Object*>::iterator obIt;
	std::vector<Background*>::iterator bgIt;

	//FOR TEST GAME
	int distanceFromStart;
	int spawnRandomization;
	sf::Font font;
	sf::Text distanceText;
};

#endif