#include "Main.h"
#include "Game.h"
#include "Player.h"
#include "Background.h"
#include "Sentinel.h"
#include "Seeker.h"
#include "Commander.h"
#include "Sentry.h"



Game::~Game()
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		for (unsigned int c = 0; c < objects[i]->components.size(); c++)
			objects[i]->components[c]->destructorMode = component::quit;
		delete objects[i];
	}
	for (unsigned int i = 0; i < bullets.size(); i++)
		delete bullets[i];
	for (unsigned int i = 0; i < backgrounds.size(); i++)
		delete backgrounds[i];
}
Game::Game(sf::RenderWindow& w) : mWindow(w)
{
	objects.reserve(MAX_OBJECTS);
	bullets.reserve(MAX_BULLETS);
	backgrounds.reserve(MAX_BACKGROUNDS);

	elements.push_back(sf::Sprite());
	elements[0].setTexture(RM.ball1Tex);
	elements[0].setOrigin(10, 10);
	elements.push_back(sf::Sprite());
	elements[1].setTexture(RM.pointerArrowTex);
	elements[1].setOrigin(10, 10);

	balanceText.setFont(RM.menuFont);
	balanceText.setString("Metal: ");
	balanceText.setColor(sf::Color(120, 170, 255, 180));
	balanceText.setCharacterSize(50 * resFactor);
	balanceText.setPosition(int(10 * resFactor), int(WINDOW_HEIGHT - balanceText.getGlobalBounds().height * 2));

	distanceText.setFont(RM.menuFont);
	distanceText.setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25 + 30);
	elements[0].setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25);
	elements[1].setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25); //playerObj->screenX, playerObj->screenY

	//Station
	stationSpr.setTexture(RM.stationTex);
	stationSpr.setOrigin(450, 450);
	stationArrow.setTexture(RM.pointerArrowTex);
	stationArrow.setColor(sf::Color(240, 190, 100));
	stationArrow.setOrigin(0, 10);
	stationArrow.setPosition(WINDOW_WIDTH - WINDOW_WIDTH / 15, WINDOW_HEIGHT - WINDOW_HEIGHT / 1.25);
	pressEnterToDockSpr.setTexture(RM.pressEnterToDockTex);
	pressEnterToDockSpr.setOrigin(100, 100);
	pressEnterToDockSpr.setColor(sf::Color(255, 255, 255, 220));
	pressEnterToDockSpr.setScale(resFactor, resFactor);
	pressEnterToDockSpr.setPosition(WINDOW_WIDTH/2, WINDOW_HEIGHT - (WINDOW_HEIGHT/40));

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
	escMenuButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - buttonBorder, buttonWidth + buttonBorder * 2, buttonHeight * 4 + buttonBorder * 2, "", int(50 * resFactor), sf::Color(15, 20, 25, 220), buttonColorText));
	escMenuButtons.push_back(Button(bi_gsetReturn, buttonX1, buttonY1, buttonWidth, buttonHeight, "Resume", int(50 * resFactor), buttonColorBG, buttonColorText));
	escMenuButtons.back().setTextAlign(ta_center);
	escMenuSliders.push_back(SliderButton(bi_gsetMusicVolume, buttonX1, buttonY1 + buttonHeight, buttonWidth, buttonHeight, "Music", int(50 * resFactor), buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &MUSIC_VOLUME));
	escMenuSliders.back().sliderState = MUSIC_VOLUME;
	escMenuSliders.back().setTextAlign(ta_center);
	escMenuSliders.push_back(SliderButton(bi_gsetSoundVolume, buttonX1, buttonY1 + buttonHeight*2, buttonWidth, buttonHeight, "Sound effects", int(50 * resFactor), buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &SFX_VOLUME));
	escMenuSliders.back().sliderState = SFX_VOLUME;
	escMenuSliders.back().setTextAlign(ta_center);
	escMenuButtons.push_back(Button(bi_gsetQuit, buttonX1, buttonY1 + 3 * buttonHeight, buttonWidth, buttonHeight, "Quit to menu", int(50 * resFactor), buttonColorBG, buttonColorText));
	escMenuButtons.back().setTextAlign(ta_center);
	
	spehsMusic.openFromFile("Audio/Music and Ambience/spehs_ambience_ver00.wav");
	spehsMusic.play();
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
	for (int i = 0; i < MAX_BACKGROUNDS; i++)
	{
		backgrounds.push_back(new Background(mWindow, this, backgrounds));
		backgrounds.back()->update();
	}

	



	while (keepRunning)
	{
		pollEvents();

		//DRAWING
		mWindow.clear(sf::Color(0, 0, 0, 100));

		//If the game "has focus", update and run the game normally
		if (focus == gf_game)
		{
			updateBackgrounds();
			updateObjects();
			updateBullets();
			updateStation();
			updateElements();
		}


		for (unsigned int i = 0; i < objects.size(); i++)
			if (objects[i]->hasCollisionChecks == true)
				objects[i]->checkCollisions(i);


		//Draw all graphical entities regardless of game focus
		//Game objects
		for (unsigned int i = 0; i < backgrounds.size(); i++)
			backgrounds[i]->draw();
		mWindow.draw(stationSpr);
		for (unsigned int i = 0; i < objects.size(); i++)
			objects[i]->draw();
		//Engine flames
		for (unsigned int i = 0; i < playerObj->components.size(); i++)
			playerObj->components[i]->drawEngineFlame();
		//GUI / buttons
		mWindow.draw(stationArrow);
		for (unsigned int i = 0; i < elements.size(); i++)
			mWindow.draw(elements[i]);
		mWindow.draw(distanceText);
		balanceText.setString("Metal: " + std::to_string(playerObj->dataPtr->money));
		mWindow.draw(balanceText);
		if (ableToDock)
			mWindow.draw(pressEnterToDockSpr);
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
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					mouseLeftPressed();
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
					mouseRightPressed();
			}
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
			case sf::Keyboard::Return:
				if (ableToDock)
					playerObj->editShip();
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
void Game::mouseRightPressed()
{
	for (unsigned int i = 0; i < escMenuSliders.size(); i++)
		switch (escMenuSliders[i].checkIfPressed(mousePos))
		{
			case bi_gsetSoundVolume:
			case bi_gsetMusicVolume:
				if (escMenuSliders[i].sliderState == 0)
					escMenuSliders[i].sliderState = 100;
				else if (escMenuSliders[i].sliderState == 100)
					escMenuSliders[i].sliderState = 0;
				else if (escMenuSliders[i].sliderState < 50)
					escMenuSliders[i].sliderState = 0;
				else
					escMenuSliders[i].sliderState = 100;
				*escMenuSliders[i].targetVariable = escMenuSliders[i].sliderState;
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
	if (backgrounds.size() < MAX_BACKGROUNDS)
	{
		backgrounds.push_back(new Background(mWindow, this, backgrounds));
		backgrounds.back()->update();
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
			if (objects.size() < MAX_OBJECTS)
				objects.push_back(new Commander(mWindow, this, 2)); //spawn different enemy types
			objects.back()->setRandomLocation();
			objects.back()->update();
		}
	}
	else
		demo();
	
}

