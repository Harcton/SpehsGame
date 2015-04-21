#include "Main.h"
#include "Button.h"
#include "GridData.h"
#include "PlayerData.h"
#include "ShipEditor.h"
#include <stdio.h>


#define CONF_X1 int(100 * resFactor)
#define CONF_Y1 int(75 * resFactor)
#define CONF_WIDTH int(WINDOW_WIDTH - 200 * resFactor)
#define CONF_HEIGHT int(WINDOW_HEIGHT - 150 * resFactor)
#define CONF_X2 int(CONF_X1 + CONF_WIDTH)
#define CONF_Y2 int(CONF_Y1 + CONF_HEIGHT)
#define CONF_BUTTON_HEIGHT int(50 * resFactor)



ShipEditor::~ShipEditor()
{
}
ShipEditor::ShipEditor(sf::RenderWindow& mw, PlayerData& pd) : playerData(pd), mWindow(mw)
{
	//Misc graphical stuff
	scaleFactor = resFactor*zoomFactor;
	shadeRect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	shadeRect.setFillColor(sf::Color(0, 0, 0, 100));
	configurationRect1.setSize(sf::Vector2f(CONF_WIDTH + 10*resFactor, CONF_HEIGHT + 10*resFactor));
	configurationRect1.setPosition(CONF_X1 - 5*resFactor, CONF_Y1 - 5*resFactor);
	configurationRect1.setFillColor(sf::Color(140, 140, 145));
	configurationRect2.setSize(sf::Vector2f(CONF_WIDTH, CONF_HEIGHT));
	configurationRect2.setPosition(CONF_X1, CONF_Y1);
	configurationRect2.setFillColor(sf::Color(100, 100, 105));
	configurationRect3.setSize(sf::Vector2f(CONF_WIDTH - 20*resFactor, CONF_HEIGHT - 70*resFactor));
	configurationRect3.setPosition(CONF_X1 + 10*resFactor, CONF_Y1 + 60*resFactor);
	configurationRect3.setFillColor(sf::Color(110, 110, 115));

	//Focus camera on core
	for (int sx = 0; sx < EDITOR_WIDTH; sx++)
	for (int sy = 0; sy < EDITOR_HEIGHT; sy++)
	if (playerData.grid[sx][sy]->core == true)
	{
		int temp_screenWidth = 0.5*(WINDOW_WIDTH/scaleFactor);
		cameraX = 100 * sx - temp_screenWidth + 50;
		int temp_screenHeight = 0.5*(WINDOW_HEIGHT/scaleFactor);
		cameraY = 100 * sy - temp_screenHeight + 50;
	}
	
	//Load editor textures
	circleSliderSpr.setTexture(RM.getTexture("circleSlider.png"));
	circleSliderSpr.setOrigin(100, 100);

	//Action buttons
	font1.loadFromFile("Font/ORANGEKI.ttf");
	actionButtons.push_back(Button(bi_false, 0, 0, 250 * resFactor, 40 * resFactor, "Component name", int(26 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	actionButtons.push_back(Button(bi_actionUpgradeArmor, 0, 0, 250 * resFactor, 40 * resFactor, "Upgrade armor", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionTurret, 0, 0, 250 * resFactor, 40 * resFactor, "Add turret", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionEngine, 0, 0, 250 * resFactor, 40 * resFactor, "Add engine", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionScrap, 0, 0, 250 * resFactor, 40 * resFactor, "Delete", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionRotate, 0, 0, 250 * resFactor, 40 * resFactor, "Rotate", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionConfiguration, 0, 0, 250 * resFactor, 40 * resFactor, "Configure...", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));

	//Configuration buttons
	configurationButtons.push_back(Button(bi_confExit, WINDOW_WIDTH/resFactor - 150, 75, RM.getTexture("xButton.png"), 2, font1));
	buttonHeight = CONF_BUTTON_HEIGHT;
	button1X1 = CONF_X1 + buttonHeight;
	button2Width = 250;
	button1Width = 300;//CONF_WIDTH - button2Width - (button1X1 - CONF_X1);
	button2X1 = button1X1 + button1Width;//CONF_X2 - button2Width - 500*resFactor;
	button3X1 = button1X1 + button1Width + button2Width + buttonHeight + 20*resFactor;

	//Core configuration buttons
	//Main header
	coreConfigurationButtons.push_back(Button(bi_false, CONF_X1, CONF_Y1, CONF_WIDTH - 50 * resFactor, buttonHeight, " Ship core configurations", int(34 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	//Ship controlls background
	coreConfigurationButtons.push_back(Button(bi_false, button1X1 - 10 * resFactor, CONF_Y1 + buttonHeight * 2 - 10 * resFactor, button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight*7 + 20 * resFactor, " ", int(33 * resFactor), font1, sf::Color(80, 80, 90), sf::Color(35, 35, 40)));
	//Ship controls header
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 2, button2X1 + button2Width - button1X1, buttonHeight, " Ship controls", int(33 * resFactor), font1, sf::Color(130, 130, 135), sf::Color(35, 35, 40)));
	//Directional movement & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 3, button1Width, buttonHeight, " Use analog stick movement", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confNodeDirectionalMovement, button2X1, CONF_Y1 + buttonHeight * 3, button2Width, buttonHeight, " " + getBoolAsString(directionalMovement), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	
	//VerticalAxis
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 4, button1Width, buttonHeight, " Vertical Axis", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindVerticalMoveAxis, button2X1, CONF_Y1 + buttonHeight * 4, button2Width, buttonHeight, " Joystick " + std::to_string(moveJoystickId) + "::" + getAxisAsString(verticalMoveAxis) + " (" + getPolarityAsString(verticalMoveAxisPolarity) + ")", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	//HorizontalAxis
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 5, button1Width, buttonHeight, " Horizontal Axis", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindHorizontalMoveAxis, button2X1, CONF_Y1 + buttonHeight * 5, button2Width, buttonHeight, " Joystick " + std::to_string(moveJoystickId) + "::" + getAxisAsString(horizontalMoveAxis) + " (" + getPolarityAsString(horizontalMoveAxisPolarity) + ")", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));

	//Turn right & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 4, button1Width, buttonHeight, " Turn right", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindTurnRight, button2X1, CONF_Y1 + buttonHeight * 4, button2Width, buttonHeight, getInputAsString(coreKeys[key_turnRight]), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	//Turn left & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 5, button1Width, buttonHeight, " Turn left", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindTurnLeft, button2X1, CONF_Y1 + buttonHeight * 5, button2Width, buttonHeight, getInputAsString(coreKeys[key_turnLeft]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	//Acceleration & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 6, button1Width, buttonHeight, " Accelerate", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindAccelerate, button2X1, CONF_Y1 + buttonHeight * 6, button2Width, buttonHeight, getInputAsString(coreKeys[key_accelerate]), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));

	//Reverse & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 7, button1Width, buttonHeight, " Reverse", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindReverse, button2X1, CONF_Y1 + buttonHeight * 7, button2Width, buttonHeight, getInputAsString(coreKeys[key_reverse]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	//Zoom in & bindong key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 8, button1Width, buttonHeight, " Zoom in", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindZoomIn, button2X1, CONF_Y1 + buttonHeight * 8, button2Width, buttonHeight, getInputAsString(coreKeys[key_zoomIn]), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	//Zoom out & binding key
	coreConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 9, button1Width, buttonHeight, " Zoom out", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindZoomOut, button2X1, CONF_Y1 + buttonHeight * 9, button2Width, buttonHeight, getInputAsString(coreKeys[key_zoomOut]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	
	//Set directional Movement button visibilities (core)
		bool temp_axisState = directionalMovement;
		bool temp_manualButtonsState = true;
		if (temp_axisState)
			temp_manualButtonsState = false;
		for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
			switch (coreConfigurationButtons[i].id)
		{
			case bi_confBindVerticalMoveAxis:
			case bi_confBindHorizontalMoveAxis:
				coreConfigurationButtons[i - 1].visible = temp_axisState; //Label
				coreConfigurationButtons[i].visible = temp_axisState;
				break;
			case bi_confBindAccelerate:
			case bi_confBindTurnLeft:
			case bi_confBindTurnRight:
				coreConfigurationButtons[i - 1].visible = temp_manualButtonsState; //Label
				coreConfigurationButtons[i].visible = temp_manualButtonsState;
				break;
		}

		//Update button locations if dirMov is enabled
		if (directionalMovement == true)
			for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
				if (coreConfigurationButtons[i].id == bi_confBindHorizontalMoveAxis)
					for (int t = 0; t < 6; t++)
						coreConfigurationButtons[i + t + 7].setPosition(coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().x, coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().y - CONF_BUTTON_HEIGHT);

		//Update useDirMove bg color if needed
		if (directionalMovement == false)
		for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
			if (coreConfigurationButtons[i].id == bi_confNodeDirectionalMovement)
			{
			coreConfigurationButtons[i - 1].red = 110;
			coreConfigurationButtons[i - 1].green = 110;
			coreConfigurationButtons[i - 1].blue = 115;
			coreConfigurationButtons[i].red = 110;
			coreConfigurationButtons[i].green = 110;
			coreConfigurationButtons[i].blue = 115;
			}

		//Update core conf. background size
		if (directionalMovement)
			coreConfigurationButtons[1].buttonRectangle.setSize(sf::Vector2f(button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight * 7 + 20 * resFactor));
		else
			coreConfigurationButtons[1].buttonRectangle.setSize(sf::Vector2f(button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight * 8 + 20 * resFactor));




	//Turret configuration buttons
	//Main header
	turretConfigurationButtons.push_back(Button(bi_false, CONF_X1, CONF_Y1, CONF_WIDTH - 50 * resFactor, buttonHeight, " Turret[" + std::to_string(selectedX) + "," + std::to_string(selectedY) + "] configurations", int(34* resFactor), font1, sf::Color(120,120,125), sf::Color(35,35,40)));
	//Ship controls background
	turretConfigurationButtons.push_back(Button(bi_false, button1X1 - 10 * resFactor, CONF_Y1 + buttonHeight * 2 - 10 * resFactor, button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight * 11 + 20 * resFactor, " ", int(33 * resFactor), font1, sf::Color(80, 80, 90), sf::Color(35, 35, 40)));
	//Turret controls header
	turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 2, button2X1 + button2Width - button1X1, buttonHeight, " Turret controls", int(33 * resFactor), font1, sf::Color(130, 130, 135), sf::Color(35, 35, 40)));
	
	
	//Fire
	turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 3, button1Width, buttonHeight, " Fire", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	turretConfigurationButtons.push_back(Button(bi_confBindFire, button2X1, CONF_Y1 + buttonHeight * 3, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_fire]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	//Enable hold to fire
	turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 4, button1Width, buttonHeight, " Enable hold to fire", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	turretConfigurationButtons.push_back(Button(bi_confNodeHoldToFire, button2X1, CONF_Y1 + buttonHeight * 4, button2Width, buttonHeight, " N/A", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	//Reload
	turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 5, button1Width, buttonHeight, " Reload", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	turretConfigurationButtons.push_back(Button(bi_confBindReload, button2X1, CONF_Y1 + buttonHeight * 5, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_reload]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	//Mouse aim (1)
	turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 6, button1Width, buttonHeight, " Enable mouse aim", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
	turretConfigurationButtons.push_back(Button(bi_confNodeMouseAim, button2X1, CONF_Y1 + buttonHeight * 6, button2Width, buttonHeight, " N/A", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
		//Mouse aim relative to center & node
		turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 7, button1Width, buttonHeight, " Enable mouse aim relative to center", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confNodeMouseAimRelative, button2X1, CONF_Y1 + buttonHeight * 7, button2Width, buttonHeight, " N/A", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
	
		//Directional aim & node (2)
		turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 8, button1Width, buttonHeight, " Enable analog stick aim", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confNodeDirectionalAim, button2X1, CONF_Y1 + buttonHeight * 8, button2Width, buttonHeight, " N/A", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
			//Vertical axis
			turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 9, button1Width, buttonHeight, " Vertical axis", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
			turretConfigurationButtons.push_back(Button(bi_confBindVerticalAimAxis, button2X1, CONF_Y1 + buttonHeight * 9, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_vertical]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
			//Horizontal axis
			turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 10, button1Width, buttonHeight, " Horizontal axis", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
			turretConfigurationButtons.push_back(Button(bi_confBindHorizontalAimAxis, button2X1, CONF_Y1 + buttonHeight * 10, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_horizontal]), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
		//Manual turret movement (3)
		//Rotate right & binding key
		turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 11, button1Width, buttonHeight, " Rotate right", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confBindTurnTurretRight, button2X1, CONF_Y1 + buttonHeight * 11, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_right]), int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
		//Rotate left & binding key
		turretConfigurationButtons.push_back(Button(bi_false, button1X1, CONF_Y1 + buttonHeight * 12, button1Width, buttonHeight, " Rotate left", int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confBindTurnTurretLeft, button2X1, CONF_Y1 + buttonHeight * 12, button2Width, buttonHeight, getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_left]), int(33 * resFactor), font1, sf::Color(105, 105, 110), sf::Color(35, 35, 40)));


		//Control schemes background
		turretConfigurationButtons.push_back(Button(bi_false, button3X1 - 10 * resFactor, CONF_Y1 + buttonHeight * 2 - 10 * resFactor, button1Width + 20*resFactor, buttonHeight * 11 + 20 * resFactor, " ", int(33 * resFactor), font1, sf::Color(80, 80, 90), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_false, button3X1, CONF_Y1 + buttonHeight * 2, button1Width, buttonHeight * 11, " ", int(33 * resFactor), font1, sf::Color(100, 100, 105), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_false, button3X1, CONF_Y1 + buttonHeight * 2, buttonHeight*0.5, buttonHeight * 11, " ", int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confScrollUp, button3X1, CONF_Y1 + buttonHeight * 5, buttonHeight*0.5, buttonHeight, "/\\", int(33 * resFactor), font1, sf::Color(150, 150, 160), sf::Color(55, 55, 55)));
		turretConfigurationButtons.push_back(Button(bi_confScrollDown, button3X1, CONF_Y1 + buttonHeight * 12, buttonHeight*0.5, buttonHeight, "\\/", int(33 * resFactor), font1, sf::Color(150, 150, 160), sf::Color(55, 55, 55)));
		//Control schemes header
		turretConfigurationButtons.push_back(Button(bi_false, button3X1, CONF_Y1 + buttonHeight * 2, button1Width, buttonHeight, " Control schemes", int(33 * resFactor), font1, sf::Color(130, 130, 135), sf::Color(35, 35, 40)));


		//Save control scheme...
		turretConfigurationButtons.push_back(Button(bi_confSaveTurretScheme, button3X1, CONF_Y1 + buttonHeight * 3, button1Width, buttonHeight, " Save control scheme...", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));
		turretConfigurationButtons.push_back(Button(bi_confLoadTurretScheme, button3X1, CONF_Y1 + buttonHeight * 4, button1Width, buttonHeight, " Load control scheme...", int(33 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(35, 35, 40)));


		reloadTurretControlSchemeList();

}


void ShipEditor::run()
{
	bool keepRunning = true;

	//Initialize editor visual lines
	for (unsigned int i = 0; i < EDITOR_WIDTH + 1; i++)
	{
		horizontalLines.push_back(sf::VertexArray(sf::Lines, 2));
		horizontalLines[horizontalLines.size() - 1][0].color = sf::Color(70, 130, 255);
		horizontalLines[horizontalLines.size() - 1][1].color = sf::Color(70, 130, 255);
	}
	for (unsigned int i = 0; i < EDITOR_HEIGHT + 1; i++)
	{
		verticalLines.push_back(sf::VertexArray(sf::Lines, 2));
		verticalLines[verticalLines.size() - 1][0].color = sf::Color(70, 130, 255);
		verticalLines[verticalLines.size() - 1][1].color = sf::Color(70, 130, 255);
	}


	//Initialize sprites
	updateGridSpriteTextures();

	while (keepRunning)
	{
		//Return/quit
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gettingUserInput == false)
			keepRunning = false;

		clickTimer++;

		//Movement
		if ((focus == editor::base || focus == editor::component) && gettingUserInput == false)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				cameraY -= 8 / scaleFactor;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				cameraY += 8 / scaleFactor;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
				cameraX -= 8 / scaleFactor;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				cameraX += 8 / scaleFactor;
		}

		

		//Mouse grabbing
		mousePos = sf::Mouse::getPosition(mWindow);
		float temp_distance = getDistance(mousePos.x, mousePos.y, circleSliderSpr.getPosition().x, circleSliderSpr.getPosition().y);
		if ((focus != editor::rotate && sf::Mouse::isButtonPressed(sf::Mouse::Right)) || 
			(temp_distance < 100 && focus == editor::rotate && (sf::Mouse::isButtonPressed(sf::Mouse::Left) == true || sf::Mouse::isButtonPressed(sf::Mouse::Right) == true)))
		{
			if (mouseGrab == false)
			{
				mouseGrabX = mousePos.x;
				mouseGrabY = mousePos.y;
				grabCameraOriginX = cameraX;
				grabCameraOriginY = cameraY;
			}			
			mouseGrab = true;
		}
		else
		{
			//std::cout << " noGrabbin' ";
			mouseGrab = false;
		}

		//Handle events
		while (mWindow.pollEvent(mEvent))
		{
			switch (mEvent.type)
			{
			case sf::Event::MouseWheelMoved:
				if (focus != editor::configuration)
					zoom(mEvent.mouseWheel.delta);
				else
				{
					scrollDelta -= scrollDelta + mEvent.mouseWheel.delta;
					if (playerData.grid[selectedX][selectedY]->turret > 0)
						updateTurretControlSchemeList();
				}
				mEvent.mouseWheel.delta = 0;
				scaleFactor = resFactor*zoomFactor;
				break;
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					mouseLeftPressed();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					mouseRightPressed();
				break;
			case sf::Event::KeyPressed:
				if (gettingUserInput == false)
					switch (mEvent.key.code)
				{
					case sf::Keyboard::Delete:
						if (focus == editor::component)
						{
							scrapComponent(selectedX, selectedY);
							updateGridSpriteTextures();
							focus = editor::base;
							std::cout << "\nFocus returned to base";
						}
						else if (focus == editor::configuration)
						{
							for (unsigned int i = 0; i < turretControlSchemeList.size(); i++)
								if (turretControlSchemeList[i].selected == true)	
									workingFileName = turretControlSchemeNameList[i];
									deleteTurretControlScheme();
								
						}
						break;
					case sf::Keyboard::Escape:
						if (focus == editor::rotate)
							applyRotation();
						selectedX = -1;
						selectedY = -1;
						focus = editor::base;

						for (unsigned int i = 0; i < turretControlSchemeList.size(); i++)
							turretControlSchemeList[i].selected = false;

						gettingUserInput = false;
						std::cout << "\nFocus returned to base";
						break;
					case sf::Keyboard::Return:
						if (focus == editor::rotate)
						{
							applyRotation();
							focus = editor::base;
						}
						break;
				}
				else
				{
					if (focus == editor::configuration && playerData.grid[selectedX][selectedY]->turret > 0)
					{
						char temp_char = getUserInput(mEvent);
						std::string temp_str;
						switch (temp_char)
						{
						default:
							if (saveTurretControlSchemeInput.size() < 20)
								saveTurretControlSchemeInput += temp_char;
							break;
						case '#':
							break;
						case '-'://Backspace
							if (saveTurretControlSchemeInput.size() <= 0)
								break;

							for (unsigned int i = 0; i < saveTurretControlSchemeInput.size()-1; i++)
								temp_str.push_back(saveTurretControlSchemeInput[i]);
							
							saveTurretControlSchemeInput = temp_str;
							break;
						case '+'://Space
							saveTurretControlSchemeInput += ' ';
							break;
						case '>'://Enter
							workingFileName = saveTurretControlSchemeInput;
							saveTurretControlScheme();
							gettingUserInput = false;
							break;
						case '<'://Escape
							for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
								if (turretConfigurationButtons[i].id == bi_confSaveTurretScheme)
									turretConfigurationButtons[i].text.setString(" Save control scheme...");
							gettingUserInput = false;
							break;
						}
						
						if (gettingUserInput == true)
							for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
							if (turretConfigurationButtons[i].id == bi_confSaveTurretScheme)
								turretConfigurationButtons[i].text.setString(saveTurretControlSchemeInput);
					}

				}
			}
		}


			/////////////////////
		   /**/drawWindow();/**/
		  /////////////////////


	}


}

void ShipEditor::drawWindow()
{
	//Drawing
	mWindow.clear(sf::Color(40, 90, 235));
	rotateGlowAngle();

	//Update camera position when mouseGrabbing
	if (mouseGrab == true)
		updateMouseGrab();

	//Selection highlight
	drawSelectedRect();

	//Handle visual lines
	updateLines();
	for (unsigned int i = 0; i < horizontalLines.size(); i++)
		mWindow.draw(horizontalLines[i]);
	for (unsigned int i = 0; i < verticalLines.size(); i++)
		mWindow.draw(verticalLines[i]);

	//Draw component sprites
	updateGridSpriteLocations();
	for (int x = 0; x < EDITOR_WIDTH; x++)
		for (int y = 0; y < EDITOR_HEIGHT; y++)
			for (unsigned int i = 0; i < gridSprites[x][y].size(); i++)
				mWindow.draw(gridSprites[x][y][i]);
	drawSelectionShadeHighlight();
	drawInheritanceSprites();

	if (focus == editor::actions)
		drawActions();
	else if (focus == editor::configuration)
		drawConfigurations();
	else if (focus == editor::rotate)
		drawCircleSlider();

	mWindow.display();
}

void ShipEditor::updateLines()
{
	for (unsigned int i = 0; i < EDITOR_WIDTH + 1; i++)
	{
		horizontalLines[i][0].position = sf::Vector2f(0, (i * 100 - cameraY)*scaleFactor);
		horizontalLines[i][1].position = sf::Vector2f(WINDOW_WIDTH, (i * 100 - cameraY)*scaleFactor);
	}
	for (unsigned int i = 0; i < EDITOR_HEIGHT + 1; i++)
	{
		verticalLines[i][0].position = sf::Vector2f((i * 100 - cameraX)*scaleFactor, 0);
		verticalLines[i][1].position = sf::Vector2f((i * 100 - cameraX)*scaleFactor, WINDOW_HEIGHT);
	}
}

void ShipEditor::updateGridSpriteTextures()
{	
	for (unsigned int x = 0; x < EDITOR_WIDTH; x++)
		for (unsigned int y = 0; y < EDITOR_HEIGHT; y++)
		{
			//Clear all previous sprites
			while (!gridSprites[x][y].empty())
				gridSprites[x][y].pop_back();
			while (!inheritanceSprites[x][y].empty())
				inheritanceSprites[x][y].pop_back();

			//Add skeleton sprite
			if (playerData.grid[x][y]->armor > 0)
			{
				gridSprites[x][y].push_back(sf::Sprite());
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTexture(RM.getTexture("skeleton.png"));
				gridSprites[x][y][gridSprites[x][y].size() - 1].setOrigin(50, 50);
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTextureRect(sf::IntRect(1400, 0, 100, 100));
			}

			//Add turret sprite
			if (playerData.grid[x][y]->turret > 0)
			{
				gridSprites[x][y].push_back(sf::Sprite());
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTexture(RM.getTexture("editorTurret.png"));
				gridSprites[x][y][gridSprites[x][y].size() - 1].setOrigin(50, 50); 
				gridSprites[x][y][gridSprites[x][y].size() - 1].setRotation(360 - playerData.grid[x][y]->angleModifier);
			}
			
			//Add engine sprite

			//Add inheritance arrows
			if (playerData.grid[x][y]->childUp == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(RM.getTexture("inheritanceArrow.png"));
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(-46, 50);
			}
			if (playerData.grid[x][y]->childDown == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(RM.getTexture("inheritanceArrow.png"));
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(59, 150);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setRotation(180);
			}
			if (playerData.grid[x][y]->childLeft == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(RM.getTexture("inheritanceArrow.png"));
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(57, 50);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setRotation(270);
			}
			if (playerData.grid[x][y]->childRight == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(RM.getTexture("inheritanceArrow.png"));
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(-44, 150);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setRotation(90);
			}
		}
}
void ShipEditor::updateGridSpriteLocations()
{
	for (unsigned int x = 0; x < EDITOR_WIDTH; x++)
		for (unsigned int y = 0; y < EDITOR_HEIGHT; y++)
		{
		for (unsigned int i = 0; i < gridSprites[x][y].size(); i++)
		{//Grid sprites
			gridSprites[x][y][i].setPosition((x * 100 - cameraX)*scaleFactor + 50 * scaleFactor, (y * 100 - cameraY)*scaleFactor + 50 * scaleFactor);
			gridSprites[x][y][i].setScale(scaleFactor, scaleFactor);
			if (playerData.grid[x][y]->core == true)
				gridSprites[x][y][i].setColor(sf::Color(240, 40, 180));

		}
		for (unsigned int i = 0; i < inheritanceSprites[x][y].size(); i++)
		{//Inheritance sprites
			inheritanceSprites[x][y][i].setPosition((x * 100 - cameraX)*scaleFactor, (y * 100 - cameraY)*scaleFactor);
			inheritanceSprites[x][y][i].setScale(scaleFactor, scaleFactor);
		}
		}
}

void ShipEditor::updateMouseGrab()
{
	if (focus == editor::actions || focus == editor::configuration)
		return;

	if (focus == editor::rotate)
	{
		double temp_angle = atan2(mousePos.y - circleSliderSpr.getPosition().y, mousePos.x - circleSliderSpr.getPosition().x);
		if (temp_angle < 0)
			temp_angle = abs(temp_angle);
		else
			temp_angle = PI*2 - temp_angle;

		circleSliderSpr.setRotation(360 - (180/PI)*temp_angle);
	}
	else
	{
		cameraX = grabCameraOriginX + (mouseGrabX - mousePos.x) / scaleFactor;
		cameraY = grabCameraOriginY + (mouseGrabY - mousePos.y) / scaleFactor;
	}
}


void ShipEditor::rotateGlowAngle()
{
	glowAngle += PI / 60;
	if (glowAngle > PI * 2)
		glowAngle = 0;
	glowAmount = 0.5 + sin(glowAngle)/2;
}

void ShipEditor::mouseLeftPressed()
{
	//Temp variables
	bool temp_axisState = directionalMovement;
	bool temp_manualButtonsState = true;

	checkX = -1;
	checkY = -1;
	switch (focus)
	{
	case editor::base:
		if (((cameraX + mousePos.x / scaleFactor) / 100.0 > 0) 
			&& ((cameraX + mousePos.x / scaleFactor) / 100.0 < EDITOR_WIDTH)
			&& ((cameraY + mousePos.y / scaleFactor) / (100.0) > 0)
			&& ((cameraY + mousePos.y / scaleFactor) / (100.0) < EDITOR_HEIGHT))
		{
			selectedX = int((cameraX + mousePos.x / scaleFactor) / (100.0));
			selectedY = int((cameraY + mousePos.y / scaleFactor) / (100.0));
		}
		else
		{
			selectedX = -1;
			selectedY = -1;
			focus = editor::base;
			std::cout << "\nFocus returned to base";
			break;
		}

		if (playerData.grid[selectedX][selectedY]->armor > 0)
		{
			std::cout << "\nFocus returned to base";
			focus = editor::component;
		}
		else
		{
			std::cout << "\nFocus returned to base";
			focus = editor::base;
		}

		std::cout << "\nSelected: " << selectedX << ", " << selectedY;
		break;
	case editor::component:
		if ((cameraX + mousePos.x / scaleFactor) / 100.0 > 0)
			 checkX = (cameraX + mousePos.x / scaleFactor) / (100.0);
		if ((cameraY + mousePos.y / scaleFactor) / (100.0) > 0)
			checkY = (cameraY + mousePos.y / scaleFactor) / (100.0);
		
		//Up
		if (checkX == selectedX && checkY == selectedY - 1 && checkY >= 0 && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childUp = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		//Down
		else if (checkX == selectedX && checkY == selectedY + 1 && checkY < EDITOR_HEIGHT && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childDown = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		//Left
		else if (checkX == selectedX - 1 && checkY == selectedY && checkX >= 0 && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childLeft = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		//Right
		else if (checkX == selectedX + 1 && checkY == selectedY && checkX < EDITOR_WIDTH && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childRight = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		else if (checkX > -1 && checkX < EDITOR_WIDTH && checkY > -1 && checkY < EDITOR_HEIGHT)
		{
			if (selectedX == checkX && selectedY == checkY)
			{
				selectedX = -1;
				selectedY = -1;
				focus = editor::base;
				std::cout << "\nFocus returned to base";
				break;
			}

			selectedX = checkX;
			selectedY = checkY;
			if (playerData.grid[selectedX][selectedY]->armor < 1)
			{
				focus = editor::base;
				std::cout << "\nFocus returned to base";
			}
		}
		else
		{
			selectedX = -1;
			selectedY = -1;
			focus = editor::base;
			std::cout << "\nFocus returned to base";
		}
		break;
	case editor::actions:
		//ButtonId pressedButton = bi_false;
		//ButtonId temp_pressedButton = bi_false;
		//for (unsigned int i = 0; i < actionButtons.size(); i++)
		//{
		//	temp_pressedButton = actionButtons[i].checkIfPressed(mousePos);
		//	if (temp_pressedButton != bi_false)
		//		pressedButton = temp_pressedButton;
		//}
		for (unsigned int i = 0; i < actionButtons.size(); i++)
			switch (actionButtons[i].checkIfPressed(mousePos))
			{
			//case bi_false:
			//	focus = editor::base;
			//	std::cout << "\nFocus returned to base (action button id bi_false)";
			//	break;
			case bi_actionTurret:
				if (playerData.grid[selectedX][selectedY]->turret > 0)
				{

				}
				else
				{
					playerData.grid[selectedX][selectedY]->turret = 1;
					playerData.grid[selectedX][selectedY]->mouseAim = true;
					playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter = false;
					playerData.grid[selectedX][selectedY]->turretFire.inputType = mouseInput;
					playerData.grid[selectedX][selectedY]->turretFire.mouseButton = sf::Mouse::Left;
					//Default controls:
					//playerData.grid[selectedX][selectedY]->turretLeft.inputType = keyboardInput;
					//playerData.grid[selectedX][selectedY]->turretLeft.keyCode = sf::Keyboard::Z;
					//playerData.grid[selectedX][selectedY]->turretRight.inputType = keyboardInput;
					//playerData.grid[selectedX][selectedY]->turretRight.keyCode = sf::Keyboard::X;
					//playerData.grid[selectedX][selectedY]->turretFire.inputType = keyboardInput;
					//playerData.grid[selectedX][selectedY]->turretFire.keyCode = sf::Keyboard::Space;
					
					updateGridSpriteTextures();
				}
				focus = editor::base;
				break;
			case bi_actionEngine:
				std::cout << "\nSelected Add engine";

				focus = editor::base;
				break;
			case bi_actionRotate:
				std::cout << "\nSelected Rotate";
				focus = editor::rotate;
				break;
			case bi_actionConfiguration:
				turretConfigurationButtons[0].text.setString(" Turret [ " + std::to_string(selectedX) + ", " + std::to_string(selectedY) + " ] configurations");
				focus = editor::configuration;

				updateTurretConfigurationButtonVisibility();

				//Set button strings when entering the conf window
				for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
					switch (turretConfigurationButtons[i].id)
				{
					case bi_confBindFire:
						turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretFire));
						break;
					case bi_confBindReload:
						turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretReload));
						break;
					case bi_confBindTurnTurretRight:
						turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretRight));
						break;
					case bi_confBindTurnTurretLeft:
						turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretLeft));
						break;
					case bi_confBindVerticalAimAxis:
						turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(playerData.grid[selectedX][selectedY]->verticalAxis) + " (" + getPolarityAsString(playerData.grid[selectedX][selectedY]->verticalAxisPolarity) + ")");
						break;
					case bi_confBindHorizontalAimAxis:
						turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(playerData.grid[selectedX][selectedY]->horizontalAxis) + " (" + getPolarityAsString(playerData.grid[selectedX][selectedY]->horizontalAxisPolarity) + ")");
						break;
						
					//Nodes
					case bi_confNodeHoldToFire:
						turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->holdToFire));
						break;
					case bi_confNodeDirectionalAim:
						turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->directionalAim));
						break;
					case bi_confNodeMouseAim:
						turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->mouseAim));
						break;
					case bi_confNodeMouseAimRelative:
						turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter));
						break;
				}

				break;

			}
		

		break;
	case editor::configuration:
		//Common configuration actions
		for (unsigned int i = 0; i < configurationButtons.size(); i++)
			switch (configurationButtons[i].checkIfPressed(mousePos))
		{
			case bi_confExit:
				std::cout << "\nPressed X (close)";
				focus = editor::base;
				gettingUserInput = false;
				break;

		}




		//Core configuration specific:
		if (playerData.grid[selectedX][selectedY]->core == true)
			for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
				switch (coreConfigurationButtons[i].checkIfPressed(mousePos))
			{
				case bi_confNodeDirectionalMovement:
					if (directionalMovement)
						directionalMovement = false;
					else
						directionalMovement = true;
					temp_axisState = directionalMovement;
					if (temp_axisState)
						temp_manualButtonsState = false;
					for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
						switch (coreConfigurationButtons[i].id)
					{
						case bi_confBindVerticalMoveAxis:
						case bi_confBindHorizontalMoveAxis:
							coreConfigurationButtons[i - 1].visible = temp_axisState; //Label
							coreConfigurationButtons[i].visible = temp_axisState;							
							break;
						case bi_confBindAccelerate:
						case bi_confBindTurnLeft:
						case bi_confBindTurnRight:
							coreConfigurationButtons[i - 1].visible = temp_manualButtonsState; //Label
							coreConfigurationButtons[i].visible = temp_manualButtonsState;
							break;
					}

					//Update button locations
					for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
						if (coreConfigurationButtons[i].id == bi_confBindHorizontalMoveAxis)
							if (directionalMovement == true)
								for (int t = 0; t < 6; t++)
									coreConfigurationButtons[i + t + 7].setPosition(coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().x, coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().y - CONF_BUTTON_HEIGHT);
							else
								for (int t = 0; t < 6; t++)
									coreConfigurationButtons[i + t + 7].setPosition(coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().x, coreConfigurationButtons[i + t + 7].buttonRectangle.getPosition().y + CONF_BUTTON_HEIGHT);

					//Update background size
					if (directionalMovement)
						coreConfigurationButtons[1].buttonRectangle.setSize(sf::Vector2f(button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight * 7 + 20 * resFactor));
					else
						coreConfigurationButtons[1].buttonRectangle.setSize(sf::Vector2f(button2X1 + button2Width - button1X1 + 20 * resFactor, buttonHeight * 8 + 20 * resFactor));

					//Update useDirMove bg color
					for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
					if (coreConfigurationButtons[i].id == bi_confNodeDirectionalMovement)
					if (directionalMovement == true)
						{
							coreConfigurationButtons[i - 1].red = 105;
							coreConfigurationButtons[i - 1].green = 105;
							coreConfigurationButtons[i - 1].blue = 110;
							coreConfigurationButtons[i].red = 105;
							coreConfigurationButtons[i].green = 105;
							coreConfigurationButtons[i].blue = 110;
						}
						else
						{
							coreConfigurationButtons[i - 1].red = 110;
							coreConfigurationButtons[i - 1].green = 110;
							coreConfigurationButtons[i - 1].blue = 115;
							coreConfigurationButtons[i].red = 110;
							coreConfigurationButtons[i].green = 110;
							coreConfigurationButtons[i].blue = 115;
						}

					coreConfigurationButtons[i].text.setString(" " + getBoolAsString(directionalMovement));
					drawWindow();
					break;
				case bi_confBindVerticalMoveAxis:
					coreConfigurationButtons[i].text.setString(">Move an analog stick down<");
					drawWindow();
					temp_idAxisPair = detectAxis();
					moveJoystickId = abs(temp_idAxisPair.first) - 1;
					verticalMoveAxis = temp_idAxisPair.second;
					verticalMoveAxisPolarity = abs(temp_idAxisPair.first) / temp_idAxisPair.first;
					coreConfigurationButtons[i].text.setString(" Joystick " + std::to_string(moveJoystickId) + "::" + getAxisAsString(verticalMoveAxis) + " (" + getPolarityAsString(temp_idAxisPair.first) + ")");
					drawWindow();
					break;
				case bi_confBindHorizontalMoveAxis:
					coreConfigurationButtons[i].text.setString(">Move an analog stick right<");
					drawWindow();
					temp_idAxisPair = detectAxis();
					moveJoystickId = abs(temp_idAxisPair.first) - 1;
					horizontalMoveAxis = temp_idAxisPair.second;
					horizontalMoveAxisPolarity = abs(temp_idAxisPair.first) / temp_idAxisPair.first;
					coreConfigurationButtons[i].text.setString(" Joystick " + std::to_string(moveJoystickId) + "::" + getAxisAsString(horizontalMoveAxis) + " (" + getPolarityAsString(temp_idAxisPair.first) + ")");
					drawWindow();
					break;
				case bi_confBindAccelerate:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_accelerate] = detectKey(bi_confBindAccelerate);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_accelerate]));
					break;
				case bi_confBindTurnRight:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_turnRight] = detectKey(bi_confBindTurnRight);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_turnRight]));
					break;
				case bi_confBindTurnLeft:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_turnLeft] = detectKey(bi_confBindTurnLeft);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_turnLeft]));
					break;
				case bi_confBindReverse:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_reverse] = detectKey(bi_confBindReverse);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_reverse]));
					break;
				case bi_confBindZoomIn:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_zoomIn] = detectKey(bi_confBindZoomIn);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_zoomIn]));
					break;
				case bi_confBindZoomOut:
					coreConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					coreKeys[key_zoomOut] = detectKey(bi_confBindZoomOut);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_zoomOut]));
					break;
			}
		else if (playerData.grid[selectedX][selectedY]->turret > 0)
		{
			for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
				switch (turretConfigurationButtons[i].checkIfPressed(mousePos))
			{
				case bi_confBindFire:
					turretConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					componentKeys[selectedX + selectedY*0.001 + KEYB_fire] = detectKey(bi_confBindFire);
					playerData.grid[selectedX][selectedY]->turretFire = componentKeys[selectedX + selectedY*0.001 + KEYB_fire];
					turretConfigurationButtons[i].text.setString(getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_fire]));
					break;
				case bi_confBindReload:
					turretConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					componentKeys[selectedX + selectedY*0.001 + KEYB_reload] = detectKey(bi_confBindReload);
					playerData.grid[selectedX][selectedY]->turretReload = componentKeys[selectedX + selectedY*0.001 + KEYB_reload];
					turretConfigurationButtons[i].text.setString(getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_reload]));
					break;
				case bi_confBindTurnTurretRight:
					turretConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					componentKeys[selectedX + selectedY*0.001 + KEYB_right] = detectKey(bi_confBindTurnTurretRight);
					playerData.grid[selectedX][selectedY]->turretRight = componentKeys[selectedX + selectedY*0.001 + KEYB_right];
					turretConfigurationButtons[i].text.setString(getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_right]));
					break;
				case bi_confBindTurnTurretLeft:
					turretConfigurationButtons[i].text.setString(">Press a key or move a joystick<");
					drawWindow();
					componentKeys[selectedX + selectedY*0.001 + KEYB_left] = detectKey(bi_confBindTurnTurretLeft);
					playerData.grid[selectedX][selectedY]->turretLeft = componentKeys[selectedX + selectedY*0.001 + KEYB_left];
					turretConfigurationButtons[i].text.setString(getInputAsString(componentKeys[selectedX + selectedY*0.001 + KEYB_left]));
					break;
				case bi_confBindVerticalAimAxis:
					turretConfigurationButtons[i].text.setString(">Move an analog stick down<");
					drawWindow();
					temp_idAxisPair = detectAxis();
					playerData.grid[selectedX][selectedY]->verticalAxisPolarity = abs(temp_idAxisPair.first) / temp_idAxisPair.first;
					playerData.grid[selectedX][selectedY]->directionalJoystickId = abs(temp_idAxisPair.first) - 1;
					playerData.grid[selectedX][selectedY]->verticalAxis = temp_idAxisPair.second; // componentKeys[selectedX + selectedY*0.001 + KEYB_vertical].second;
					turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(temp_idAxisPair.second) + " (" + getPolarityAsString(temp_idAxisPair.first) + ")");
					break;
				case bi_confBindHorizontalAimAxis:
					turretConfigurationButtons[i].text.setString(">Move an analog stick right<");
					drawWindow();
					temp_idAxisPair = detectAxis();
					playerData.grid[selectedX][selectedY]->horizontalAxisPolarity = abs(temp_idAxisPair.first) / temp_idAxisPair.first;
					playerData.grid[selectedX][selectedY]->directionalJoystickId = abs(temp_idAxisPair.first) - 1;
					playerData.grid[selectedX][selectedY]->horizontalAxis = temp_idAxisPair.second; // componentKeys[selectedX + selectedY*0.001 + KEYB_vertical].second;
					turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(temp_idAxisPair.second) + " (" + getPolarityAsString(temp_idAxisPair.first) + ")");
					break;

					//Nodes
				case bi_confNodeHoldToFire:
					if (playerData.grid[selectedX][selectedY]->holdToFire == true)
					{//Set false
						turretConfigurationButtons[i].text.setString(" False");
						playerData.grid[selectedX][selectedY]->holdToFire = false;
					}
					else
					{//Set true
						turretConfigurationButtons[i].text.setString(" True");
						playerData.grid[selectedX][selectedY]->holdToFire = true;
					}
					break;
				case bi_confNodeMouseAim:
					if (playerData.grid[selectedX][selectedY]->mouseAim == true)
					{//Set false
						turretConfigurationButtons[i].text.setString(" False");
						playerData.grid[selectedX][selectedY]->mouseAim = false;
					}
					else
					{//Set true
						turretConfigurationButtons[i].text.setString(" True");
						playerData.grid[selectedX][selectedY]->mouseAim = true;
					}
					updateTurretConfigurationButtonVisibility();
					break;
				case bi_confNodeMouseAimRelative:
					if (playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter == true)
					{//Set false
						turretConfigurationButtons[i].text.setString(" False");
						playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter = false;
					}
					else
					{//Set true
						turretConfigurationButtons[i].text.setString(" True");
						playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter = true;
					}
					break;
				case bi_confNodeDirectionalAim:
					if (playerData.grid[selectedX][selectedY]->directionalAim == true)
					{//Set false
						turretConfigurationButtons[i].text.setString(" False");
						playerData.grid[selectedX][selectedY]->directionalAim = false;
					}
					else
					{//Set true
						turretConfigurationButtons[i].text.setString(" True");
						playerData.grid[selectedX][selectedY]->directionalAim = true;
					}
					updateTurretConfigurationButtonVisibility();
					break;
					//Control schemes
				case bi_confSaveTurretScheme:
					if (gettingUserInput == false)
					{
						turretConfigurationButtons[i].text.setString(" >Input name for this control scheme<");
						saveTurretControlSchemeInput = "";
						gettingUserInput = true;
					}
					else
					{
						workingFileName = saveTurretControlSchemeInput;
						saveTurretControlScheme();
						gettingUserInput = false;
					}
					break;
				case bi_confLoadTurretScheme:
					for (unsigned int j = 0; j < turretControlSchemeList.size(); j++)
						if (turretControlSchemeList[j].selected == true)
						{
						workingFileName = turretControlSchemeNameList[j];
						loadTurretControlScheme();
						}
					break;
				case bi_confScrollUp:
					scrollDelta = -1;
					if (playerData.grid[selectedX][selectedY]->turret > 0)
						updateTurretControlSchemeList();
					break;
				case bi_confScrollDown:
					scrollDelta = 1;
					if (playerData.grid[selectedX][selectedY]->turret > 0)
						updateTurretControlSchemeList();
					break;
			}
			for (unsigned int j = 0; j < turretControlSchemeList.size(); j++)
				if (turretControlSchemeList[j].mouseOverlap(mousePos) && turretControlSchemeList[j].visible == true)
				{
				if (turretControlSchemeList[j].selected == false)
				{
					clickTimer = 0;
					for (unsigned int k = 0; k < turretControlSchemeList.size(); k++)
						turretControlSchemeList[k].selected = false;
					turretControlSchemeList[j].selected = true;
				}
				else if (clickTimer < 30)
				{
					workingFileName = turretControlSchemeNameList[j];
					loadTurretControlScheme();
					turretControlSchemeList[j].selected = false;
				}
				else
					turretControlSchemeList[j].selected = false;
				}
		}//End of turret conf
		else if (playerData.grid[selectedX][selectedY]->engine > 0)
			for (unsigned int i = 0; i < engineConfigurationButtons.size(); i++)
				switch (engineConfigurationButtons[i].checkIfPressed(mousePos))
			{

			}
		break;
	}
}
void ShipEditor::mouseRightPressed()
{
	checkX = -1;
	checkY = -1;
	switch (focus)
	{
	case editor::base:
	case editor::component:
	case editor::actions:
		if ((cameraX + mousePos.x / scaleFactor) / 100.0 > 0)
			checkX = (cameraX + mousePos.x / scaleFactor) / (100.0);
		if ((cameraY + mousePos.y / scaleFactor) / (100.0) > 0)
			checkY = (cameraY + mousePos.y / scaleFactor) / (100.0);

		//Select component and open component actions
		if (checkX > -1 && checkX < EDITOR_WIDTH && checkY > -1 && checkY < EDITOR_HEIGHT && playerData.grid[checkX][checkY]->armor > 0)
		{
			selectedX = checkX;
			selectedY = checkY;

			std::cout << "\nEntered actions popup window";
			focus = editor::actions;
			if (playerData.grid[selectedX][selectedY]->core == false)
			for (unsigned int i = 0; i < actionButtons.size(); i++)
			{			
				actionButtons[i].buttonRectangle.setPosition(mousePos.x, mousePos.y + 30 * i * resFactor);
				actionButtons[i].text.setPosition(mousePos.x + 10 * resFactor, mousePos.y + 30 * i * resFactor);				
			}
			else
			{
				actionButtons[0].buttonRectangle.setPosition(mousePos.x, mousePos.y);
				actionButtons[0].text.setPosition(mousePos.x + 10 * resFactor, mousePos.y);
				actionButtons[actionButtons.size()-1].buttonRectangle.setPosition(mousePos.x, mousePos.y + 30 * resFactor);
				actionButtons[actionButtons.size()-1].text.setPosition(mousePos.x + 10 * resFactor, mousePos.y + 30 * resFactor);
			}
		}

		break;
	}
}

