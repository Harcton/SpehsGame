#include <fstream>
#include "Main.h"
#include "Player.h"
#include "Game.h"
#include "Component.h"
#include "Object.h"
#include "ShipEditor.h"



Player::~Player()
{//Player constructor CANNOT BE CALLED FROM Game.cpp! Game's objects's vector calls for object destructor instead...
	//std::cout << "\nBegin player DECONSTRUCTOR";
}
Player::Player(sf::RenderWindow& windowref, Game* game, int cx, int cy) : Object(windowref, game, cx, cy)
{
	centerObj = this;

	xAccVariable = 1;
	yAccVariable = 1;

	massCenterX = 0;
	massCenterY = 0;
	data = PlayerData();
	dataPtr = &data;
	loadPlayerData();
	applyPlayerData();
	loadKeybindings();
	//Reserve memory for all of the components
	components.reserve(EDITOR_WIDTH*EDITOR_HEIGHT);
}

bool Player::update()
{
	//Update mousePosition
	mousePosition = sf::Mouse::getPosition(mWindow);
	mWindow.pollEvent(mEvent);

	if (data.directionalMovement == false)
	{
		//Accelerate
		if (testInput(data.keyAccelerate, mGame->mEvent))
			if (data.keyAccelerate.axisType != noAxis)
				accelerate(abs(sf::Joystick::getAxisPosition(data.keyAccelerate.joystickIndex, data.keyAccelerate.joystickAxis)));
			else
				accelerate(100);
		//Turn right
		if (testInput(data.keyTurnRight, mGame->mEvent))
			if (data.keyTurnRight.axisType != noAxis)
				turnRight(abs(sf::Joystick::getAxisPosition(data.keyTurnRight.joystickIndex, data.keyTurnRight.joystickAxis)));
			else
				turnRight(100);
		//Turn left
		if (testInput(data.keyTurnLeft, mGame->mEvent))
			if (data.keyTurnLeft.axisType != noAxis)
				turnLeft(abs(sf::Joystick::getAxisPosition(data.keyTurnLeft.joystickIndex, data.keyTurnLeft.joystickAxis)));
			else
				turnLeft(100);
	}
	else if (abs(sf::Joystick::getAxisPosition(data.moveJoystickId, data.verticalMoveAxis)) + abs(sf::Joystick::getAxisPosition(data.moveJoystickId, data.horizontalMoveAxis)) > 15)
	{//Detect directional movement
		joystickDirection = -1 * atan2(data.verticalMoveAxisPolarity*sf::Joystick::getAxisPosition(data.moveJoystickId, data.verticalMoveAxis), data.horizontalMoveAxisPolarity*sf::Joystick::getAxisPosition(data.moveJoystickId, data.horizontalMoveAxis));
		if (joystickDirection < 0)
			joystickDirection = 2 * PI + joystickDirection;

		//Detect acceleration
		temp_accelerationPower = 100 * (pow(sf::Joystick::getAxisPosition(data.moveJoystickId, data.verticalMoveAxis) / 100, 2) + pow(sf::Joystick::getAxisPosition(data.moveJoystickId, data.horizontalMoveAxis) / 100, 2));
		if (temp_accelerationPower > 10)
		{
			if (temp_angleVar < PI / 2)
			{
				if (temp_accelerationPower > 100)
					temp_accelerationPower = 100;
				accelerate(temp_accelerationPower);

			}
		}

		//Rotation
		temp_angleVar = angle - joystickDirection;
		if (abs(temp_angleVar) < PI / 18)
		{
			turnSpeed = turnSpeed*0.90;
		}
		else if (temp_angleVar < 0)
		{//-
			if (joystickDirection < angle + PI)
			{//1st L
				turnLeft(100);
			}
			else
			{//4th R
				turnRight(100);
			}
		}//-
		else
		{//+
			if (joystickDirection < angle - PI)
			{//2nd L
				turnLeft(100);
			}
			else
			{//3rd R
				turnRight(100);
			}
		}//+
	}
	else
	{//DirectionalMovement == true && no input -> slow down rotation
		if (turnSpeed > 100 * rotationSpeed*(10.0 / (10 + shipMass)))
			turnRight(100);
		else if (turnSpeed < -100 * rotationSpeed*(10.0 / (10 + shipMass)))
			turnLeft(100);
	}

	//Reverse
	if (testInput(data.keyReverse, mGame->mEvent))
		if (data.keyReverse.axisType != noAxis)
			reverse(abs(sf::Joystick::getAxisPosition(data.keyReverse.joystickIndex, data.keyReverse.joystickAxis)));
		else
			reverse(100);

		//Zoom in
	if (testInput(data.keyZoomIn, mGame->mEvent))
		{
		if (data.keyZoomIn.axisType != noAxis)
			zoomIn(abs(sf::Joystick::getAxisPosition(data.keyZoomIn.joystickIndex, data.keyZoomIn.joystickAxis)));
			else
				zoomIn(100);
		}
		//Zoom out
	if (testInput(data.keyZoomOut, mGame->mEvent))
		{
		if (data.keyZoomOut.axisType != noAxis)
			zoomOut(abs(sf::Joystick::getAxisPosition(data.keyZoomOut.joystickIndex, data.keyZoomOut.joystickAxis)));
			else
				zoomOut(100);
		}

		//Fire/reload turrets
		{// <-need a scope for turretCount...
			for (unsigned int i = 0; i < components.size(); i++)
			{
				if (components[i]->type == component::turret)
				{
					if (testInput(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_fire], mGame->mEvent))
					{
						if (data.grid[components[i]->gridLocationX][components[i]->gridLocationY].holdToFire == true ||
							(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].holdToFire == false && components[i]->hasFired == false))
						{
							components[i]->fire();
						}
					}
					else
						components[i]->hasFired = false;
					if (testInput(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_reload], mGame->mEvent))
					if (components[i]->reloading == false)
						components[i]->reload(); 
				}
				else if (components[i]->type == component::engine)
				{
					if (testInput(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust], mGame->mEvent))
					{//Receiving input
						if (data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust].inputType == joystickInput)
							if (data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust].axisType != noAxis)
								components[i]->thrust(abs(sf::Joystick::getAxisPosition(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust].joystickIndex,
								data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust].joystickAxis)));
							else
								components[i]->thrust(100);
						else
							components[i]->thrust(100);
					}
				}
			}
		}//End of fire/reload turrets





		{//Components
			int turretCount = 0;
			for (unsigned int i = 0; i < components.size(); i++)
			{
					if (components[i]->type == component::turret)
				{//Turret i
					turretCount++;

					//double turretMinAngle = angle - components[i]->maxAngle;
					//double turretMaxAngle = angle + components[i]->maxAngle;
					//if (turretMinAngle < 0)
					//	turretMinAngle += 2 * PI;
					//if (turretMaxAngle > 2 * PI)
					//	turretMaxAngle -= 2 * PI;

					if (data.grid[components[i]->gridLocationX][components[i]->gridLocationY].directionalAim == true)
					{//Use directional aiming
						//-------------------
						if (abs(sf::Joystick::getAxisPosition(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].directionalJoystickId, data.grid[components[i]->gridLocationX][components[i]->gridLocationY].verticalAxis)) + 
							abs(sf::Joystick::getAxisPosition(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].directionalJoystickId, data.grid[components[i]->gridLocationX][components[i]->gridLocationY].horizontalAxis)) > 15)
						{//Detect directional movement
							joystickDirection = -1 * atan2(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].verticalAxisPolarity * sf::Joystick::getAxisPosition(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].directionalJoystickId, data.grid[components[i]->gridLocationX][components[i]->gridLocationY].verticalAxis),
								data.grid[components[i]->gridLocationX][components[i]->gridLocationY].horizontalAxisPolarity * sf::Joystick::getAxisPosition(data.grid[components[i]->gridLocationX][components[i]->gridLocationY].directionalJoystickId, data.grid[components[i]->gridLocationX][components[i]->gridLocationY].horizontalAxis));
							
							if (joystickDirection < 0)
								joystickDirection = 2 * PI + joystickDirection;


							temp_angleVar = components[i]->angle - joystickDirection;
							if (temp_angleVar < 0)
							{//-
								if (joystickDirection < components[i]->angle + PI)
								{//1st L
									components[i]->angle += components[i]->turningSpeed;
								}
								else
								{//4th R
									components[i]->angle -= components[i]->turningSpeed;
								}
							}//-
							else
							{//+
								if (joystickDirection < components[i]->angle - PI)
								{//2nd L
									components[i]->angle += components[i]->turningSpeed;
								}
								else
								{//3rd R
									components[i]->angle -= components[i]->turningSpeed;
								}
							}//+

						}
						//-------------------
					}
					else if (components[i]->mouseAim == true)
					{//Mouse input
						//Set positive mouse direction
						if (components[i]->mouseAimRelativeToCenter == true)
							mouseDirection = -1 * atan2(mousePosition.y - screenY, mousePosition.x - screenX);
						else
							mouseDirection = -1 * atan2(mousePosition.y - components[i]->screenY, mousePosition.x - components[i]->screenX);

						if (mouseDirection < 0)
							mouseDirection += 2 * PI;

						temp_angleVar = components[i]->angle - mouseDirection;
						if (temp_angleVar < 0)
						{//
							if (mouseDirection < components[i]->angle + PI)
							{//1st L +
								if (abs(temp_angleVar) < components[i]->turningSpeed)
									components[i]->angle = mouseDirection;
								else
									components[i]->angle += components[i]->turningSpeed;
							}
							else
							{//4th R -
								if (mouseDirection - components[i]->angle < components[i]->turningSpeed)
									components[i]->angle = mouseDirection;
								else
									components[i]->angle -= components[i]->turningSpeed;
							}
						}//
						else
						{//
							if (mouseDirection < components[i]->angle - PI)
							{//2nd L +
								if (components[i]->angle - mouseDirection < components[i]->turningSpeed)
									components[i]->angle = mouseDirection;
								else
									components[i]->angle += components[i]->turningSpeed;
							}
							else
							{//3rd R -
								if (abs(temp_angleVar) < components[i]->turningSpeed)
									components[i]->angle = mouseDirection;
								else
									components[i]->angle -= components[i]->turningSpeed;
							}
						}//
					}//End of mouse aiming
					else
					{//Use manual turret rotation (press button)
						if (testInput(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_left], mGame->mEvent))
						{//Rotate turret i CCW
							//if (turretMaxAngle > turretMinAngle)
							//{
							//	if (components[i]->angle < turretMaxAngle)
									components[i]->angle += components[i]->turningSpeed;
							//}
							//else
							//{
							//	if (components[i]->angle > turretMinAngle - components[i]->turningSpeed)
							//		components[i]->angle += components[i]->turningSpeed;
							//	else if (components[i]->angle < turretMaxAngle)
							//		components[i]->angle += components[i]->turningSpeed;
							//}
						}
						if (testInput(data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_right], mGame->mEvent))
						{//Rotate turret i CW
							//if (turretMaxAngle > turretMinAngle)
							//{
							//	if (components[i]->angle > turretMinAngle)
									components[i]->angle -= components[i]->turningSpeed;
							//}
							//else
							//{
							//	if (components[i]->angle >= 0 && components[i]->angle < turretMaxAngle + components[i]->turningSpeed)
							//		components[i]->angle -= components[i]->turningSpeed;
							//	else if (components[i]->angle > turretMinAngle)
							//		components[i]->angle -= components[i]->turningSpeed;
							//}
						}
					}


				}
			}
		}
	Object::update();


	//SCREEN X/Y UPDATE
		
	if (xSpeed1 != 0)
		xAcceleration = (xSpeed / xSpeed1);
	else
		xAcceleration = 1;

	if (ySpeed1 != 0)
		yAcceleration = (ySpeed / ySpeed1);
	else
		yAcceleration = 1;


	if (abs(xAcceleration) > 1)
		xAccVariable = xAccVariable * 0.999;
	else
		xAccVariable = xAccVariable * 1.01;
	if (abs(xAccVariable) > 1)
		xAccVariable = 1;

	if (abs(yAcceleration) > 1)
		yAccVariable = yAccVariable*0.999;
	else
		yAccVariable = yAccVariable * 1.01;
	if (abs(yAccVariable) > 1)
		yAccVariable = 1;


	xScreenDistance = (WINDOW_WIDTH / 2.5 - abs(scrSpeedX)) / (WINDOW_WIDTH / 2.5);
	yScreenDistance = (WINDOW_HEIGHT / 2.5 - abs(scrSpeedY)) / (WINDOW_WIDTH / 2.5);
	//scrSpeeds
	//Check if ship is accelerating, limit and set scrSpeed
	//X
	//NOT ACCELERATING
	if (abs(xAcceleration) == 1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedX = 0.99 * scrSpeedX;
			relativeSpeedX = 0.95 * relativeSpeedX;
		}
	}
	//ACCELERATING
	else
	{
		scrSpeedX += (((relativeSpeedX * abs(relativeSpeedX)) * (xScreenDistance*0.2))*zoomFactor);
		//scrSpeedX = scrSpeedX * xAccVariable;
	}
	/*std::cout << "scrSpeedX: " << scrSpeedX << std::endl;
	std::cout << "xAccVariable: " << xAccVariable << std::endl;*/

	//Y
	//NOT ACCELERATING
	if (abs(yAcceleration) == 1)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			scrSpeedY = 0.99 * scrSpeedY;
			relativeSpeedY = 0.95 * relativeSpeedY;
		}
	}
	//ACCELERATING
	else
	{
		scrSpeedY += (((relativeSpeedY * abs(relativeSpeedY)) * (yScreenDistance*0.2))*zoomFactor);
		//scrSpeedY = scrSpeedY * yAccVariable;
	}
	/*std::cout << "scrSpeedY: " << scrSpeedY << std::endl;
	std::cout << "yAccVariable: " << yAccVariable << std::endl;*/

	screenX = WINDOW_WIDTH / 2 - (scrSpeedX);// - (massCenterX*cos(angle) + massCenterY*sin(angle))*resFactor*zoomFactor;
	screenY = WINDOW_HEIGHT / 2 - (scrSpeedY);// + (massCenterX*sin(angle) - massCenterY*cos(angle))*resFactor*zoomFactor;
	double shipRadius = (getDistance(0, 0, shipWidth, shipHeight)*100.0f*zoomFactor*resFactor)/2.0f;
	if (WINDOW_WIDTH - shipRadius > shipRadius)
		screenX = limitWithin(double(shipRadius), screenX, double(WINDOW_WIDTH - shipRadius));
	else
		screenX = WINDOW_WIDTH / 2.0f;
	if (WINDOW_HEIGHT - shipRadius > shipRadius)
		screenY = limitWithin(double(shipRadius), screenY, double(WINDOW_HEIGHT - shipRadius));
	else
		screenY = WINDOW_HEIGHT / 2.0f;

	xSpeed1 = xSpeed;
	ySpeed1 = ySpeed;
	//

	updateComponents();
	//////////////

	return true;
}


