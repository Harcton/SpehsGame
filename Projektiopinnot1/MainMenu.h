#ifndef MAINMENU_H
#define MAINMENU_H
#include <fstream>

class Button;

enum MainMenuFocus
{
	mmf_base,
	mmf_load,
};


class MainMenu
{
public:
	~MainMenu();
	MainMenu(sf::RenderWindow& );

	void run();
	void pollEvents();
	void draw();

	void mouseLeftPressed();
	void mouseRightPressed();
	void scrollSaves(int);
	void dragScrollBar();

	//Actions
	void launchGame();
	void launchSettings();


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
	bool receivingTextInput = false;
	std::string textInput = "";
	std::vector<std::string> playerSavesList;

	//Misc
	sf::Font font1;
	bool keepRunning = true;
	sf::Music introMusic;
	MainMenuFocus focus = mmf_base;

	//Background
	sf::Sprite planetSpr;
	sf::Sprite menuLogo;
	std::vector<sf::Sprite> bgSprVector;

	//Buttons
	sf::Color buttonColorBG;
	sf::Color buttonColorText;
	std::vector<Button> buttons;
	std::vector<Button> playerSaveButtons;
	std::vector<Button> loadSaveButtons;
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

};

#endif