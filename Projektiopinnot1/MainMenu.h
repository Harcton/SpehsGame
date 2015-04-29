#ifndef MAINMENU_H
#define MAINMENU_H


class Button;

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

	//Actions
	void launchGame();
	void launchSettings();

private:
	sf::RenderWindow& mWindow;
	sf::Event mEvent;
	sf::Vector2i mousePos;

	//Misc
	sf::Font font1;
	bool keepRunning = true;
	sf::Music introMusic;

	//Background
	sf::Sprite planetSpr;
	sf::Sprite menuLogo;
	std::vector<sf::Sprite> bgSprVector;

	//Buttons
	sf::Color buttonColorBG;
	sf::Color buttonColorText;
	std::vector<Button> buttons;
	int button1Width;
	int button1Height;
	int buttonX1;
	int buttonY1;
	int buttonBorder;

};

#endif