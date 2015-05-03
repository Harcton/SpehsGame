#include "Main.h"
#include "Button.h"
#include "SliderButton.h"
#include "MainMenu.h"
#include "Game.h"
#include "GridData.h"
#include "PlayerData.h"

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
	visibleSaveButtonCount = floor((WINDOW_HEIGHT - buttonY1 - button1Height - 2 * buttonBorder) / float(button1Height));
	settingsButtonY1 = int((WINDOW_HEIGHT - button1Height * 3)*0.5f);

	buttonColorBG = sf::Color(20, 45, 55, 160);
	buttonColorText = sf::Color(200, 210, 250, 220);
	
	buttons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - buttonBorder, button1Width + buttonBorder * 2, button1Height*4 + buttonBorder*2, "", int(50 * resFactor), &font1, sf::Color(15, 20, 25, 220), buttonColorText));
	buttons.push_back(Button(bi_mmNewGame, buttonX1, buttonY1, button1Width, button1Height, "New Game", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmLoadGame, buttonX1, buttonY1 + button1Height, button1Width, button1Height, "Load Game", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmSettings, buttonX1, buttonY1 + button1Height * 2, button1Width, button1Height, "Settings", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmQuit, buttonX1, buttonY1 + button1Height * 3, button1Width, button1Height, "Quit", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);

	loadSaveButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - 2*button1Height - buttonBorder, button1Width + 2 * buttonBorder, 2 * buttonBorder + button1Height, "", int(50 * resFactor), &font1, sf::Color(15, 20, 25, 220), buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmReturn, buttonX1, buttonY1 - 2*button1Height, button1Width, button1Height, "Back to main menu", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmLoadSave, buttonX1, buttonY1 - button1Height, button1Width, button1Height, "Load save", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmDeleteSave, buttonX1, buttonY1, button1Width, button1Height, "Delete save", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_false, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height + buttonBorder, int(button1Height*0.5f), button1Height*visibleSaveButtonCount, "", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollUp, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height + buttonBorder, int(button1Height*0.5f), button1Height, "/\\", int(50 * resFactor), &font1, sf::Color(10, 45, 55, 140), buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollDown, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + buttonBorder + button1Height*(visibleSaveButtonCount), int(0.5f*button1Height), button1Height, "\\/", int(50 * resFactor), &font1, sf::Color(10, 45, 55, 140), buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollBar, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height*2 + buttonBorder, int(button1Height*0.5f), button1Height, "", int(50 * resFactor), &font1, sf::Color(40, 100, 150, 140), buttonColorText));
	reloadPlayerSaves();

	settingsButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, settingsButtonY1 - buttonBorder, button1Width + 2 * buttonBorder, 2 * buttonBorder + button1Height * 5, "", int(50 * resFactor), &font1, sf::Color(15, 20, 25, 220), buttonColorText));
	settingsButtons.push_back(Button(bi_mmReturn, buttonX1, settingsButtonY1, button1Width, button1Height, "Back to main menu", int(50 * resFactor), &font1, buttonColorBG, buttonColorText));
	settingsButtons.back().setTextAlign(ta_center);
	settingsSliderButtons.push_back(SliderButton(bi_setMusicSlider, buttonX1, settingsButtonY1 + button1Height, button1Width, button1Height, "Music", int(50 * resFactor), &font1, buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &MUSIC_VOLUME));
	settingsSliderButtons.back().setTextAlign(ta_center);
	settingsSliderButtons.push_back(SliderButton(bi_setSoundEffectSlider, buttonX1, settingsButtonY1 + button1Height*2, button1Width, button1Height, "Sound effects", int(50 * resFactor), &font1, buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &SFX_VOLUME));
	settingsSliderButtons.back().setTextAlign(ta_center);

	

	//Intro music
	//introMusic.openFromFile("Audio/Music/giantFollis.wav");
	//introMusic.play();


	//Background
	menuLogo.setTexture(RM.getTexture("menuLogo.png"));
	menuLogo.setOrigin(672, 0);
	menuLogo.setScale(resFactor, resFactor);
	menuLogo.setPosition(WINDOW_WIDTH/2, 0);
	menuLogo.setColor(sf::Color(255, 255, 255, 220));
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

		if (usingScrollBar)
			dragScrollBar();
		


		//Drawing
		mWindow.clear(sf::Color(0, 0, 0));
		draw();
		mWindow.display();
	}


}

