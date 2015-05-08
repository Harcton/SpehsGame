#include "Main.h"
#include "Turret.h"//includes component.h
#include "Engine.h"//includes component.h
#include "Object.h"
#include "Game.h"



Component::~Component()
{
	switch (destructorMode)
	{
	case component::wreck:
		//Destroy component's children
		while (childComponents.size() > 0)
		{
			for (unsigned int i = 0; i < master->components.size(); i++)
				if (master->components[i]->id == childComponents.back())
				{
				master->components[i]->hp = -999;
				break;
				}
			childComponents.pop_back();
		}
		master->notifyComponentDestruction(this);
		break;
	case component::quit:
		break;
	}
}
Component::Component(Object* mstr, Object* cntr, double xo, double yo, int gx, int gy) : Component::Component(mstr, cntr, xo, yo)
{
	gridLocationX = gx;
	gridLocationY = gy;
}
Component::Component(Object* mstr, Object* cntr, double xo, double yo)
{
	//std::cout << "  componentConstructor";
	currentId++;
	id = currentId;
	//std::cout << "\nComponent " << id << " constructor";
	master = mstr;
	centerObj = cntr;

	xOffset = xo;
	yOffset = yo;
	xOffsetOriginal = xo;
	yOffsetOriginal = yo;

	hp = 100;
	maxHp = hp;
	textureRadius = 50;

	drawCollisionCircle = false;

}
Component::Component(const Component& other) : master(other.master), centerObj(other.master->mGame->playerObj)
{//Copy constructor
	std::cout << "\n  Component copy constructor";
	for (unsigned int i = 0; i < other.types.size(); i++)
		types.push_back(other.types[i]);
	for (unsigned int i = 0; i < other.sprites.size(); i++)
		sprites.push_back(other.sprites[i]);
	for (unsigned int i = 0; i < other.animatedSprites.size(); i++)
		animatedSprites.push_back(other.animatedSprites[i]);
	for (unsigned int i = 0; i < other.childComponents.size(); i++)
		childComponents.push_back(other.childComponents[i]);
	currentId++;
	id = currentId;
	masterComponent = other.masterComponent;
	xOffset = other.xOffset;
	yOffset = other.yOffset;
	xOffsetOriginal = other.xOffsetOriginal;
	yOffsetOriginal = other.yOffsetOriginal;
	x = other.x;
	y = other.y;
	screenX = other.screenX;
	screenY = other.screenY;
	angle = other.angle;
	angleModifier = other.angleModifier;
	maxAngle = other.maxAngle;
	capacity = other.capacity;
	rechargeInterval = other.rechargeInterval;
	maxSpeed = other.maxSpeed;
	damage = other.damage;
	fireRateInterval = other.fireRateInterval;
	thrustStrength = other.thrustStrength;
	rechargeAmount = other.rechargeAmount;
	mouseAim = other.mouseAim;
	mouseAimRelativeToCenter = other.mouseAimRelativeToCenter;
	hp = other.hp;
	maxHp = other.maxHp;
	textureRadius = other.textureRadius;
	gridLocationX = other.gridLocationX;
	gridLocationY = other.gridLocationY;
	hasFired = other.hasFired;
	reloading = other.reloading;
	charge = other.charge;
	rotationDirection = other.rotationDirection;
	holdToThrust = other.holdToThrust;
	autoThrusting = other.autoThrusting;
}
Component& Component::operator=(Component other)
{//Assignment operator
	std::cout << "\n  Component assignemnt operator called";
	types.clear();
	sprites.clear();
	animatedSprites.clear();
	childComponents.clear();
	for (unsigned int i = 0; i < other.types.size(); i++)
		types.push_back(other.types[i]);
	for (unsigned int i = 0; i < other.sprites.size(); i++)
		sprites.push_back(other.sprites[i]);
	for (unsigned int i = 0; i < other.animatedSprites.size(); i++)
		animatedSprites.push_back(other.animatedSprites[i]);
	for (unsigned int i = 0; i < other.childComponents.size(); i++)
		childComponents.push_back(other.childComponents[i]);
	currentId++;
	id = currentId;
	masterComponent = other.masterComponent;
	xOffset = other.xOffset;
	yOffset = other.yOffset;
	xOffsetOriginal = other.xOffsetOriginal;
	yOffsetOriginal = other.yOffsetOriginal;
	x = other.x;
	y = other.y;
	screenX = other.screenX;
	screenY = other.screenY;
	angle = other.angle;
	angleModifier = other.angleModifier;
	maxAngle = other.maxAngle;
	capacity = other.capacity;
	rechargeInterval = other.rechargeInterval;
	maxSpeed = other.maxSpeed;
	damage = other.damage;
	fireRateInterval = other.fireRateInterval;
	thrustStrength = other.thrustStrength;
	rechargeAmount = other.rechargeAmount;
	mouseAim = other.mouseAim;
	mouseAimRelativeToCenter = other.mouseAimRelativeToCenter;
	hp = other.hp;
	maxHp = other.maxHp;
	textureRadius = other.textureRadius;
	gridLocationX = other.gridLocationX;
	gridLocationY = other.gridLocationY;
	hasFired = other.hasFired;
	reloading = other.reloading;
	charge = other.charge;
	rotationDirection = other.rotationDirection;
	holdToThrust = other.holdToThrust;
	autoThrusting = other.autoThrusting;
	return *this;
}
Component& Component::operator=(Component&&)
{
	std::cout << "\n  -Component move assigment";
	return *this;
}
Component::Component(Component&& other)
{
	std::cout << "\n  -Component move constructor";
}


