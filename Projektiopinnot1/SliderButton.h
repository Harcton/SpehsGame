#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H



class SliderButton : public Button
{
public:
	SliderButton(ButtonId, float, float, int, int, std::string, int, sf::Font*, sf::Color, sf::Color, sf::Color, int*);
	~SliderButton();

	void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);

	int sliderState = 40;
private:
	sf::RectangleShape slider;
	int sliderWidth = 0;
	std::string originalButtonString;
	int* targetVariable;
};

#endif