void MainMenu::pollEvents()
{
	doubleClickTimer++;
	while (mWindow.pollEvent(mEvent))
		switch (mEvent.type)
	{
		default:
			break;			
		case sf::Event::MouseWheelMoved:
			if (focus == mmf_load)
			{
				//scrollDelta -= scrollDelta + mEvent.mouseWheel.delta;
				if (mEvent.mouseWheel.delta > 0 && scrollState > 0)
					scrollSaves(-1);
				else if (mEvent.mouseWheel.delta < 0 && scrollState < playerSaveButtons.size() - visibleSaveButtonCount)
					scrollSaves(1);
			}
			mEvent.mouseWheel.delta = 0;
			break;
		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseLeftPressed();
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				mouseRightPressed();
			break;
		case sf::Event::KeyPressed:
			if (receivingTextInput == false)
				switch (mEvent.key.code)
			{
				default:
					break;
				case sf::Keyboard::Escape:
					if (focus == mmf_base)
						keepRunning = false;
					else if (focus == mmf_load || focus == mmf_settings)
						focus = mmf_base;
					break;
				case sf::Keyboard::Return:
					if (focus == mmf_load)
						for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
							if (playerSaveButtons[i].selected == true)
						{
						playerName = playerSavesList[i];
						launchGame();
						}
					break;
				case sf::Keyboard::Delete:
					if (focus == mmf_load)
						for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
							if (playerSaveButtons[i].selected == true)
								deleteSave(playerSavesList[i]);
							
					break;
			}
			else
			{
				char input = getUserInput(mEvent);
				switch (input)
				{
				default:
					if (textInput.size() < 20)
						textInput += input;
					break;
				case '#':
					//Not defined characters
					break;
				case '-'://Backspace
				{
					if (textInput.size() <= 0)
						break;
					//Delete last char
					std::string temp_str;
					for (unsigned int i = 0; i < textInput.size()-1; i++)
						temp_str += textInput[i];
					textInput = temp_str;
					break;
				}
				case '>'://Enter
					if (textInput.size() > 1)
					{
						if (tryCreateSaveFile() == true)
							launchGame();
					}
					break;
				case '<'://Escape
					for (unsigned int i = 0; i < buttons.size(); i++)
						if (buttons[i].id == bi_mmNewGame)
							buttons[i].text.setString("New Game");
					receivingTextInput = false;
					textInput = "";
					break;
				}

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
	//End of background drawing


	if (focus == mmf_base)
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].id == bi_mmNewGame)
		{
			if (receivingTextInput == true)
			{
				if (textInput == "")
				{
					for (unsigned int i = 0; i < buttons.size(); i++)
						if (buttons[i].id == bi_mmNewGame)
							if (buttons[i].text.getString() != "A save with this name already exists!")
					{
						buttons[i].text.setString(">Enter save name<");
					}
				}
				else
					buttons[i].text.setString(textInput);

				buttons[i].setTextAlign(ta_center);
			}
			else
			{
				buttons[i].text.setString("New Game");
				buttons[i].setTextAlign(ta_center);
			}
		}

		buttons[i].draw(mWindow, mousePos);
	}
	else if (focus == mmf_load)
	{
		if (playerSavesList.size() < visibleSaveButtonCount)
		{
			for (unsigned int i = 0; i < loadSaveButtons.size(); i++)
				if ( i== 0/*bg rect*/ || loadSaveButtons[i].id == bi_mmLoadSave || loadSaveButtons[i].id == bi_mmReturn || loadSaveButtons[i].id == bi_mmDeleteSave)
					loadSaveButtons[i].draw(mWindow, mousePos);
		}
		else
		for (unsigned int i = 0; i < loadSaveButtons.size(); i++)
			loadSaveButtons[i].draw(mWindow, mousePos);

		for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
			playerSaveButtons[i].draw(mWindow, mousePos);
	}
	else if (focus == mmf_settings)
	{
		for (unsigned int i = 0; i < settingsButtons.size(); i++)
			settingsButtons[i].draw(mWindow, mousePos);
		for (unsigned int i = 0; i < settingsSliderButtons.size(); i++)
			settingsSliderButtons[i].draw(mWindow, mousePos);
	}

}


