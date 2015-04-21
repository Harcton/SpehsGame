#ifndef SHIPEDITOR_H
#define SHIPEDITOR_H
#include <fstream>

namespace editor
{
	enum Focus
	{
		base,
		component,
		actions,
		rotate,
		configuration
	};

	enum Action
	{
		accelerate,
		reverse,
		turnRight,
		turnLeft,
		in,
		out,
		turretRight,
		turretLeft,
		fire,
		thrust
	};
}


class Button;

class ShipEditor
{
public:
	~ShipEditor();
	ShipEditor(sf::RenderWindow&, PlayerData&);

	void run();
	void updateLines();
	void updateGridSpriteTextures();
	void updateGridSpriteLocations();
	void updateMouseGrab();
	void scrapComponent(int, int);
	void rotateGlowAngle();
	void applyRotation();
	void updateTurretConfigurationButtonVisibility();
	void updateTurretConfigurationButtonStrings();
	void reloadTurretControlSchemeList();
	void updateTurretControlSchemeList();

	void drawWindow();
	void drawSelectedRect();
	void drawSelectionShadeHighlight();
	void drawInheritanceSprites();
	void drawActions();
	void drawCircleSlider();

	void drawConfigurations();

	char getUserInput(sf::Event&);

	//Control schemes
	void saveTurretControlScheme();
	void loadTurretControlScheme();
	void deleteTurretControlScheme();
	void writeTurretControlSchemes(std::vector<std::string>&);
	void extractNamesFromString(std::string&, std::vector<std::string>&);

	//Input
	MyKeys detectKey(ButtonId);
	std::pair<int, sf::Joystick::Axis> detectAxis();
	std::string getInputAsString(MyKeys);
	std::string getAxisAsString(sf::Joystick::Axis);
	std::string getPolarityAsString(float);
	void mouseLeftPressed();
	void mouseRightPressed();
	void zoom(int);

private:
	PlayerData& playerData;
	sf::RenderWindow& mWindow;
	sf::Event mEvent;
	float scaleFactor;
	float glowAngle = 0; //What it does is... spin around in the game loop. Can be used to apply glowing effects
	float glowAmount = 0; // 0.0 - 1.0
	sf::Font font1;

	//Temps
	int buttonHeight;
	int button1X1;
	int button2Width;
	int button1Width;
	int button2X1;
	int button3X1;
	std::pair<int, sf::Joystick::Axis> temp_idAxisPair;


	float cameraX = 0; //(EDITOR_WIDTH / 2) * 100;
	float cameraY = 0; //(EDITOR_HEIGHT / 2) * 100;

	sf::Vector2i mousePos;
	bool mouseGrab = false;
	float mouseGrabX = 0;
	float mouseGrabY = 0;
	float grabCameraOriginX = 0;
	float grabCameraOriginY = 0;
	int checkX;
	int checkY;
	int scrollDelta = 0;
	int scrollState = 0;
	std::string saveTurretControlSchemeInput;
	std::string workingFileName;
	std::fstream mFileStream;
	std::vector<std::string> turretControlSchemeNameList;
	bool gettingUserInput = false;
	int clickTimer = 0;

	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<sf::Sprite> inheritanceSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<Button> actionButtons;
	std::vector<Button> configurationButtons;
	std::vector<Button> coreConfigurationButtons;
	std::vector<Button> turretConfigurationButtons;
	std::vector<Button> turretControlSchemeList;
	std::vector<Button> engineConfigurationButtons;

	editor::Focus focus = editor::base;
	int selectedX = -1;
	int selectedY = -1;
	sf::RectangleShape selectedRect;
	sf::RectangleShape shadeRect;
	sf::RectangleShape configurationRect1;
	sf::RectangleShape configurationRect2;
	sf::RectangleShape configurationRect3;

	//Editor textures

	sf::Sprite circleSliderSpr;

};

#endif