void Player::turnRight(double factor)
{
	if (components.size() <= 0)
		return;
	turnSpeed -= factor*rotationSpeed*(10.0 / (10 + shipMass));
}

void Player::turnLeft(double factor)
{
	if (components.size() <= 0)
		return;
	turnSpeed += factor*rotationSpeed*(10.0 / (10 + shipMass));
}

void Player::accelerate(double factor)
{
	if (components.size() <= 0)
		return;
	xSpeed += factor*cos(2 * PI - angle)*movementSpeed*(10.0 / (10 + shipMass));
	ySpeed += factor*sin(2 * PI - angle)*movementSpeed*(10.0 / (10 + shipMass));

	relativeSpeedX += factor*cos(2 * PI - angle)*movementSpeed*(10.0 / (10 + shipMass));
	relativeSpeedY += factor*sin(2 * PI - angle)*movementSpeed*(10.0 / (10 + shipMass));
}

void Player::reverse(double factor)
{
	if (components.size() <= 0)
		return;
	xSpeed -= factor*(cos(2 * PI - angle)*movementSpeed*0.6);
	ySpeed -= factor*(sin(2 * PI - angle)*movementSpeed)*0.6;

	relativeSpeedX -= factor*(cos(2 * PI - angle)*movementSpeed);
	relativeSpeedY -= factor*(sin(2 * PI - angle)*movementSpeed);
}

