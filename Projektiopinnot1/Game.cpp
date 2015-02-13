#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"
#include "Player.h"

#include "Debris.h"
#include "Enemy.h"
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
	sf::Event mEvent;
	

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
	for (bgIt = backgrounds.begin(); bgIt != backgrounds.end();)
		if ((*bgIt)->updateBackground() == false)
		{
		delete * bgIt;
		bgIt = backgrounds.erase(bgIt);
		}
		else
			++bgIt;
	if (backgrounds.size() < 10)
		backgrounds.push_back(new Background(mWindow, this, backgrounds));

	for (unsigned int i = 0; i < backgrounds.size(); i++)
		backgrounds[i]->draw();

}


void Game::updateObjects()
{
	//Object update
	for (obIt = objects.begin(); obIt != objects.end();)
		if ((*obIt)->update() == false)
		{
		delete * obIt;
		obIt = objects.erase(obIt);
		}
		else
			++obIt;
	if (objects.size() < 50) 
		
		//if (flipCoin())
			objects.push_back(new Debris(mWindow, this, objects));
		//else
		
			//objects.push_back(new Enemy(mWindow, this, objects));
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->draw();
}