void MainMenu::mouseLeftPressed()
{
	if (focus == mmf_base)
	{
		for (unsigned int i = 0; i < buttons.size(); i++)
			switch (buttons[i].checkIfPressed(mousePos))
		{
			case bi_mmNewGame:
				if (receivingTextInput && textInput.size() > 1)
				{
					if (tryCreateSaveFile() == true)
						launchGame();
				}
				else
					receivingTextInput = true;
				break;
			case bi_mmLoadGame:
				if (playerSavesList.size() > 0)
				{
					reloadPlayerSaves();
					focus = mmf_load;
				}
				break;
			case bi_mmSettings:
				focus = mmf_settings;
				launchSettings();
				break;
			case bi_mmQuit:
				keepRunning = false;
				break;
		}
	}
	else if (focus == mmf_load)
	{
		for (unsigned int i = 0; i < loadSaveButtons.size(); i++)
			switch (loadSaveButtons[i].checkIfPressed(mousePos))
			{
			case bi_mmReturn:
				focus = mmf_base;
				break;
			case bi_mmLoadSave:
				for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
				if (playerSaveButtons[i].selected == true)
				{
					playerName = playerSavesList[i];
					launchGame();
					break;
				}
				break;
			case bi_mmDeleteSave:
				for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
					if (playerSaveButtons[i].selected == true)
						deleteSave(playerSavesList[i]);
				
				break;
			case bi_confScrollUp:
				if (scrollState > 0)
					scrollSaves(-1);
				break;
			case bi_confScrollDown:
				if (scrollState < playerSaveButtons.size() - visibleSaveButtonCount)
					scrollSaves(1);
				break;
			case bi_confScrollBar:
				usingScrollBar = true;
				mouseGrabYdiff = mousePos.y - loadSaveButtons[i].buttonRectangle.getPosition().y;
				break;
			}

		//Check save list buttons
		{
			for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
				if (playerSaveButtons[i].checkIfPressed(mousePos))
					if (playerSaveButtons[i].selected == false)
					{
						for (unsigned int j = 0; j < playerSaveButtons.size(); j++)
							playerSaveButtons[j].selected = false;
						playerSaveButtons[i].selected = true;
					}
					else
					{
						if (doubleClickTimer > 20)
							playerSaveButtons[i].selected = false;
						else
						{//Double click == load
							playerName = playerSavesList[i];
							launchGame();
						}
					}
			
		}
	}
	else if (focus == mmf_settings)
	{
		for (unsigned int i = 0; i < settingsButtons.size(); i++)
			switch (settingsButtons[i].checkIfPressed(mousePos))
		{
			case bi_mmReturn:
				focus = mmf_base;
				break;
		}
	}

	doubleClickTimer = 0;
}
void MainMenu::mouseRightPressed()
{
	if (focus == mmf_settings)
	{
		for (unsigned int i = 0; i < settingsSliderButtons.size(); i++)
			switch (settingsSliderButtons[i].checkIfPressed(mousePos))
		{
			case bi_setSoundEffectSlider:
			case bi_setMusicSlider:
				if (settingsSliderButtons[i].sliderState == 0)
					settingsSliderButtons[i].sliderState = 100;
				else if (settingsSliderButtons[i].sliderState == 100)
					settingsSliderButtons[i].sliderState = 0;
				else if (settingsSliderButtons[i].sliderState < 50)
					settingsSliderButtons[i].sliderState = 0;
				else
					settingsSliderButtons[i].sliderState = 100;
				break;
		}
	}
}

