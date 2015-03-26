#ifndef BUTTON_H
#define BUTTON_H

/*
The constructor parameters are:
button id (use bi_true if not using id system)
x, y
width, height
string, string size, font&
box color, text color
*/
class Button
{
public:
	~Button();
	Button(ButtonId, float, float, int, int, std::string, int, sf::Font&, sf::Color, sf::Color);
	friend class ShipEditor;

	bool mouseOverlap(sf::Vector2i& mousePos);
	ButtonId checkIfPressed(sf::Vector2i& mousePos);
	void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);

private:
	ButtonId id;
	sf::RectangleShape buttonRectangle;
	sf::Text text;
	sf::Font& font;

	//Rectangle original colors
	int red;
	int green;
	int blue;

};

#endif