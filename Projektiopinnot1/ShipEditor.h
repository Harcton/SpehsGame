#ifndef SHIPEDITOR_H
#define SHIPEDITOR_H

namespace editor
{
	enum Focus
	{
		base,
		component,
		actions,
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




	enum Key
	{
		//Keyboard
		Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9, Num0,
		Q, W, E, R, T, Y, U, I, O, P, A, S, D, F, G, H, J, K, L, Z, X, C, V, B, N, M, Comma, Period, Dash,
		Tab, CapsLock, LShift, LControl, Alt, Space, AltGr, RControl, RShift, Enter, Backspace, 
		up, down, left, right,
		np0, np1, np2, np3, np4, np5, np6, np7, np8, np9, npDot, npEnter, npPlus, npMinus, npFactor, npSlash,

		//Mouse
		mouseLeft, mouseRight, mouseWheelUp, mouseWheelDown, mouseMiddle, mouse1, mouse2,

		//Joystick1
		js1_X, js1_Y, js1_Z, js1_R, js1_U, js1_V, js1_PovX, js1_PovY, 
		js1_1, js1_2, js1_3, js1_4, js1_5, js1_6, js1_7, js1_8, js1_9, js1_10, 
		js1_11, js1_12, js1_13, js1_14, js1_15, js1_16, js1_17, js1_18, js1_19, js1_20,
		js1_21, js1_22, js1_23, js1_24, js1_25, js1_26, js1_27, js1_28, js1_29, js1_30, js1_31, js1_32,
		//Joystick2
		js2_X, js2_Y, js2_Z, js2_R, js2_U, js2_V, js2_PovX, js2_PovY,
		js2_1, js2_2, js2_3, js2_4, js2_5, js2_6, js2_7, js2_8, js2_9, js2_10,
		js2_11, js2_12, js2_13, js2_14, js2_15, js2_16, js2_17, js2_18, js2_19, js2_20,
		js2_21, js2_22, js2_23, js2_24, js2_25, js2_26, js2_27, js2_28, js2_29, js2_30, js2_31, js2_32,
		//Joystick3
		js3_X, js3_Y, js3_Z, js3_R, js3_U, js3_V, js3_PovX, js3_PovY,
		js3_1, js3_2, js3_3, js3_4, js3_5, js3_6, js3_7, js3_8, js3_9, js3_10,
		js3_11, js3_12, js3_13, js3_14, js3_15, js3_16, js3_17, js3_18, js3_19, js3_20,
		js3_21, js3_22, js3_23, js3_24, js3_25, js3_26, js3_27, js3_28, js3_29, js3_30, js3_31, js3_32,
		//Joystick4
		js4_X, js4_Y, js4_Z, js4_R, js4_U, js4_V, js4_PovX, js4_PovY,
		js4_1, js4_2, js4_3, js4_4, js4_5, js4_6, js4_7, js4_8, js4_9, js4_10,
		js4_11, js4_12, js4_13, js4_14, js4_15, js4_16, js4_17, js4_18, js4_19, js4_20,
		js4_21, js4_22, js4_23, js4_24, js4_25, js4_26, js4_27, js4_28, js4_29, js4_30, js4_31, js4_32,
		//Joystick5
		js5_X, js5_Y, js5_Z, js5_R, js5_U, js5_V, js5_PovX, js5_PovY,
		js5_1, js5_2, js5_3, js5_4, js5_5, js5_6, js5_7, js5_8, js5_9, js5_10,
		js5_11, js5_12, js5_13, js5_14, js5_15, js5_16, js5_17, js5_18, js5_19, js5_20,
		js5_21, js5_22, js5_23, js5_24, js5_25, js5_26, js5_27, js5_28, js5_29, js5_30, js5_31, js5_32,
		//Joystick6
		js6_X, js6_Y, js6_Z, js6_R, js6_U, js6_V, js6_PovX, js6_PovY,
		js6_1, js6_2, js6_3, js6_4, js6_5, js6_6, js6_7, js6_8, js6_9, js6_10,
		js6_11, js6_12, js6_13, js6_14, js6_15, js6_16, js6_17, js6_18, js6_19, js6_20,
		js6_21, js6_22, js6_23, js6_24, js6_25, js6_26, js6_27, js6_28, js6_29, js6_30, js6_31, js6_32,
		//Joystick7
		js7_X, js7_Y, js7_Z, js7_R, js7_U, js7_V, js7_PovX, js7_PovY,
		js7_1, js7_2, js7_3, js7_4, js7_5, js7_6, js7_7, js7_8, js7_9, js7_10,
		js7_11, js7_12, js7_13, js7_14, js7_15, js7_16, js7_17, js7_18, js7_19, js7_20,
		js7_21, js7_22, js7_23, js7_24, js7_25, js7_26, js7_27, js7_28, js7_29, js7_30, js7_31, js7_32,
		//Joystick8
		js8_X, js8_Y, js8_Z, js8_R, js8_U, js8_V, js8_PovX, js8_PovY,
		js8_1, js8_2, js8_3, js8_4, js8_5, js8_6, js8_7, js8_8, js8_9, js8_10,
		js8_11, js8_12, js8_13, js8_14, js8_15, js8_16, js8_17, js8_18, js8_19, js8_20,
		js8_21, js8_22, js8_23, js8_24, js8_25, js8_26, js8_27, js8_28, js8_29, js8_30, js8_31, js8_32,

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


	void drawSelectedRect();
	void drawSelectionShadeHighlight();
	void drawInheritanceSprites();
	void drawActions();

	void drawConfigurations();

	//Input
	MyKeys detectKey(ButtonId);
	std::string getInputAsString(MyKeys);
	void mouseLeftPressed();
	void mouseRightPressed();
	void zoom(int);

private:
	PlayerData& playerData;
	sf::RenderWindow& mWindow;
	float scaleFactor;
	float glowAngle = 0; //What it does is... spin around in the game loop. Can be used to apply glowing effects
	float glowAmount = 0; // 0.0 - 1.0
	sf::Font font1;

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

	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<sf::Sprite> inheritanceSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<Button> actionButtons;
	std::vector<Button> configurationButtons;
	std::vector<Button> coreConfigurationButtons;
	std::vector<Button> componentConfigurationButtons;

	editor::Focus focus = editor::base;
	int selectedX = -1;
	int selectedY = -1;
	sf::RectangleShape selectedRect;
	sf::RectangleShape shadeRect;
	sf::RectangleShape configurationRect;

	//Editor textures
	sf::Texture inheritanceArrowTex;
	sf::Texture xButtonTex;

};

#endif