void Player::zoomIn(double f)
{
	zoomFactor += 0.01;
	limitZoom();
}

void Player::zoomOut(double f)
{
	zoomFactor -= 0.01;
	limitZoom();
}





void Player::loadPlayerData()
{
	if (playerName.size() < 2)
		return;
	std::cout << "\nLoading player data [" << playerName << ".dat]";

	std::fstream mFile;
	mFile.open("Settings/PlayerSaves/" + playerName + ".dat", std::ofstream::binary | std::ios::in);
	if (mFile)
	{
		mFile.read((char*)&data, sizeof(PlayerData));
	}

	dataPtr = &data;
}

void Player::applyPlayerData()
{
	angle = 0;
	while (!components.empty())
	{
		components.back()->destructorMode = component::quit;
		delete components.back();
		components.pop_back();
	}

	//Search for the core and start adding child components in chain reaction
	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data.grid[ex][ey].core == true)
			{
			coreX = ex;
			coreY = ey;
			addFromGrid(ex, ey);
			}
		}

	calculateCenterOfMass();
	reloadSkeletonSprites();
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->type == component::hull)
		setArmorLooks(components[i]->gridLocationX, components[i]->gridLocationY);
}

void Player::calculateCenterOfMass()
{
	//Calculate center of mass
	int temp_componentCount = 0;
	int temp_rowWeight[EDITOR_WIDTH] = {};
	int temp_pillarWeight[EDITOR_HEIGHT] = {};

	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data.grid[ex][ey].armor > 0)
		{
			temp_componentCount++;
			temp_pillarWeight[ex] += 1;
			temp_rowWeight[ey] += 1;
		}
		}
	shipMass = temp_componentCount;
	double centerX = 0;
	double centerY = 0;

	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
	{
		centerX += (temp_pillarWeight[ex] * (1 + ex));
	}
	centerX = centerX / temp_componentCount;
	massCenterX = 100.0*(centerX - double(coreX + 1));

	for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
	{
		centerY += (temp_rowWeight[ey] * (1 + ey));
	}
	centerY = centerY / temp_componentCount;
	massCenterY = 100.0*(centerY - double(coreY + 1));

	//Apply center of mass to all the components
	for (unsigned int i = 0; i < components.size(); i++)
	{
		components[i]->xOffset = components[i]->xOffsetOriginal - massCenterX;
		components[i]->yOffset = components[i]->yOffsetOriginal - massCenterY;
	}

	//Calculate ship width and height
	int temp_firstHorizontal = -1;
	int temp_firstVertical = -1;
	int temp_lastHorizontal = -1;
	int temp_lastVertical = -1;
	for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
		if (data.grid[ex][ey].armor > 0 && temp_firstHorizontal == -1)
			temp_firstHorizontal = ex;
		if (data.grid[ex][ey].armor > 0)
			temp_lastHorizontal = ex;
		}
	for (int ey = 0; ey < EDITOR_HEIGHT; ey++)
		for (int ex = 0; ex < EDITOR_WIDTH; ex++)
		{
		if (data.grid[ex][ey].armor > 0 && temp_firstVertical == -1)
			temp_firstVertical = ey;
		if (data.grid[ex][ey].armor > 0)
			temp_lastVertical = ey;
		}
	shipWidth = temp_lastHorizontal - temp_firstHorizontal + 1;
	shipHeight = temp_lastVertical - temp_firstVertical + 1;

	std::cout << "\nShip width/height: " << shipWidth << " / " << shipHeight;
	std::cout << "\nShip component mass: " << shipMass;
}

