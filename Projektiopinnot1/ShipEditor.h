#ifndef SHIPEDITOR_H
#define SHIPEDITOR_H

namespace editor
{
	enum Focus
	{
		base,
		component,
		actions,
		configuration
	};
}


class Button;

class ShipEditor
{
public:
	~ShipEditor();
	ShipEditor(sf::RenderWindow&, PlayerData&);

	void run();
	void updateLines();
	void updateGridSpriteTextures();
	void updateGridSpriteLocations();
	void updateMouseGrab();

	void rotateGlowAngle();
	void drawSelectedRect();
	void drawSelectionShadeHighlight();
	void drawInheritanceSprites();
	void drawActions();
	void scrapComponent(int, int);

	//Input
	void mouseLeftPressed();
	void mouseRightPressed();
	void zoom(int);

private:
	PlayerData& playerData;
	sf::RenderWindow& mWindow;
	float scaleFactor;
	float glowAngle = 0; //What it does is... spin around in the game loop. Can be used to apply glowing effects
	float glowAmount = 0; // 0.0 - 1.0

	float cameraX = 0; //(EDITOR_WIDTH / 2) * 100;
	float cameraY = 0; //(EDITOR_HEIGHT / 2) * 100;

	sf::Vector2i mousePos;
	bool mouseGrab = false;
	float mouseGrabX = 0;
	float mouseGrabY = 0;
	float grabCameraOriginX = 0;
	float grabCameraOriginY = 0;
	int checkX;
	int checkY;

	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<sf::Sprite> inheritanceSprites[EDITOR_WIDTH][EDITOR_HEIGHT];
	std::vector<Button> actionButtons;

	editor::Focus focus = editor::base;
	int selectedX = -1;
	int selectedY = -1;
	sf::RectangleShape selectedRect;
	sf::RectangleShape shadeRect;

	sf::Texture inheritanceArrowTex;

};

#endif