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
	void updateDebris();
	void drawGui();

	Object* playerObj;
	std::vector<Object*> objects;
	std::vector<Object*> debriss;
	std::vector<Object*> bullets;
	std::vector<Object*>::iterator bulletIt;
	std::vector<VisualEffect> backVisualEffects;
	std::vector<VisualEffect> frontVisualEffects;
	void drawVisualEffects(std::vector<VisualEffect>&);
	void updateBackgrounds();
	void updateObjects();
	void demo1();
	void demo2();
	std::string intToString(int);
	sf::Event mEvent;

	sf::RenderWindow& mWindow;
	bool keepRunning = true;


	//Locations
	Location spawnLocation;
	Location nearestStationLocation;
	
protected:
	GameFocus focus = gf_game;
	sf::Vector2i mousePos;
	std::vector<Background*> backgrounds;
	int spawnRandomization;
	int enemyBehaviourDifficulty;
	int spawnZone;
	unsigned int debrisAmount;
	sf::Music spehsMusic;


	//Space station
	sf::Sprite stationSpr;
	sf::Sprite stationSpr2;
	sf::Sprite stationArrow;
	bool ableToDock = false;
	sf::Sprite pressEnterToDockSpr;




	//Esc menu
	sf::RectangleShape escMenuShade;
	std::vector<Button> escMenuButtons;
	std::vector<SliderButton> escMenuSliders;

	//Iterators
	std::vector<Object*>::iterator obIt;
	std::vector<Background*>::iterator bgIt;

	//GUI
	//Compass coordinates used for building the gui
	float guiScale;
	int compassX;
	int compassY;
	int compassRadius;
	sf::Sprite compassSpr;
	sf::Sprite displayMeterSpr;
	sf::Sprite compassTabSpr;
	sf::Sprite spawnArrowSpr;
	int distanceFromStart;
	int distanceFromStation;
	sf::Text displayLabelText;
	sf::Text displaySpawnText;
	sf::Text displayStationText;
	sf::Text displayMetalText;
	int displayTextX;
	int displayTextY1;
	int displayTextYDifference;


};

#endif