void ShipEditor::zoom(int delta)
{
	float tempZoom = zoomFactor;
	zoomFactor += 0.1*delta;
	float zoomCut = limitZoom();
	cameraX += (mousePos.x / float(WINDOW_WIDTH)) * ((WINDOW_WIDTH / (tempZoom*resFactor)) - (WINDOW_WIDTH / (zoomFactor*resFactor)));
	cameraY += (mousePos.y / float(WINDOW_HEIGHT)) * ((WINDOW_HEIGHT / (tempZoom*resFactor)) - (WINDOW_HEIGHT / (zoomFactor*resFactor)));
	

}

void ShipEditor::scrapComponent(int x, int y)
{
	//if selection is invalid-> return
	if (selectedX < 0 || selectedX > EDITOR_WIDTH - 1 || selectedY < 0 || selectedY > EDITOR_HEIGHT - 1)
		return;

	//Notify parent component
	if (x > 0)
		playerData.grid[x + 1][y]->childLeft = false;
	if (x < EDITOR_WIDTH - 1)
		playerData.grid[x - 1][y]->childRight = false;
	if (y > 0)
		playerData.grid[x][y - 1]->childDown = false;
	if (y < EDITOR_HEIGHT - 1)
		playerData.grid[x][y + 1]->childUp = false;




	if (playerData.grid[x][y]->childUp == false
		&& playerData.grid[x][y]->childDown == false
		&& playerData.grid[x][y]->childLeft == false
		&& playerData.grid[x][y]->childRight == false)
	{//No child components-> scrap and return
		std::cout << "\n" << x << ", " << y << " Deleted - no child componenets found";
		delete playerData.grid[x][y];
		playerData.grid[x][y] = new GridData;
		return;
	}

	//The component has child components-> scrap children first
	if (playerData.grid[x][y]->childUp == true)
		scrapComponent(x, y - 1);
	if (playerData.grid[x][y]->childDown == true)
		scrapComponent(x, y + 1);
	if (playerData.grid[x][y]->childLeft == true)
		scrapComponent(x - 1, y);
	if (playerData.grid[x][y]->childRight == true)
		scrapComponent(x + 1, y);

	delete playerData.grid[x][y];
	playerData.grid[x][y] = new GridData;
	std::cout << "\n" << x << ", " << y << " Deleted";
}

