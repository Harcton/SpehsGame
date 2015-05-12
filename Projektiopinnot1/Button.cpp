#include "Main.h"
#include "Button.h"


Button::~Button()
{
}
Button::Button(ButtonId bid, float x_pos, float y_pos, int wth, int hht, std::string str, int txtsize, sf::Color color, sf::Color textColor)
{//Text button constructor
	type = bt_text;
	id = bid;
	text.setFont(RM.menuFont);
	text.setCharacterSize(txtsize);
	text.setString(str);
	text.setColor(textColor);
	red = color.r;
	green = color.g;
	blue = color.b;
	opacity = color.a;
	buttonRectangle.setFillColor(sf::Color(red, green, blue, opacity));
	buttonRectangle.setSize(sf::Vector2f(wth, hht));
	//buttonRectangle.setOutlineColor(sf::Color(sf::Color::Black));
	setPosition(x_pos, y_pos);
}
Button::Button(ButtonId bid, float x_pos, float y_pos, sf::Texture& tex, float scale)
{//Sprite button constructor
	type = bt_sprite;
	id = bid;
	spr.setTexture(tex);
	//spr.setTextureRect(sf::IntRect(0, 0, 25, 25));
	spr.setPosition(x_pos, y_pos);
	spr.setScale(scale*resFactor, scale*resFactor);
}

bool Button::mouseOverlap(sf::Vector2i& mousePos)
{
	if (id == bi_false)
		return false;
	bool returnValue = false;

	if (type == bt_text)
	{
		if (mousePos.x > buttonRectangle.getPosition().x &&
			mousePos.x < buttonRectangle.getPosition().x + buttonRectangle.getGlobalBounds().width &&
			mousePos.y >  buttonRectangle.getPosition().y &&
			mousePos.y < buttonRectangle.getPosition().y + buttonRectangle.getGlobalBounds().height)
		returnValue = true;
	}
	else if (type == bt_sprite)
	{
		if (mousePos.x > spr.getPosition().x &&
			mousePos.x < spr.getPosition().x + spr.getGlobalBounds().width &&
			mousePos.y >  spr.getPosition().y &&
			mousePos.y < spr.getPosition().y + spr.getGlobalBounds().height)
			returnValue = true;
	}


	if (returnValue == true && hoverSoundPlayed == false)
	{
		RM.menuButtonHoverSound.play();
		hoverSoundPlayed = true;
	}
	else if (returnValue == false)
		hoverSoundPlayed = false;

	return returnValue;
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
		if (selected == true && shadow > -20)
			shadow -= 2;
		else if (mouseOverlap(mousePos) && id != bi_false)
		{
			if (shadow < 20)
				shadow += 2;
			else if (shadow > 20)
				shadow -= 2;
		}
		else if (shadow > 0)
			shadow -= 2;
		else if (shadow < 0)
			shadow += 2;
		
		buttonRectangle.setFillColor(sf::Color(limitWithin(20, red - shadow, 255), limitWithin(20, green - shadow, 255), limitWithin(20, blue - shadow, 255), opacity));

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

void Button::setTextAlign(TextAlign ta)
{
	textAlign = ta;

	float temp_textX = buttonRectangle.getPosition().x;
	float temp_textY = buttonRectangle.getPosition().y; 
	float temp_buttonWidth = buttonRectangle.getLocalBounds().width;
	float temp_textWidth = text.getLocalBounds().width;
	switch (textAlign)
	{
	case ta_left:
		text.setPosition(int(temp_textX), int(temp_textY));
		break;
	case ta_center:
		text.setPosition(int(temp_textX + 0.5f * temp_buttonWidth - 0.5f * temp_textWidth), int(temp_textY));
		break;
	case ta_right:
		text.setPosition(int(temp_textX + temp_buttonWidth - temp_textWidth), int(temp_textY));
		break;
	}
}
void Button::setPosition(float x_pos, float y_pos)
{
	buttonRectangle.setPosition(x_pos, y_pos);
	spr.setPosition(x_pos, y_pos);
	text.setPosition(x_pos, y_pos);
	setTextAlign(textAlign);
}