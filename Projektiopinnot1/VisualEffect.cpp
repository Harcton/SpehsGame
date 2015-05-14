#include "Main.h"
#include "VisualEffect.h"
#include "Game.h"
#include "Object.h"

VisualEffect::~VisualEffect()
{}
VisualEffect::VisualEffect(sf::Texture& t, float mx, float my, int frameduration, int frames) : sge::Sprite(t)
{
	x = mx;
	y = my;
	frameDuration = frameduration;
	animationTimer = frameduration;
	lifespan = frameduration*frames;
}


void VisualEffect::draw(sf::RenderWindow& window, Game* game)
{
	lifespan--;
	screenX = game->playerObj->screenX + resFactor*zoomFactor*(x - game->playerObj->x);
	screenY = game->playerObj->screenY + resFactor*zoomFactor*(y - game->playerObj->y);

	setPosition(screenX, screenY);
	sprite.setScale(xScale*resFactor*zoomFactor, xScale*resFactor*zoomFactor);

	sge::Sprite::draw(window);
}

void VisualEffect::setScale(double sx, double sy)
{
	xScale = sx;
	yScale = sy;
}

void VisualEffect::setFrameSize(int w, int h)
{
	sge::Sprite::setFrameSize(w, h);
	setFrame(0, 0);
}
