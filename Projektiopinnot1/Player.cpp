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

	if (sf::Joystick::isConnected(0))
		controller = true;
	else
		controller = false;

	components.push_back(new Turret(this, this, 30, 0));
	components.push_back(new Turret(this, this, -10, -30));
	components.push_back(new Turret(this, this, -10, -60));
	components.push_back(new Turret(this, this, -10, 30));
	components.push_back(new Turret(this, this, -10, 60));
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
	}

	if (controller == true)
	{
		///CONTROLLER SUPPORT
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 20)
			turnRight(abs(sf::Joystick::getAxisPosition(0, sf::Joystick::X)));
		if (sf::Joystick::getAxisPosition(0, sf::Joystick::X) < -20)
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
	}

	//Components//
	for (unsigned int i = 0; i < components.size(); i++)
	{
		//Look through component i's types vector
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
		if (components[i]->types[k] == turret)
		{//Turret


			double turretMinAngle = angle - components[k]->maxAngle;
			double turretMaxAngle = angle + components[k]->maxAngle;
			if (turretMinAngle < 0)
				turretMinAngle += 2 * PI;
			if (turretMaxAngle > 2 * PI)
				turretMaxAngle -= 2 * PI;

			if (controller == true)
			{//Controller
				if (sf::Joystick::isButtonPressed(0, 4))
				{
					std::cout << "4.pressed ";
					if (turretMaxAngle > turretMinAngle)
					{
						if (components[i]->angle < turretMaxAngle)
							components[i]->angle += components[i]->turningSpeed;
					}
					else
					{


					}
					//if (turretMaxAngle > turretMinAngle)
					//{
					//	components[i]->angle += components[i]->turningSpeed;
					//}
					//else if (turretMaxAngle >= components[k]->angle)
					//{
					//	components[i]->angle += components[i]->turningSpeed;
					//}
					//else 
					//{

					//}

				}
				
				if (sf::Joystick::isButtonPressed(0, 5))
				{
					std::cout << "5.pressed ";
					if (turretMaxAngle > turretMinAngle)
					{
						if (components[i]->angle > turretMinAngle)
							components[i]->angle -= components[i]->turningSpeed;
					}
					else
					{


					}


					//if (turretMaxAngle > turretMinAngle)
					//{
					//	components[i]->angle -= components[i]->turningSpeed;
					//}
					//else if (turretMaxAngle >= components[k]->angle)
					//{
					//}
					//else
					//{
					//}
				}

			}
			else
			{//Mouse input
				//Set positive mouse direction
				mouseDirection = -1 * atan2(mousePosition.y - screenY, mousePosition.x - screenX);
				if (mouseDirection < 0)
					mouseDirection += 2 * PI;

				if (turretMaxAngle > turretMinAngle)
				{
					if (mouseDirection > turretMinAngle && mouseDirection < turretMaxAngle)
					{//"Case 1"
						if (components[k]->angle < mouseDirection)
						{
							components[k]->angle += components[k]->turningSpeed;
							if (components[k]->angle > turretMaxAngle)
								components[k]->angle = turretMaxAngle;
						}
						else if (components[k]->angle > mouseDirection)
						{
							components[k]->angle -= components[k]->turningSpeed;
							if (components[k]->angle < turretMinAngle)
								components[k]->angle = turretMinAngle;
						}
					}
				}
				else if (turretMaxAngle >= components[k]->angle)
				{
					if (turretMaxAngle >= mouseDirection || mouseDirection >= turretMinAngle)
					{//"Case 2"
						if (mouseDirection > components[k]->angle && mouseDirection <= turretMaxAngle)	// I
							components[k]->angle += components[k]->turningSpeed;
						else if (mouseDirection >= 0 && mouseDirection < components[k]->angle)			// II
							components[k]->angle -= components[k]->turningSpeed;
						else if (mouseDirection >= turretMinAngle)
							components[k]->angle -= components[k]->turningSpeed;
					}
				}
				else //if (turretMinAngle > turretMaxAngle && components[k]->angle > turretMaxAngle)
				{
					if (mouseDirection <= turretMaxAngle || mouseDirection >= turretMinAngle)
					{//"Case 3"
						if (mouseDirection >= 0 && mouseDirection <= turretMaxAngle)					// I
							components[k]->angle += components[k]->turningSpeed;
						else if (mouseDirection > components[k]->angle)// II
							components[k]->angle += components[k]->turningSpeed;
						else if (mouseDirection < components[k]->angle && mouseDirection >= turretMinAngle)// III
							components[k]->angle -= components[k]->turningSpeed;
					}
				}
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					components[i]->fire();
			}
			


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
}

void Player::reverse(double factor)
{
	xSpeed -= factor*(cos(2 * PI - angle)*0.0002);
	ySpeed -= factor*(sin(2 * PI - angle)*0.0002);
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