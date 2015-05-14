#ifndef GAME_H
#define GAME_H
//Forward declared dependencies
class Object;
class Background;
//Included dependencies
#include <vector>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Window/Window.hpp>
#include <SFML/Window/Event.hpp>
#include "SliderButton.h"
#include "VisualEffect.h"



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
	void mouseRightPressed();
	void drawEscMenu();
	void reloadEscMenuButtonStrings();
	void updateBullets();
	void updateStation();
	void updateElements();

	Object* playerObj;
	std::vector<Object*> objects;
	std::vector<Object*> bullets;
	std::vector<Object*>::iterator bulletIt;
	std::vector<VisualEffect> backVisualEffects;
	std::vector<VisualEffect> frontVisualEffects;
	void drawVisualEffects(std::vector<VisualEffect>&);
	void updateBackgrounds();
	void updateObjects();
	void demo();
	std::string intToString(int);
	sf::Event mEvent;

	sf::RenderWindow& mWindow;
	bool keepRunning = true;

	int nearestStationX = 0;
	int nearestStationY = 0;

protected:
	GameFocus focus = gf_game;
	sf::Vector2i mousePos;
	std::vector<Background*> backgrounds;
	sf::Text balanceText;


	//Space station
	sf::Sprite stationSpr;
	sf::Sprite stationArrow;
	bool ableToDock = false;
	sf::Sprite pressEnterToDockSpr;


	//GUI elements
	std::vector<sf::Sprite> elements;
	//Esc menu
	sf::RectangleShape escMenuShade;
	std::vector<Button> escMenuButtons;
	std::vector<SliderButton> escMenuSliders;

	//Iterators
	std::vector<Object*>::iterator obIt;
	std::vector<Background*>::iterator bgIt;

	//FOR TEST GAME
	int distanceFromStart;
	int distanceFromStation;
	int spawnRandomization;
	int enemyBehaviourDifficulty;

	sf::Text distanceText;

	sf::Music spehsMusic;
};

#endif