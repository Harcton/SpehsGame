#include "Main.h"
#include "MainMenu.h"
#include "PlayerData.h"
#include "Game.h"

#define BACKGROUNDSPRITES 25


MainMenu::~MainMenu()
{
}
MainMenu::MainMenu(sf::RenderWindow& window) : mWindow(window)
{

	font1.loadFromFile("Font/ORANGEKI.ttf");
	button1Width = int(500 * resFactor);
	button1Height = int(75 * resFactor);
	buttonX1 = int((WINDOW_WIDTH - button1Width) * 0.5f);
	buttonY1 = int(WINDOW_HEIGHT*0.5f);	//buttonY1 = int((WINDOW_HEIGHT - button1Height * 4)*0.5f);
	buttonBorder = button1Height*0.25f;
	visibleSaveButtonCount = floor((WINDOW_HEIGHT - buttonY1 - button1Height - 2 * buttonBorder) / float(button1Height));
	settingsButtonY1 = int((WINDOW_HEIGHT - button1Height * 3)*0.5f);

	buttonColorBG = sf::Color(20, 45, 55, 160);
	buttonColorText = sf::Color(200, 210, 250, 220);
	
	buttons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - buttonBorder, button1Width + buttonBorder * 2, button1Height*4 + buttonBorder*2, "", int(50 * resFactor), sf::Color(15, 20, 25, 220), buttonColorText));
	buttons.push_back(Button(bi_mmNewGame, buttonX1, buttonY1, button1Width, button1Height, "New Game", int(50 * resFactor), buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmLoadGame, buttonX1, buttonY1 + button1Height, button1Width, button1Height, "Load Game", int(50 * resFactor), buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmSettings, buttonX1, buttonY1 + button1Height * 2, button1Width, button1Height, "Settings", int(50 * resFactor), buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);
	buttons.push_back(Button(bi_mmQuit, buttonX1, buttonY1 + button1Height * 3, button1Width, button1Height, "Quit", int(50 * resFactor), buttonColorBG, buttonColorText));
	buttons[buttons.size() - 1].setTextAlign(ta_center);

	loadSaveButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, buttonY1 - 2*button1Height - buttonBorder, button1Width + 2 * buttonBorder, 2 * buttonBorder + button1Height, "", int(50 * resFactor), sf::Color(15, 20, 25, 220), buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmReturn, buttonX1, buttonY1 - 2*button1Height, button1Width, button1Height, "Back to main menu", int(50 * resFactor), buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmLoadSave, buttonX1, buttonY1 - button1Height, button1Width, button1Height, "Load save", int(50 * resFactor), buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_mmDeleteSave, buttonX1, buttonY1, button1Width, button1Height, "Delete save", int(50 * resFactor), buttonColorBG, buttonColorText));
	loadSaveButtons.back().setTextAlign(ta_center);
	loadSaveButtons.push_back(Button(bi_false, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height + buttonBorder, int(button1Height*0.5f), button1Height*visibleSaveButtonCount, "", int(50 * resFactor), buttonColorBG, buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollUp, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height + buttonBorder, int(button1Height*0.5f), button1Height, "/\\", int(50 * resFactor), sf::Color(10, 45, 55, 140), buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollDown, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + buttonBorder + button1Height*(visibleSaveButtonCount), int(0.5f*button1Height), button1Height, "\\/", int(50 * resFactor), sf::Color(10, 45, 55, 140), buttonColorText));
	loadSaveButtons.push_back(Button(bi_confScrollBar, buttonX1 + button1Width - int(0.5f*button1Height), buttonY1 + button1Height*2 + buttonBorder, int(button1Height*0.5f), button1Height, "", int(50 * resFactor), sf::Color(40, 100, 150, 140), buttonColorText));
	reloadPlayerSaves();

	//SETTINGS buttons
	settingsButtons.push_back(Button(bi_false, buttonX1 - buttonBorder, settingsButtonY1 - buttonBorder, button1Width + 2 * buttonBorder, 2 * buttonBorder + button1Height * 6, "", int(50 * resFactor), sf::Color(15, 20, 25, 220), buttonColorText));
	settingsButtons.push_back(Button(bi_mmReturn, buttonX1, settingsButtonY1, button1Width, button1Height, "Back to main menu", int(50 * resFactor), buttonColorBG, buttonColorText));
	settingsButtons.back().setTextAlign(ta_center);
	settingsButtons.push_back(Button(bi_setFullscreenNode, buttonX1, settingsButtonY1 + button1Height, button1Width, button1Height, "", int(50 * resFactor), buttonColorBG, buttonColorText));
	settingsButtons.back().setTextAlign(ta_center);
	settingsButtons.push_back(Button(bi_setWindowWidth, buttonX1, settingsButtonY1 + button1Height*2, button1Width, button1Height, "", int(50 * resFactor), buttonColorBG, buttonColorText));
	settingsButtons.back().setTextAlign(ta_center);
	settingsButtons.push_back(Button(bi_setWindowHeight, buttonX1, settingsButtonY1 + button1Height*3, button1Width, button1Height, "", int(50 * resFactor), buttonColorBG, buttonColorText));
	settingsButtons.back().setTextAlign(ta_center);
	reloadSettingsButtonStrings();
	//Sliders
	settingsSliderButtons.push_back(SliderButton(bi_setMusicSlider, buttonX1, settingsButtonY1 + button1Height*4, button1Width, button1Height, "Music", int(50 * resFactor), buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &MUSIC_VOLUME));
	settingsSliderButtons.back().sliderState = MUSIC_VOLUME;
	settingsSliderButtons.back().setTextAlign(ta_center);
	settingsSliderButtons.push_back(SliderButton(bi_setSoundEffectSlider, buttonX1, settingsButtonY1 + button1Height*5, button1Width, button1Height, "Sound effects", int(50 * resFactor), buttonColorBG, buttonColorText, sf::Color(50, 80, 130, 140), &SFX_VOLUME));
	settingsSliderButtons.back().sliderState = SFX_VOLUME;
	settingsSliderButtons.back().setTextAlign(ta_center);

	

	//Intro music
	introMusic.openFromFile("Audio/Music and Ambience/main_menu_ver03.wav");
	introMusic.setVolume(MUSIC_VOLUME);
	introMusic.play();
	introMusic.setLoop(true);


	//Background
	menuLogoAnimatedTex.loadFromFile("Texture/Menu/menuLogoAnimated.png");
	menuLogoAnimated.setTexture(menuLogoAnimatedTex);
	menuLogoAnimated.setOrigin(350, 0);
	menuLogoAnimated.setFrameSize(700, 250);
	menuLogoAnimated.setTilesetSize(5, 4);
	menuLogoAnimated.setFrameDuration(2);
	menuLogoAnimated.setPosition(int(WINDOW_WIDTH / 2), 0);
	menuLogoAnimated.setColor(sf::Color(255, 255, 255, 255));
	menuLogoAnimated.setScale(2*resFactor, 2*resFactor);
	menuLogoAnimated.setLooping(false);


	for (int i = 0; i < BACKGROUNDSPRITES; i++)
	{
		bgSprVector.push_back(sf::Sprite());
		bgSprVector.back().setTexture(RM.menuSpace1Tex);
		bgSprVector.back().setOrigin(200, 175);
		bgSprVector.back().setRotation(irandom(0, 360));
		bgSprVector.back().setPosition(irandom(-200, WINDOW_WIDTH + 400), irandom(-200, WINDOW_HEIGHT + 400));
		bgSprVector.back().setColor(sf::Color(irandom(100, 155), irandom(100, 155), irandom(100, 155), irandom(120, 255)));
		int temp_scale = resFactor*(1.0f / 100.0f)*irandom(50, 1500);
		bgSprVector.back().setScale(temp_scale, temp_scale);
	}


	//Intro
	intro1Text.setFont(RM.menuFont);
	intro1Text.setCharacterSize(80 * resFactor);
	intro1Text.setString("   A game by\n  Teo Hiltunen\n        and\nJuuso Turunen");
	intro1Text.setPosition(int(WINDOW_WIDTH / 2 - intro1Text.getGlobalBounds().width / 2), int(WINDOW_HEIGHT / 2 - intro1Text.getGlobalBounds().height / 2));
	intro1Text.setColor(sf::Color(255,255,255,0));
	introShade.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
	introShade.setFillColor(sf::Color(0, 0, 0, 254));
}



