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
		configuration,
		esc,
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
enum ButtonId;

class ShipEditor
{
public:
	~ShipEditor();
	ShipEditor(sf::RenderWindow&, PlayerData&);

	int run();
	void savePlayerData();
	void runSettings();

	void updateLines();
	void updateGridSpriteTextures();
	void updateGridSpriteLocations();
	void updateMouseGrab();
	void scrapComponent(int, int);
	void rotateGlowAngle();
	void applyRotation();
	void updateCoreConfigurationButtonVisibility();
	void updateTurretConfigurationButtonVisibility();
	void updateTurretConfigurationButtonStrings();
	void updateCoreConfigurationButtonStrings();
	void updateEngineConfigurationButtonStrings();
	void drawWindow();
	void drawSelectedRect();
	void drawSelectionShadeHighlight();
	void drawInheritanceSprites();
	void drawActions();
	void drawEscButtons();
	void reloadActions();
	void closeActions(editor::Focus);
	void drawCircleSlider();

	void drawConfigurations();

	char getUserInput(sf::Event&);
	void adjustColor();
	void updateTurretColorPreview();

	void closeCoreConfigurations();
	void closeTurretConfigurations();
	void closeEngineConfigurations();

	//Control schemes
	void extractNamesFromString(std::string&, std::vector<std::string>&);
	void setSelectionJoystickIndex(int);
	void deleteControlScheme(std::string, std::string);
	void writeControlSchemes(std::string, std::vector<std::string>&);
	//Turret
	void saveTurretControlScheme();
	void loadTurretControlScheme();
	void reloadTurretControlSchemeList();
	void updateTurretControlSchemeList();
	//Core
	void saveCoreControlScheme();
	void loadCoreControlScheme();
	void reloadCoreControlSchemeList();
	void updateCoreControlSchemeList();


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
	//References outside class
	PlayerData& playerData;
	sf::RenderWindow& mWindow;

	//Misc
	int keepRunning = 1;
	float scaleFactor;
	float glowAngle = 0; //What it does is... spin around in the game loop. Can be used to apply glowing effects
	float glowAmount = 0; // 0.0 - 1.0
	sf::Event mEvent;
	sf::Font font1;
	std::pair<int, sf::Joystick::Axis> temp_idAxisPair;
	float cameraX = 0; //(EDITOR_WIDTH / 2) * 100;
	float cameraY = 0; //(EDITOR_HEIGHT / 2) * 100;
	std::string saveControlSchemeInput;
	std::string workingFileName;
	std::fstream mFileStream;
	std::vector<std::string> turretControlSchemeNameList;
	std::vector<std::string> coreControlSchemeNameList;


	//Button lengths
	//Common
	int actionButtonWidth;
	int actionButtonHeight;
	int buttonHeight;
	int button1Width;
	int button2Width;
	int buttonBorder;
	//Turret conf
	int turretConfX1;
	int turretConfY1;
	int button1X1;
	int button2X1;
	int button3X1;
	int turretConfScrollBarY1;
	//Core conf
	int coreConfX1;
	int coreConfY1;
	int coreConfSchemeY;
	int coreConfScrollBarY1;
	//Engine conf
	int engineConfX1;
	int engineConfY1;

	//Misc states
	bool gettingUserInput = false;
	int clickTimer = 0;
	int adjustingColor = 0;
	bool mouseGrab = false;
	float mouseGrabX = 0;
	float mouseGrabY = 0;
	float grabCameraOriginX = 0;
	float grabCameraOriginY = 0;
	editor::Focus focus = editor::base;
	int selectedX = -1;
	int selectedY = -1;
	int checkX;
	int checkY;
	bool usingScrollBar = false;
	int scrollDelta = 0;
	int scrollState = 0;
	sf::Vector2i mousePos;

	//Editor sprites/shapes
	sf::RectangleShape selectedRect;
	sf::RectangleShape shadeRect;
	sf::RectangleShape coreConfigurationRect1;
	sf::RectangleShape coreConfigurationRect2;
	sf::RectangleShape coreConfigurationRect3;
	sf::RectangleShape turretConfigurationRect1;
	sf::RectangleShape turretConfigurationRect2;
	sf::RectangleShape turretConfigurationRect3;
	sf::RectangleShape engineConfigurationRect1;
	sf::RectangleShape engineConfigurationRect2;
	sf::RectangleShape engineConfigurationRect3;
	sf::Sprite circleSliderSpr;
	sf::Sprite colorPreviewTurret;
	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<sf::Sprite> inheritanceSprites[EDITOR_WIDTH][EDITOR_HEIGHT];

	//Buttons
	std::vector<Button> escButtons;
	std::vector<Button> actionButtons;
	std::vector<Button> actionTurretSchemeButtons;
	std::vector<Button> actionCoreSchemeButtons;
	std::vector<Button> actionJoystickIndexButtons;
	std::vector<Button> configurationButtons;
	std::vector<Button> coreConfigurationButtons;
	std::vector<Button> turretConfigurationButtons;
	std::vector<Button> turretControlSchemeList;
	std::vector<Button> coreControlSchemeList;
	std::vector<Button> engineConfigurationButtons;
	bool actionTurretSchemeSelectionOpen = false;
	bool actionCoreSchemeSelectionOpen = false;
	bool actionJoystickIndexSelectionOpen = false;



};

#endif