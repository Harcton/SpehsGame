#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"

Component::~Component()
{
	for (unsigned int i = 0; i < childComponents.size(); i++)
	{
		master->removeComponent(childComponents[i]);
	}
}
Component::Component(Object* mstr, Object* cntr, double xo, double yo)
{
	currentId++;
	id = currentId;

	master = mstr;
	centerObj = cntr;

	xOffset = xo;
	yOffset = yo;

	hp = 100;
	textureRadius = 50;
	spr.setTexture(skeletonTex);
	spr.setTextureRect(sf::IntRect(1400, 0, 100, 100));
}

bool Component::update()
{
	if (hp <= 0)
		return false;

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

	spr.setPosition(screenX, screenY);
	spr.setRotation(360 - (angle / PI) * 180);
	spr.setScale(resFactor*zoomFactor, resFactor*zoomFactor);

	spr.setColor(sf::Color(255, 255 * (hp / 100.f), 255 * (hp / 100.f)));

	return true;
}


void Component::draw()
{
	master->mWindow.draw(spr);
}

void Component::createChild(double ox, double oy, int tp)
{
	switch (tp)
	{
	case 0:
		master->components.push_back(new Component(master, centerObj, ox, oy));
		childComponents.push_back(master->components[master->components.size()-1]->id);
		break;
	case 1: //Turret
		master->components.push_back(new Turret(master, centerObj, ox, oy));
		childComponents.push_back(master->components[master->components.size() - 1]->id);
		std::cout << "TURRET";
		break;

	}


}