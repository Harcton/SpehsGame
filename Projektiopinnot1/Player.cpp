#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"
#include "Component.h"
#include "Bullet.h"
#include "Turret.h"



Player::~Player()
{
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
}
Player::Player(sf::RenderWindow& windowref, Game* game, int cx, int cy) : Object(windowref, game, cx, cy)
{
	centerObj = this;

	tex.loadFromFile("Texture/ship_base.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;	

	components.push_back(new Turret(this, this, 30, 0));
}

bool Player::update()
{



	mWindow.pollEvent(mEvent);


	//Speed control
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		accelerate();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		reverse();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		turnLeft();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		turnRight();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;
	}

	updateComponents();

	for (unsigned int i = 0; i < components.size(); i++)
	{
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
		if (components[i]->types[k] == turret)
		{
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			//	components[i]->angle += 0.03;
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			//	components[i]->angle -= 0.03;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				components[i]->fire();
		}
	}


	Object::update();
	return true;
}

void Player::turnRight()
{
	turnSpeed -= PI / 1800;
}

void Player::turnLeft()
{
	turnSpeed += PI / 1800;
}

void Player::accelerate()
{
	xSpeed += cos(2*PI - angle)*0.04;
	ySpeed += sin(2 * PI - angle)*0.04;
}

void Player::reverse()
{
	xSpeed -= cos(2 * PI - angle)*0.02;
	ySpeed -= sin(2 * PI - angle)*0.02;
}


void Player::updateComponents()
{
	//Bullet update
	for (componentIt = components.begin(); componentIt != components.end();)
		if ((*componentIt)->update() == false)
		{
		delete (*componentIt);
		componentIt = components.erase(componentIt);
		}
		else
		{
			++componentIt;
		}
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}