#ifndef BACKGROUND_H
#define BACKGROUND_H



class Background : public Object
{
public:
	Background(sf::RenderWindow& windowref, Game* game, std::vector<Background*>&);
	~Background();

	bool updateBackground();

private:
	std::vector<Background*>& homeVector;
};

#endif