#ifndef MAINMENU_H
#define MAINMENU_H
//Forward declared dependencies
//Included dependencies
#include <fstream>
#include <string>
#include <vector>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Color.hpp>
#include "SliderButton.h"//includes "Button.h"
#include "Sprite.h"


enum MainMenuFocus
{
	mmf_intro1,
	mmf_intro2,
	mmf_base,
	mmf_load,
	mmf_settings,
};

enum TextInputType
{
	tit_none,
	tit_saveName,
	tit_windowWidth,
	tit_windowHeight,
};


class MainMenu
{
public:
	~MainMenu();
	MainMenu(sf::RenderWindow& );

	int run();
	void pollEvents();
	void draw();

	void launchGame();
	void mouseLeftPressed();
	void mouseRightPressed();
	void scrollSaves(int);
	void dragScrollBar();
	void reloadSettingsButtonStrings();


	//Filestream
	bool tryCreateSaveFile();
	void createSaveFile();
	void reloadPlayerSaves();
	void deleteSave(std::string);


private:
	sf::RenderWindow& mWindow;
	sf::Event mEvent;
	sf::Vector2i mousePos;

	//Player saves
	std::fstream mFileStream;
	TextInputType receivingTextInput = tit_none;
	std::string textInput = "";
	std::vector<std::string> playerSavesList;

	//Misc
	sf::Font font1;
	int keepRunning = 1;
	sf::Music introMusic;
	MainMenuFocus focus = mmf_intro1;
	int doubleClickTimer = 0;

	//Background
	sf::Sprite planetSpr;
	sf::Texture menuLogoAnimatedTex;
	sge::Sprite menuLogoAnimated;
	int logoLayer = 0;
	int logoLayerTimer = 60;
	std::vector<sf::Sprite> bgSprVector;

	//Buttons
	sf::Color buttonColorBG;
	sf::Color buttonColorText;
	std::vector<Button> buttons;
	std::vector<Button> playerSaveButtons;
	std::vector<Button> loadSaveButtons;
	std::vector<Button> settingsButtons;
	std::vector<SliderButton> settingsSliderButtons;
	//Button* loadSaveButton;
	//Button* loadSaveBackground;

	//Scrolling
	int visibleSaveButtonCount;
	bool usingScrollBar = false;
	int scrollState = 0;
	int mouseGrabYdiff = 0;

	int button1Width;
	int button1Height;
	int buttonX1;
	int buttonY1;
	int buttonBorder;
	int settingsButtonY1;


	//Intro
	sf::Text intro1Text;
	short int introTextAlphaPolarity = 1;
	sf::RectangleShape introShade;
};

#endif