//This is the only way allowed to add components to a player ship
void Player::addFromGrid(int gx, int gy)
{
	components.push_back(new Component(this, this, (gx - coreX) * 100, (gy - coreY) * 100, gx, gy));
	components.back()->type = component::hull;
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.skeletonTex);
	components.back()->sprites.back().setOrigin(50, 50);
	//Push armor sprites in
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.shipArmor1Tex);
	components.back()->sprites.back().setOrigin(0, 50);
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.shipArmor1Tex);
	components.back()->sprites.back().setOrigin(0, 0);
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.shipArmor1Tex);
	components.back()->sprites.back().setOrigin(50, 0);
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.shipArmor1Tex);
	components.back()->sprites.back().setOrigin(50, 50);
	
	if (data.grid[gx][gy].core)
	{//For the core piece, add the additional sprite
		components.back()->sprites.push_back(sf::Sprite());
		components.back()->sprites.back().setTexture(RM.coreTex);
		components.back()->sprites.back().setOrigin(50, 50);
	}	
	else if (data.grid[gx][gy].turret > 0)
	{//Add turret
		components.back()->createChild((gx - coreX) * 100, (gy - coreY) * 100, component::turret);
		components.back()->gridLocationX = gx;
		components.back()->gridLocationY = gy;
		//Set looks
		setTurretLooks(gx, gy);
		//Set stats
		setTurretStats(gx, gy);
		components.back()->angleModifier = data.grid[gx][gy].angleModifier*(PI/180);

		//Color all the sprites
		for (unsigned int i = 0; i < components.back()->sprites.size(); i++)
			components.back()->sprites[i].setColor(sf::Color(data.grid[gx][gy].red, data.grid[gx][gy].green, data.grid[gx][gy].blue));
		for (unsigned int i = 0; i < components.back()->animatedSprites.size(); i++)
			components.back()->animatedSprites[i].setColor(sf::Color(data.grid[gx][gy].red, data.grid[gx][gy].green, data.grid[gx][gy].blue));
		components.back()->setLaserPointerColor(sf::Color(data.grid[gx][gy].red, data.grid[gx][gy].green, data.grid[gx][gy].blue));
	}
	else if (data.grid[gx][gy].engine > 0)
	{//Add an engine
		components.back()->createChild((gx - coreX) * 100, (gy - coreY) * 100, component::engine);
		components.back()->gridLocationX = gx;
		components.back()->gridLocationY = gy;
		//Set stats
		components.back()->angleModifier = data.grid[gx][gy].angleModifier*(PI / 180);
		components.back()->angle = components.back()->angleModifier + PI;
		components.back()->rotationDirection = data.grid[gx][gy].rotationDirection;
		components.back()->holdToThrust = data.grid[gx][gy].holdToFire;
		//Set looks
		setEngineLooks(gx, gy);
		//Set stats
		setEngineStats(gx, gy);
	}
	
	//Handle children	
	int selfIndex = components.size() - 1;
	if (data.grid[gx][gy].childUp == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx, gy - 1);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data.grid[gx][gy].childDown == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx, gy + 1);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data.grid[gx][gy].childRight == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx + 1, gy);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
	if (data.grid[gx][gy].childLeft == true)
	{
		int tempIndex = components.size();
		addFromGrid(gx - 1, gy);
		components[selfIndex]->childComponents.push_back(components[tempIndex]->id);
	}
}

