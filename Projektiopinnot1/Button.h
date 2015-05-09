#ifndef BUTTON_H
#define BUTTON_H
//Forward declared dependencies
//Included dependencies
#include <string>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>




enum ButtonType
{
	bt_text,
	bt_sprite,
};


enum ButtonId
{
	bi_false,
	bi_true,
	//Actions popup
	bi_actionUpgrade,
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



	//Main menu buttons
	//Common
	bi_mmReturn,
	//Title screen
	bi_mmNewGame,
	bi_mmLoadGame,
	bi_mmSettings,
	bi_mmQuit,
	//Load screen
	bi_mmLoadSave,
	bi_mmSaveListElement,
	bi_mmDeleteSave,
	//Settings screen
	bi_setMusicSlider,
	bi_setSoundEffectSlider,
	bi_setFullscreenNode,
	bi_setWindowWidth,
	bi_setWindowHeight,


	//Ingame settings
	bi_gsetReturn,
	bi_gsetMusicVolume,
	bi_gsetSoundVolume,
	bi_gsetQuit,


	//SHIP EDITOR
	bi_escReturn,
	bi_escUndock,
	bi_escSave,
	bi_escSettings,
	bi_escQuit,
	//Configuration screen buttons
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

	bi_confBindThrust,
	bi_confNodeHoldToThrust,
	bi_confNodeEngineMode,

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

//Upgrades
	//Armor
	bi_upgradeArmor,
	//Turret upgrades
	bi_upgradeTurretDamage,
	bi_upgradeTurretAngle,
	bi_upgradeTurretTurnSpeed,
	bi_upgradeTurretMagazine,
	bi_upgradeTurretReloadSpeed,
	bi_upgradeTurretBulletSpeed,
	bi_upgradeTurretRecoilTime,

	//Engine upgrades
	bi_upgradeEngineStrength,
	bi_upgradeEngineCapacity,
	bi_upgradeEngineRechargeRate,


};

enum TextAlign
{
	ta_left, ta_center, ta_right
};

class Button
{
public:
	~Button();
	Button(){}
/*
The 1st constructor parameters are:
button id (use bi_true if not using id system)
x, y
width, height
string, string size, font*
box color, text color
----
The 2nd constructor parameters are:
button id (use bi_true if not using id system)
x, y
texture path, texture scale
*/
	Button(ButtonId, float, float, int, int, std::string, int, sf::Color, sf::Color);
	Button(ButtonId, float, float, sf::Texture&, float);
	friend class ShipEditor;
	friend class MainMenu;
	friend class Game;
	friend class UpgradeButton;

	virtual bool mouseOverlap(sf::Vector2i& mousePos);
	virtual ButtonId checkIfPressed(sf::Vector2i& mousePos);
	virtual void draw(sf::RenderWindow& window, sf::Vector2i& mouse_pos);
	void setPosition(float, float);
	void setTextAlign(TextAlign);

protected:
	ButtonType type;
	ButtonId id;
	sf::RectangleShape buttonRectangle;
	sf::Text text;
	TextAlign textAlign = ta_left;
	sf::Sprite spr;
	bool visible = true;
	bool selected = false;

	int shadow = 0;
	//Rectangle original colors
	int red;
	int green;
	int blue;
	int opacity;

};

#endif