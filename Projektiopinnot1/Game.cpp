#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Debris.h"
#include "Player.h"
#include "Enemy.h"
#include "Background.h"


Game::Game()
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
	sf::RenderWindow mWindow{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Wrench Space" };
	mWindow.setFramerateLimit(60);
	sf::Event mEvent;
	std::vector<Background*>::iterator bgit;
<<<<<<< HEAD
	std::vector<Enemy*>::iterator enemyIt;
=======
	std::vector<Debris*>::iterator deit;
	
>>>>>>> origin/master

	objects.push_back(new Player(mWindow, this, true, 0, 0));
	playerObj = objects[0];

	for (int i = 0; i < 10; i++)
		backgrounds.push_back(new Background(mWindow, this, backgrounds));

	for (unsigned int i = 0; i < 20; i++)
		debris.push_back(new Debris(mWindow, this, debris));
	
	for (int i = 0; i < 3; i++)
		enemies.push_back(new Enemy(mWindow, this, enemies));
		
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
		mWindow.clear(sf::Color(0, 0, 0, 100));
		//DRAWING

		//Background update
		for (bgit = backgrounds.begin(); bgit != backgrounds.end();)
			if ((*bgit)->updateBackground() == false)
			{
				delete * bgit;
				bgit = backgrounds.erase(bgit);
			}
			else
				++bgit;
		if (backgrounds.size() < 30)
			backgrounds.push_back(new Background(mWindow, this, backgrounds));

		for (unsigned int i = 0; i < backgrounds.size(); i++)
			backgrounds[i]->draw();


		//Enemy update
		for (enemyIt = enemies.begin(); enemyIt != enemies.end();)
		{
			if ((*enemyIt)->spawnUpdate() == false)
			{
				delete *enemyIt;
				enemyIt = enemies.erase(enemyIt);
			}
			else
				enemyIt++;			
		}
		if (enemies.size() < 3)
			enemies.push_back(new Enemy(mWindow, this, enemies));
		
		for (unsigned int i = 0; i < enemies.size(); i++)
			enemies[i]->draw();
		

		//Debris update
		for (deit = debris.begin(); deit != debris.end();)
			if ((*deit)->updateDebris() == false)
			{
			delete * deit;
			deit = debris.erase(deit);
			}
			else
				++deit;
		if (debris.size() < 30)
			debris.push_back(new Debris(mWindow, this, debris));

		for (unsigned int i = 0; i < debris.size(); i++)
			debris[i]->draw();





		for (unsigned int i = 0; i < objects.size(); i++)
		{
			objects[i]->update();
			objects[i]->draw();
		}


		//END DRAWING
		mWindow.display();
		/////////////

	}		
}