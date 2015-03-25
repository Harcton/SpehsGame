#include "Main.h"
#include "Button.h"


Button::~Button()
{
}
Button::Button(ButtonId bid, float x_pos, float y_pos, int wth, int hht, std::string str, int txtsize, sf::Font& fnt, sf::Color color, sf::Color textColor) : font(fnt)
{
	id = bid;
	text.setFont(font);
	text.setCharacterSize(txtsize);
	text.setString(str);
	text.setColor(textColor);
	red = color.r;
	green = color.g;
	blue = color.b;
	buttonRectangle.setFillColor(sf::Color(red, green, blue));
	buttonRectangle.setSize(sf::Vector2f(wth, hht));
	buttonRectangle.setOutlineColor(sf::Color(sf::Color::Black)); 
	buttonRectangle.setPosition(x_pos*resFactor, y_pos*resFactor);
}

bool Button::mouseOverlap(sf::Vector2i& mousePos)
{
	if (mousePos.x > buttonRectangle.getPosition().x &&
		mousePos.x < buttonRectangle.getPosition().x + buttonRectangle.getSize().x &&
		mousePos.y >  buttonRectangle.getPosition().y &&
		mousePos.y <  buttonRectangle.getPosition().y + buttonRectangle.getSize().y)
		return true;
	else
		return false;
}

ButtonId Button::checkIfPressed(sf::Vector2i& mousePos)
{
	if (mouseOverlap(mousePos))
		return id;
	else
		return bi_false;
}

void Button::draw(sf::RenderWindow& window, sf::Vector2i& mousePos)
{
	//Set color
	if (mouseOverlap(mousePos))
		buttonRectangle.setFillColor(sf::Color(red-20, green-20, blue-20));
	else
		buttonRectangle.setFillColor(sf::Color(red, green, blue));

	window.draw(buttonRectangle);
	window.draw(text);
}
