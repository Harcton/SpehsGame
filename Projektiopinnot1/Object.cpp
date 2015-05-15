#include "Main.h"
#include "Object.h"
#include "Game.h"
#include "Component.h"

Object::~Object()
{
	for (unsigned int i = 0; i < components.size(); i++)
		delete components[i];
}
Object::Object(sf::RenderWindow& windowref, Game* game) : mWindow(windowref)
{
	mGame = game;
	centerObj = game->playerObj;
	scale = 1;

	massCenterX = 0;
	massCenterY = 0;
}
Object::Object(sf::RenderWindow& windowref, Game* game, int cx, int cy) : mWindow(windowref)
{
	mGame = game;
	centerObj = mGame->playerObj;
	x = cx;
	y = cy;
}
Object::Object(Game* game, Object* mstr, int mx, int my, float angl, float spd, int dmg) : mWindow(game->mWindow), mGame(game)
{//Bullet constructor
	scale = 1;
	centerObj = game->playerObj;
	isBullet = dmg;
	master = mstr;
	x = mx;
	y = my;
	xSpeed = cos(angl)*spd + master->xSpeed;
	ySpeed = sin(angl)*spd + master->ySpeed;
	spr.setTexture(RM.bullet1Tex);
	spr.setOrigin(5, 5);
}
Object::Object(const Object& other) : mWindow(other.mWindow), mGame(other.mGame), centerObj(other.centerObj), dataPtr(other.dataPtr)
{//Copy constructor
	std::cout << "\n  Component copy constructor";
	for (unsigned int i = 0; i < other.components.size(); i++)
		components.push_back(other.components[i]);
	spr = other.spr;
	textureRadius = other.textureRadius;
	scale = other.scale;
	x = other.x;
	y = other.y;
	massCenterX = other.massCenterX;
	massCenterY = other.massCenterY;
	screenX = other.screenY;
	screenY = other.screenX;
	angle = other.angle;
	xSpeed = other.xSpeed;
	ySpeed = other.ySpeed;
	turnSpeed = other.turnSpeed;
	accTimer = other.accTimer;
	xAcc = other.xAcc;
	yAcc = other.yAcc;
	xSpeed0 = other.xSpeed0;
	ySpeed0 = other.ySpeed0;
	scrSpeedX = other.scrSpeedX;
	scrSpeedY = other.scrSpeedY;
	relativeSpeedX = other.relativeSpeedX;
	relativeSpeedY = other.relativeSpeedY;
	xScreenDistance = other.xScreenDistance;
	yScreenDistance = other.yScreenDistance;
	hasCollisionChecks = other.hasCollisionChecks;
	opacity = other.opacity;
	collisionCheckAngle = other.collisionCheckAngle;
	checkCollisionDistance = other.checkCollisionDistance;
	checkCollisionRange = other.checkCollisionRange;
	hp = other.hp;
}
Object& Object::operator=(Object other)
{//Assignment operator
	std::cout << "\n  Object assignemnt operator called";
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
	for (unsigned int i = 0; i < other.components.size(); i++)
		components.push_back(other.components[i]);
	spr = other.spr;
	textureRadius = other.textureRadius;
	scale = other.scale;
	x = other.x;
	y = other.y;
	massCenterX = other.massCenterX;
	massCenterY = other.massCenterY;
	screenX = other.screenY;
	screenY = other.screenX;
	angle = other.angle;
	xSpeed = other.xSpeed;
	ySpeed = other.ySpeed;
	turnSpeed = other.turnSpeed;
	accTimer = other.accTimer;
	xAcc = other.xAcc;
	yAcc = other.yAcc;
	xSpeed0 = other.xSpeed0;
	ySpeed0 = other.ySpeed0;
	scrSpeedX = other.scrSpeedX;
	scrSpeedY = other.scrSpeedY;
	relativeSpeedX = other.relativeSpeedX;
	relativeSpeedY = other.relativeSpeedY;
	xScreenDistance = other.xScreenDistance;
	yScreenDistance = other.yScreenDistance;
	hasCollisionChecks = other.hasCollisionChecks;
	opacity = other.opacity;
	collisionCheckAngle = other.collisionCheckAngle;
	checkCollisionDistance = other.checkCollisionDistance;
	checkCollisionRange = other.checkCollisionRange;
	hp = other.hp;
	return *this;
}
Object& Object::operator=(Object&&)
{
	std::cout << "\n  -Object move assigment";
	return *this;
}
Object::Object(Object&& other) : mWindow(other.mWindow)
{
	std::cout << "\n  -Object move constructor";
}


