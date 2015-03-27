#include "Main.h"
#include "Button.h"
#include "GridData.h"
#include "PlayerData.h"
#include "ShipEditor.h"



ShipEditor::~ShipEditor()
{
}
ShipEditor::ShipEditor(sf::RenderWindow& mw, PlayerData& pd) : playerData(pd), mWindow(mw)
{
	//Misc graphical stuff
	scaleFactor = resFactor*zoomFactor;
	shadeRect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	shadeRect.setFillColor(sf::Color(0, 0, 0, 100));
	configurationRect.setSize(sf::Vector2f(WINDOW_WIDTH - 200 * resFactor, WINDOW_HEIGHT - 150 * resFactor));
	configurationRect.setPosition(100 * resFactor, 75 * resFactor);
	configurationRect.setFillColor(sf::Color(110, 110, 115));


	//Load editor textures
	inheritanceArrowTex.loadFromFile("Texture/inheritanceArrow.png");
	xButtonTex.loadFromFile("Texture/Menu/xButton.png");


	//Action buttons
	font1.loadFromFile("Font/ORANGEKI.ttf");
	actionButtons.push_back(Button(bi_false, 0, 0, 250 * resFactor, 40 * resFactor, "Component name", int(26 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	actionButtons.push_back(Button(bi_actionUpgradeArmor, 0, 0, 250 * resFactor, 40 * resFactor, "Upgrade armor", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionTurret, 0, 0, 250 * resFactor, 40 * resFactor, "Add turret", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionEngine, 0, 0, 250 * resFactor, 40 * resFactor, "Add engine", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionEngine, 0, 0, 250 * resFactor, 40 * resFactor, "Delete", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));
	actionButtons.push_back(Button(bi_actionConfiguration, 0, 0, 250 * resFactor, 40 * resFactor, "Configure...", int(25 * resFactor), font1, sf::Color(110, 110, 115), sf::Color(20, 20, 20)));

	//Configuration buttons
	configurationButtons.push_back(Button(bi_confExit, WINDOW_WIDTH/resFactor - 150, 75, xButtonTex, 2, font1));
	int temp_y1 = int(75 * resFactor);
	int temp_y2 = int(50 * resFactor);

	//Core configuration buttons
	coreConfigurationButtons.push_back(Button(bi_false, 100 * resFactor, temp_y1, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Ship core configurations", int(34 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_false, 150 * resFactor, temp_y1 + temp_y2*2, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Ship controls", int(33 * resFactor), font1, sf::Color(130, 130, 135), sf::Color(35, 35, 40)));
	
	coreConfigurationButtons.push_back(Button(bi_false, 150 * resFactor, temp_y1 + temp_y2 * 3, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Accelerate", int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindAccelerate, WINDOW_WIDTH - 450 * resFactor, temp_y1 + temp_y2 * 3, 350 * resFactor, 50 * resFactor, getInputAsString(coreKeys[key_accelerate]), int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));

	coreConfigurationButtons.push_back(Button(bi_false, 150 * resFactor, temp_y1 + temp_y2 * 4, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Reverse", int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));	
	coreConfigurationButtons.push_back(Button(bi_confBindReverse, WINDOW_WIDTH - 450 * resFactor, temp_y1 + temp_y2 * 4, 350 * resFactor, 50 * resFactor, getInputAsString(coreKeys[key_reverse]), int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	
	coreConfigurationButtons.push_back(Button(bi_false, 150 * resFactor, temp_y1 + temp_y2 * 5, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Turn right", int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindTurnRight, WINDOW_WIDTH - 450 * resFactor, temp_y1 + temp_y2 * 5, 350 * resFactor, 50 * resFactor, getInputAsString(coreKeys[key_turnRight]), int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	
	coreConfigurationButtons.push_back(Button(bi_false, 150 * resFactor, temp_y1 + temp_y2 * 6, WINDOW_WIDTH - 250 * resFactor, 50 * resFactor, " Turn left", int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));
	coreConfigurationButtons.push_back(Button(bi_confBindTurnLeft, WINDOW_WIDTH - 450 * resFactor, temp_y1 + temp_y2 * 6, 350 * resFactor, 50 * resFactor, getInputAsString(coreKeys[key_turnLeft]), int(33 * resFactor), font1, sf::Color(120, 120, 125), sf::Color(35, 35, 40)));

	//Component configuration buttons


}


void ShipEditor::run()
{
	bool keepRunning = true;
	sf::Event mEvent;

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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			keepRunning = false;

		//Movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			cameraY -= 8 / scaleFactor;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			cameraY += 8 / scaleFactor;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			cameraX -= 8 / scaleFactor;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			cameraX += 8 / scaleFactor;

		//Mouse grabbing
		mousePos = sf::Mouse::getPosition(mWindow);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
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
			mouseGrab = false;

		//Handle events
		while (mWindow.pollEvent(mEvent))
		{
			switch (mEvent.type)
			{
			case sf::Event::MouseWheelMoved:
				zoom(mEvent.mouseWheel.delta);
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
				switch (mEvent.key.code)
				{
				case sf::Keyboard::Delete:
					scrapComponent(selectedX, selectedY);
					updateGridSpriteTextures();
					focus = editor::base;
					break;
				case sf::Keyboard::Escape:
					selectedX = -1;
					selectedY = -1;
					focus = editor::base;
					break;
				}
			}
		}

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

		mWindow.display();
	}


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
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTexture(skeletonTex);
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTextureRect(sf::IntRect(1400, 0, 100, 100));
			}

			//Add turret sprite
			if (playerData.grid[x][y]->turret > 0)
			{
				gridSprites[x][y].push_back(sf::Sprite());
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTexture(editorTurretTex);
				gridSprites[x][y][gridSprites[x][y].size() - 1].setOrigin(-30,0);
			}
			
			//Add engine sprite

			//Add inheritance arrows
			if (playerData.grid[x][y]->childUp == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(inheritanceArrowTex);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(-46, 50);
			}
			if (playerData.grid[x][y]->childDown == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(inheritanceArrowTex);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(59, 150);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setRotation(180);
			}
			if (playerData.grid[x][y]->childLeft == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(inheritanceArrowTex);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setOrigin(57, 50);
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setRotation(270);
			}
			if (playerData.grid[x][y]->childRight == true)
			{
				inheritanceSprites[x][y].push_back(sf::Sprite());
				inheritanceSprites[x][y][inheritanceSprites[x][y].size() - 1].setTexture(inheritanceArrowTex);
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
			gridSprites[x][y][i].setPosition((x * 100 - cameraX)*scaleFactor, (y * 100 - cameraY)*scaleFactor);
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

	cameraX = grabCameraOriginX + (mouseGrabX - mousePos.x) / scaleFactor;
	cameraY = grabCameraOriginY + (mouseGrabY - mousePos.y) / scaleFactor;
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
			break;
		}

		if (playerData.grid[selectedX][selectedY]->armor > 0)
			focus = editor::component;
		else
			focus = editor::base;
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
				break;
			}

			selectedX = checkX;
			selectedY = checkY;
			if (playerData.grid[selectedX][selectedY]->armor < 1)
				focus = editor::base;
		}
		else
		{
			selectedX = -1;
			selectedY = -1;
			focus = editor::base;
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
			case bi_false:
				focus = editor::base;
				break;
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

				focus = editor::base;
				break;
			case bi_actionConfiguration:

				focus = editor::configuration;
				break;

			}
		

		break;
	case editor::configuration:
		//Common configuration actions
		for (unsigned int i = 0; i < configurationButtons.size(); i++)
			switch (configurationButtons[i].checkIfPressed(mousePos))
		{
			case bi_confExit:
				focus = editor::base;
				break;

		}




		//Core configuration specific:
		if (playerData.grid[selectedX][selectedY]->core == true)
			for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
				switch (coreConfigurationButtons[i].checkIfPressed(mousePos))
			{
				case bi_confBindAccelerate:
					coreKeys[key_accelerate] = detectKey(bi_confBindAccelerate);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_accelerate]));
					break;
				case bi_confBindReverse:
					coreKeys[key_reverse] = detectKey(bi_confBindReverse);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_reverse]));
					break;
				case bi_confBindTurnRight:
					coreKeys[key_turnRight] = detectKey(bi_confBindTurnRight);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_turnRight]));
					break;
				case bi_confBindTurnLeft:
					coreKeys[key_turnLeft] = detectKey(bi_confBindTurnLeft);
					coreConfigurationButtons[i].text.setString(getInputAsString(coreKeys[key_turnLeft]));
					break;

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
			focus = editor::actions;
			for (unsigned int i = 0; i < actionButtons.size(); i++)
			{
				actionButtons[i].buttonRectangle.setPosition(mousePos.x, mousePos.y + 30 * i * resFactor);
				actionButtons[i].text.setPosition(mousePos.x + 10 * resFactor, mousePos.y + 30 * i * resFactor);
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
	if (focus != editor::component)
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
	for (unsigned int i = 0; i < actionButtons.size(); i++)
	{
		actionButtons[0].text.setString("Component [" + std::to_string(selectedX) + "][" + std::to_string(selectedY) + "]");
		actionButtons[i].draw(mWindow, mousePos);
	}
}
void ShipEditor::drawConfigurations()
{
	mWindow.draw(configurationRect);
	for (unsigned int i = 0; i < configurationButtons.size(); i++)
	{
		configurationButtons[i].draw(mWindow, mousePos);
	}

	if (playerData.grid[selectedX][selectedY]->core == true)
	{
		for (unsigned int i = 0; i < coreConfigurationButtons.size(); i++)
		{
			coreConfigurationButtons[i].draw(mWindow, mousePos);
		}
	}
	else
	{
		for (unsigned int i = 0; i < componentConfigurationButtons.size(); i++)
		{
			componentConfigurationButtons[i].draw(mWindow, mousePos);
		}
	}
}