void Player::setArmorLooks(int gx, int gy)
{
	Component* cPtr = nullptr;
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->gridLocationX == gx && components[i]->gridLocationY == gy && components[i]->type == component::hull)
			cPtr = components[i];
	if (cPtr == nullptr)
		return;


	bool temp_N = false;
	bool temp_NE = false;
	bool temp_E = false;
	bool temp_SE = false;
	bool temp_S = false;
	bool temp_SW = false;
	bool temp_W = false;
	bool temp_NW = false;
	if (gx > 0)
	{
		if (data.grid[gx - 1][gy].armor != 0)
			temp_W = true;
		if (gy > 0)
			if (data.grid[gx - 1][gy - 1].armor != 0)
				temp_NW = true;
		if (gy < EDITOR_HEIGHT - 1)
			if (data.grid[gx - 1][gy + 1].armor != 0)
				temp_SW = true;
	}
	if (gx < EDITOR_WIDTH - 1)
	{
		if (data.grid[gx + 1][gy].armor != 0)
			temp_E = true;
		if (gy > 0)
			if (data.grid[gx + 1][gy - 1].armor != 0)
				temp_NE = true;
		if (gy < EDITOR_HEIGHT - 1)
			if (data.grid[gx + 1][gy + 1].armor != 0)
				temp_SE = true;
	}
	if (gy > 0)
		if (data.grid[gx][gy - 1].armor != 0)
			temp_N = true;
	if (gy < EDITOR_HEIGHT - 1)
		if (data.grid[gx][gy + 1].armor != 0)
			temp_S = true;

	//1st piece
	if (temp_N == false && temp_NE == false && temp_E == false)
		cPtr->sprites[1].setTextureRect(sf::IntRect(0, 0, 50, 50));
	else if (temp_N == false && temp_NE == false && temp_E == true)
		cPtr->sprites[1].setTextureRect(sf::IntRect(50, 0, 50, 50));
	else if (temp_N == false && temp_NE == true && temp_E == false)
		cPtr->sprites[1].setTextureRect(sf::IntRect(100, 0, 50, 50));
	else if (temp_N == false && temp_NE == true && temp_E == true)
		cPtr->sprites[1].setTextureRect(sf::IntRect(150, 0, 50, 50));
	else if (temp_N == true && temp_NE == false && temp_E == false)
		cPtr->sprites[1].setTextureRect(sf::IntRect(200, 0, 50, 50));
	else if (temp_N == true && temp_NE == false && temp_E == true)
		cPtr->sprites[1].setTextureRect(sf::IntRect(250, 0, 50, 50));
	else if (temp_N == true && temp_NE == true && temp_E == false)
		cPtr->sprites[1].setTextureRect(sf::IntRect(300, 0, 50, 50));
	else
		cPtr->sprites[1].setTextureRect(sf::IntRect(350, 0, 50, 50));
	//2nd piece
	if (temp_S == false && temp_SE == false && temp_E == false)
		cPtr->sprites[2].setTextureRect(sf::IntRect(0, 50, 50, 50));
	else if (temp_S == false && temp_SE == false && temp_E == true)
		cPtr->sprites[2].setTextureRect(sf::IntRect(50, 50, 50, 50));
	else if (temp_S == false && temp_SE == true && temp_E == false)
		cPtr->sprites[2].setTextureRect(sf::IntRect(100, 50, 50, 50));
	else if (temp_S == false && temp_SE == true && temp_E == true)
		cPtr->sprites[2].setTextureRect(sf::IntRect(150, 50, 50, 50));
	else if (temp_S == true && temp_SE == false && temp_E == false)
		cPtr->sprites[2].setTextureRect(sf::IntRect(200, 50, 50, 50));
	else if (temp_S == true && temp_SE == false && temp_E == true)
		cPtr->sprites[2].setTextureRect(sf::IntRect(250, 50, 50, 50));
	else if (temp_S == true && temp_SE == true && temp_E == false)
		cPtr->sprites[2].setTextureRect(sf::IntRect(300, 50, 50, 50));
	else
		cPtr->sprites[2].setTextureRect(sf::IntRect(350, 50, 50, 50));
	//3rd piece
	if (temp_S == false && temp_SW == false && temp_W == false)
		cPtr->sprites[3].setTextureRect(sf::IntRect(350, 150, 50, 50));
	else if (temp_S == false && temp_SW == false && temp_W == true)
		cPtr->sprites[3].setTextureRect(sf::IntRect(300, 150, 50, 50));
	else if (temp_S == false && temp_SW == true && temp_W == false)
		cPtr->sprites[3].setTextureRect(sf::IntRect(250, 150, 50, 50));
	else if (temp_S == false && temp_SW == true && temp_W == true)
		cPtr->sprites[3].setTextureRect(sf::IntRect(200, 150, 50, 50));
	else if (temp_S == true && temp_SW == false && temp_W == false)
		cPtr->sprites[3].setTextureRect(sf::IntRect(150, 150, 50, 50));
	else if (temp_S == true && temp_SW == false && temp_W == true)
		cPtr->sprites[3].setTextureRect(sf::IntRect(100, 150, 50, 50));
	else if (temp_S == true && temp_SW == true && temp_W == false)
		cPtr->sprites[3].setTextureRect(sf::IntRect(50, 150, 50, 50));
	else
		cPtr->sprites[3].setTextureRect(sf::IntRect(0, 150, 50, 50));
	//4th piece
	if (temp_N == false && temp_NW == false && temp_W == false)
		cPtr->sprites[4].setTextureRect(sf::IntRect(350, 100, 50, 50));
	else if (temp_N == false && temp_NW == false && temp_W == true)
		cPtr->sprites[4].setTextureRect(sf::IntRect(300, 100, 50, 50));
	else if (temp_N == false && temp_NW == true && temp_W == false)
		cPtr->sprites[4].setTextureRect(sf::IntRect(250, 100, 50, 50));
	else if (temp_N == false && temp_NW == true && temp_W == true)
		cPtr->sprites[4].setTextureRect(sf::IntRect(200, 100, 50, 50));
	else if (temp_N == true && temp_NW == false && temp_W == false)
		cPtr->sprites[4].setTextureRect(sf::IntRect(150, 100, 50, 50));
	else if (temp_N == true && temp_NW == false && temp_W == true)
		cPtr->sprites[4].setTextureRect(sf::IntRect(100, 100, 50, 50));
	else if (temp_N == true && temp_NW == true && temp_W == false)
		cPtr->sprites[4].setTextureRect(sf::IntRect(50, 100, 50, 50));
	else
		cPtr->sprites[4].setTextureRect(sf::IntRect(0, 100, 50, 50));
}