bool Component::alive()
{
	if (hp <= 0)
	{
		return false;
	}

	return true;
}
void Component::update()
{
	//Spin damage
	if (rollDie(200) == 0)
		hp -= floor((getDistance(0, 0, xOffset, yOffset)*abs(master->turnSpeed))/40);
	

	//Angle
	angle += master->turnSpeed;
	if (angle > 2 * PI)
		angle -= 2 * PI;
	else if (angle < 0)
		angle += 2 * PI;


	// X/Y
	x = master->x + xOffset*cos(master->angle) + yOffset*sin(master->angle);
	y = master->y - xOffset*sin(master->angle) + yOffset*cos(master->angle);

	screenX = centerObj->screenX + resFactor*zoomFactor*(x - centerObj->x);
	screenY = centerObj->screenY + resFactor*zoomFactor*(y - centerObj->y);

	for (unsigned int i = 0; i < sprites.size(); i++)
	{
		sprites[i].setPosition(screenX, screenY);
		sprites[i].setRotation(360 - (angle / PI) * 180);
		sprites[i].setScale(resFactor*zoomFactor, resFactor*zoomFactor);
	}
	for (unsigned int i = 0; i < animatedSprites.size(); i++)
	{
		animatedSprites[i].setPosition(screenX, screenY);
		animatedSprites[i].setRotation(360 - (angle / PI) * 180);
		animatedSprites[i].setScale(resFactor*zoomFactor, resFactor*zoomFactor);
	}

	for (unsigned int i = 0; i < types.size(); i++)
		if (types[i] == component::hull)
			for (unsigned int s = 0; s < sprites.size(); s++)
			{
				int temp_notRed = int(255.0f*(hp / float(maxHp)));
				sprites[s].setColor(sf::Color(255, temp_notRed, temp_notRed));
			}


	//DEBUG
	if (drawCollisionCircle == false)
		return;
	collisionCircle.setRadius(textureRadius);
	collisionCircle.setFillColor(sf::Color(95, 125, 255, 70));
	collisionCircle.setPosition(screenX - resFactor*zoomFactor*textureRadius, screenY - resFactor*zoomFactor*textureRadius);
	collisionCircle.setScale(resFactor*zoomFactor, resFactor*zoomFactor);


}


void Component::draw()
{
	if (sprites.size() > 0)
		for (unsigned int i = 0; i < sprites.size(); i++)
			master->mWindow.draw(sprites[i]);
	if (animatedSprites.size() > 0)
		for (unsigned int i = 0; i < animatedSprites.size(); i++)
			animatedSprites[i].draw(master->mWindow);

	//DEBUG
	if (drawCollisionCircle == false)
		return;
	master->mWindow.draw(collisionCircle);
}

void Component::createChild(double ox, double oy, component::Type tp)
{
	switch (tp)
	{
	case component::hull:
		master->components.push_back(new Component(master, centerObj, ox, oy));
		childComponents.push_back(master->components.back()->id);
		master->components.back()->sprites.push_back(sf::Sprite());
		master->components.back()->sprites.back().setTexture(RM.getTexture("skeleton.png"));
		master->components.back()->sprites.back().setTextureRect(sf::IntRect(1400, 0, 100, 100));
		break;
	case component::turret: //Turret
		master->components.push_back(new Turret(master, centerObj, ox, oy));
		childComponents.push_back(master->components.back()->id);
		break;
	case component::engine: //Engine
		master->components.push_back(new Engine(master, ox, oy));
		childComponents.push_back(master->components.back()->id);
		break;
	}


}