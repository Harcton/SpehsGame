#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Enemy.h"


Enemy::Enemy(sf::RenderWindow& windowref, Game* game, std::vector<Object*>& rVector) : refVector(rVector), Object(windowref, game)
{
	tex.loadFromFile("Texture/enemy_base.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;
}


Enemy::~Enemy()
{
		
}




bool Enemy::update()
{	
	return Object::update();
}