void Game::updateBullets()
{
	//Bullet update
	for (bulletIt = bullets.begin(); bulletIt != bullets.end();)
		if ((*bulletIt)->update() == false)
		{
		delete (*bulletIt);
		bulletIt = bullets.erase(bulletIt);
		}
		else
		{
			++bulletIt;
		}

	//Draw bullets
	for (unsigned int i = 0; i < bullets.size(); i++)
		bullets[i]->draw();
}

void Game::demo()
{
	distanceFromStart = getDistance(0, 0, playerObj->x, playerObj->y);
	distanceFromStation = getDistance(nearestStationX, nearestStationY, playerObj->x, playerObj->y);
	if (distanceFromStart < 50000)
		enemyBehaviourDifficulty = 1;
	else if (distanceFromStart < 100000)
		enemyBehaviourDifficulty = 2;
	else
		enemyBehaviourDifficulty = 3;

	if (objects.size() < distanceFromStation / 1300)
	{
		if (distanceFromStart / 1000 > 50)
			spawnRandomization = irandom(1, 4);
		else
			spawnRandomization = irandom(1, 3);

		if (spawnRandomization == 1 && objects.size() < MAX_OBJECTS)
		{
			objects.push_back(new Sentinel(mWindow, this, enemyBehaviourDifficulty));
			objects.back()->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 2 && objects.size() < MAX_OBJECTS)
		{
			objects.push_back(new Seeker(mWindow, this, enemyBehaviourDifficulty));
			objects.back()->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 3 && objects.size() < MAX_OBJECTS)
		{
			objects.push_back(new Sentry(mWindow, this, enemyBehaviourDifficulty));
			objects.back()->setRandomLocation();
			objects.back()->update();
		}
		if (spawnRandomization == 4 && objects.size() < MAX_OBJECTS)
		{
			objects.push_back(new Commander(mWindow, this, enemyBehaviourDifficulty));
			objects.back()->setRandomLocation();
			objects.back()->update();
		}
	}
}

void Game::updateElements()
{
	//Distance meter TO DO: rounded 2-4 decimal part
	char distanceSuffix = ' ';
	int convertedDistance = distanceFromStart;
	if (distanceFromStart > 1000000000000)
	{
		convertedDistance = distanceFromStart / 1000000000000.0f;
		distanceSuffix = 'T';
	}
	else if (distanceFromStart > 1000000000)
	{
		convertedDistance = distanceFromStart / 1000000000.0f;
		distanceSuffix = 'G';
	}
	else if (distanceFromStart > 1000000)
	{
		convertedDistance = distanceFromStart / 1000000.0f;
		distanceSuffix = 'M';
	}
	else if (distanceFromStart > 1000)
	{
		convertedDistance = distanceFromStart / 1000.0f;
		distanceSuffix = 'k';
	}
	distanceText.setString(std::to_string(convertedDistance) + " " + distanceSuffix + "m");
	//distanceText.setString(intToString(distanceFromStart));
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


void Game::updateStation()
{
	//Update station coordinates
	nearestStationX = round(playerObj->x / STATION_INTERVAL)*STATION_INTERVAL;
	nearestStationY = round(playerObj->y / STATION_INTERVAL)*STATION_INTERVAL;

	//Check if player is in range and if enter key is pressed
	if (getDistance(playerObj->x, playerObj->y, nearestStationX, nearestStationY) < 450)
		ableToDock = true;
	else
		ableToDock = false;

	stationSpr.setRotation(stationSpr.getRotation() + 0.1);
	stationSpr.setScale(resFactor*zoomFactor, resFactor*zoomFactor);
	stationSpr.setPosition(playerObj->screenX + (nearestStationX - playerObj->x)*resFactor*zoomFactor, playerObj->screenY + (nearestStationY - playerObj->y)*resFactor*zoomFactor);

	float temp_stationDirection = -1 * atan2(playerObj->y - nearestStationY, playerObj->x - nearestStationX);
	if (temp_stationDirection < 0)
		temp_stationDirection = 2 * PI + temp_stationDirection;
	stationArrow.setRotation(180 - temp_stationDirection*(180 / PI));
}