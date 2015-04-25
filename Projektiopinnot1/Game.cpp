#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"
#include "Player.h"

#include "Debris.h"
#include "Enemy.h"
#include "Sentinel.h"
#include "Sentry.h"
#include "Seeker.h"
#include "Commander.h"
#include "Background.h"


Game::Game(sf::RenderWindow& w) : mWindow(w)
{
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

	



	bool keepRunning = true;
	while (keepRunning)
	{
		//Events
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			keepRunning = false;
		while (mWindow.pollEvent(mEvent))
		{
			if (mEvent.type == sf::Event::Closed)
				keepRunning = false;
		}
		/////////



		//DRAWING
		mWindow.clear(sf::Color(0, 0, 0, 100));
		updateBackgrounds();
		updateObjects();

		for (unsigned int i = 0; i < objects.size(); i++)
			if (objects[i]->hasCollisionChecks == true)
				objects[i]->checkCollisions(i);

		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects[i]->draw();
		}

		mWindow.display();
		/////////////

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
		//std::cout << "\nGame.cpp: Removing background";
		}
	if (backgrounds.size() < 7)
	{
		backgrounds.push_back(new Background(mWindow, this, backgrounds));
		backgrounds[backgrounds.size() - 1]->update();
	}

	for (unsigned int i = 0; i < backgrounds.size(); i++)
		backgrounds[i]->draw();

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
		//std::cout << "\nGame.cpp: Removing object";
		}

	//for (obIt = objects.begin(); obIt != objects.end();)
	//	if ((*obIt)->update() == false)
	//	{
	//	Object* temp_objPtr = *obIt;
	//	obIt = objects.erase(obIt);
	//	delete temp_objPtr;

	//	std::cout << "\nGame.cpp: Removing object";
	//	}
	//	else
	//		++obIt;

	/*
	if (objects.size() < 2)
	{
		objects.push_back(new Commander(mWindow, this, 1)); //spawn different enemy types
		objects[objects.size() - 1]->setRandomLocation();
	}
	*/


	//TEST GAME 17.4
	distanceFromStart = getDistance(0, 0, playerObj->x, playerObj->y);
	if (objects.size() < distanceFromStart/1300)
	{
		if (distanceFromStart/1000 > 8)
			spawnRandomization = irandom(1, 4);
		else
			spawnRandomization = irandom(1, 3);

		if (spawnRandomization == 1)
		{
			objects.push_back(new Sentinel(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
		}
		if (spawnRandomization == 2)
		{
			objects.push_back(new Seeker(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
		}
		if (spawnRandomization == 3)
		{
			objects.push_back(new Sentry(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
		}
		if (spawnRandomization == 4)
		{
			objects.push_back(new Commander(mWindow, this, 1));
			objects[objects.size() - 1]->setRandomLocation();
		}
	}
	//


	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->draw();
}