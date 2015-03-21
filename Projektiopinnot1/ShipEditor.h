#ifndef SHIPEDITOR_H
#define SHIPEDITOR_H



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

private:
	PlayerData& playerData;
	sf::RenderWindow& mWindow;

	float cameraX = 0; //(EDITOR_WIDTH / 2) * 100;
	float cameraY = 0; //(EDITOR_HEIGHT / 2) * 100;

	sf::Vector2i mousePos;
	bool mouseGrab = false;
	float mouseGrabX = 0;
	float mouseGrabY = 0;
	float grabCameraOriginX = 0;
	float grabCameraOriginY = 0;

	std::vector<sf::VertexArray> horizontalLines;
	std::vector<sf::VertexArray> verticalLines;
	std::vector<sf::Sprite> gridSprites[EDITOR_WIDTH][EDITOR_HEIGHT];


};

#endif