std::string ShipEditor::getInputAsString(MyKeys input)
{
	std::string str = "";
	if (input.inputType == keyboardInput)
	{
		str = "Keyboard::";
		switch (input.keyCode)
		{
		case sf::Keyboard::Num0:
			str += "0";
			break;
		case sf::Keyboard::Num1:
			str += "1";
			break;
		case sf::Keyboard::Num2:
			str += "2";
			break;
		case sf::Keyboard::Num3:
			str += "3";
			break;
		case sf::Keyboard::Num4:
			str += "4";
			break;
		case sf::Keyboard::Num5:
			str += "5";
			break;
		case sf::Keyboard::Num6:
			str += "6";
			break;
		case sf::Keyboard::Num7:
			str += "7";
			break;
		case sf::Keyboard::Num8:
			str += "8";
			break;
		case sf::Keyboard::Num9:
			str += "9";
			break;
		}

	}
	else if (input.inputType == mouseInput)
	{
		str = "Mouse::";
	}
	else if (input.inputType == joystickInput)
	{
		str = "Joystick";
	}
	return str;
}
MyKeys ShipEditor::detectKey(ButtonId targetButton)
{
	MyKeys returnKey;
	while (true)
	{
		//Keyboard
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num0;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num1;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num2;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num3;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num4;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num5;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num6;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num7;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num8;
			return returnKey;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
		{
			returnKey.inputType = keyboardInput;
			returnKey.keyCode = sf::Keyboard::Num9;
			return returnKey;
		}

		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))		return editor::Q;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))		return editor::W;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))		return editor::E;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))		return editor::R;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))		return editor::T;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))		return editor::Y;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))		return editor::U;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))		return editor::I;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))		return editor::O;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))		return editor::P;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))		return editor::A;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))		return editor::S;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))		return editor::D;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))		return editor::F;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))		return editor::G;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))		return editor::H;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))		return editor::J;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))		return editor::K;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))		return editor::L;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))		return editor::Z;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))		return editor::X;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))		return editor::C;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))		return editor::V;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))		return editor::B;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))		return editor::N;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))		return editor::M;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))		return editor::Comma;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))		return editor::Period;
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash))		return editor::Dash;
	}

		/*
		§, Tab, CapsLock, LShift, LControl, Alt, Space, AltGr, RControl, RShift, Enter, Backspace, ´,
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


		*/
	return returnKey;
}