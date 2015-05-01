#include "Main.h"
#include "Button.h"
#include "MainMenu.h"
#include "Game.h"


MainMenu::~MainMenu()
{
}
MainMenu::MainMenu(sf::RenderWindow& window) : mWindow(window)
{

	font1.loadFromFile("Font/ORANGEKI.ttf");
	button1Width = int(500 * resFactor);
	button1Height = int(75 * resFactor);
	buttonX1 = int((WINDOW_WIDTH - button1Width) * 0.5f);
	buttonY1 = int((WINDOW_HEIGHT - button1Height * 4)*0.5f);
	buttonBorder = button1Height*0.25f;

	buttonColorBG = sf::Color(20, 35, 50);
	buttonColorText = sf::Color(200, 210, 250);


	buttons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - buttonBorder, button1Width + buttonBorder * 2, button1Height*4 + buttonBorder*2, "", int(50 * resFactor), font1, sf::Color(15, 20, 25), buttonColorText));
	buttons.push_back(Button(bi_mmNewGame, buttonX1, buttonY1, button1Width, button1Height, "New Game", int(50 * resFactor), font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmLoadGame, buttonX1, buttonY1 + button1Height, button1Width, button1Height, "Load Game", int(50 * resFactor), font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmSettings, buttonX1, buttonY1 + button1Height * 2, button1Width, button1Height, "Settings", int(50 * resFactor), font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmQuit, buttonX1, buttonY1 + button1Height * 3, button1Width, button1Height, "Quit", int(50 * resFactor), font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);



	//Intro music
	//introMusic.openFromFile("Audio/Music/giantFollis.wav");
	//introMusic.play();


	//Background
	menuLogo.setTexture(RM.getTexture("menuLogo.png"));
	menuLogo.setOrigin(672, 0);
	menuLogo.setScale(resFactor, resFactor);
	menuLogo.setPosition(WINDOW_WIDTH/2, 0);
	planetSpr.setTexture(RM.getTexture("menuPlanet.png"));
	planetSpr.setOrigin(550, 500);
	planetSpr.setScale(2 * resFactor, 2 * resFactor);
	planetSpr.setPosition(WINDOW_WIDTH*0.8, WINDOW_HEIGHT + 512 * 2 * resFactor*0.4);

	for (int i = 0; i < 25; i++)
	{
		bgSprVector.push_back(sf::Sprite());
		bgSprVector.back().setTexture(RM.getTexture("menuSpace1.png"));
		bgSprVector.back().setOrigin(200, 175);
		bgSprVector.back().setRotation(irandom(0, 360));
		bgSprVector.back().setPosition(irandom(-200, WINDOW_WIDTH + 400), irandom(-200, WINDOW_HEIGHT + 400));
		bgSprVector.back().setColor(sf::Color(irandom(0, 255), irandom(0, 255), irandom(0, 255), irandom(100, 155)));
		int temp_scale = resFactor*(1.0f / 100.0f)*irandom(50, 1500);
		bgSprVector.back().setScale(temp_scale, temp_scale);
	}

}



void MainMenu::run()
{


	while (keepRunning)
	{
		mousePos = sf::Mouse::getPosition(mWindow);

		pollEvents();



		//Drawing
		mWindow.clear(sf::Color(0, 0, 0));
		draw();
		mWindow.display();
	}


}

void MainMenu::pollEvents()
{
	while (mWindow.pollEvent(mEvent))
		switch (mEvent.type)
	{
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseLeftPressed();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				mouseRightPressed();
			break;
		case sf::Event::KeyPressed:
			switch (mEvent.key.code)
			{
			case sf::Keyboard::Escape:
				keepRunning = false;
				break;
			case sf::Keyboard::Return:
				launchGame();
				break;
			}
			break;
	}
}
void MainMenu::draw()
{
	//Draw background first
	for (unsigned int i = 0; i < bgSprVector.size(); i++)
	{
		int temp_i = 1 - (2*(bgSprVector[i].getColor().r%2));
		bgSprVector[i].setRotation(bgSprVector[i].getRotation() + temp_i);
		sf::Color temp_c = bgSprVector[i].getColor();
		if (rollDie(6))
			temp_c.a--;
		bgSprVector[i].setColor(temp_c);

		if (bgSprVector[i].getColor().a > 0)
			mWindow.draw(bgSprVector[i]);
		else
		{
			bgSprVector.erase(bgSprVector.begin() + i);
			bgSprVector.push_back(sf::Sprite());
			bgSprVector.back().setTexture(RM.getTexture("menuSpace1.png"));
			bgSprVector.back().setOrigin(200, 175);
			bgSprVector.back().setRotation(irandom(0, 360));
			bgSprVector.back().setPosition(irandom(-200, WINDOW_WIDTH + 400), irandom(-200, WINDOW_HEIGHT + 400));
			bgSprVector.back().setColor(sf::Color(irandom(0, 255), irandom(0, 255), irandom(0, 255), irandom(100, 155)));
			int temp_scale = resFactor*(1.0f / 100.0f)*irandom(50, 1500);
			bgSprVector.back().setScale(temp_scale, temp_scale);
		}
	}

	planetSpr.setRotation(planetSpr.getRotation() + 0.01);
	mWindow.draw(planetSpr);
	mWindow.draw(menuLogo);

	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		buttons[i].draw(mWindow, mousePos);
	}

}


void MainMenu::mouseLeftPressed()
{
	for (unsigned int i = 0; i < buttons.size(); i++)
	switch (buttons[i].checkIfPressed(mousePos))
	{
		case bi_mmNewGame:
			playerName = "";
			launchGame();
			break;
		case bi_mmLoadGame:
			loadGame();
			break;
		case bi_mmSettings:
			launchSettings();
			break;
		case bi_mmQuit:
			keepRunning = false;
			break;
	}

}
void MainMenu::mouseRightPressed()
{

}


void MainMenu::launchGame()
{
	Game game(mWindow);
	game.run();
}

void MainMenu::loadGame()
{
	playerName = "player1";
	launchGame();
}

void MainMenu::launchSettings()
{

}