void Player::setTurretLooks(int gx, int gy)
{
	//Platform
	components[components.size() - 2]->turretPlatform = sf::Sprite();
	components[components.size() - 2]->turretPlatform.setTexture(RM.turretPlatformTex);
	components[components.size() - 2]->turretPlatform.setOrigin(50, 50);
	components[components.size() - 2]->turretPlatform.setTextureRect(sf::IntRect((data.grid[gx][gy].maxAngle - 1) * 100, 0, 100, 100));
	components[components.size() - 2]->turretPlatformAngle = data.grid[gx][gy].angleModifier*(PI / 180.f);
	//Recoil absorber
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.turretRecoilAbsorberTex);
	components.back()->sprites.back().setOrigin(50, 50);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].recoilTime - 1) * 100, 0, 100, 100));
	//Recoil absorber
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.turretReloaderTex);
	components.back()->sprites.back().setOrigin(50, 50);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].rechargeSpeed - 1) * 100, 0, 100, 100));
	//Magazine
	components.back()->magazineSpr = sf::Sprite();
	components.back()->magazineSpr.setTexture(RM.turretMagazineTex);
	components.back()->magazineSpr.setTextureRect(sf::IntRect((data.grid[gx][gy].capacity - 1) * 100, 0, 100, 100));
	//Barrel
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.turretBarrelTex);
	components.back()->sprites.back().setOrigin(0, 50);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].bulletSpeed - 1) * 100, 0, 100, 100));
	//Frame
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.turretFrameTex);
	components.back()->sprites.back().setOrigin(50, 50);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].turret - 1) * 100, 0, 100, 100));
	//RotatingEngine
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.turretRotationEngineTex);
	components.back()->sprites.back().setOrigin(50, 50);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].turnSpeed - 1) * 100, 0, 100, 100));

}
void Player::setTurretStats(int gx, int gy)
{
	//(!) This function assumes that the last component in the components vector is the target turret
	switch (data.grid[gx][gy].maxAngle)
	{//MAX ANGLE
	case 2:
		components.back()->maxAngle = 0.4*PI;
		break;
	case 3:
		components.back()->maxAngle = 0.6*PI;
		break;
	case 4:
		components.back()->maxAngle = PI;
		break;
	}
	switch (data.grid[gx][gy].turnSpeed)
	{//TURN SPEED
	case 2:
		components.back()->turningSpeed = PI/90;
		break;
	case 3:
		components.back()->turningSpeed = PI / 20;
		break;
	}
	switch (data.grid[gx][gy].bulletSpeed)
	{//BULLET SPEED
	case 2:
		components.back()->maxSpeed = 25;
		break;
	case 3:
		components.back()->maxSpeed = 50;//Update turret update -> laser drawing
		break;
	}
	switch (data.grid[gx][gy].capacity)
	{//CAPACITY
	case 2:
		components.back()->capacity = 8;
		break;
	case 3:
		components.back()->capacity = 15;
		break;
	case 4:
		components.back()->capacity = 20;
		break;
	}
	components.back()->charge = components.back()->capacity;
	switch (data.grid[gx][gy].recoilTime)
	{//RECOIL TIME
	case 2:
		components.back()->fireRateInterval = 30;
		break;
	case 3:
		components.back()->fireRateInterval = 5;
		break;
	}
	switch (data.grid[gx][gy].rechargeSpeed)
	{//RELOAD SPEED
	case 2:
		components.back()->rechargeInterval = 22;
		break;
	case 3:
		components.back()->rechargeInterval = 10;
		break;
	}
	switch (data.grid[gx][gy].turret)
	{
	case 2:
		components.back()->damage = 18;
		break;
	case 3:
		components.back()->damage = 23;
		break;
	case 4:
		components.back()->damage = 30;
		break;
	case 5:
		components.back()->damage = 40;
		components.back()->bulletTexPtr = &RM.bullet2Tex;
		break;
	}
}
void Player::setEngineLooks(int gx, int gy)
{
	//Recharger
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.engineRechargerTex);
	components.back()->sprites.back().setOrigin(27, 25);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].rechargeSpeed - 1) * 50, 0, 50, 50));
	//Fuel tank
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.engineFuelTankTex);
	components.back()->sprites.back().setOrigin(7, 25);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].capacity - 1) * 50, 0, 50, 50));
	//Thruster
	components.back()->sprites.push_back(sf::Sprite());
	components.back()->sprites.back().setTexture(RM.engineThrusterTex);
	components.back()->sprites.back().setOrigin(78, 25);
	components.back()->sprites.back().setTextureRect(sf::IntRect((data.grid[gx][gy].engine - 1) * 75, 0, 75, 50));
	//Thruster-engine flame
	switch (data.grid[gx][gy].engine)
	{
	case 1:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireTier1Animation);
		break;
	case 2:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireTier1Animation);
		break;
	case 3:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireAnimation);
		break;
	case 4:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireAnimation);
		break;
	case 5:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireTier4Animation);
		break;
	case 6:
		components.back()->getEngineFlameSpr().setTexture(RM.engineFireTier4Animation);
		break;
	}

}
void Player::setEngineStats(int gx, int gy)
{

	switch (data.grid[gx][gy].engine)
	{//Thrust strength
	case 2:
		components.back()->thrustStrength = 0.0008;
		break;
	case 3:
		components.back()->thrustStrength = 0.0014;
		components.back()->chargeConsumption = 2;
		break;
	case 4:
		components.back()->thrustStrength = 0.0018;
		components.back()->chargeConsumption = 2;
		break;
	case 5:
		components.back()->thrustStrength = 0.0024;
		components.back()->chargeConsumption = 3;
		break;
	case 6:
		components.back()->thrustStrength = 0.0035;
		components.back()->chargeConsumption = 4;
		break;
	}
	switch (data.grid[gx][gy].capacity)
	{//CAPACITY
	case 2:
		components.back()->capacity = 250;
		components.back()->getChargeRectangle().setSize(sf::Vector2f(17.0f, 8.0f));
		components.back()->getChargeRectangle().setOrigin(0, 4);
		break;
	case 3:
		components.back()->capacity = 350;
		components.back()->getChargeRectangle().setSize(sf::Vector2f(23.0f, 10.0f));
		components.back()->getChargeRectangle().setOrigin(0, 5);
		break;
	case 4:
		components.back()->capacity = 500;
		components.back()->getChargeRectangle().setSize(sf::Vector2f(29.0f, 12.0f));
		components.back()->getChargeRectangle().setOrigin(0, 6);
		components.back()->getChargeRectangle().setFillColor(sf::Color(255, 0, 70, 190));
		break;
	case 5:
		components.back()->capacity = 800;
		components.back()->getChargeRectangle().setSize(sf::Vector2f(29.0f, 14.0f));
		components.back()->getChargeRectangle().setOrigin(0, 7);
		components.back()->getChargeRectangle().setFillColor(sf::Color(255, 0, 100, 190));
		break;
	case 6:
		components.back()->capacity = 1500;
		components.back()->getChargeRectangle().setSize(sf::Vector2f(34.0f, 14.0f));
		components.back()->getChargeRectangle().setOrigin(0, 7);
		components.back()->getChargeRectangle().setFillColor(sf::Color(255, 0, 140, 190));
		break;
	}
	switch (data.grid[gx][gy].capacity)
	{//Recharge rate
	case 2:
		components.back()->rechargeAmount = 2;
		break;
	case 3:
		components.back()->rechargeAmount = 3;
		break;
	case 4:
		components.back()->rechargeAmount = 4;
		break;
	case 5:
		components.back()->rechargeAmount = 5;
		break;
	}
	components.back()->charge = components.back()->capacity;




}

