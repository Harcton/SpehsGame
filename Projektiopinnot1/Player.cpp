#include "Main.h"
#include "Game.h"
#include "Object.h"
#include "Player.h"


Player::~Player()
{
}
Player::Player(sf::RenderWindow& windowref, Game* game, bool this_is_the_player, int cx, int cy) : Object(windowref, game, cx, cy)
{
	centerObj = this;

	tex.loadFromFile("Texture/ship_base.png");
	spr.setTexture(tex);
	spr.setOrigin(50, 50);
	textureRadius = 50;

}

void Player::update()
{

		

	mWindow.pollEvent(mEvent);
	
	
	//Speed control
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		accelerate();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		reverse();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		turnLeft();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		turnRight();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		xSpeed = 0;
		ySpeed = 0;
		turnSpeed = 0;
	}

	Object::checkCollisions();

	Object::update();

	std::cout << "\n" << angle;
	//std::cout << "\n" << angle << "  xSpeed: " << xSpeed << " ySpeed: " << ySpeed;
	//std::cout << "\nX: " << x << "\n  Y: " << y;


}

void Player::turnRight()
{
	turnSpeed -= PI / 1800;
}

void Player::turnLeft()
{
	turnSpeed += PI / 1800;
}

void Player::accelerate()
{
	xSpeed += cos(angle)*0.04;
	ySpeed += sin(angle)*0.04;


}

void Player::reverse()
{
	xSpeed -= cos(angle)*0.02;
	ySpeed -= sin(angle)*0.02;
}