#pragma once
#ifndef UPGRADEBUTTON_H
#define UPGRADEBUTTON_H
#include "Button.h"

#define upgradeButtonWidth int(550*resFactor)
#define upgradeButtonHeight int(60*resFactor)
#define upgrade_width int(200*resFactor)


class UpgradeButton : public Button
{
public:
	~UpgradeButton();
	UpgradeButton(ButtonId, int, int, std::string, short int*, int);
	friend class ShipEditor;

	void updateIndicators();
	void setPosition(float, float);

	bool mouseOverlap(sf::Vector2i& mousePos);
	ButtonId checkIfPressed(sf::Vector2i& mousePos);
	void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);

private:
	std::vector<sf::RectangleShape> levelIndicators;
	Button upgrade;
	int x;
	int y;
	int maxLevel;
	short int* targetInt;
};

#endif