void ShipEditor::applyRotation()
{
	playerData.grid[selectedX][selectedY]->angleModifier = int(360 - circleSliderSpr.getRotation());
	std::cout << "\nrotation " << playerData.grid[selectedX][selectedY]->angleModifier;
	updateGridSpriteTextures();
}

void ShipEditor::drawInheritanceSprites()
{
	for (int x = 0; x < EDITOR_WIDTH; x++)
		for (int y = 0; y < EDITOR_HEIGHT; y++)
			for (unsigned int i = 0; i < inheritanceSprites[x][y].size(); i++)
			{
				inheritanceSprites[x][y][i].setColor(sf::Color(200 + int(55 * glowAmount), 200 + int(55 * glowAmount), 200 + int(55 * glowAmount)));
				mWindow.draw(inheritanceSprites[x][y][i]);
			}
}
void ShipEditor::drawSelectionShadeHighlight()
{
	if (focus != editor::component && focus != editor::rotate)
		return;

	mWindow.draw(shadeRect);
	for (unsigned int i = 0; i < gridSprites[selectedX][selectedY].size(); i++)
		mWindow.draw(gridSprites[selectedX][selectedY][i]);

}
void ShipEditor::drawSelectedRect()
{
	//if selection is out of grid, return
	if (selectedX < 0 || selectedX > EDITOR_WIDTH - 1 || selectedY < 0 || selectedY > EDITOR_HEIGHT - 1)
		return;

	selectedRect.setSize(sf::Vector2f(100 * scaleFactor, 100 * scaleFactor));
	selectedRect.setPosition((selectedX * 100 - cameraX)*scaleFactor, (selectedY * 100 - cameraY)*scaleFactor);
	selectedRect.setFillColor(sf::Color(0 + int(glowAmount * 10), 60 + int(glowAmount * 10), 195 + int(glowAmount * 10)));

	mWindow.draw(selectedRect);
}
void ShipEditor::drawActions()
{

	if (playerData.grid[selectedX][selectedY]->core == false)
	{
		actionButtons[0].text.setString("Component [" + std::to_string(selectedX) + "][" + std::to_string(selectedY) + "]");
		for (unsigned int i = 0; i < actionButtons.size(); i++)
			actionButtons[i].draw(mWindow, mousePos);
	}
	else
	{
		actionButtons[0].text.setString("Core [" + std::to_string(selectedX) + "][" + std::to_string(selectedY) + "]");
		actionButtons[0].draw(mWindow, mousePos);
		actionButtons[actionButtons.size()-1].draw(mWindow, mousePos);
	}
}
void ShipEditor::drawConfigurations()
{
	mWindow.draw(configurationRect1);
	mWindow.draw(configurationRect2);
	mWindow.draw(configurationRect3);
	for (unsigned int i = 0; i < configurationButtons.size(); i++)
		configurationButtons[i].draw(mWindow, mousePos);
	

	if (playerData.grid[selectedX][selectedY]->core == true)
		for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
			coreConfigurationButtons[i].draw(mWindow, mousePos);
	else if (playerData.grid[selectedX][selectedY]->turret > 0)
	{
		for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
			turretConfigurationButtons[i].draw(mWindow, mousePos);
		for (unsigned int i = 0; i < turretControlSchemeList.size(); i++)
			turretControlSchemeList[i].draw(mWindow, mousePos);
	}
	else if (playerData.grid[selectedX][selectedY]->engine > 0)
		for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
			engineConfigurationButtons[i].draw(mWindow, mousePos);
	
}
void ShipEditor::drawCircleSlider()
{
	circleSliderSpr.setPosition((selectedX * 100 - cameraX)*scaleFactor + 50 * scaleFactor, (selectedY * 100 - cameraY)*scaleFactor + 50 * scaleFactor);
	circleSliderSpr.setColor(sf::Color(200+glowAmount*55, 200+glowAmount*55, 200+glowAmount*55));
	mWindow.draw(circleSliderSpr);
}

