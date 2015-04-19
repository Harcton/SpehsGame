#ifndef BUTTON_H
#define BUTTON_H

enum ButtonType
{
	bt_text,
	bt_sprite,
};

class Button
{
public:
	~Button();
/*
The 1st constructor parameters are:
button id (use bi_true if not using id system)
x, y
width, height
string, string size, font&
box color, text color
----
The 2nd constructor parameters are:
button id (use bi_true if not using id system)
x, y
texture path, texture scale
font reference... (not in use)
*/
	Button(ButtonId, float, float, int, int, std::string, int, sf::Font&, sf::Color, sf::Color);
	Button(ButtonId, float, float, sf::Texture&, float, sf::Font&);
	friend class ShipEditor;

	bool mouseOverlap(sf::Vector2i& mousePos);
	ButtonId checkIfPressed(sf::Vector2i& mousePos);
	void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);
	void setPosition(float, float);

private:
	ButtonType type;
	ButtonId id;
	sf::RectangleShape buttonRectangle;
	sf::Text text;
	sf::Font& font;
	sf::Sprite spr;
	bool visible = true;
	bool selected = false;

	//Rectangle original colors
	int red;
	int green;
	int blue;

};

#endif