bool MainMenu::tryCreateSaveFile()
{
	if (textInput == "")
		return false;


	reloadPlayerSaves();//Make sure player save files list is up to date
	for (unsigned int i = 0; i < playerSavesList.size(); i++)
	if (textInput == playerSavesList[i])		
	{//Save name already exists, notify user
		for (unsigned int i = 0; i < buttons.size(); i++)
			if (buttons[i].id == bi_mmNewGame)
			{
			buttons[i].text.setString("A save with this name already exists!");
			buttons[i].setTextAlign(ta_center);
			}
		return false;
	}

	playerName = textInput;
	createSaveFile();
	textInput = "";
	return true;
}

void MainMenu::createSaveFile()
{
	mFileStream.open("Settings/PlayerSavesList.txt", std::ios::app);
	if (!mFileStream.fail())
	{
		//Save name into list
		mFileStream << playerName << "#";
		mFileStream.close();

		//Create .dat file
		mFileStream.open("Settings/PlayerSaves/" + playerName + ".dat", std::ofstream::binary | std::ios::out);
		if (mFileStream)
		{
			PlayerData newData;
			mFileStream.write((char*)&newData, sizeof(PlayerData));
			mFileStream.close();
			reloadPlayerSaves();
		}
		else
			std::cout << "\nError. Failed create new save:\n" << "Settings/PlayerSaves/" << playerName << ".dat";
		mFileStream.close();
	}
	else
		std::cout << "\nFailed to write save name into PlayerSavesList.txt";
}

void MainMenu::deleteSave(std::string name)
{
	std::cout << "\nDeleting player save: [" << name << "]";


	std::vector<std::string> temp_schemes;
	mFileStream.open("Settings/PlayerSavesList.txt");
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
			if (temp_schemes[i] == name)
				temp_schemes.erase(temp_schemes.begin() + i);
		}
		writeVectorIntoFile(temp_schemes, "Settings/PlayerSavesList.txt");



		//Remove the .dat file...
		std::string directoryPath = "Settings/PlayerSaves/";
		char temp_charPtr[100];
		for (unsigned int i = 0; i < directoryPath.size(); i++)
			temp_charPtr[i] = directoryPath[i];

		for (unsigned int i = 0; i < name.size(); i++)
		{
			temp_charPtr[directoryPath.size() + i] = name[i];
		}
		temp_charPtr[directoryPath.size() + name.size()] = '.';
		temp_charPtr[directoryPath.size() + name.size() + 1] = 'd';
		temp_charPtr[directoryPath.size() + name.size() + 2] = 'a';
		temp_charPtr[directoryPath.size() + name.size() + 3] = 't';
		temp_charPtr[directoryPath.size() + name.size() + 4] = '\0';
		remove(temp_charPtr);
		std::cout << "\nDelete complete: " << temp_charPtr;
	}
	else
		std::cout << "\nFailed to open PlayerSavesList.txt";
	
	reloadPlayerSaves();

	if (scrollState > playerSaveButtons.size() - visibleSaveButtonCount)
		scrollState--;
	scrollSaves(0);
}


void MainMenu::launchGame()
{
	Game game(mWindow);
	game.run();
	focus = mmf_base;
	textInput = "";
	receivingTextInput = false;
	reloadPlayerSaves();
}


void MainMenu::launchSettings()
{

}