void ShipEditor::updateTurretConfigurationButtonVisibility()
{
	for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
		if (playerData.grid[selectedX][selectedY]->mouseAim == true)
		{//Mouse aim enabled
		switch (turretConfigurationButtons[i].id)
		{
		case bi_confNodeDirectionalAim:
		case bi_confBindTurnTurretRight:
		case bi_confBindTurnTurretLeft:
		case bi_confBindVerticalAimAxis:
		case bi_confBindHorizontalAimAxis:
			turretConfigurationButtons[i].text.setColor(sf::Color
				(turretConfigurationButtons[i].text.getColor().r,
				turretConfigurationButtons[i].text.getColor().g,
				turretConfigurationButtons[i].text.getColor().b, 70));
			turretConfigurationButtons[i-1].text.setColor(sf::Color
				(turretConfigurationButtons[i-1].text.getColor().r,
				turretConfigurationButtons[i-1].text.getColor().g,
				turretConfigurationButtons[i-1].text.getColor().b, 70));
			break;
		case bi_confNodeMouseAim:
		case bi_confNodeMouseAimRelative:
			turretConfigurationButtons[i].text.setColor(sf::Color
				(turretConfigurationButtons[i].text.getColor().r,
				turretConfigurationButtons[i].text.getColor().g,
				turretConfigurationButtons[i].text.getColor().b, 255));
			turretConfigurationButtons[i - 1].text.setColor(sf::Color
				(turretConfigurationButtons[i - 1].text.getColor().r,
				turretConfigurationButtons[i - 1].text.getColor().g,
				turretConfigurationButtons[i - 1].text.getColor().b, 255));
			break;
		}
		}
		else if (playerData.grid[selectedX][selectedY]->directionalAim == true)
		{//Mouse aim disabled, analog aim enabled
			switch (turretConfigurationButtons[i].id)
			{
			case bi_confNodeMouseAimRelative:
			case bi_confBindTurnTurretRight:
			case bi_confBindTurnTurretLeft:
				turretConfigurationButtons[i].text.setColor(sf::Color
					(turretConfigurationButtons[i].text.getColor().r,
					turretConfigurationButtons[i].text.getColor().g,
					turretConfigurationButtons[i].text.getColor().b, 70));
				turretConfigurationButtons[i - 1].text.setColor(sf::Color
					(turretConfigurationButtons[i - 1].text.getColor().r,
					turretConfigurationButtons[i - 1].text.getColor().g,
					turretConfigurationButtons[i - 1].text.getColor().b, 70));
				break;
			case bi_confNodeMouseAim:
			case bi_confNodeDirectionalAim:
			case bi_confBindVerticalAimAxis:
			case bi_confBindHorizontalAimAxis:
				turretConfigurationButtons[i].text.setColor(sf::Color
					(turretConfigurationButtons[i].text.getColor().r,
					turretConfigurationButtons[i].text.getColor().g,
					turretConfigurationButtons[i].text.getColor().b, 255));
				turretConfigurationButtons[i - 1].text.setColor(sf::Color
					(turretConfigurationButtons[i - 1].text.getColor().r,
					turretConfigurationButtons[i - 1].text.getColor().g,
					turretConfigurationButtons[i - 1].text.getColor().b, 255));
				break;
			}
		}
		else
		{//Manual button press aim enabled
			switch (turretConfigurationButtons[i].id)
			{
			case bi_confNodeMouseAimRelative:
			case bi_confBindVerticalAimAxis:
			case bi_confBindHorizontalAimAxis:
				turretConfigurationButtons[i].text.setColor(sf::Color
					(turretConfigurationButtons[i].text.getColor().r,
					turretConfigurationButtons[i].text.getColor().g,
					turretConfigurationButtons[i].text.getColor().b, 70));
				turretConfigurationButtons[i - 1].text.setColor(sf::Color
					(turretConfigurationButtons[i - 1].text.getColor().r,
					turretConfigurationButtons[i - 1].text.getColor().g,
					turretConfigurationButtons[i - 1].text.getColor().b, 70));
				break;
			case bi_confNodeMouseAim:
			case bi_confNodeDirectionalAim:
			case bi_confBindTurnTurretRight:
			case bi_confBindTurnTurretLeft:
				turretConfigurationButtons[i].text.setColor(sf::Color
					(turretConfigurationButtons[i].text.getColor().r,
					turretConfigurationButtons[i].text.getColor().g,
					turretConfigurationButtons[i].text.getColor().b, 255));
				turretConfigurationButtons[i - 1].text.setColor(sf::Color
					(turretConfigurationButtons[i - 1].text.getColor().r,
					turretConfigurationButtons[i - 1].text.getColor().g,
					turretConfigurationButtons[i - 1].text.getColor().b, 255));
				break;
			}
		}
}


