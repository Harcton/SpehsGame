#ifndef MAIN_H
#define MAIN_H
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <iostream>
#include <math.h>
#include <cmath>
#include <time.h>


#define PI 3.14159265
#define SPAWN_RANGE 500
#define DESPAWN_RANGE 1000

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

double getDistance(double, double, double, double);
int irandom(int min, int max);
bool flipCoin();
bool rollDie(unsigned int sides);


#endif