void MainMenu::reloadPlayerSaves()
{
	visibleSaveButtonCount = floor((WINDOW_HEIGHT - buttonY1 - button1Height - 2*buttonBorder)/float(button1Height));
	playerSavesList.clear();
	playerSaveButtons.clear();
	std::string temp_str;
	mFileStream.open("Settings/PlayerSavesList.txt", std::ios::in);
	if (!mFileStream.fail())
	{//Load all scheme names into temp_schemes
		while (!mFileStream.eof())
			mFileStream >> temp_str;
		mFileStream.close();
		extractNamesFromString(temp_str, playerSavesList);
	}

	std::cout << "\nPlayer save reload complete.";
	if (playerSavesList.size() > 0)
	{
		std::cout << "\n Saves found:";
		for (unsigned int i = 0; i < playerSavesList.size(); i++)
		{
			std::cout << "\n[" << playerSavesList[i] << "]";
			if (playerSavesList.size() > visibleSaveButtonCount)
				playerSaveButtons.push_back(Button(bi_mmSaveListElement, buttonX1, buttonY1 + buttonBorder + (1 + i)*button1Height, button1Width - int(button1Height*0.5f), button1Height, playerSavesList[i], int(50 * resFactor), &font1, sf::Color(50, 90, 100, 180), buttonColorText));
			else
				playerSaveButtons.push_back(Button(bi_mmSaveListElement, buttonX1, buttonY1 + buttonBorder + (1 + i)*button1Height, button1Width, button1Height, playerSavesList[i], int(50 * resFactor), &font1, sf::Color(50, 90, 100, 180), buttonColorText));
			playerSaveButtons.back().setTextAlign(ta_center);
		}


	}
	else
		std::cout << "\n No saves found!";

	if (playerSavesList.size() > 0)
	{
		if (playerSavesList.size() < visibleSaveButtonCount)
			loadSaveButtons[0].buttonRectangle.setSize(sf::Vector2f(button1Width + 2*buttonBorder, buttonBorder*3 + (3 + playerSavesList.size())*button1Height));
		else
			loadSaveButtons[0].buttonRectangle.setSize(sf::Vector2f(button1Width + 2 * buttonBorder, buttonBorder * 3 + (3 + visibleSaveButtonCount)*button1Height));


		for (unsigned int i = 0; i < buttons.size(); i++)
			if (buttons[i].id == bi_mmLoadGame)
			buttons[i].visible = true;
	}
	else
	{//No saves found...
		for (unsigned int i = 0; i < buttons.size(); i++)
			if (buttons[i].id == bi_mmLoadGame)
				buttons[i].visible = false;
	}
	scrollSaves(0);
}

void MainMenu::scrollSaves(int direction)
{
	//scrollDelta -= scrollDelta + mEvent.mouseWheel.delta;
	if (direction < 0 && scrollState > 0)
		scrollState--;
	else if (direction > 0 && scrollState < playerSaveButtons.size() - visibleSaveButtonCount)
		scrollState++;

	//Apply scroll state changes to button elements
	for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
	{
		playerSaveButtons[i].setPosition(playerSaveButtons[i].buttonRectangle.getPosition().x, buttonY1 + buttonBorder + (1 + i - scrollState)*button1Height);
		if (i < scrollState)
			playerSaveButtons[i].visible = false;
		else if (i > visibleSaveButtonCount - 1 + scrollState)
			playerSaveButtons[i].visible = false;
		else
			playerSaveButtons[i].visible = true;

	}

	for (unsigned int i = 0; i < loadSaveButtons.size(); i++)
		if (loadSaveButtons[i].id == bi_confScrollBar)
		{
		int temp_y1 = buttonY1 + buttonBorder + 2 * button1Height;
		int temp_y2 = button1Height*(visibleSaveButtonCount - 3);
		int temp_y = temp_y1 + float(temp_y2)*(float(scrollState) / (playerSavesList.size() - visibleSaveButtonCount));
		loadSaveButtons[i].setPosition(loadSaveButtons[i].buttonRectangle.getPosition().x, temp_y);
	}

}


void MainMenu::dragScrollBar()
{
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		usingScrollBar = false;
	else
	{
		int temp_y1 = 0;
		for (unsigned int i = 0; i < loadSaveButtons.size(); i++)
			if (loadSaveButtons[i].id == bi_confScrollUp)
				temp_y1 = loadSaveButtons[i].buttonRectangle.getPosition().y + button1Height;

		int temp_y2 = temp_y1 + button1Height*(visibleSaveButtonCount - 1);
		float temp_factor = (mousePos.y - temp_y1 - mouseGrabYdiff)/(float(visibleSaveButtonCount - 3)*button1Height);

		scrollState = round(temp_factor*(playerSavesList.size() - visibleSaveButtonCount));

		if (scrollState < 0)
			scrollState = 0;
		else if (scrollState > playerSavesList.size() - visibleSaveButtonCount)
			scrollState = playerSavesList.size() - visibleSaveButtonCount;

		std::cout << "\nScroll[" << scrollState << "/" << playerSavesList.size() - visibleSaveButtonCount << "]";
		scrollSaves(0);
	}
}