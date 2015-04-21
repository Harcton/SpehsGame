#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"

Component::~Component()
{
	//std::cout << "  componentDECONSTRUCTOR";

	//Remove child components
	for (unsigned int i = 0; i < childComponents.size(); i++)
	{
		master->removeComponent(childComponents[i]);
	}

	//Remove bullets
	while (!bullets.empty())
	{
		delete bullets.back();
		bullets.pop_back();
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
	textureRadius = 50;

	drawCollisionCircle = false;

}

bool Component::alive()
{
	if (hp <= 0)
	{
		std::cout << "\nComponent destroyed! [" << gridLocationX << ", " << gridLocationY << "]";
		master->notifyComponentDestruction(id);
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
	for (unsigned int i = 0; i < sprites.size(); i++)
		master->mWindow.draw(sprites[i]);
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
		childComponents.push_back(master->components[master->components.size() - 1]->id);
		master->components[master->components.size() - 1]->sprites.push_back(sf::Sprite());
		master->components[master->components.size() - 1]->sprites[master->components[master->components.size() - 1]->sprites.size() - 1].setTexture(RM.getTexture("skeleton.png"));
		master->components[master->components.size() - 1]->sprites[master->components[master->components.size() - 1]->sprites.size() - 1].setTextureRect(sf::IntRect(1400, 0, 100, 100));
		break;
	case component::turret: //Turret
		master->components.push_back(new Turret(master, centerObj, ox, oy));
		childComponents.push_back(master->components[master->components.size() - 1]->id);
		break;
	}


}