void Player::editShip()
{
	ShipEditor editor(mWindow, data);
	switch (editor.run())
	{
	case 0:
	{
		//Player exits with return to spehs
		applyPlayerData();
		loadKeybindings();
		//Update enemy nearestComponent pointers
		for (unsigned int i = 1; i < mGame->objects.size(); i++)
			mGame->objects[i]->update();


		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;
		screenX = WINDOW_WIDTH / 2;
		screenY = WINDOW_HEIGHT / 2;
		break;
	}
	case 2:
		//Player wishes to quit to menu
		mGame->keepRunning = false;
		break;
	}
}



void Player::notifyComponentDestruction(Component* component)
{
	int gx = component->gridLocationX;
	int gy = component->gridLocationY;
	if (gx < 0 || gy < 0)
		return;

	
	//Notify parent component
	if (gx < EDITOR_WIDTH - 1)
		data.grid[gx + 1][gy].childLeft = false;
	if (gx > 0)
		data.grid[gx - 1][gy].childRight = false;
	if (gy < EDITOR_HEIGHT - 1)
		data.grid[gx][gy + 1].childUp = false;
	if (gy > 0)
		data.grid[gx][gy - 1].childDown = false;

	//Remove actual data
	data.grid[gx][gy] = GridData();

	calculateCenterOfMass();
	reloadSkeletonSprites();
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->type == component::hull)
			setArmorLooks(components[i]->gridLocationX, components[i]->gridLocationY);
}

