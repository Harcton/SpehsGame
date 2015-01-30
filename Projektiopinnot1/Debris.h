#ifndef DEBRIS_H
#define DEBRIS_H



class Debris : public Object
{
public:
	~Debris();
	Debris(sf::RenderWindow& windowref, Game* game, std::vector<Debris*>& hw);
	Debris(sf::RenderWindow& windowref, Game* game, std::vector<Debris*>& hw, int cx, int cy);
	bool updateDebris();

	void randomizeLook();

protected:
	std::vector<Debris*>& homeVector;
};

#endif