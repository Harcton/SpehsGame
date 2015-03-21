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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			keepRunning = false;

		//Zoom
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
			zoomFactor += 0.004;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
			zoomFactor -= 0.004;

		//Movement
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			cameraY -= 8 / (resFactor*zoomFactor);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			cameraY += 8 / (resFactor*zoomFactor);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			cameraX -= 8 / (resFactor*zoomFactor);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			cameraX += 8 / (resFactor*zoomFactor);

		//Mouse grabbing
		mousePos = sf::Mouse::getPosition();
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (mouseGrab == false)
			{
				mousePos = sf::Mouse::getPosition();
				mouseGrabX = mousePos.x;
				mouseGrabY = mousePos.y;
				grabCameraOriginX = cameraX;
				grabCameraOriginY = cameraY;
			}
			
			mouseGrab = true;

		}
		else
			mouseGrab = false;

		mWindow.pollEvent(mEvent);
		switch (mEvent.type)
		{
		case sf::Event::MouseWheelMoved:
			zoomFactor += 0.1*mEvent.mouseWheel.delta;
			mEvent.mouseWheel.delta = 0;
			limitZoom();
			break;
		}
		

		//Drawing
		mWindow.clear(sf::Color(40, 90, 235));

		//Update camera position when mouseGrabbing
		if (mouseGrab == true)
			updateMouseGrab();

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


		mWindow.display();
	}


}


void ShipEditor::updateLines()
{
	float factor = resFactor*zoomFactor;

	for (unsigned int i = 0; i < EDITOR_WIDTH + 1; i++)
	{
		horizontalLines[i][0].position = sf::Vector2f(0, (i*100 - cameraY)*factor);
		horizontalLines[i][1].position = sf::Vector2f(WINDOW_WIDTH, (i * 100 - cameraY)*factor);
	}
	for (unsigned int i = 0; i < EDITOR_HEIGHT + 1; i++)
	{
		verticalLines[i][0].position = sf::Vector2f((i*100 - cameraX)*factor, 0);
		verticalLines[i][1].position = sf::Vector2f((i * 100 - cameraX)*factor, WINDOW_HEIGHT);
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

			//Add skeleton sprite
			if (playerData.grid[x][y]->armor > 0)
			{
				gridSprites[x][y].push_back(sf::Sprite());
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTexture(skeletonTex);
				gridSprites[x][y][gridSprites[x][y].size() - 1].setTextureRect(sf::IntRect(1400, 0, 100, 100));
			}

			//Add turret sprite
			
			//Add engine sprite

		}
}
void ShipEditor::updateGridSpriteLocations()
{
	float factor = resFactor*zoomFactor;

	for (unsigned int x = 0; x < EDITOR_WIDTH; x++)
		for (unsigned int y = 0; y < EDITOR_HEIGHT; y++)
			for (unsigned int i = 0; i < gridSprites[x][y].size(); i++)
			{
				gridSprites[x][y][i].setPosition((x * 100 - cameraX)*factor, (y * 100 - cameraY)*factor);
				gridSprites[x][y][i].setScale(factor, factor);
				if (playerData.grid[x][y]->core == true)
					gridSprites[x][y][i].setColor(sf::Color(240, 40, 180));

			}
}

void ShipEditor::updateMouseGrab()
{
	cameraX = grabCameraOriginX + (mouseGrabX - mousePos.x) / (zoomFactor*resFactor);
	cameraY = grabCameraOriginY + (mouseGrabY - mousePos.y) / (zoomFactor*resFactor);
}