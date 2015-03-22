#include "Main.h"
#include "GridData.h"
#include "PlayerData.h"
#include "ShipEditor.h"



ShipEditor::~ShipEditor()
{
}
ShipEditor::ShipEditor(sf::RenderWindow& mw, PlayerData& pd) : playerData(pd), mWindow(mw)
{
}


void ShipEditor::run()
{
	bool keepRunning = true;
	sf::Event mEvent;
	scaleFactor = resFactor*zoomFactor;
	inheritanceArrowTex.loadFromFile("Texture/inheritanceArrow.png");
	shadeRect.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	shadeRect.setFillColor(sf::Color(0, 0, 0, 100));

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
		mWindow.pollEvent(mEvent);
		switch (mEvent.type)
		{
		case sf::Event::MouseWheelMoved:
			zoom(mEvent.mouseWheel.delta);
			mEvent.mouseWheel.delta = 0;
			scaleFactor = resFactor*zoomFactor;
			break;
		case sf::Event::MouseButtonPressed:
			switch (mEvent.mouseButton.button)
			{
			case sf::Mouse::Left:
			mouseLeftPressed();
				break;
			}
			break;
		case sf::Event::KeyPressed:
			switch (mEvent.key.code)
			{
			case sf::Keyboard::Delete:
				scrapComponent(selectedX, selectedY);
				updateGridSpriteTextures();
				focus = ef_base;
				break;
			case sf::Keyboard::Escape:
				selectedX = -1;
				selectedY = -1;
				focus = ef_base;
				break;
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
	cameraX = grabCameraOriginX + (mouseGrabX - mousePos.x) / scaleFactor;
	cameraY = grabCameraOriginY + (mouseGrabY - mousePos.y) / scaleFactor;
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

void ShipEditor::rotateGlowAngle()
{
	glowAngle += PI / 60;
	if (glowAngle > PI * 2)
		glowAngle = 0;
	glowAmount = 0.5 + sin(glowAngle)/2;
}

void ShipEditor::mouseLeftPressed()
{
	switch (focus)
	{
	case ef_base:
		selectedX = int((cameraX + mousePos.x / scaleFactor) / (100.0));
		selectedY = int((cameraY + mousePos.y / scaleFactor) / (100.0));
		if (playerData.grid[selectedX][selectedY]->armor > 0)
			focus = ef_component;
		else
			focus = ef_base;
		break;
	case ef_component:
		int checkX = int((cameraX + mousePos.x / scaleFactor) / (100.0));
		int checkY = int((cameraY + mousePos.y / scaleFactor) / (100.0));
		
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
		if (checkX == selectedX && checkY == selectedY + 1 && checkY < EDITOR_HEIGHT && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childDown = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		//Left
		if (checkX == selectedX - 1 && checkY == selectedY && checkX >= 0 && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childLeft = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
		}
		//Right
		if (checkX == selectedX + 1 && checkY == selectedY && checkX < EDITOR_WIDTH && playerData.grid[checkX][checkY]->armor == 0)
		{
			playerData.grid[selectedX][selectedY]->childRight = true;
			playerData.grid[checkX][checkY]->armor = 1;
			selectedX = checkX;
			selectedY = checkY;
			updateGridSpriteTextures();
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
	if (focus != ef_component)
		return;

	mWindow.draw(shadeRect);
	for (unsigned int i = 0; i < gridSprites[selectedX][selectedY].size(); i++)
		mWindow.draw(gridSprites[selectedX][selectedY][i]);

}