void ShipEditor::saveTurretControlScheme()
{
	if (workingFileName == "" || workingFileName == " " || workingFileName == "  " || workingFileName == "\n" || workingFileName == "\0")
		return;

	for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
		if (turretConfigurationButtons[i].id == bi_confSaveTurretScheme)
			turretConfigurationButtons[i].text.setString(" Save control scheme...");
	
	gettingUserInput = false;

	mFileStream.open("Settings/TurretControlSchemes/" + workingFileName + ".dat", std::ofstream::binary | std::ios::out);
	if (mFileStream)
	{
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->turretFire, sizeof(MyKeys));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->holdToFire, sizeof(bool));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->turretReload, sizeof(MyKeys));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->mouseAim, sizeof(bool));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter, sizeof(bool));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->directionalAim, sizeof(bool));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->verticalAxis, sizeof(sf::Joystick::Axis));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->horizontalAxis, sizeof(sf::Joystick::Axis));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->turretRight, sizeof(MyKeys));
		mFileStream.write((char*)&playerData.grid[selectedX][selectedY]->turretLeft, sizeof(MyKeys));
		mFileStream.close();

		mFileStream.open("Settings/TurretControlSchemesList.txt");
		if (!mFileStream.fail())
		{
			std::vector<std::string> temp_schemes;
			std::string temp_str;
			while (!mFileStream.eof())
				getline(mFileStream, temp_str);
			mFileStream.close();

			std::cout << "\nGetline results:" << temp_str;
			extractNamesFromString(temp_str, temp_schemes);


			//Check if the fileName already exists in schemes.txt
			bool temp_alreadyListed = false;
			for (unsigned int i = 0; i < temp_schemes.size(); i++)
				if (temp_schemes[i] == workingFileName)
					temp_alreadyListed = true;
			if (temp_alreadyListed == false)
			{//Save the scheme in the list
				mFileStream.open("Settings/TurretControlSchemesList.txt", std::ios::app);
				if (!mFileStream.fail())
					mFileStream << workingFileName << "#";
				else
					std::cout << "\nFailed to save new scheme to TurretControlSchemes.List.txt";
				mFileStream.close();
			}
		}			
		reloadTurretControlSchemeList();
	}
	else
		std::cout << "\nError. Failed to save turret control scheme:\n" << "Settings/TurretControlSchemes/" << workingFileName << ".dat";

	mFileStream.close();
}

