#ifndef DEBRIS_H
#define DEBRIS_H
//Included dependencies
#include "Object.h"

class Debris : public Object
{
public:
	~Debris();
	Debris(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& hw);
	Debris(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& hw, int cx, int cy);
	bool updateDebris();

	void randomizeLook();

protected:
	std::vector<Object*>& homeVector;
};

#endif