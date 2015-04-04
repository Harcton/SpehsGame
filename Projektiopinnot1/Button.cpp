#include "Main.h"
#include "Button.h"


Button::~Button()
{
}
Button::Button(ButtonId bid, float x_pos, float y_pos, int wth, int hht, std::string str, int txtsize, sf::Font& fnt, sf::Color color, sf::Color textColor) : font(fnt)
{
	type = bt_text;
	id = bid;
	text.setFont(font);
	text.setCharacterSize(txtsize);
	text.setString(str);
	text.setColor(textColor);
	text.setPosition(x_pos, y_pos);
	red = color.r;
	green = color.g;
	blue = color.b;
	buttonRectangle.setFillColor(sf::Color(red, green, blue));
	buttonRectangle.setSize(sf::Vector2f(wth, hht));
	buttonRectangle.setOutlineColor(sf::Color(sf::Color::Black));
	buttonRectangle.setPosition(x_pos, y_pos);
}
Button::Button(ButtonId bid, float x_pos, float y_pos, sf::Texture& tex, float scale, sf::Font& fnt) : font(fnt)
{
	type = bt_sprite;
	id = bid;
	//if (!tex.loadFromFile(path))
	//	std::cout << "\nCould not load Button texture: " << path;
	//else
	//	std::cout << "\nButton texture loaded succesfully: " << path;
	spr.setTexture(tex);
	spr.setTextureRect(sf::IntRect(0, 0, 25, 25));
	spr.setPosition(x_pos*resFactor, y_pos*resFactor);
	spr.setScale(scale*resFactor, scale*resFactor);


}

bool Button::mouseOverlap(sf::Vector2i& mousePos)
{
	if (id == bi_false)
		return false;

	if (type == bt_text)
	{
		if (mousePos.x > buttonRectangle.getPosition().x &&
			mousePos.x < buttonRectangle.getPosition().x + buttonRectangle.getGlobalBounds().width - 5 &&
			mousePos.y >  buttonRectangle.getPosition().y &&
			mousePos.y < buttonRectangle.getPosition().y + buttonRectangle.getGlobalBounds().height - 5)
			return true;
		else
			return false;
	}
	else if (type == bt_sprite)
	{
		if (mousePos.x > spr.getPosition().x &&
			mousePos.x < spr.getPosition().x + spr.getGlobalBounds().width &&
			mousePos.y >  spr.getPosition().y &&
			mousePos.y < spr.getPosition().y + spr.getGlobalBounds().height)
			return true;
		else
			return false;
	}
	return false;
}

ButtonId Button::checkIfPressed(sf::Vector2i& mousePos)
{
	if (visible == false)
		return bi_false;

	if (mouseOverlap(mousePos))
		return id;
	else
		return bi_false;
}

void Button::draw(sf::RenderWindow& window, sf::Vector2i& mousePos)
{
	if (visible == false)
		return;

	//Set color
	if (type == bt_text)
	{
	if (mouseOverlap(mousePos) && id != bi_false)
		buttonRectangle.setFillColor(sf::Color(red-20, green-20, blue-20));
	else
		buttonRectangle.setFillColor(sf::Color(red, green, blue));

	window.draw(buttonRectangle);
	window.draw(text);
	}
	else if (type == bt_sprite)
	{
		if (mouseOverlap(mousePos) && id != bi_false)
			spr.setColor(sf::Color(200, 200, 200));
		else
			spr.setColor(sf::Color(255, 255, 255));
		window.draw(spr);
	}

}

void Button::setPosition(float x_pos, float y_pos)
{
	buttonRectangle.setPosition(x_pos, y_pos);
	text.setPosition(x_pos, y_pos);
	spr.setPosition(x_pos, y_pos);
}