void ShipEditor::extractNamesFromString(std::string& string, std::vector<std::string>& vector)
{
	std::cout << "\n\nBeginning name extraction...";
	if (string.size() > 0)
	{
		int temp_scheme_index = 0;
		vector.push_back("");
		for (unsigned int i = 0; i < string.size(); i++)
		{
			if (string[i] != '#')
				vector[temp_scheme_index] += string[i];
			else if (i < string.size() - 1)
			{
				vector.push_back("");
				temp_scheme_index++;
			}
		}
	}


	//Remove unwanted names
	for (unsigned int i = 0; i < vector.size(); i++)
		if (vector[i].size() < 2)
	{
		std::cout << "\nErasing (size "<< vector[i].size() << "): " << vector[i];
		vector.erase(vector.begin() + i);
		i--;
	}

	std::cout << "\nName extraction complete...";
	std::cout << "\nOriginal string: " << string;
	std::cout << "\nVector: [";
	for (unsigned int i = 0; i < vector.size(); i++)
		if (i == vector.size() - 1)
			std::cout << vector[i];
		else
			std::cout << vector[i] << "/";
	std::cout << "]\n";
	
}

void ShipEditor::loadTurretControlScheme()
{
	if (workingFileName == "" || workingFileName == " " || workingFileName == "  " || workingFileName == "\n" || workingFileName == "\0")
		return;

	mFileStream.open("Settings/TurretControlSchemes/" + workingFileName + ".dat", std::ios::binary | std::ios::in);
	if (mFileStream)
	{
		std::cout << "\nLoading " << workingFileName << ".dat...";
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->turretFire, sizeof(MyKeys));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->holdToFire, sizeof(bool));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->turretReload, sizeof(MyKeys));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->mouseAim, sizeof(bool));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter, sizeof(bool));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->directionalAim, sizeof(bool));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->verticalAxis, sizeof(sf::Joystick::Axis));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->horizontalAxis, sizeof(sf::Joystick::Axis));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->turretRight, sizeof(MyKeys));
		mFileStream.read((char*)&playerData.grid[selectedX][selectedY]->turretLeft, sizeof(MyKeys));
		mFileStream.close();
		updateTurretConfigurationButtonStrings();
		updateTurretConfigurationButtonVisibility();
	}
	else
		std::cout << "\nError. Failed to load turret control scheme";

}


void ShipEditor::writeTurretControlSchemes(std::vector<std::string>& vector)
{
	mFileStream.open("Settings/TurretControlSchemesList.txt", std::ios::out);
	if (!mFileStream.fail())
	{
		mFileStream.clear();
		mFileStream.seekg(0, std::ios::beg);
		for (unsigned int i = 0; i < vector.size(); i++)
		{
			mFileStream << vector[i] << "#";
		}
		//Use the null terminator to purge all the unholy lines
		mFileStream << "###\0";
	}

	mFileStream.close();
}

void ShipEditor::deleteTurretControlScheme()
{
	std::cout << "\nDeleting turret control scheme: [" << workingFileName << "]";
	//Remove the scheme from the scheme list
	std::vector<std::string> temp_schemes;
	mFileStream.open("Settings/TurretControlSchemesList.txt");
	if (!mFileStream.fail())
	{
		std::string temp_str;
		while (!mFileStream.eof())
			mFileStream >> temp_str;
		mFileStream.close();

		extractNamesFromString(temp_str, temp_schemes);

		//Delete fileName from the list
		for (unsigned int i = 0; i < temp_schemes.size(); i++)
		{
			if (temp_schemes[i] == workingFileName)
			{
				temp_schemes.erase(temp_schemes.begin() + i);
				std::cout << "\nDelete complete: " << workingFileName;
			}
		}
	}

	writeTurretControlSchemes(temp_schemes);

	//Remove the scheme's .dat file...
	char temp_charPtr[60] = "Settings/TurretControlSchemes/";
	for (unsigned int i = 0; i < workingFileName.size(); i++)
	{
		temp_charPtr[30 + i] = workingFileName[i];
	}
	temp_charPtr[30 + workingFileName.size()] = '.';
	temp_charPtr[31 + workingFileName.size()] = 'd';
	temp_charPtr[32 + workingFileName.size()] = 'a';
	temp_charPtr[33 + workingFileName.size()] = 't';
	temp_charPtr[34 + workingFileName.size()] = '\0';
	remove(temp_charPtr);

	reloadTurretControlSchemeList();

	std::cout << "\n-End of delete scheme-";
}


void ShipEditor::updateTurretConfigurationButtonStrings()
{
	//Update turret configuration button strings
	for (unsigned int i = 0; i < turretConfigurationButtons.size(); i++)
		switch (turretConfigurationButtons[i].id)
	{
		case bi_confBindFire:
			turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretFire));
			break;
		case bi_confNodeHoldToFire:
			turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->holdToFire));
			break;
		case bi_confBindReload:
			turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretReload));
			break;
		case bi_confNodeMouseAim:
			turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->mouseAim));
			break;
		case bi_confNodeMouseAimRelative:
			turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->mouseAimRelativeToCenter));
			break;
		case bi_confNodeDirectionalAim:
			turretConfigurationButtons[i].text.setString(" " + getBoolAsString(playerData.grid[selectedX][selectedY]->directionalAim));
			break;
		case bi_confBindVerticalAimAxis:
			turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(playerData.grid[selectedX][selectedY]->verticalAxis) + " (" + getPolarityAsString(playerData.grid[selectedX][selectedY]->verticalAxisPolarity) + ")");
			break;
		case bi_confBindHorizontalAimAxis:
			turretConfigurationButtons[i].text.setString(" Joystick " + std::to_string(playerData.grid[selectedX][selectedY]->directionalJoystickId) + "::" + getAxisAsString(playerData.grid[selectedX][selectedY]->horizontalAxis) + " (" + getPolarityAsString(playerData.grid[selectedX][selectedY]->horizontalAxisPolarity) + ")");
			break;
		case bi_confBindTurnTurretRight:
			turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretRight));
			break;
		case bi_confBindTurnTurretLeft:
			turretConfigurationButtons[i].text.setString(getInputAsString(playerData.grid[selectedX][selectedY]->turretLeft));
			break;
		case bi_confSaveTurretScheme:
			turretConfigurationButtons[i].text.setString(" Save control scheme...");
			break;

	}
}