void Object::setLocation(double mx, double my)
{
	x = mx;
	y = my;
}
void Object::setRandomLocation()
{
	float tempLocator = irandom(0, 359) * (PI / 180.0f);
	int tempDistance = irandom(SPAWN_RANGE + 300, SPAWN_RANGE);

	x = centerObj->x + tempDistance * cos(tempLocator);
	y = centerObj->y + tempDistance * sin(tempLocator);
}

bool Object::update()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE || hp <= 0)
	{
		return false;
	}

	//Bullet update
	if (isBullet != 0)
		if (isBulletUpdate() == false)
			return false;




	////update opacity
	//if (opacity < 255)
	//{
	//	opacity += 17;
	//	spr.setColor(sf::Color(255, 255, 255, opacity));
	//}

	//Update variable values
	angle += turnSpeed;	
	x += xSpeed;
	y += ySpeed;

	///////////////////////////////////////////////////////////////////
	//Usefull code dont remove: (Rotate point x1,y1 around point x2,y2)
	//x1 += cos(angle)*x2 - cos(angle + turnSpeed)*x2 + sin(angle)*y2 - sin(angle + turnSpeed)*y2;
	//y1 -= sin(angle)*x2 - sin(angle + turnSpeed)*x2 - cos(angle)*y2 + cos(angle + turnSpeed)*y2;
	///////////////////////////////////////////////////////////////////

	//Keep angle positi
	if (angle > 2*PI)
		angle -= 2*PI;
	else if (angle < 0)
		angle += 2*PI;


	//Update screen positions
	if (centerObj != this) //If the object is not the player
	{
		screenX = centerObj->screenX + resFactor*zoomFactor*(x - centerObj->x);
		screenY = centerObj->screenY + resFactor*zoomFactor*(y - centerObj->y);
	}

	//Apply variables
	spr.setPosition(screenX, screenY);
	spr.setScale(scale*resFactor*zoomFactor, scale*resFactor*zoomFactor);
	spr.setRotation(360 - (angle/PI)*180);
	return true;
}

void Object::updateComponents()
{
	int k = 0;
	while (k < components.size())
	{
		if (components[k]->alive() == false)
		{
			Component* temp_componentPointer = components[k];
			components.erase(components.begin() + k);
			delete temp_componentPointer;
			k = 0;
		}
		else
			++k;
	}

	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->update();
}

bool Object::isBulletUpdate()
{
	for (unsigned int i = 0; i < mGame->objects.size(); i++)
		if (mGame->objects[i] != master)
		{
		mGame->objects[i]->checkBulletCollision(this);
		}

	if (isBullet == 0)
		return false;
	return true;
}


void Object::draw()
{
	mWindow.draw(spr);
	for (unsigned int i = 0; i < components.size(); i++)
		components[i]->draw();
}

void Object::checkCollisions(unsigned int selfIndex)//Does this actually do anything?
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


void Object::checkBulletCollision(Object* b)
{
	float temp_coordinateModifier = resFactor*zoomFactor*textureRadius;
	for (unsigned int i = 0; i < components.size(); i++)
	{
		b->collisionCheckAngle = -1 * atan2(components[i]->y - b->y - temp_coordinateModifier, components[i]->x - b->x - temp_coordinateModifier);
		if (b->collisionCheckAngle < 0)
			b->collisionCheckAngle = ((2 * PI) + b->collisionCheckAngle);


		b->checkCollisionDistance = getDistance(b->x, b->y, components[i]->x - temp_coordinateModifier, components[i]->y - temp_coordinateModifier);
		b->checkCollisionRange = b->textureRadius + components[i]->textureRadius;

		if (b->checkCollisionDistance < b->checkCollisionRange)
		{
			if (b->isBullet != 0)
			{
				components[i]->hp -= b->isBullet;
				b->isBullet = 0;
			}

			//Bounce
			b->xSpeed = b->xSpeed*0.75;
			b->ySpeed = b->ySpeed*0.75;
			b->angle = PI / 2 + (irandom(0, 180) / double(180))*PI;
			b->xSpeed = cos(2 * PI - b->angle) * getDistance(0, 0, b->xSpeed, b->ySpeed);
			b->ySpeed = sin(2 * PI - b->angle) * getDistance(0, 0, b->xSpeed, b->ySpeed);
		}
	}
}