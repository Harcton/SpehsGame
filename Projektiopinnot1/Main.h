#ifndef MAIN_H
#define MAIN_H
//Included dependencies
#include <iostream>
#include "ResourceManager.h"
#include "MyKeys.h"
#include <SFML/Window/Event.hpp>


#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
//
//#include <vector>
//#include <math.h>
//#include <cmath>
//#include <time.h>
//#include <map>



#define PI 3.14159265
#define SPAWN_RANGE 2800
#define DESPAWN_RANGE 4000
#define MAX_OBJECTS 200
#define MAX_BULLETS 500

#define EDITOR_WIDTH 41
#define EDITOR_HEIGHT 41

//Ship core keys
#define key_turnRight 0.2
#define key_turnLeft 0.3
#define key_accelerate 0.4
#define key_reverse 0.5
#define key_zoomIn 0.6
#define key_zoomOut 0.7
//Component specific key values
#define KEYB_fire 0.0001
#define KEYB_thrust 0.0002
#define KEYB_reload 0.0003
#define KEYB_vertical 0.0004
#define KEYB_horizontal 0.0005
#define KEYB_right 0.0006
#define KEYB_left 0.0007
#define kgrid_fire 0
#define kgrid_thrust 1
#define kgrid_reload 2
#define kgrid_vertical 3
#define kgrid_horizontal 4
#define kgrid_right 5
#define kgrid_left 6

extern bool FULLSCREEN;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int MUSIC_VOLUME;
extern int SFX_VOLUME;
extern double resFactor;
extern double zoomFactor;
extern std::string playerName;

extern ResourceManager RM;

double getDistance(double, double, double, double);
int irandom(int min, int max);
bool flipCoin();
bool rollDie(unsigned int sides);
float limitZoom();
void flipBool(bool&);
std::string getBoolAsString(bool b);
//File handling
void extractNamesFromString(std::string&, std::vector<std::string>&);
void writeVectorIntoFile(std::vector<std::string>&, std::string);
char getUserInput(sf::Event& eventRef);
bool loadSettings();
void saveSettings();

template <typename T>
T limitWithin(T min, T value, T max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}






//extern std::map<float, MyKeys> coreKeys;
//extern std::map<float, MyKeys> componentKeys;
extern bool starMap;
extern float radarRange;
extern float radarSpeed;
extern float movementSpeed;
extern float rotationSpeed;
bool testInput(MyKeys k, sf::Event&);





#endif