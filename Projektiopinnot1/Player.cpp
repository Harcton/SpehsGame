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

	
	//Component test
	components.push_back(new Component(this, this, -50, -50));
	components[components.size() - 1]->tex.loadFromFile("Texture/ship_base.png");
	components[components.size() - 1]->spr.setTexture(components[components.size() - 1]->tex);
	components[0]->createChild(-120, -50, 0);
	components[1]->createChild(-190, -50, 0);
	components[2]->createChild(-260, -50, 0);
	components[3]->createChild(-260, -100, 0);
	components[3]->createChild(-260, 0, 0);
	components[4]->createChild(-240, -130, 0);
	components[5]->createChild(-240, 30, 0);

	//Front "shield"
	components[0]->createChild(0, -35, 0);
	components[0]->createChild(-20, -15, 0);
	components[0]->createChild(0, -65, 0);
	components[0]->createChild(-20, -85, 0);

	//components.push_back(new Turret(this, this, 30, 0));
	//components.push_back(new Turret(this, this, -10, -30));
	//components.push_back(new Turret(this, this, -10, -60));
	//components.push_back(new Turret(this, this, -10, 30));
	//components.push_back(new Turret(this, this, -10, 60));

	//Dynamic key binding
	MyKeys key;
	//Keyboard setup

	//Controller setup
	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::D;
	Keys[key_turnRight] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::A;
	Keys[key_turnLeft] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::S;
	Keys[key_reverse] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::W;
	Keys[key_accelerate] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::Period;
	Keys[key_zoomIn] = key;

	key.inputType = keyboardInput;
	key.keyCode = sf::Keyboard::Comma;
	Keys[key_zoomOut] = key;

	//Handle turrets
	int turretCount = 0;
	for (unsigned int i = 0; i < components.size(); i++)
		for (unsigned int k = 0; k < components[i]->types.size(); k++)
			if (components[i]->types[k] == turret)
				turretCount++;
	for (int i = 0; i < turretCount; i++)
	{//Turret rotation
		key.inputType = keyboardInput;
		key.keyCode = sf::Keyboard::E;
		Keys[-1 * (i + 1)] = key;
		key.keyCode = sf::Keyboard::Q;
		Keys[i + 1] = key;
		//Turret fire
		key.inputType = mouseInput;
		key.mouseButton = sf::Mouse::Left;
		Keys[i + 1 + 0.1] = key;
	}





	//Controller setup
	//key.inputType = joystickInput;
	//key.axisType = positiveAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::X;
	//Keys[key_turnRight] = key;

	//key.inputType = joystickInput;
	//key.axisType = negativeAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::X;
	//Keys[key_turnLeft] = key;

	//key.inputType = joystickInput;
	//key.axisType = positiveAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::Z;
	//Keys[key_reverse] = key;

	//key.inputType = joystickInput;
	//key.axisType = negativeAxis;
	//key.joystickIndex = 0;
	//key.joystickAxis = sf::Joystick::Z;
	//Keys[key_accelerate] = key;

	//key.inputType = joystickInput;
	//key.axisType = noAxis;
	//key.joystickIndex = 0;
	//key.joystickButton = 7;
	//Keys[key_zoomIn] = key;

	//key.inputType = joystickInput;
	//key.axisType = noAxis;
	//key.joystickIndex = 0;
	//key.joystickButton = 6;
	//Keys[key_zoomOut] = key;

	////Handle turrets
	//int turretCount = 0;
	//for (unsigned int i = 0; i < components.size(); i++)
	//	for (unsigned int k = 0; k < components[i]->types.size(); k++)
	//		if (components[i]->types[k] == turret)
	//			turretCount++;	
	//for (int i = 0; i < turretCount; i++)
	//{//Turret rotation
	//	key.inputType = joystickInput;
	//	key.axisType = noAxis;
	//	key.joystickIndex = 0;
	//	key.joystickButton = 4;
	//	Keys[i+1] = key;
	//	key.joystickButton = 5;
	//	Keys[-1 * (i + 1)] = key;
	//	//Turret fire
	//	key.inputType = joystickInput;
	//	key.axisType = noAxis;
	//	key.joystickIndex = 0;
	//	key.joystickButton = 0;
	//	Keys[i + 1 + 0.1] = key;
	//}

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





		{//Components
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
			{
				//Look through component i's types vector
				for (unsigned int k = 0; k < components[i]->types.size(); k++)
					if (components[i]->types[k] == turret)
					{//Turret i
					turretCount++;

					double turretMinAngle = angle - components[i]->maxAngle;
					double turretMaxAngle = angle + components[i]->maxAngle;
					if (turretMinAngle < 0)
						turretMinAngle += 2 * PI;
					if (turretMaxAngle > 2 * PI)
						turretMaxAngle -= 2 * PI;

					if (testInput(Keys[turretCount]) && components[i]->mouseAim == false)
					{//Rotate turret i CCW
						if (turretMaxAngle > turretMinAngle)
						{
							if (components[i]->angle < turretMaxAngle)
								components[i]->angle += components[i]->turningSpeed;
						}
						else
						{
							if (components[i]->angle > turretMinAngle - components[i]->turningSpeed)
								components[i]->angle += components[i]->turningSpeed;
							else if (components[i]->angle < turretMaxAngle)
								components[i]->angle += components[i]->turningSpeed;
						}
					}

					if (testInput(Keys[-1 * turretCount]) && components[i]->mouseAim == false)
					{//Rotate turret i CW
						if (turretMaxAngle > turretMinAngle)
						{
							if (components[i]->angle > turretMinAngle)
								components[i]->angle -= components[i]->turningSpeed;
						}
						else
						{
							if (components[i]->angle >= 0 && components[i]->angle < turretMaxAngle + components[i]->turningSpeed)
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
								if (components[i]->angle < mouseDirection)
								{
									components[i]->angle += components[i]->turningSpeed;
									if (components[i]->angle > turretMaxAngle)
										components[i]->angle = turretMaxAngle;
								}
								else if (components[i]->angle > mouseDirection)
								{
									components[i]->angle -= components[i]->turningSpeed;
									if (components[i]->angle < turretMinAngle)
										components[i]->angle = turretMinAngle;
								}
							}
							else if (angle >= PI)
							{//Case 1.1 
								if (mouseDirection < angle && mouseDirection > angle - PI)
								{
									if (components[i]->angle > turretMinAngle)
										components[i]->angle -= components[i]->turningSpeed;
								}
								else if (mouseDirection > angle || mouseDirection < angle - PI)
								{
									if (components[i]->angle < turretMaxAngle && components[i]->angle > turretMinAngle - components[i]->turningSpeed)
									{
											components[i]->angle += components[i]->turningSpeed;										
									}
								}
							}
							else // angle < PI
							{//Case 1.2
								if (mouseDirection > angle && mouseDirection < angle + PI)
								{// +
									if (components[i]->angle < turretMaxAngle)
										components[i]->angle += components[i]->turningSpeed;
								}
								else if (mouseDirection < angle || mouseDirection > angle + PI)
								{// -
									if (components[i]->angle > turretMinAngle)
										components[i]->angle -= components[i]->turningSpeed;
								}
							}
						}
						else if (turretMaxAngle + components[i]->turningSpeed >= components[i]->angle)
						{
							if (turretMaxAngle >= mouseDirection || mouseDirection >= turretMinAngle)
							{//"Case 2"
								if (mouseDirection > components[i]->angle && mouseDirection <= turretMaxAngle)	// I
									components[i]->angle += components[i]->turningSpeed;
								else if (mouseDirection >= 0 && mouseDirection < components[i]->angle)			// II
									components[i]->angle -= components[i]->turningSpeed;
								else if (mouseDirection >= turretMinAngle)
									components[i]->angle -= components[i]->turningSpeed;
							}
							else //mouseDirection is out of range
							{
								if ((angle >= PI && (mouseDirection > angle || mouseDirection < angle - PI)) || (angle < PI && mouseDirection > angle && mouseDirection < angle + PI))
								{// +
									if (turretMinAngle > components[i]->angle)//'+components[i]
									{
										if (components[i]->angle < turretMaxAngle || components[i]->angle > turretMinAngle)
											components[i]->angle += components[i]->turningSpeed;
									}
									else if (components[i]->angle)
										components[i]->angle += components[i]->turningSpeed;
									
								}
								else
								{// -
									if (turretMinAngle > components[i]->angle)//'+components[i]
									{
										if (components[i]->angle < turretMaxAngle + components[i]->turningSpeed)
											components[i]->angle -= components[i]->turningSpeed;
										else if (components[i]->angle > turretMinAngle)
											components[i]->angle -= components[i]->turningSpeed;
									}
									else if (components[i]->angle > turretMinAngle)
									{
										components[i]->angle -= components[i]->turningSpeed;
									}
								}
							}
						}
						else //if (turretMinAngle > turretMaxAngle && components[k]->angle >= turretMaxAngle)
						{
							if (mouseDirection <= turretMaxAngle || mouseDirection >= turretMinAngle)
							{//"Case 3"
								if (mouseDirection >= 0 && mouseDirection <= turretMaxAngle)					// I
									components[i]->angle += components[i]->turningSpeed;
								else if (mouseDirection > components[i]->angle)// II
									components[i]->angle += components[i]->turningSpeed;
								else if (mouseDirection < components[i]->angle && mouseDirection >= turretMinAngle)// III
									components[i]->angle -= components[i]->turningSpeed;
							}
							else //mouseDirection is out of range
							{
								if ((angle >= PI && (mouseDirection > angle || mouseDirection < angle - PI)) || (angle < PI && mouseDirection > angle && mouseDirection < angle + PI))//(mouseDirection > angle || mouseDirection < (angle - PI))
								{// +
									if (turretMaxAngle > components[i]->angle)
									{
										if (components[i]->angle < turretMaxAngle)
											components[i]->angle += components[i]->turningSpeed;
									}
									else
									{
										if (components[i]->angle < turretMaxAngle)
											components[i]->angle += components[i]->turningSpeed;
										else 
											components[i]->angle += components[i]->turningSpeed;
									}
								}
								else
								{// -
									if (turretMaxAngle > turretMinAngle)
									{
										if (components[i]->angle > turretMinAngle)
											components[i]->angle -= components[i]->turningSpeed;
									}
									else
									{
										if (components[i]->angle > turretMinAngle || components[i]->angle < turretMaxAngle + components[i]->turningSpeed)
											components[i]->angle -= components[i]->turningSpeed;
									}
								}
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

void Player::checkBulletCollision(Bullet* b)
{
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y, components[i]->x - b->x);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x, components[i]->y);
		b->checkCollisionRange = b->textureRadius + components[i]->textureRadius;

		if (b->checkCollisionDistance < b->checkCollisionRange)
		{
			if (b->canDamage == true)
			{
				components[i]->hp -= b->damage;
				b->canDamage = false;
				x += 6 * cos(angle);
				y += -6 * sin(angle);
			}

			b->speed = b->speed*0.75;

			b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			b->xSpeed = cos(2 * PI - b->angle) * b->speed;
			b->ySpeed = sin(2 * PI - b->angle) * b->speed;
		}
	}

}


void Player::removeComponent(int cid)
{
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->id == cid)
		{
		delete components[i];
		components.erase(components.begin() + i);
		}
}