void ShipEditor::reloadTurretControlSchemeList()
{
	turretControlSchemeList.clear();
	turretControlSchemeNameList.clear();

	std::string temp_str;
	std::vector<std::string> temp_schemes;
	mFileStream.open("Settings/TurretControlSchemesList.txt", std::ios::in);
	if (!mFileStream.fail())
	{//Load all scheme names into temp_schemes
		while (!mFileStream.eof())
			mFileStream >> temp_str;
		mFileStream.close();

		extractNamesFromString(temp_str, temp_schemes);
	}
	std::cout << "\nTemp_schemes.size() @ reloadListElements" <<  temp_schemes.size();
	//std::cout << "\ntemp_schemes(reload):\n";
	//for (unsigned int i = 0; i < temp_schemes.size(); i++)
	//	std::cout << temp_schemes[i] << '/';

	if (temp_schemes.size() > 0)
		for (unsigned int i = 0; i < temp_schemes.size(); i++)
		{
		turretControlSchemeList.push_back(Button(bi_confTurretControlScheme, button3X1 + buttonHeight*0.5, CONF_Y1 + buttonHeight * 5 + buttonHeight*i, button1Width - buttonHeight*0.5, buttonHeight, temp_schemes[i], int(33 * resFactor), font1,
		sf::Color(95 + round(i % 2) * 5, 95 + round(i % 2) * 5, 100 + round(i % 2) * 5),
		sf::Color(35, 35, 40)));
		turretControlSchemeNameList.push_back(temp_schemes[i]);
		}

	if (turretControlSchemeList.size() > 0)
		updateTurretControlSchemeList();

	std::cout << "\n\nTurretControlSchemeList elements reload complete\n";
}
void ShipEditor::updateTurretControlSchemeList()
{
	if (scrollDelta != 0)
		scrollDelta = abs(scrollDelta) / scrollDelta;
	
	if (turretControlSchemeList.size() > 8)
	{
		if (scrollDelta > 0 && scrollState < turretControlSchemeList.size() - 8)
			scrollState++;
		else if (scrollDelta < 0 && scrollState > 0)
			scrollState--;
	}
	else
		scrollState = 0;

	for (unsigned int i = 0; i < turretControlSchemeList.size(); i++)
	{
		turretControlSchemeList[i].setPosition(turretControlSchemeList[i].buttonRectangle.getPosition().x, CONF_Y1 + buttonHeight * 5 + buttonHeight*i - buttonHeight*scrollState);
		turretControlSchemeList[i].text.setPosition(turretControlSchemeList[i].buttonRectangle.getPosition().x + button1Width - buttonHeight*0.5 - turretControlSchemeList[i].text.getGlobalBounds().width - 10*zoomFactor, turretControlSchemeList[i].buttonRectangle.getPosition().y);
		if (i < scrollState || i > 7 + scrollState)
			turretControlSchemeList[i].visible = false;
		else
			turretControlSchemeList[i].visible = true;
	}


}
















































char ShipEditor::getUserInput(sf::Event& eventRef)
{
	switch (eventRef.key.code)
	{
	default:
		return '#';
	case sf::Keyboard::Q:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Q';
		else
			return 'q';
	case sf::Keyboard::W:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'W';
		else
			return 'w';
	case sf::Keyboard::E:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'E';
		else
			return 'e';
	case sf::Keyboard::R:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'R';
		else
			return 'r';
	case sf::Keyboard::T:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'T';
		else
			return 't';
	case sf::Keyboard::Y:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Y';
		else
			return 'y';
	case sf::Keyboard::U:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'U';
		else
			return 'u';
	case sf::Keyboard::I:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'I';
		else
			return 'i';
	case sf::Keyboard::O:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'O';
		else
			return 'o';
	case sf::Keyboard::P:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'P';
		else
			return 'p';
	case sf::Keyboard::A:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'A';
		else
			return 'a';
	case sf::Keyboard::S:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'S';
		else
			return 's';
	case sf::Keyboard::D:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'D';
		else
			return 'd';
	case sf::Keyboard::F:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'F';
		else
			return 'f';
	case sf::Keyboard::G:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'G';
		else
			return 'g';
	case sf::Keyboard::H:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'H';
		else
			return 'h';
	case sf::Keyboard::J:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'J';
		else
			return 'j';
	case sf::Keyboard::K:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'K';
		else
			return 'k';
	case sf::Keyboard::L:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'L';
		else
			return 'l';
	case sf::Keyboard::Z:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Z';
		else
			return 'z';
	case sf::Keyboard::X:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'X';
		else
			return 'x';
	case sf::Keyboard::C:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'C';
		else
			return 'c';
	case sf::Keyboard::V:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'V';
		else
			return 'v';
	case sf::Keyboard::B:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'B';
		else
			return 'b';
	case sf::Keyboard::N:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'N';
		else
			return 'n';
	case sf::Keyboard::M:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'M';
		else
			return 'm';
	case sf::Keyboard::Num1:
		return '1';
	case sf::Keyboard::Num2:
		return '2';
	case sf::Keyboard::Num3:
		return '3';
	case sf::Keyboard::Num4:
		return '4';
	case sf::Keyboard::Num5:
		return '5';
	case sf::Keyboard::Num6:
		return '6';
	case sf::Keyboard::Num7:
		return '7';
	case sf::Keyboard::Num8:
		return '8';
	case sf::Keyboard::Num9:
		return '9';
	case sf::Keyboard::Num0:
		return '0';
	case sf::Keyboard::BackSpace:
		return '-';
	//case sf::Keyboard::Space:
	//	return '+';
	case sf::Keyboard::Return:
		return '>';
	case sf::Keyboard::Escape:
		return '<';
	}
}

