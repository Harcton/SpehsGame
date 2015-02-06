#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector) : refVector(rVector), Object(windowref, game)
{
	tex.loadFromFile("Texture/enemy_base_cc.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;
}


Enemy::~Enemy()
{
		
}




bool Enemy::update()
{	

	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	playerDirection = -1 * atan2(mGame->playerObj->y - y, mGame->playerObj->x - x);
	if (playerDirection < 0)
		playerDirection = ((2 * PI) + playerDirection);
	
	//random movement?


	//"AI" to follow the player... sort of
	follow = false;

	distance = getDistance(x, y, mGame->playerObj->x, mGame->playerObj->y);
	if (distance > 300)
	{
		follow = true;
	}
	else if (distance < 300)
	{
		follow = false;
	}
	else if (distance > 600)
	{
		follow = false;
	}


	if (follow == true)
	{
		angle = playerDirection;
		xSpeed += cos(2 * PI - angle)*0.06;
		ySpeed += sin(2 * PI - angle)*0.06;
	}
	else
	{
		angle += 0.01;
		xSpeed -= cos(2 * PI - angle)*0.1;
		ySpeed -= sin(2 * PI - angle)*0.1;
	}
	//

	return Object::update();

}