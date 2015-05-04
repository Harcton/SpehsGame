#include "Main.h"
#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "Sentinel.h"
#include "Seeker.h"
#include "Commander.h"
#include "Sentry.h"



Game::Game(sf::RenderWindow& w) : mWindow(w)
{
	elements.push_back(sf::Sprite());
	elements[0].setTexture(RM.getTexture("ball.png"));
	elements[0].setOrigin(10, 10);
	elements.push_back(sf::Sprite());
	elements[1].setTexture(RM.getTexture("pointer_arrow.png"));
	elements[1].setOrigin(10, 10);

	font.loadFromFile("Font/ORANGEKI.ttf");
	distanceText.setFont(font);
	distanceText.setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25 + 30);
	elements[0].setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25);
	elements[1].setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25); //playerObj->screenX, playerObj->screenY

	//Esc menu
	escMenuShade.setSize(sf::Vector2f(mWindow.getSize().x, mWindow.getSize().y));
	escMenuShade.setFillColor(sf::Color(0, 0, 0, 0));
	int buttonWidth = 400 * resFactor;
	int buttonHeight = 75 * resFactor;
	int buttonBorder = int(0.25f*buttonHeight);
	int buttonX1 = int(WINDOW_WIDTH / 2.0f) - int(buttonWidth/2.0f);
	int buttonY1 = int(WINDOW_HEIGHT / 2.0f) - 2 * buttonHeight;
	sf::Color buttonColorBG(20, 45, 55, 160);
	sf::Color buttonColorText(200, 210, 250, 220);
	escMenuButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - buttonBorder, buttonWidth + buttonBorder * 2, buttonHeight * 4 + buttonBorder * 2, "", int(50 * resFactor), &font, sf::Color(15, 20, 25, 220), buttonColorText));
	escMenuButtons.push_back(Button(bi_gsetReturn, buttonX1, buttonY1, buttonWidth, buttonHeight, "Resume", int(50 * resFactor), &font, buttonColorBG, buttonColorText));
	escMenuButtons.back().setTextAlign(ta_center);
	escMenuSliders.push_back(SliderButton(bi_gsetMusicVolume, buttonX1, buttonY1 + buttonHeight, buttonWidth, buttonHeight, "Music", int(50 * resFactor), &font, buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &MUSIC_VOLUME));
	escMenuSliders.back().sliderState = MUSIC_VOLUME;
	escMenuSliders.back().setTextAlign(ta_center);
	escMenuSliders.push_back(SliderButton(bi_gsetSoundVolume, buttonX1, buttonY1 + buttonHeight*2, buttonWidth, buttonHeight, "Sound effects", int(50 * resFactor), &font, buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &SFX_VOLUME));
	escMenuSliders.back().sliderState = SFX_VOLUME;
	escMenuSliders.back().setTextAlign(ta_center);
	escMenuButtons.push_back(Button(bi_gsetQuit, buttonX1, buttonY1 + 3 * buttonHeight, buttonWidth, buttonHeight, "Quit to menu", int(50 * resFactor), &font, buttonColorBG, buttonColorText));
	escMenuButtons.back().setTextAlign(ta_center);
}


Game::~Game()
{
	while (!objects.empty())
	{
		delete objects.back(); objects.pop_back();
	}

	while (!backgrounds.empty())
	{
		delete backgrounds.back(); backgrounds.pop_back();
	}
}


void Game::run()
{
	

	//Filling objects vector with something
	objects.push_back(new Player(mWindow, this, 0, 0));
	playerObj = objects[0];


	/*
	for (unsigned int i = 0; i < 20; i++)
		objects.push_back(new Debris(mWindow, this, objects));
	
	for (int i = 0; i < 1; i++)//Creating enemies
		objects.push_back(new Enemy(mWindow, this, objects));*/

	//Creating background objects
	for (int i = 0; i < 10; i++)
		backgrounds.push_back(new Background(mWindow, this, backgrounds));

	



	while (keepRunning)
	{
		//DEBUG
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			keepRunning = false;

		pollEvents();

		//DRAWING
		mWindow.clear(sf::Color(0, 0, 0, 100));

		//If the game "has focus", update and run the game normally
		if (focus == gf_game)
		{
			updateBackgrounds();
			updateObjects();
		}


		for (unsigned int i = 0; i < objects.size(); i++)
			if (objects[i]->hasCollisionChecks == true)
				objects[i]->checkCollisions(i);


		//Draw all graphical entities regardless of game focus
		//Game objects
		for (unsigned int i = 0; i < backgrounds.size(); i++)
			backgrounds[i]->draw();
		for (unsigned int i = 0; i < objects.size(); i++)
			objects[i]->draw();
		//GUI / buttons
		for (unsigned int i = 0; i < elements.size(); i++)
			mWindow.draw(elements[i]);
		mWindow.draw(distanceText);
		if (focus == gf_escMenu)
			drawEscMenu();
		



		//Display the window
		mWindow.display();
		//////////////////

	}
}