MyKeys ShipEditor::detectKey(ButtonId targetButton)
{
	MyKeys returnKey;
	sf::Event tEvent;

	while (true)
	{
		//Poll for input
		tEvent.mouseWheel.delta = 0;
		mWindow.pollEvent(tEvent);

		//Keyboard
		returnKey.inputType = keyboardInput;
		for (int i = 0; i < 101; i++)
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
			{
				returnKey.keyCode = sf::Keyboard::Key(i);
				return returnKey;
			}


		//Mouse
		returnKey.inputType = mouseInput;
		if (tEvent.type == sf::Event::MouseWheelMoved)
		{//Check mouseWheel input
			if (tEvent.mouseWheel.delta > 0)
				returnKey.wheelInput = positiveAxis;
			else
				returnKey.wheelInput = negativeAxis;

			return returnKey;
		}
		//Mouse buttons
		for (int i = 0; i < 5; i++)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Button(i)) && i != 0)
			{
				returnKey.mouseButton = sf::Mouse::Button(i);
				return returnKey;
			}

			if (i == 0 && tEvent.type == sf::Event::MouseButtonPressed && tEvent.mouseButton.button == sf::Mouse::Left)
			{
				returnKey.mouseButton = sf::Mouse::Button(i);
				return returnKey;
			}
		}

		//Joystick(s)
		returnKey.inputType = joystickInput;
		returnKey.axisType = noAxis;

		if (tEvent.type == sf::Event::JoystickMoved)
		{
			for (int axis = 0; axis < 8; axis++)
			if (tEvent.joystickMove.axis == sf::Joystick::Axis(axis))
				if (abs(sf::Joystick::getAxisPosition(tEvent.joystickMove.joystickId, sf::Joystick::Axis(axis))) > 50)
				{//Axis input detected
				returnKey.joystickIndex = tEvent.joystickMove.joystickId;
				returnKey.joystickAxis = sf::Joystick::Axis(axis);
				if (sf::Joystick::getAxisPosition(tEvent.joystickMove.joystickId, sf::Joystick::Axis(axis)) > 0)
					returnKey.axisType = positiveAxis;
				else
					returnKey.axisType = negativeAxis;
				return returnKey;
				}
		}


	

		for (int js = 0; js < 8; js++)
		{
			//for (int axis = 0; axis < 8; axis++)
			//	if (mEvent.type == sf::Event::JoystickMoved && mEvent.joystickMove.axis == sf::Joystick::Axis(axis))
			//	if (abs(sf::Joystick::getAxisPosition(js, sf::Joystick::Axis(axis))) > 50)
			//	{//Axis input detected
			//	returnKey.joystickIndex = js;
			//	returnKey.joystickAxis = sf::Joystick::Axis(axis);
			//	if (sf::Joystick::getAxisPosition(js, sf::Joystick::Axis(axis)) > 0)
			//		returnKey.axisType = positiveAxis;
			//	else
			//		returnKey.axisType = negativeAxis;
			//	return returnKey;
			//	}

			for (int button = 0; button < 31; button++)
				if (sf::Joystick::isButtonPressed(js, button))
				{//Joystick button input detected
				returnKey.joystickIndex = js;
				returnKey.joystickButton = button;
				return returnKey;
				}
		}

		
	}
	return returnKey;
}
std::string ShipEditor::getInputAsString(MyKeys input)
{
	std::string str = "  ";
	if (input.inputType == keyboardInput)
	{
		str += "Keyboard::";
		switch (input.keyCode)
		{
		case sf::Keyboard::A:        ///< The A key
			str += "A";break;
		case sf::Keyboard::B:            ///< The B key
			str += "B"; break;
		case sf::Keyboard::C:            ///< The C key
			str += "C"; break;
		case sf::Keyboard::D:            ///< The D key
			str += "D"; break;
		case sf::Keyboard::E:            ///< The E key
			str += "E"; break;
		case sf::Keyboard::F:            ///< The F key
			str += "F"; break;
		case sf::Keyboard::G:            ///< The G key
			str += "G"; break;
		case sf::Keyboard::H:            ///< The H key
			str += "H"; break;
		case sf::Keyboard::I:            ///< The I key
			str += "I"; break;
		case sf::Keyboard::J:           ///< The J key
			str += "J"; break;
		case sf::Keyboard::K:            ///< The K key
			str += "K"; break;
		case sf::Keyboard::L:            ///< The L key
			str += "L"; break;
		case sf::Keyboard::M:            ///< The M key
			str += "M"; break;
		case sf::Keyboard::N:            ///< The N key
			str += "N"; break;
		case sf::Keyboard::O:            ///< The O key
			str += "O"; break;
		case sf::Keyboard::P:            ///< The P key
			str += "P"; break;
		case sf::Keyboard::Q:            ///< The Q key
			str += "Q"; break;
		case sf::Keyboard::R:            ///< The R key
			str += "R"; break;
		case sf::Keyboard::S:            ///< The S key
			str += "S"; break;
		case sf::Keyboard::T:            ///< The T key
			str += "T"; break;
		case sf::Keyboard::U:            ///< The U key
			str += "U"; break;
		case sf::Keyboard::V:            ///< The V key
			str += "V"; break;
		case sf::Keyboard::W:            ///< The W key
			str += "W"; break;
		case sf::Keyboard::X:            ///< The X key
			str += "X"; break;
		case sf::Keyboard::Y:            ///< The Y key
			str += "Y"; break;
		case sf::Keyboard::Z:            ///< The Z key
			str += "Z"; break;
		case sf::Keyboard::Num0:        ///< The 0 key
			str += "0"; break;
		case sf::Keyboard::Num1:         ///< The 1 key
			str += "1"; break;
		case sf::Keyboard::Num2:         ///< The 2 key
			str += "2"; break;
		case sf::Keyboard::Num3:         ///< The 3 key
			str += "3"; break;
		case sf::Keyboard::Num4:         ///< The 4 key
			str += "4"; break;
		case sf::Keyboard::Num5:         ///< The 5 key
			str += "5"; break;
		case sf::Keyboard::Num6:         ///< The 6 key
			str += "6"; break;
		case sf::Keyboard::Num7:         ///< The 7 key
			str += "7"; break;
		case sf::Keyboard::Num8:         ///< The 8 key
			str += "8"; break;
		case sf::Keyboard::Num9:         ///< The 9 key
			str += "9"; break;
		case sf::Keyboard::Escape:       ///< The Escape key
			str += "Escape"; break;
		case sf::Keyboard::LControl:     ///< The left Control key
			str += "Left Control"; break;
		case sf::Keyboard::LShift:       ///< The left Shift key
			str += "Left Shift"; break;
		case sf::Keyboard::LAlt:         ///< The left Alt key
			str += "Left Alt"; break;
		case sf::Keyboard::LSystem:      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
			str += "Left System"; break;
		case sf::Keyboard::RControl:     ///< The right Control key
			str += "Right Control"; break;
		case sf::Keyboard::RShift:       ///< The right Shift key
			str += "Right Shift"; break;
		case sf::Keyboard::RAlt:         ///< The right Alt key
			str += "Right Alt"; break;
		case sf::Keyboard::RSystem:      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
			str += "Right System"; break;
		case sf::Keyboard::Menu:         ///< The Menu key
			str += "Menu"; break;
		case sf::Keyboard::LBracket:     ///< The [ key
			str += "Left Bracket"; break;
		case sf::Keyboard::RBracket:     ///< The ] key
			str += "Right Bracket"; break;
		case sf::Keyboard::SemiColon:    ///< The ; key
			str += ";"; break;
		case sf::Keyboard::Comma:        ///< The , key
			str += ","; break;
		case sf::Keyboard::Period:       ///< The . key
			str += "."; break;
		case sf::Keyboard::Quote:        ///< The ' key
			str += "'"; break;
		case sf::Keyboard::Slash:        ///< The / key
			str += "/"; break;
		case sf::Keyboard::BackSlash:    ///< The \ key
			str += "\\"; break;
		case sf::Keyboard::Tilde:        ///< The ~ key
			str += "~"; break;
		case sf::Keyboard::Equal:        ///< The = key
			str += "="; break;
		case sf::Keyboard::Dash:         ///< The - key
			str += "-"; break;
		case sf::Keyboard::Space:        ///< The Space key
			str += "Space"; break;
		case sf::Keyboard::Return:       ///< The Return key
			str += "Enter"; break;
		case sf::Keyboard::BackSpace:    ///< The Backspace key
			str += "BackSpace"; break;
		case sf::Keyboard::Tab:          ///< The Tabulation key
			str += "Tab"; break;
		case sf::Keyboard::PageUp:       ///< The Page up key
			str += "Page Up"; break;
		case sf::Keyboard::PageDown:     ///< The Page down key
			str += "Page Down"; break;
		case sf::Keyboard::End:          ///< The End key
			str += "End"; break;
		case sf::Keyboard::Home:         ///< The Home key
			str += "Home"; break;
		case sf::Keyboard::Insert:       ///< The Insert key
			str += "Insert"; break;
		case sf::Keyboard::Delete:       ///< The Delete key
			str += "Delete"; break;
		case sf::Keyboard::Add:          ///< The + key
			str += "Add"; break;
		case sf::Keyboard::Subtract:     ///< The - key
			str += "numpad -"; break;
		case sf::Keyboard::Multiply:     ///< The * key
			str += "numpad *"; break;
		case sf::Keyboard::Divide:       ///< The / key
			str += "numpad //"; break;
		case sf::Keyboard::Left:         ///< Left arrow
			str += "Left arrow"; break;
		case sf::Keyboard::Right:        ///< Right arrow
			str += "Right arrow"; break;
		case sf::Keyboard::Up:           ///< Up arrow
			str += "Up arrow"; break;
		case sf::Keyboard::Down:         ///< Down arrow
			str += "Down arrow"; break;
		case sf::Keyboard::Numpad0:      ///< The numpad 0 key
			str += "numpad 0"; break;
		case sf::Keyboard::Numpad1:      ///< The numpad 1 key
			str += "numpad 1"; break;
		case sf::Keyboard::Numpad2:      ///< The numpad 2 key
			str += "numpad 2"; break;
		case sf::Keyboard::Numpad3:      ///< The numpad 3 key
			str += "numpad 3"; break;
		case sf::Keyboard::Numpad4:      ///< The numpad 4 key
			str += "numpad 4"; break;
		case sf::Keyboard::Numpad5:      ///< The numpad 5 key
			str += "numpad 5"; break;
		case sf::Keyboard::Numpad6:      ///< The numpad 6 key
			str += "numpad 6"; break;
		case sf::Keyboard::Numpad7:      ///< The numpad 7 key
			str += "numpad 7"; break;
		case sf::Keyboard::Numpad8:      ///< The numpad 8 key
			str += "numpad 8"; break;
		case sf::Keyboard::Numpad9:      ///< The numpad 9 key
			str += "numpad 9"; break;
		case sf::Keyboard::F1:           ///< The F1 key
			str += "F1"; break;
		case sf::Keyboard::F2:           ///< The F2 key
			str += "F2"; break;
		case sf::Keyboard::F3:           ///< The F3 key
			str += "F3"; break;
		case sf::Keyboard::F4:           ///< The F4 key
			str += "F4"; break;
		case sf::Keyboard::F5:           ///< The F5 key
			str += "F5"; break;
		case sf::Keyboard::F6:           ///< The F6 key
			str += "F6"; break;
		case sf::Keyboard::F7:           ///< The F7 key
			str += "F7"; break;
		case sf::Keyboard::F8:           ///< The F8 key
			str += "F8"; break;
		case sf::Keyboard::F9:           ///< The F9 key
			str += "F9"; break;
		case sf::Keyboard::F10:          ///< The F10 key
			str += "F10"; break;
		case sf::Keyboard::F11:          ///< The F11 key
			str += "F11"; break;
		case sf::Keyboard::F12:          ///< The F12 key
			str += "F12"; break;
		case sf::Keyboard::F13:          ///< The F13 key
			str += "F13"; break;
		case sf::Keyboard::F14:          ///< The F14 key
			str += "F14"; break;
		case sf::Keyboard::F15:          ///< The F15 key
			str += "F15"; break;
		case sf::Keyboard::Pause:        ///< The Pause key
			str += "Pause"; break;
		}

	}
	else if (input.inputType == mouseInput)
	{
		str += "Mouse::";
		switch (input.wheelInput)
		{
		case noAxis:
			switch (input.mouseButton)
			{
			case sf::Mouse::Left:
				str += "Left";
				break;
			case sf::Mouse::Right:
				str += "Right";
				break;
			case sf::Mouse::Middle:
				str += "Middle";
				break;
			case sf::Mouse::XButton1:
				str += "Button 1";
				break;
			case sf::Mouse::XButton2:
				str += "Button 2";
				break;
			}
			break;
		case positiveAxis:
			str += "Wheel Up";
			break;
		case negativeAxis:
			str += "Wheel Down";
			break;
		}
	}
	else if (input.inputType == joystickInput)
	{
		str += "Joystick " + std::to_string(input.joystickIndex) + "::";
		switch (input.axisType)
		{
		case noAxis:
			str += "Button " + std::to_string(input.joystickButton);
			break;
		case positiveAxis:
			switch (input.joystickAxis)
			{
			case sf::Joystick::X:
				str += "X-axis (Positive)";
				break;
			case sf::Joystick::Y:
				str += "Y-axis (Positive)";
				break;
			case sf::Joystick::Z:
				str += "Z-axis (Positive)";
				break;
			case sf::Joystick::R:
				str += "R-axis (Positive)";
				break;
			case sf::Joystick::U:
				str += "U-axis (Positive)";
				break;
			case sf::Joystick::V:
				str += "V-axis (Positive)";
				break;
			case sf::Joystick::PovX:
				str += "PovX-axis (Positive)";
				break;
			case sf::Joystick::PovY:
				str += "PovY-axis (Positive)";
				break;
			}
			break;
		case negativeAxis:
			switch (input.joystickAxis)
			{
			case sf::Joystick::X:
				str += "X-axis (Negative)";
				break;
			case sf::Joystick::Y:
				str += "Y-axis (Negative)";
				break;
			case sf::Joystick::Z:
				str += "Z-axis (Negative)";
				break;
			case sf::Joystick::R:
				str += "R-axis (Negative)";
				break;
			case sf::Joystick::U:
				str += "U-axis (Negative)";
				break;
			case sf::Joystick::V:
				str += "V-axis (Negative)";
				break;
			case sf::Joystick::PovX:
				str += "PovX-axis (Negative)";
				break;
			case sf::Joystick::PovY:
				str += "PovY-axis (Negative)";
				break;
			}
			break;
		}
	}

	if (str == "  ")
		str = "  No valid key bind";
	return str;
}

std::pair< int,sf::Joystick::Axis> ShipEditor::detectAxis()
{
	sf::Event tEvent;

	while (true)
	{
		mWindow.pollEvent(tEvent);
		if (tEvent.type == sf::Event::JoystickMoved)
		{
			for (int axis = 0; axis < 8; axis++)
				if (tEvent.joystickMove.axis == sf::Joystick::Axis(axis))
					if (abs(sf::Joystick::getAxisPosition(tEvent.joystickMove.joystickId, sf::Joystick::Axis(axis))) > 50)
					{//Axis input detected
						if (sf::Joystick::getAxisPosition(tEvent.joystickMove.joystickId, sf::Joystick::Axis(axis)) > 0)
							return std::make_pair(tEvent.joystickMove.joystickId+1, sf::Joystick::Axis(axis));
						else
							return std::make_pair((tEvent.joystickMove.joystickId+1)*-1, sf::Joystick::Axis(axis));
					}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) ||
			(tEvent.type == sf::Event::MouseButtonPressed && tEvent.mouseButton.button == sf::Mouse::Left))
			return std::make_pair(1, sf::Joystick::X);
	}

}

std::string ShipEditor::getAxisAsString(sf::Joystick::Axis axis)
{
	switch (axis)
	{
	case sf::Joystick::X:
		return "X-Axis";
	case sf::Joystick::Y:
		return "Y-Axis";
	case sf::Joystick::Z:
		return "Z-Axis";
	case sf::Joystick::R:
		return "R-Axis";
	case sf::Joystick::U:
		return "U-Axis";
	case sf::Joystick::V:
		return "V-Axis";
	case sf::Joystick::PovX:
		return "PovX-Axis";
	case sf::Joystick::PovY:
		return "PovY-Axis";
	}
}

std::string ShipEditor::getPolarityAsString(float polarity)
{
	if (polarity < 0)
		return "negative";
	else if (polarity > 0)
		return "positive";
	else
		return "polarity error";
}
