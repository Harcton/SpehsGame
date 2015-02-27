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


	//Dynamic key binding
	MyKeys key;

	//Controller setup
	key.inputType = joystickInput;
	key.axisType = positiveAxis;
	key.joystickIndex = 0;
	key.joystickAxis = sf::Joystick::X;
	Keys[key_turnRight] = key;

	key.inputType = joystickInput;
	key.axisType = negativeAxis;
	key.joystickIndex = 0;
	key.joystickAxis = sf::Joystick::X;
	Keys[key_turnLeft] = key;

	key.inputType = joystickInput;
	key.axisType = positiveAxis;
	key.joystickIndex = 0;
	key.joystickAxis = sf::Joystick::Z;
	Keys[key_reverse] = key;

	key.inputType = joystickInput;
	key.axisType = negativeAxis;
	key.joystickIndex = 0;
	key.joystickAxis = sf::Joystick::Z;
	Keys[key_accelerate] = key;

	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 7;
	Keys[key_zoomIn] = key;

	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 6;
	Keys[key_zoomOut] = key;

	//Handle turrets
	int turretCount = 0;
	for (unsigned int i = 0; i < components.size(); i++)
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
			if (components[i]->types[k] == turret)
				turretCount++;	
	for (int i = 0; i < turretCount; i++)
	{//Turret rotation
		key.inputType = joystickInput;
		key.axisType = noAxis;
		key.joystickIndex = 0;
		key.joystickButton = 4;
		Keys[i+1] = key;
		key.joystickButton = 5;
		Keys[-1*(i+1)] = key;
	}
	//Turret fire
	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 0;
	Keys[1.1] = key;
	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 1;
	Keys[2.1] = key;
	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 2;
	Keys[3.1] = key;
	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 3;
	Keys[4.1] = key;
	key.inputType = joystickInput;
	key.axisType = noAxis;
	key.joystickIndex = 0;
	key.joystickButton = 2;
	Keys[5.1] = key;

}

bool Player::update()
{
	//Update mousePosition
	mousePosition = sf::Mouse::getPosition(mWindow);
	mWindow.pollEvent(mEvent);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;

		relativeSpeedX = 0;
		relativeSpeedY = 0;
	}



		//Accelerate
		if (testInput(Keys[key_accelerate]))
			if (Keys[key_accelerate].axisType != noAxis)
				accelerate(abs(sf::Joystick::getAxisPosition(Keys[key_accelerate].joystickIndex, Keys[key_accelerate].joystickAxis)));
			else
				accelerate(100);

		//Reverse
		if (testInput(Keys[key_reverse]))
			if (Keys[key_reverse].axisType != noAxis)
				reverse(abs(sf::Joystick::getAxisPosition(Keys[key_reverse].joystickIndex, Keys[key_reverse].joystickAxis)));
			else
				reverse(100);

		//Turn right
		if (testInput(Keys[key_turnRight]))
			if (Keys[key_turnRight].axisType != noAxis)
				turnRight(abs(sf::Joystick::getAxisPosition(Keys[key_turnRight].joystickIndex, Keys[key_turnRight].joystickAxis)));
			else
				turnRight(100);

		//Turn left
		if (testInput(Keys[key_turnLeft]))
			if (Keys[key_turnLeft].axisType != noAxis)
				turnLeft(abs(sf::Joystick::getAxisPosition(Keys[key_turnLeft].joystickIndex, Keys[key_turnLeft].joystickAxis)));
			else
				turnLeft(100);

		//Zoom in
		if (testInput(Keys[key_zoomIn]))
		{
			if (Keys[key_zoomIn].axisType != noAxis)
				zoomIn(abs(sf::Joystick::getAxisPosition(Keys[key_zoomIn].joystickIndex, Keys[key_zoomIn].joystickAxis)));
			else
				zoomIn(100);
		}

		//Zoom out
		if (testInput(Keys[key_zoomOut]))
		{
			if (Keys[key_zoomOut].axisType != noAxis)
				zoomOut(abs(sf::Joystick::getAxisPosition(Keys[key_zoomOut].joystickIndex, Keys[key_zoomOut].joystickAxis)));
			else
				zoomOut(100);
		}

		//Fire turrets
		{// <-need a scope for turretCount...
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
			for (unsigned int k = 0; k < components[i]->types.size(); k++)
				if (components[i]->types[k] == turret)
				{
				turretCount++;
				if (testInput(Keys[turretCount + 0.1]))
					components[i]->fire();
				}
		}//End of fire turrets




	//Components//
		{
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				//Look through component i's types vector
				for (unsigned int k = 0; k < components[i]->types.size(); k++)
					if (components[i]->types[k] == turret)
					{//Turret i
					turretCount++;

					double turretMinAngle = angle - components[k]->maxAngle;
					double turretMaxAngle = angle + components[k]->maxAngle;
					if (turretMinAngle < 0)
						turretMinAngle += 2 * PI;
					if (turretMaxAngle > 2 * PI)
						turretMaxAngle -= 2 * PI;

					if (testInput(Keys[turretCount]) && components[i]->mouseAim == false)
					{
						if (turretMaxAngle > turretMinAngle)
						{
							if (components[i]->angle < turretMaxAngle)
								components[i]->angle += components[i]->turningSpeed;
						}
						else
						{
							if (components[i]->angle > turretMinAngle)
								components[i]->angle += components[i]->turningSpeed;
							else if (components[i]->angle < turretMaxAngle)
								components[i]->angle += components[i]->turningSpeed;
						}
					}

					if (testInput(Keys[-1 * turretCount]) && components[i]->mouseAim == false)
					{
						if (turretMaxAngle > turretMinAngle)
						{
							if (components[i]->angle > turretMinAngle)
								components[i]->angle -= components[i]->turningSpeed;
						}
						else
						{
							if (components[i]->angle >= 0)
								components[i]->angle -= components[i]->turningSpeed;
							else if (components[i]->angle > turretMinAngle)
								components[i]->angle -= components[i]->turningSpeed;
						}
					}



					if (components[i]->mouseAim == true)
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
					}//End of mouse aiming
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
	std::cout << "\nTurning right...";
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

void Player::zoomIn(double f)
{
	zoomFactor += 0.01;
	std::cout << "\nZooming in...";
}

void Player::zoomOut(double f)
{
	zoomFactor -= 0.01;
	std::cout << "\nZooming out...";
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