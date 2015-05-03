#ifndef GAME_H
#define GAME_H
#include "Button.h"

class Object;
class Background;
class Enemy;
class Player;
class Debris;

enum GameFocus
{
	gf_game,
	gf_escMenu,
};


class Game
{
public:
	Game(sf::RenderWindow&);
	~Game();

	void run();
	void pollEvents();
	void mouseLeftPressed();
	void drawEscMenu();

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
	GameFocus focus = gf_game;
	sf::Vector2i mousePos;
	std::vector<Background*> backgrounds;



	//GUI elements
	std::vector<sf::Sprite> elements;
	//Esc menu
	sf::RectangleShape escMenuShade;
	std::vector<Button> escMenuButtons;

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