int MainMenu::run()
{


	while (keepRunning == 1)
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

	return keepRunning;
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
			if (receivingTextInput == tit_none)
				switch (mEvent.key.code)
			{
				default:
					break;
				case sf::Keyboard::Escape:
					if (focus == mmf_base)
						keepRunning = 0;
					else if (focus == mmf_load)
						focus = mmf_base;
					else if (focus == mmf_settings)
					{
						saveSettings();
						focus = mmf_base;
					}
					else if (focus == mmf_intro1 || focus == mmf_intro2)
					{//Skip intro
						focus = mmf_base;
					}
					break;
				case sf::Keyboard::Return:
					if (focus == mmf_load)
					{
						for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
							if (playerSaveButtons[i].selected == true)
							{
							playerName = playerSavesList[i];
							launchGame();
							}
					}
					break;
				case sf::Keyboard::Delete:
					if (focus == mmf_load)
						for (unsigned int i = 0; i < playerSaveButtons.size(); i++)
							if (playerSaveButtons[i].selected == true)
								deleteSave(playerSavesList[i]);
							
					break;
			}
			else if (receivingTextInput == tit_saveName)
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
					receivingTextInput = tit_none;
					textInput = "";
					break;
				}

			}
			else if (receivingTextInput == tit_windowWidth || receivingTextInput == tit_windowHeight)
			{
				char input = getUserInput(mEvent);
				switch (input)
				{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if (textInput.size() < 4)
						textInput += input;
					break;
				case '-'://Backspace
				{
					if (textInput.size() <= 0)
						break;
					//Delete last char
					std::string temp_str;
					for (unsigned int i = 0; i < textInput.size() - 1; i++)
						temp_str += textInput[i];
					textInput = temp_str;
					break;
				}
				case '>'://Enter
					if (textInput == "")
					{/*nothing happens to window width or height*/}
					else if (receivingTextInput == tit_windowWidth)
						WINDOW_WIDTH = std::stoi(textInput);
					else//window height
						WINDOW_HEIGHT = std::stoi(textInput);

					WINDOW_WIDTH = limitWithin(100, WINDOW_WIDTH, 10000);
					WINDOW_HEIGHT = limitWithin(100, WINDOW_HEIGHT, 10000);
					receivingTextInput = tit_none;
					textInput = "";
					reloadSettingsButtonStrings();
					break;
				case '<'://Escape
					receivingTextInput = tit_none;
					textInput = "";
					reloadSettingsButtonStrings();
					break;
				}

			}
			break;
	}
}
void MainMenu::draw()
{
	//Intro 1 drawing
	if (focus == mmf_intro1)
	{
		if (introTextAlphaPolarity == 1)
		{
			if (intro1Text.getColor().a < 254)
				intro1Text.setColor(sf::Color(255, 255, 255, intro1Text.getColor().a + 1));
			else
				introTextAlphaPolarity = -1;
		}
		else
		{
			if (intro1Text.getColor().a > 0)
				intro1Text.setColor(sf::Color(255, 255, 255, intro1Text.getColor().a - 2));
			else
				focus = mmf_intro2;
		}
		mWindow.draw(intro1Text);
		return;
	}


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
			bgSprVector.back().setTexture(RM.menuSpace1Tex);
			bgSprVector.back().setOrigin(200, 175);
			bgSprVector.back().setRotation(irandom(0, 360));
			bgSprVector.back().setPosition(irandom(-200, WINDOW_WIDTH + 400), irandom(-200, WINDOW_HEIGHT + 400));
			bgSprVector.back().setColor(sf::Color(irandom(0, 255), irandom(0, 255), irandom(0, 255), irandom(100, 155)));
			int temp_scale = resFactor*(1.0f / 100.0f)*irandom(50, 1500);
			bgSprVector.back().setScale(temp_scale, temp_scale);
		}


		//Draw logo based on layer
		if (logoLayer == i)
			menuLogoAnimated.draw(mWindow);

	}

	if (logoLayer < BACKGROUNDSPRITES - 1)
	{//if the layer is not in the front
		if (logoLayerTimer > 0)
			logoLayerTimer--;
		else
		{
			if (introShade.getFillColor().a <= 0)
				logoLayer++;
			logoLayerTimer = 4;
		}
	}
	else if (focus == mmf_intro2)
		focus = mmf_base;




	if (focus == mmf_base)
	for (unsigned int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].id == bi_mmNewGame)
		{
			if (receivingTextInput == tit_saveName)
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
		if (receivingTextInput == tit_windowWidth || receivingTextInput == tit_windowHeight)
			reloadSettingsButtonStrings();

		for (unsigned int i = 0; i < settingsButtons.size(); i++)
			settingsButtons[i].draw(mWindow, mousePos);
		for (unsigned int i = 0; i < settingsSliderButtons.size(); i++)
			settingsSliderButtons[i].draw(mWindow, mousePos);
	}

	if (introShade.getFillColor().a > 0)
		introShade.setFillColor(sf::Color(0, 0, 0, introShade.getFillColor().a - 2));
	mWindow.draw(introShade);
}


