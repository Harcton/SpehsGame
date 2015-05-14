#ifndef VISUALEFFECT_H
#define VISUALEFFECT_H

class Game;
#include "Sprite.h"

class VisualEffect : public sge::Sprite
{
public:
	VisualEffect(sf::Texture& tex, float mx, float my, int frameduration, int frames);
	~VisualEffect();
	friend class Game;

	void draw(sf::RenderWindow&, Game*);
	void setFrameSize(int, int);
	void setScale(double, double);

private:
	int lifespan = 0;
	int frameCount = 0;
	int frameIndex = 0;
	float x;
	float y;
	float screenX;
	float screenY;
	float xScale = 1;
	float yScale = 1;
};

#endif