void Player::loadKeybindings()
{
	//Erase previous component bindings
	for (int x = 0; x < EDITOR_WIDTH; x++)
		for (int y = 0; y < EDITOR_HEIGHT; y++)
			for (int k = 0; k < 7; k++)
				data.keyGrid[x][y][k] = MyKeys();

	//Dynamic key binding per component
	for (unsigned int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == component::turret)
		{
			components[i]->mouseAim = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].mouseAim;
			components[i]->mouseAimRelativeToCenter = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].mouseAimRelativeToCenter;
			data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_left] = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].turretLeft;
			data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_right] = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].turretRight;
			data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_fire] = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].turretFire;
			data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_reload] = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].turretReload;
		}
		else if (components[i]->type == component::engine)
		{
			data.keyGrid[components[i]->gridLocationX][components[i]->gridLocationY][kgrid_thrust] = data.grid[components[i]->gridLocationX][components[i]->gridLocationY].engineThrust;
		}
	}
}

void Player::reloadSkeletonSprites()
{
	int temp_state;
	for (unsigned int i = 0; i < components.size(); i++)
		if (components[i]->sprites.size() > 0 )
			if (components[i]->type == component::hull)
	{
		temp_state = 0;
		
		data.grid[components[i]->gridLocationX][components[i]->gridLocationY];
		if (components[i]->gridLocationY > 0)
			if (data.grid[components[i]->gridLocationX][components[i]->gridLocationY - 1].armor > 0)
				temp_state += 8;
		if (components[i]->gridLocationY < EDITOR_HEIGHT - 1)
			if (data.grid[components[i]->gridLocationX][components[i]->gridLocationY + 1].armor > 0)
				temp_state += 4;
		if (components[i]->gridLocationX > 0)
			if (data.grid[components[i]->gridLocationX - 1][components[i]->gridLocationY ].armor > 0)
				temp_state += 2;
		if (components[i]->gridLocationX < EDITOR_WIDTH - 1)
			if (data.grid[components[i]->gridLocationX + 1][components[i]->gridLocationY].armor > 0)
				temp_state += 1;
		
		//Assuming that sprites[0] is the skeleton sprite...
		switch (temp_state)
		{
		case 4://scs_S:
			components[i]->sprites[0].setTextureRect(sf::IntRect(0, 0, 100, 100));
			break;
		case 1://scs_E:
			components[i]->sprites[0].setTextureRect(sf::IntRect(100, 0, 100, 100));
			break;
		case 2://scs_W:
			components[i]->sprites[0].setTextureRect(sf::IntRect(200, 0, 100, 100));
			break;
		case 8://scs_N:
			components[i]->sprites[0].setTextureRect(sf::IntRect(300, 0, 100, 100));
			break;
		case 5://scs_SE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(400, 0, 100, 100));
			break;
		case 10://scs_NW:
			components[i]->sprites[0].setTextureRect(sf::IntRect(500, 0, 100, 100));
			break;
		case 9://scs_NE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(600, 0, 100, 100));
			break;
		case 6://scs_SW:
			components[i]->sprites[0].setTextureRect(sf::IntRect(700, 0, 100, 100));
			break;
		case 12://scs_NS:
			components[i]->sprites[0].setTextureRect(sf::IntRect(800, 0, 100, 100));
			break;
		case 3://scs_WE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(900, 0, 100, 100));
			break;
		case 11://scs_NWE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(1000, 0, 100, 100));
			break;
		case 14://scs_NSW:
			components[i]->sprites[0].setTextureRect(sf::IntRect(1100, 0, 100, 100));
			break;
		case 13://scs_NSE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(1200, 0, 100, 100));
			break;
		case 7://scs_SWE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(1300, 0, 100, 100));
			break;
		case 15://scs_NSWE:
			components[i]->sprites[0].setTextureRect(sf::IntRect(1400, 0, 100, 100));
			break;
		case 0://scs_none:
			if (data.grid[components[i]->gridLocationX][components[i]->gridLocationY].childUp == false)
			components[i]->sprites[0].setTextureRect(sf::IntRect(1500, 0, 100, 100));
			break;
		}
	}
}


int Player::getMass()
{
	return shipMass;
}