void MainMenu::mouseLeftPressed()
{
	if (focus == mmf_base)
	{
		for (unsigned int i = 0; i < buttons.size(); i++)
			switch (buttons[i].checkIfPressed(mousePos))
		{
			case bi_mmNewGame:
				if (receivingTextInput == tit_saveName && textInput.size() > 1)
				{
					if (tryCreateSaveFile() == true)
						launchGame();
				}
				else
					receivingTextInput = tit_saveName;
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
				textInput = "";
				reloadSettingsButtonStrings();
				break;
			case bi_mmQuit:
				keepRunning = 0;
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
		//Standard buttons
		for (unsigned int i = 0; i < settingsButtons.size(); i++)
			switch (settingsButtons[i].checkIfPressed(mousePos))
		{
			case bi_mmReturn:
				saveSettings();
				focus = mmf_base;
				if (WINDOW_WIDTH != mWindow.getSize().x || WINDOW_HEIGHT != mWindow.getSize().y)
					keepRunning = -1;
				break;
			case bi_setFullscreenNode:
				flipBool(FULLSCREEN);
				reloadSettingsButtonStrings();
				break;
			case bi_setWindowWidth:
				if (receivingTextInput != tit_windowWidth)
					receivingTextInput = tit_windowWidth;
				else if (textInput != "")
				{
					receivingTextInput = tit_none;
					WINDOW_WIDTH = std::stoi(textInput);
					WINDOW_WIDTH = limitWithin(100, WINDOW_WIDTH, 10000);
					reloadSettingsButtonStrings();
				}
				else
				{
					receivingTextInput = tit_none;
					reloadSettingsButtonStrings();
				}
				break;
			case bi_setWindowHeight:
				if (receivingTextInput != tit_windowHeight)
					receivingTextInput = tit_windowHeight;
				else if (textInput != "")
				{
					receivingTextInput = tit_none;
					WINDOW_HEIGHT = std::stoi(textInput);
					WINDOW_HEIGHT = limitWithin(100, WINDOW_HEIGHT, 10000);
					reloadSettingsButtonStrings();
				}
				else
				{
					receivingTextInput = tit_none;
					reloadSettingsButtonStrings();
				}
				break;
		}
		//Sliders
		for (unsigned int i = 0; i < settingsSliderButtons.size(); i++)
			switch (settingsSliderButtons[i].checkIfPressed(mousePos))
		{//The function of these sliders is mostly automated using this function (checkIfPresssd).
			case bi_setMusicSlider:
				break;
			case bi_setSoundEffectSlider:
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
				*settingsSliderButtons[i].targetVariable = settingsSliderButtons[i].sliderState;
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
	introMusic.stop();
	Game game(mWindow);
	game.run();
	menuLogoAnimated.restart();
	introMusic.setVolume(MUSIC_VOLUME);
	introMusic.play();
	focus = mmf_base;
	textInput = "";
	receivingTextInput = tit_none;
	reloadPlayerSaves();
	for (unsigned int i = 0; i < settingsSliderButtons.size(); i++)
	{
		settingsSliderButtons[i].sliderState = *settingsSliderButtons[i].targetVariable;
		settingsSliderButtons[i].updateString();
	}
	
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
				playerSaveButtons.push_back(Button(bi_mmSaveListElement, buttonX1, buttonY1 + buttonBorder + (1 + i)*button1Height, button1Width - int(button1Height*0.5f), button1Height, playerSavesList[i], int(50 * resFactor), sf::Color(50, 90, 100, 180), buttonColorText));
			else
				playerSaveButtons.push_back(Button(bi_mmSaveListElement, buttonX1, buttonY1 + buttonBorder + (1 + i)*button1Height, button1Width, button1Height, playerSavesList[i], int(50 * resFactor), sf::Color(50, 90, 100, 180), buttonColorText));
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

void MainMenu::reloadSettingsButtonStrings()
{
	for (unsigned int i = 0; i < settingsButtons.size(); i++)
		switch (settingsButtons[i].id)
	{
		case bi_setFullscreenNode:
			if (FULLSCREEN == true)
				settingsButtons[i].text.setString("Window mode: Fullscreen");
			else
				settingsButtons[i].text.setString("Window mode: Windowed");
			settingsButtons[i].setTextAlign(ta_center);
			break;
		case bi_setWindowWidth:
			if (receivingTextInput != tit_windowWidth)
				settingsButtons[i].text.setString("Window width: " + std::to_string(WINDOW_WIDTH));
			else
				settingsButtons[i].text.setString("Window width: [ " + textInput + " ]");
			settingsButtons[i].setTextAlign(ta_center);
			break;
		case bi_setWindowHeight:
			if (receivingTextInput != tit_windowHeight)
				settingsButtons[i].text.setString("Window height: " + std::to_string(WINDOW_HEIGHT));
			else
				settingsButtons[i].text.setString("Window height: [ " + textInput + " ]");
			settingsButtons[i].setTextAlign(ta_center);
	}
}

