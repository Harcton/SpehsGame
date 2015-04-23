#ifndef BUTTON_H
#define BUTTON_H

enum ButtonType
{
	bt_text,
	bt_sprite,
};


enum ButtonId
{
	bi_false,
	bi_true,
	bi_actionUpgradeArmor,
	bi_actionTurret,
	bi_actionEngine,
	bi_actionRotate,
	bi_actionConfiguration,
	bi_actionScrap,
	bi_actionSetTurretScheme,
	bi_actionTurretScheme,
	bi_actionSetCoreScheme,
	bi_actionCoreScheme,
	bi_actionSetJoystickIndex,

	bi_actionSetJoystick0,
	bi_actionSetJoystick1,
	bi_actionSetJoystick2,
	bi_actionSetJoystick3,
	bi_actionSetJoystick4,
	bi_actionSetJoystick5,
	bi_actionSetJoystick6,
	bi_actionSetJoystick7,

	bi_confExit,
	bi_confNodeDirectionalMovement,
	bi_confBindVerticalMoveAxis,
	bi_confBindHorizontalMoveAxis,
	bi_confBindAccelerate,
	bi_confBindTurnRight,
	bi_confBindTurnLeft,
	bi_confBindReverse,
	bi_confBindZoomIn,
	bi_confBindZoomOut,

	bi_confBindFire,
	bi_confNodeHoldToFire,
	bi_confBindReload,
	bi_confNodeMouseAim,
	bi_confNodeMouseAimRelative,
	bi_confNodeDirectionalAim,
	bi_confBindVerticalAimAxis,
	bi_confBindHorizontalAimAxis,
	bi_confBindTurnTurretRight,
	bi_confBindTurnTurretLeft,

	bi_confTurretControlScheme,
	bi_confSaveTurretScheme,
	bi_confLoadTurretScheme,
	bi_confCoreControlScheme,
	bi_confSaveCoreScheme,
	bi_confLoadCoreScheme,
	bi_confScrollUp,
	bi_confScrollDown,
	bi_confScrollBar,

	bi_confChangeJoystickIndexTo0,
	bi_confChangeJoystickIndexTo1,
	bi_confChangeJoystickIndexTo2,
	bi_confChangeJoystickIndexTo3,
	bi_confChangeJoystickIndexTo4,
	bi_confChangeJoystickIndexTo5,
	bi_confChangeJoystickIndexTo6,
	bi_confChangeJoystickIndexTo7,

	bi_confRedSlider,
	bi_confGreenSlider,
	bi_confBlueSlider,

	bi_confBindThrust,
	bi_confNodeHoldToThrust,
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