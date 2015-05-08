#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H
//Included dependencies
#include "Button.h"


class SliderButton : public Button
{
public:
	SliderButton(ButtonId, float, float, int, int, std::string, int, sf::Color, sf::Color, sf::Color, int*);
	~SliderButton();
	friend class MainMenu;
	friend class Game;

	void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);
	void updateString();
	ButtonId checkIfPressed(sf::Vector2i& mousePos);

private:
	sf::RectangleShape slider;
	int sliderWidth = 0;
	std::string originalButtonString;
	int* targetVariable;
	int sliderState;
	bool hasFocus = false;
};

#endif