void Game::pollEvents()
{
	while (mWindow.pollEvent(mEvent))
		switch (mEvent.type)
	{
		default:
			break;
		case sf::Event::Closed:
			keepRunning = false;
			break;
		case sf::Event::MouseButtonPressed:
			if (focus == gf_escMenu)
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					mouseLeftPressed();
			break;
		case sf::Event::KeyPressed:
			switch (mEvent.key.code)
			{
			case sf::Keyboard::Escape:
				if (focus == gf_game)
					focus = gf_escMenu;
				else if (focus == gf_escMenu)
				{
					escMenuShade.setFillColor(sf::Color(0, 0, 0, 0));
					focus = gf_game;
				}
				break;

			}
			break;
	}
}

void Game::drawEscMenu()
{
	//update mouse position
	mousePos = sf::Mouse::getPosition(mWindow);

	//Update & draw shade
	if (escMenuShade.getFillColor().a < 160)
		escMenuShade.setFillColor(sf::Color(0, 0, 0, escMenuShade.getFillColor().a + 5));
	mWindow.draw(escMenuShade);

	//Draw buttons
	for (unsigned int i = 0; i < escMenuButtons.size(); i++)
		escMenuButtons[i].draw(mWindow, mousePos);
	for (unsigned int i = 0; i < escMenuSliders.size(); i++)
		escMenuSliders[i].draw(mWindow, mousePos);
}
void Game::mouseLeftPressed()
{
	for (unsigned int i = 0; i < escMenuButtons.size(); i++)
		switch (escMenuButtons[i].checkIfPressed(mousePos))
	{
		case bi_gsetReturn:
			focus = gf_game;
			saveSettings();
			break;
		case bi_gsetQuit:
			keepRunning = false;
			saveSettings();
			break;
	}

	//Sliders
	for (unsigned int i = 0; i < escMenuSliders.size(); i++)
		switch (escMenuSliders[i].checkIfPressed(mousePos))
	{//The function of these sliders is mostly automated using this function (checkIfPresssd).
		case bi_gsetMusicVolume:
			break;
		case bi_gsetSoundVolume:
			break;
	}
}


void Game::updateBackgrounds()
{
	//Background update
	for (int i = 0; i < backgrounds.size(); i++)
		if (backgrounds[i]->update() == false)
		{
			Object* temp_objPtr = backgrounds[i];
			backgrounds.erase(backgrounds.begin() + i);
			delete temp_objPtr;
			i--;
		}
	if (backgrounds.size() < 7)
	{
		backgrounds.push_back(new Background(mWindow, this, backgrounds));
		backgrounds[backgrounds.size() - 1]->update();
	}
}


void Game::updateObjects()
{
	//Object update
	for (int i = 0; i < objects.size(); i++)
		if (objects[i]->update() == false)
		{
			Object* temp_objPtr = objects[i];
			objects.erase(objects.begin() + i);
			if (temp_objPtr == playerObj)
				playerObj = nullptr;
			delete temp_objPtr;
			i--;
		}




	//THE DEMO VERSION = 0
	//UNIT TESTING = 1
	////////////////////////////////////
	bool tempDevelopmentSelection = 0;//
	////////////////////////////////////
	if (tempDevelopmentSelection)
	{
		if (objects.size() < 2)
		{
			objects.push_back(new Sentinel(mWindow, this, 1)); //spawn different enemy types
			objects[objects.size() - 1]->setRandomLocation();
		}
	}
	else
		demo();
	
}


void Game::demo()
{
	distanceFromStart = getDistance(0, 0, playerObj->x, playerObj->y);

	if (objects.size() < distanceFromStart / 1300)
	{
		if (distanceFromStart / 1000 > 8)
			spawnRandomization = irandom(1, 4);
		else
			spawnRandomization = irandom(1, 3);

		if (spawnRandomization == 1)
		{
			objects.push_back(new Sentinel(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 2)
		{
			objects.push_back(new Seeker(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 3)
		{
			objects.push_back(new Sentry(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 4)
		{
			objects.push_back(new Commander(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
			objects.back()->update();
		}
	}

	distanceText.setString(intToString(distanceFromStart));
	double temp_angle = atan2(playerObj->y, playerObj->x);
	if (temp_angle < 0)
		temp_angle = abs(temp_angle);
	else
		temp_angle = PI * 2 - temp_angle;
	elements[1].setRotation(180 - (180 / PI)*temp_angle);


}


std::string Game::intToString(int a)
{
	std::stringstream returnString;
	returnString << a;
	return returnString.str();
}

void Game::reloadEscMenuButtonStrings()
{
	for (unsigned int i = 0; i < escMenuSliders.size(); i++)
		switch (escMenuSliders[i].id)
	{
		case bi_gsetMusicVolume:
			break;
	}
}
