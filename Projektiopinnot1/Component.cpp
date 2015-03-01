#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Component.h"

Component::~Component()
{
}
Component::Component(Object* mstr, Object* cntr, double xo, double yo)
{
	master = mstr;
	centerObj = cntr;
	angle = master->angle;

	xOffset = xo;
	yOffset = yo;

}

bool Component::update()
{
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


	return true;
}


void Component::draw()
{
	master->mWindow.draw(spr);
}