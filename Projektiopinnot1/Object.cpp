#include "Main.h"
#include "Game.h"
#include "Object.h"

Object::~Object()
{
}
Object::Object(sf::RenderWindow& windowref, Game* game) : mWindow(windowref)
{
	mGame = game;
	centerObj = game->playerObj;
	scale = 1;

	//Randomize x/y
	if (flipCoin())
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		x = centerObj->x + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	if (flipCoin())
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);
	else
		y = centerObj->y + irandom(-SPAWN_RANGE, SPAWN_RANGE);

	spr.setPosition(x, y);
}
Object::Object(sf::RenderWindow& windowref, Game* game, int cx, int cy) : mWindow(windowref)
{
	mGame = game;
	centerObj = mGame->playerObj;
	x = cx;
	y = cy;
	spr.setPosition(x, y);
}

void Object::update()
{
	//update opacity
	if (opacity < 255)
	{
		opacity += 3;
		spr.setColor(sf::Color(255, 255, 255, opacity));
	}

	//Update variable values
	angle += turnSpeed;
	x += xSpeed;
	y += ySpeed;

	//Keep angle positi
	if (angle > 2*PI)
		angle -= 2*PI;
	else if (angle < 0)
		angle += 2*PI;




	//Update screen positions
	if (centerObj != this) //If the object is not the player
	{
		screenX = centerObj->screenX +(x - centerObj->x);
		screenY = centerObj->screenY + (y - centerObj->y);
	}
	else
	{//Player update, make the player object appear in the center of the screen
		screenX = WINDOW_WIDTH / 2;
		screenY = WINDOW_HEIGHT / 2;
	}



	//Apply variables
	spr.setPosition(screenX, screenY);
	spr.setScale(scale, scale);
	spr.setRotation((angle/PI)*180);

	//if (centerObj != this)
	//	std::cout << "\nObject distance to player: " << getDistance(x, y, centerObj->x, centerObj->y);

}

void Object::draw()
{


	mWindow.draw(spr);
}

void Object::checkCollisions()
{
	for (unsigned int i = 0; i < mGame->objects.size(); i++)
		if (mGame->objects[i] != this)
		{
			float anglerad = atan2(y - mGame->objects[i]->y, x - mGame->objects[i]->x) * (180 / PI);
			std::cout << "\n" << anglerad;
			double distance = getDistance(x, y, mGame->objects[i]->x, mGame->objects[i]->y);
			double collisionRange = textureRadius + mGame->objects[i]->textureRadius;
			if (distance < collisionRange)
			{


			}
		}
}