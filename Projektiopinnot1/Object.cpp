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
		opacity += 17;
		spr.setColor(sf::Color(255, 255, 255, opacity));
	}

	//Update variable values
	angle += turnSpeed;
	x += xSpeed;
	y += ySpeed;

	if (hasCollisions)
		checkCollisions();

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
	spr.setRotation(360 - (angle/PI)*180);

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

			double distance = getDistance(x, y, mGame->objects[i]->x, mGame->objects[i]->y);
			double collisionRange = textureRadius + mGame->objects[i]->textureRadius;
			if (distance < collisionRange)
			{
				float anglerad = -1 * atan2(mGame->objects[i]->y - y, mGame->objects[i]->x - x);
				if (anglerad < 0)
					anglerad = ((2 * PI) + anglerad);


				//std::cout << "\n" << anglerad;
				//std::cout << "\ncos: " << cos(anglerad);


				double ys;
				if (ySpeed > 0)
					ys = 1 + ySpeed;
				else
					ys = 1 - ySpeed;
				ySpeed += 0.1*sin(anglerad)*mGame->objects[i]->mass*ys;
				y += 3 * sin(anglerad);

				double xs;
				if (xSpeed > 0)
					xs = 1 + xSpeed;
				else
					xs = 1 - xSpeed;
				xSpeed += 0.1*cos(anglerad)*mGame->objects[i]->mass*xs;
				x += -3*cos(anglerad);


			}
		}
}