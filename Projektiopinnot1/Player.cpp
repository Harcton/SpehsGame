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
	//Update mousePosition
	mousePosition = sf::Mouse::getPosition(mWindow);
	mWindow.pollEvent(mEvent);

	//Speed control
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		accelerate(100);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		reverse(100);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		turnLeft(100);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		turnRight(100);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;

		relativeSpeedX = 0;
		relativeSpeedY = 0;
	}


	///CONTROLLER SUPPORT

	if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 10 )
		turnRight(abs(sf::Joystick::getAxisPosition(0, sf::Joystick::X)));
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -10 )
		turnLeft(abs(sf::Joystick::getAxisPosition(0, sf::Joystick::X)));
	//right stick axes: R,U
	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) < -20)
		accelerate(abs(sf::Joystick::getAxisPosition(0, sf::Joystick::Z)));

	if (sf::Joystick::getAxisPosition(0, sf::Joystick::Z) > 20)
		reverse(sf::Joystick::getAxisPosition(0, sf::Joystick::Z));

	if (sf::Joystick::isButtonPressed(0, 0))
	for (unsigned int i = 0; i < components.size(); i++)
	for (unsigned int k = 0; k < components[i]->types.size(); k++)
	if (components[i]->types[k] == turret)
		components[i]->fire();

	//Components//
	for (unsigned int i = 0; i < components.size(); i++)
	{
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
		if (components[i]->types[k] == turret)
		{
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			//	components[i]->angle += 0.03;
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
			//	components[i]->angle -= 0.03;
			mouseDirection = -1 * atan2(mousePosition.y - screenY, mousePosition.x - screenX);
			if (mouseDirection < 0)
				mouseDirection = ((2 * PI) + mouseDirection);

			//components[i]->angle = mouseDirection;
			components[i]->fixAngle();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				components[i]->fire();
		}
	}
	updateComponents();
	//////////////



	Object::update();
	return true;
}

void Player::turnRight(double factor)
{
	turnSpeed -= factor*(PI / 180000);
}

void Player::turnLeft(double factor)
{
	turnSpeed += factor*(PI / 180000);
}

void Player::accelerate(double factor)
{
	xSpeed += factor*(cos(2*PI - angle)*0.0004);
	ySpeed += factor*(sin(2 * PI - angle)*0.0004);

	relativeSpeedX += factor*(cos(2 * PI - angle)*0.0004);
	relativeSpeedY += factor*(sin(2 * PI - angle)*0.0004);
}

void Player::reverse(double factor)
{
	xSpeed -= factor*(cos(2 * PI - angle)*0.0002);
	ySpeed -= factor*(sin(2 * PI - angle)*0.0002);

	relativeSpeedX -= factor*(cos(2 * PI - angle)*0.0004);
	relativeSpeedY -= factor*(sin(2 * PI - angle)*0.0004);
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