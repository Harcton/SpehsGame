#include "Main.h"
#include "UpgradeButton.h"


UpgradeButton::~UpgradeButton()
{
}
UpgradeButton::UpgradeButton(ButtonId bid, int mx, int my, std::string str, short int* target, int max)
	: upgrade(Button(bid, mx + upgradeButtonWidth - upgrade_width, my, upgrade_width, upgradeButtonHeight, "Upgrade", int(36 * resFactor), sf::Color(120, 120, 125), sf::Color(35, 35, 40)))
{
	targetInt = target;
	maxLevel = max;
	upgrade.setTextAlign(ta_center);
	type = bt_text;
	id = bi_false;
	x = mx;
	y = my;
	id = bid;
	text.setFont(RM.menuFont);
	text.setCharacterSize(48*resFactor);
	text.setString(str);
	text.setColor(sf::Color(20,20,20));
	text.setPosition(sf::Vector2f(mx, my));
	buttonRectangle.setFillColor(sf::Color(95, 95, 100));
	buttonRectangle.setSize(sf::Vector2f(upgradeButtonWidth, upgradeButtonHeight));
	buttonRectangle.setPosition(x, y);

	//Level indicators
	float indicatorWidth = 20 * resFactor;
	float indicatorHeight = 10 * resFactor;
	float indicatorBorder = 7.5 * resFactor;
	float indicatorDistanceFromEachOther;
	if (maxLevel != 1)
		indicatorDistanceFromEachOther = (upgrade_width - 2 * indicatorBorder - maxLevel*indicatorWidth) / float(maxLevel - 1);
	if (indicatorDistanceFromEachOther < 0)
		indicatorDistanceFromEachOther = 0;
	float indicator1X1 = mx + upgradeButtonWidth - upgrade_width + indicatorBorder;

	for (unsigned int i = 0; i < max; i++)
	{
		levelIndicators.push_back(sf::RectangleShape(sf::Vector2f(indicatorWidth, indicatorHeight)));
		levelIndicators.back().setPosition(sf::Vector2f(indicator1X1 + indicatorWidth*i + indicatorDistanceFromEachOther*i, my + upgradeButtonHeight - indicatorBorder - indicatorHeight));
	}
}



bool UpgradeButton::mouseOverlap(sf::Vector2i& mousePos)
{
	if (mousePos.x > buttonRectangle.getPosition().x &&
		mousePos.x < buttonRectangle.getPosition().x + buttonRectangle.getGlobalBounds().width &&
		mousePos.y >  buttonRectangle.getPosition().y &&
		mousePos.y < buttonRectangle.getPosition().y + buttonRectangle.getGlobalBounds().height)
		return true;
	else
		return false;
}
ButtonId UpgradeButton::checkIfPressed(sf::Vector2i& mousePos)
{
	return upgrade.checkIfPressed(mousePos);
}
void UpgradeButton::draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos)
{

	window.draw(buttonRectangle);
	window.draw(text);

	upgrade.draw(window, mouse_pos);
	for (unsigned int i = 0; i < levelIndicators.size(); i++)
		window.draw(levelIndicators[i]);
}

void UpgradeButton::updateIndicators()
{
	for (unsigned int i = 0; i < levelIndicators.size(); i++)
		if (targetInt != nullptr)
	{
		if (*targetInt < i + 1)
			levelIndicators[i].setFillColor(sf::Color(80, 80, 80));
		else
			levelIndicators[i].setFillColor(sf::Color(255, 200, 100));
	}
}
