#include "Main.h"
#include "Object.h"
#include "Component.h"
#include "Turret.h"
#include "Bullet.h"

Turret::~Turret()
{
	while (!bullets.empty())
	{
		delete bullets.back();
		bullets.pop_back();
	}
}
Turret::Turret(Object* mstr, Object* cntr, double xo, double yo) : Component(mstr, cntr, xo, yo)
{
	tex.loadFromFile("Texture/turret.png");
	spr.setTexture(tex);
	spr.setOrigin(20, 50);
	maxAngle = PI/3;

	types.push_back(turret);
}


bool Turret::update()
{

	updateBullets();

	if (Component::update() == false)
		return false;


<<<<<<< HEAD






	//if (master->angle >= PI / 2 && master->angle <= PI + PI / 2)
	//{//master angle is within 2nd and 3rd section
	//	if (angle < master->angle - PI / 2)
	//		angle = master->angle - PI / 2;
	//	else if (angle > master->angle + PI / 2)
	//		angle = master->angle + PI / 2;
	//}
	//else
	//{//master angle is within 1st and 4th section
	//	if (master->angle >= 0 && master->angle < PI/2) //master angle within 1st section
	//	{
	//		if (angle >= master->angle && angle < master->angle + PI)
	//		{
	//			if (angle > master->angle + PI / 2)
	//				angle = master->angle + PI / 2;
	//		}
	//		else if (angle > PI/2)
	//			if (angle < master->angle + PI + PI / 2)
	//				angle = master->angle + PI + PI / 2;
	//	}
	//	else //master angle is within 4th section
	//	{
	//		if (angle < master->angle && angle > master->angle - PI)
	//		{
	//			if (angle < master->angle - PI / 2)
	//				angle = master->angle - PI / 2;
	//		}
	//		else if (angle < master->angle) // angle is within 1st or 2nd section
	//			if (angle > master->angle - PI - PI / 2)
	//				angle = master->angle - PI - PI / 2;
	//	}
	//}



=======
>>>>>>> origin/master
	if (master->angle >= maxAngle && master->angle <= 2*PI - maxAngle)
	{
		if (angle < master->angle - maxAngle)
			angle = master->angle - maxAngle;
		else if (angle > master->angle + maxAngle)
			angle = master->angle + maxAngle;
	}
	else
	{
		if (master->angle >= 0 && master->angle < maxAngle) //master angle is within 0 -> +maxAngle
		{
			if (angle > master->angle)
			{
				if (angle < master->angle + PI)
				{
					if (angle > master->angle + maxAngle)
						angle = master->angle + maxAngle;
				}
				else if (angle < master->angle + 2 * PI - maxAngle)
					angle = master->angle + 2 * PI - maxAngle;
			}
		}
		else //master angle is within 0 -> -maxAngle
		{
			if (angle < master->angle) //if angle is out of the {0 -> -maxAngle} sector  ->it might be invalid
			{
				if (angle > master->angle - PI)
				{
					if (angle < master->angle - maxAngle) //A
						angle = master->angle - maxAngle;
				}
				else if (angle > master->angle - 2*PI + maxAngle) //B
					angle = master->angle - 2*PI + maxAngle;
			}
		}
	}
	return true;
}

void Turret::fixAngle()
{
	if (master->angle >= maxAngle && master->angle <= 2 * PI - maxAngle)
	{
		if (angle < master->angle - maxAngle)
			angle = master->angle - maxAngle;
		else if (angle > master->angle + maxAngle)
			angle = master->angle + maxAngle;
	}
	else
	{
		if (master->angle >= 0 && master->angle < maxAngle) //master angle is within 0 -> +maxAngle
		{
			if (angle > master->angle)
			{
				if (angle < master->angle + PI)
				{
					if (angle > master->angle + maxAngle)
						angle = master->angle + maxAngle;
				}
				else if (angle < master->angle + 2 * PI - maxAngle)
					angle = master->angle + 2 * PI - maxAngle;
			}
		}
		else //master angle is within 0 -> -maxAngle
		{
			if (angle < master->angle) //if angle is out of the {0 -> -maxAngle} sector  ->it might be invalid
			{
				if (angle > master->angle - PI)
				{
					if (angle < master->angle - maxAngle) //A
						angle = master->angle - maxAngle;
				}
				else if (angle > master->angle - 2 * PI + maxAngle) //B
					angle = master->angle - 2 * PI + maxAngle;
			}
		}
	}
}




void Turret::updateBullets()
{
	//Bullet update
	for (bulletIt = bullets.begin(); bulletIt != bullets.end();)
		if ((*bulletIt)->update() == false)
		{
		delete (*bulletIt);
		bulletIt = bullets.erase(bulletIt);
		}
		else
		{
			++bulletIt;
		}
	for (unsigned int i = 0; i < bullets.size(); i++)
		bullets[i]->draw();
}

void Turret::fire()
{
	bullets.push_back(new Bullet(this, this->master->mWindow, this->master->mGame, this->x, this->y, this->angle, 25));
}




//Turret angle for fixed maxAngle PI/2
//
//if (master->angle >= PI / 2 && master->angle <= PI + PI / 2)
//{//master angle is within 2nd and 3rd section
//	if (angle < master->angle - PI / 2)
//		angle = master->angle - PI / 2;
//	else if (angle > master->angle + PI / 2)
//		angle = master->angle + PI / 2;
//}
//else
//{//master angle is within 1st and 4th section
//	if (master->angle >= 0 && master->angle < PI/2) //master angle within 1st section
//	{
//		if (angle >= master->angle && angle < master->angle + PI)
//		{
//			if (angle > master->angle + PI / 2)
//				angle = master->angle + PI / 2;
//		}
//		else if (angle > PI/2)
//			if (angle < master->angle + PI + PI / 2)
//				angle = master->angle + PI + PI / 2;
//	}
//	else //master angle is within 4th section
//	{
//		if (angle < master->angle && angle > master->angle - PI)
//		{
//			if (angle < master->angle - PI / 2)
//				angle = master->angle - PI / 2;
//		}
//		else if (angle < master->angle) // angle is within 1st or 2nd section
//			if (angle > master->angle - PI - PI / 2)
//				angle = master->angle - PI - PI / 2;
//	}
//}
