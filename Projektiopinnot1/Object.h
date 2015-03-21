#ifndef OBJECT_H
#define OBJECT_H


class Game;
class Bullet;
class PlayerData;

class Object
{
public:
	~Object();
	Object(sf::RenderWindow&, Game*, int, int);
	Object(sf::RenderWindow&, Game*);
	friend class Component;
	friend class Enemy;

	virtual bool update();
	void checkCollisions(unsigned int);
	virtual void checkBulletCollision(Bullet*);
	virtual void removeComponent(int){};
	void draw();

	sf::Texture tex;
	sf::Sprite spr;
	double textureRadius;
	double scale = 1;

	double x;
	double y;

	double screenY;
	double screenX;

	double angle = 0;

	double xSpeed = 0;
	double ySpeed = 0;
	double turnSpeed; // +CCW, -CW

	int accTimer;
	double xAcc;
	double yAcc;
	double xSpeed0;
	double ySpeed0;

	double scrSpeedX;
	double scrSpeedY;
	double relativeSpeedX;
	double relativeSpeedY;
	double xScreenDistance;
	double yScreenDistance;

	//Get methods
	int& getHpRef(){ return hp; }


	bool hasCollisionChecks = true;

	sf::RenderWindow& mWindow;
	Game* mGame;

protected:
	Object* centerObj;
	std::vector<Component*> components;
	PlayerData* data;//Only for player objects
	int opacity = 0;
	double collisionCheckAngle;
	double checkCollisionDistance;
	double checkCollisionRange;
	int hp = 100;
};

#endif