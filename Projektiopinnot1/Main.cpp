#include <time.h>
#include <fstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Main.h"
#include "MainMenu.h"



//DEFAULT SETTINGS FOR NEW USERS
bool FULLSCREEN = false;
int WINDOW_WIDTH = 1600;
int WINDOW_HEIGHT = 900;
extern int MUSIC_VOLUME = 100;
extern int SFX_VOLUME = 100;


double resFactor = WINDOW_HEIGHT/1080.0;
double zoomFactor = 1;
std::string playerName = "";
//Global control settings


bool starMap = false;
float radarRange = 0;
float radarSpeed = 0;
float movementSpeed = 0.0004;
float rotationSpeed = PI / 180000;

//Global textures
ResourceManager RM;
void initializeResourceManager(ResourceManager&);


void main()
{
	initializeResourceManager(RM);

	//Load game settings. If no settings file exists, create one
	if (loadSettings() == false)
		saveSettings();

	//Randomize randomization...
	time_t t;
	srand((unsigned)time(&t));

	sf::RenderWindow mWindow{ sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spehs Game" };
	if (FULLSCREEN)
		mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spehs Game", sf::Style::Fullscreen);
	else
		mWindow.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Spehs Game", sf::Style::Close);

	mWindow.setFramerateLimit(60);
	mWindow.setMouseCursorVisible(true);

	MainMenu mainMenu(mWindow);
	mainMenu.run();	


}



void initializeResourceManager(ResourceManager& rm)
{
//TEXTURES
	//PLAYER
	rm.loadTexture("Texture/Player/skeleton.png");
	rm.loadTexture("Texture/Player/core.png");
	rm.loadTexture("Texture/Player/turret.png");
	rm.loadTexture("Texture/Player/engine.png");
	rm.loadTexture("Texture/Player/engine_fire.png");
	rm.loadTexture("Texture/Player/engine_fire_animation.png");
	rm.loadTexture("Texture/Player/engine_fire_animation_testversion.png");

	//ENEMY
	rm.loadTexture("Texture/Enemy/enemy_commander_ship_upgraded.png");
	rm.loadTexture("Texture/Enemy/Flier.png");
	rm.loadTexture("Texture/Enemy/enemy_base_purple.png");
	rm.loadTexture("Texture/Enemy/enemy_base_green.png");
	rm.loadTexture("Texture/Enemy/enemy_base.png");
		//Seeker
	rm.loadTexture("Texture/Enemy/Seeker.png");
	rm.loadTexture("Texture/Enemy/seeker_active_animation.png");
	rm.loadTexture("Texture/Enemy/seeker_passive_animation.png");
		//Sentinel
	rm.loadTexture("Texture/Enemy/Sentinel.png");

	//MENU
	rm.loadTexture("Texture/Menu/inheritanceArrow.png");
	rm.loadTexture("Texture/Menu/xButton.png");
	rm.loadTexture("Texture/Menu/circleSlider.png");
	rm.loadTexture("Texture/Menu/slider1.png");
	rm.loadTexture("Texture/Menu/editorTurret.png");
	rm.loadTexture("Texture/Menu/editorSkeleton.png");
	rm.loadTexture("Texture/Menu/editorEngine.png");
	rm.loadTexture("Texture/Menu/menuPlanet.png");
	rm.loadTexture("Texture/Menu/menuSpace1.png");
	rm.loadTexture("Texture/Menu/menuLogo.png");
	
	//EFFECTS

	//BACKGROUNDS
	rm.loadTexture("Texture/Background/sky.png");

	//MISC
	rm.loadTexture("Texture/Misc/bullet1.png");
	rm.loadTexture("Texture/Misc/debris1.png");
	rm.loadTexture("Texture/Misc/ball.png");
	rm.loadTexture("Texture/Misc/pointer_arrow.png");



	//SOUND BUFFERS
	



	std::cout << "\nResourceManager intialized";
}




//Returns a random integer between min and max
int irandom(int min, int max)
{
	return (min + (rand() % (int)(max - min + 1)));
}
//Return true with a chance of 50%
bool flipCoin()
{
	switch (irandom(0, 1))
	{
	default:
		return true;
	case 1:
		return  false;
	}
}
//Returns true with a chance of 1/sides
bool rollDie(unsigned int sides)
{
	switch (irandom(0, sides - 1))
	{
	case 0:
		return true;
	default:
		return false;
	}

}
double getDistance(double x1, double y1, double x2, double y2)
{
	int xDiff = abs(x1 - x2);
	int yDiff = abs(y1 - y2);

	return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

bool testInput(MyKeys k, sf::Event& mEvent)
{
	//Mouse
	if (k.inputType == mouseInput && sf::Mouse::isButtonPressed(k.mouseButton) && k.wheelInput == noAxis)
		return true;
	else if (k.inputType == mouseInput && k.wheelInput != noAxis)
	{
		if (mEvent.type == sf::Event::MouseWheelMoved)
			if ((mEvent.mouseWheel.delta > 0 && k.wheelInput == positiveAxis) || (mEvent.mouseWheel.delta < 0 && k.wheelInput == negativeAxis))
			{
				mEvent.mouseWheel.delta = 0;
				return true;
			}
		return false;		
	}
	else if (k.inputType == mouseInput)
		return false;
	
	//Keyboard
	if (k.inputType == keyboardInput && sf::Keyboard::isKeyPressed(k.keyCode))
		return (true);
	else if (k.inputType == keyboardInput)
		return false;
	
	//Joystick
	if (sf::Joystick::isConnected(k.joystickIndex) == false)
		return false;
	if (k.inputType == joystickInput && ((k.axisType == positiveAxis && sf::Joystick::getAxisPosition(k.joystickIndex, k.joystickAxis) > k.threshold) || (k.axisType == negativeAxis && sf::Joystick::getAxisPosition(k.joystickIndex, k.joystickAxis) < -k.threshold) || (k.axisType == noAxis && sf::Joystick::isButtonPressed(k.joystickIndex, k.joystickButton))))
		return true;


	return (false);
}

//Returns the amount cut-out from the zoomFactor
float limitZoom()
{
	float cutValue = 0;
	if (zoomFactor < 0.2)
	{
		cutValue += 0.2 - zoomFactor;
		zoomFactor = 0.2;
	}
	else if (zoomFactor > 4)
	{
		cutValue += zoomFactor - 4;
		zoomFactor = 4;
	}
	return cutValue;
}


std::string getBoolAsString(bool b)
{
	if (b)
		return "True";
	else
		return "False";
}

void flipBool(bool& b)
{
	if (b == true)
		b = false;
	else
		b = true;
}

bool loadSettings()
{
	std::cout << "\nLoading settings...";
	std::ifstream load("Settings/Settings.txt");
	if (!load.fail())
	{
		std::string temp_str;
		load >> temp_str;
		if (temp_str == "fullscreen")
			FULLSCREEN = true;
		else
			FULLSCREEN = false;

		load >> WINDOW_WIDTH;
		load >> WINDOW_HEIGHT;
		load >> MUSIC_VOLUME;
		load >> SFX_VOLUME;
		load.close();
		std::cout << " Success";
		resFactor = WINDOW_HEIGHT / 1080.0;
		return true;
	}
	else
		std::cout << " Failed to load settings.txt";
	return false;
}
void saveSettings()
{
	std::cout << "\nSaving settings...";
	std::ofstream save("Settings/Settings.txt");
	if (!save.fail())
	{
		if (FULLSCREEN == true)
			save << "fullscreen" << "\n";
		else
			save << "window" << "\n";

		save << WINDOW_WIDTH << "\n";
		save << WINDOW_HEIGHT << "\n";
		save << MUSIC_VOLUME << "\n";
		save << SFX_VOLUME;
		save.close();

		std::cout << " Success";
		resFactor = WINDOW_HEIGHT / 1080.0;
	}
	else
		std::cout << " Failed to save settings.txt";
}







void extractNamesFromString(std::string& string, std::vector<std::string>& vector)
{
	if (string.size() > 0)
	{
		int temp_scheme_index = 0;
		vector.push_back("");
		for (unsigned int i = 0; i < string.size(); i++)
		{
			if (string[i] != '#')
				vector[temp_scheme_index] += string[i];
			else if (i < string.size() - 1)
			{
				vector.push_back("");
				temp_scheme_index++;
			}
		}
	}


	//Remove unwanted names
	for (unsigned int i = 0; i < vector.size(); i++)
		if (vector[i].size() < 2)
		{
		vector.erase(vector.begin() + i);
		i--;
		}
}


//Writes elements from vector seperated by '#' into .txt, path given in the first paramaeter
void writeVectorIntoFile(std::vector<std::string>& vector, std::string path)
{
	std::fstream mFileStream;
	mFileStream.open(path, std::ios::out);
	if (!mFileStream.fail())
	{
		mFileStream.clear();
		mFileStream.seekg(0, std::ios::beg);
		for (unsigned int i = 0; i < vector.size(); i++)
		{
			mFileStream << vector[i] << "#";
		}
		//Use the null terminator to purge all the unholy lines
		mFileStream << "###\0";
	}

	mFileStream.close();
}


char getUserInput(sf::Event& eventRef)
{
	switch (eventRef.key.code)
	{
	default:
		return '#';
	case sf::Keyboard::Q:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Q';
		else
			return 'q';
	case sf::Keyboard::W:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'W';
		else
			return 'w';
	case sf::Keyboard::E:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'E';
		else
			return 'e';
	case sf::Keyboard::R:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'R';
		else
			return 'r';
	case sf::Keyboard::T:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'T';
		else
			return 't';
	case sf::Keyboard::Y:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Y';
		else
			return 'y';
	case sf::Keyboard::U:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'U';
		else
			return 'u';
	case sf::Keyboard::I:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'I';
		else
			return 'i';
	case sf::Keyboard::O:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'O';
		else
			return 'o';
	case sf::Keyboard::P:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'P';
		else
			return 'p';
	case sf::Keyboard::A:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'A';
		else
			return 'a';
	case sf::Keyboard::S:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'S';
		else
			return 's';
	case sf::Keyboard::D:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'D';
		else
			return 'd';
	case sf::Keyboard::F:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'F';
		else
			return 'f';
	case sf::Keyboard::G:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'G';
		else
			return 'g';
	case sf::Keyboard::H:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'H';
		else
			return 'h';
	case sf::Keyboard::J:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'J';
		else
			return 'j';
	case sf::Keyboard::K:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'K';
		else
			return 'k';
	case sf::Keyboard::L:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'L';
		else
			return 'l';
	case sf::Keyboard::Z:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'Z';
		else
			return 'z';
	case sf::Keyboard::X:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'X';
		else
			return 'x';
	case sf::Keyboard::C:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'C';
		else
			return 'c';
	case sf::Keyboard::V:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'V';
		else
			return 'v';
	case sf::Keyboard::B:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'B';
		else
			return 'b';
	case sf::Keyboard::N:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'N';
		else
			return 'n';
	case sf::Keyboard::M:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			return 'M';
		else
			return 'm';
	case sf::Keyboard::Num1:
		return '1';
	case sf::Keyboard::Num2:
		return '2';
	case sf::Keyboard::Num3:
		return '3';
	case sf::Keyboard::Num4:
		return '4';
	case sf::Keyboard::Num5:
		return '5';
	case sf::Keyboard::Num6:
		return '6';
	case sf::Keyboard::Num7:
		return '7';
	case sf::Keyboard::Num8:
		return '8';
	case sf::Keyboard::Num9:
		return '9';
	case sf::Keyboard::Num0:
		return '0';
	case sf::Keyboard::BackSpace:
		return '-';
		//case sf::Keyboard::Space:
		//	return '+';
	case sf::Keyboard::Return:
		return '>';
	case sf::Keyboard::Escape:
		return '<';
	}
}