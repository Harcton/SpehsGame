#ifndef BACKGROUND_H
#define BACKGROUND_H
//Included dependencies
#include "Object.h"



class Background : public Object
{
public:
	Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>&);
	~Background();

	bool updateBackground();

private:
	std::vector<Background*>& homeVector; //this doesn't do anything I think
	double distanceFromPlayer;
};

#endif