#include "Main.h"
#include "Game.h"
#include "Object.h"

#include <math.h>

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
}

bool Object::update()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE || hp <= 0)
		return false;

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

	//Keep angle positi
	if (angle > 2*PI)
		angle -= 2*PI;
	else if (angle < 0)
		angle += 2*PI;

	//acceleration timer
	if (accTimer <= 1)
	{
		accTimer++;
	}
	else
		accTimer = 0;
	
	//acceleration
	if (accTimer == 1)
	{
		xSpeed0 = xSpeed;
		ySpeed0 = ySpeed;
	}
	xAcc = (xSpeed / xSpeed0);
	yAcc = (ySpeed / ySpeed0);
	
	//REALTIVESPEED VALUES WOBBLE TOO MUCH - FIX!!!!!

	//scrSpeeds
	//Check if ship is accelerating, limit and set scrSpeed
	if (centerObj->xAcc == 1 || centerObj->xAcc == -1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedX = 0.99 * scrSpeedX;
			relativeSpeedX = 0.99 * relativeSpeedX;
		}
	}
	else
	{
		scrSpeedX += relativeSpeedX * (abs(relativeSpeedX));
		if (screenY>WINDOW_WIDTH / 2)
			relativeSpeedX -= scrSpeedX / 4000;
	}
	if (centerObj->yAcc == 1 || centerObj->yAcc == -1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedY = 0.99 * scrSpeedY;
			relativeSpeedY = 0.99 * relativeSpeedY;
		}
	}
	else
	{
		scrSpeedY += relativeSpeedY * (abs(relativeSpeedY));
		if (screenY>WINDOW_HEIGHT/2)
			relativeSpeedY -= scrSpeedY / 2000;
	}
	
	//Update screen positions
	if (centerObj != this) //If the object is not the player
	{
		screenX = centerObj->screenX + resFactor*zoomFactor*(x - centerObj->x);
		screenY = centerObj->screenY + resFactor*zoomFactor*(y - centerObj->y);
	}
	else
	{//Player update, make the player object appear in the center of the screen
		screenX = WINDOW_WIDTH / 2 - (scrSpeedX);
		screenY = WINDOW_HEIGHT / 2 - (scrSpeedY);
	}

	//Apply variables
	spr.setPosition(screenX, screenY);
	spr.setScale(scale*resFactor*zoomFactor, scale*resFactor*zoomFactor);
	spr.setRotation(360 - (angle/PI)*180);
	return true;
}

void Object::draw()
{
	mWindow.draw(spr);
}

void Object::checkCollisions(unsigned int selfIndex)
{
	for (unsigned int i = 0; i < mGame->objects.size(); i++)
		if (i != selfIndex)
		{
			collisionCheckAngle = -1 * atan2(mGame->objects[i]->y - y, mGame->objects[i]->x - x);
			if (collisionCheckAngle < 0)
				collisionCheckAngle = ((2 * PI) + collisionCheckAngle);


			checkCollisionDistance = getDistance(x, y, mGame->objects[i]->x, mGame->objects[i]->y);
			checkCollisionRange = textureRadius + mGame->objects[i]->textureRadius;

			if (checkCollisionDistance < checkCollisionRange)
			{
				double ys;
				if (ySpeed > 0)
					ys = 1 + ySpeed;
				else
					ys = 1 - ySpeed;
				ySpeed += 0.001*sin(collisionCheckAngle)*ys;
				y += 3 * sin(collisionCheckAngle);

				double xs;
				if (xSpeed > 0)
					xs = 1 - xSpeed;
				else
					xs = 1 + xSpeed;
				xSpeed += 0.001*cos(collisionCheckAngle)*xs;
				x += -3 * cos(collisionCheckAngle);
			}
		}
}