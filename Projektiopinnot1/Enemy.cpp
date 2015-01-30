#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Enemy*>& rVector) : refVector(rVector), Object(windowref, game)
{
	tex.loadFromFile("Texture/enemy_base.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;
}


Enemy::~Enemy()
{
		
}




bool Enemy::spawnUpdate()
{
	if (getDistance(x, y, centerObj->x, centerObj->y) > DESPAWN_RANGE)
		return false;

	Object::update();

	return true;
}