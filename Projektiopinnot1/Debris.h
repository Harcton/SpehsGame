#ifndef DEBRIS_H
#define DEBRIS_H



class Debris : public Object
{
public:
	~Debris();
	Debris(sf::RenderWindow& windowref, Game* game);
	Debris(sf::RenderWindow& windowref, Game* game, int cx, int cy);

	void randomizeLook();
};

#endif