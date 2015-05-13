#ifndef OBJECT_H
#define OBJECT_H
//Forward declared dependencies
class Game;
class Component;
class PlayerData;
//Included dependencies
#include <math.h>
#include <vector>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderWindow.hpp>


class Object
{
public:
	~Object();
	Object(sf::RenderWindow&, Game*, int, int);
	Object(sf::RenderWindow&, Game*);
	Object(Game* game, Object* master, int x, int y, float angle, float speed, int damage);//Bullet constructor
	Object(const Object& other);
	Object& operator=(Object);
	Object(Object&& other);
	Object& operator=(Object&&);

	friend class Game;
	friend class Component;
	friend class Engine;
	friend class Enemy;
	friend class Seeker;
	friend class Sentinel;
	friend class Flier;
	friend class Commander;
	friend class Sentry;

	virtual bool update();
	void updateComponents();
	bool isBulletUpdate();
	void checkCollisions(unsigned int);
	void checkBulletCollision(Object*);
	virtual void notifyComponentDestruction(Component*){}
	virtual void draw();
	void setRandomLocation();
	void setLocation(double, double);


	sf::Sprite spr;
	double textureRadius;
	double scale = 1;

	double x;
	double y;

	double massCenterX;
	double massCenterY;

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

	//Virtual methods
	virtual int getMass(){ return 1; };
	virtual void editShip(){}

protected:
	Object* centerObj;
	std::vector<Component*> components;
	PlayerData* dataPtr;
	int opacity = 0;
	double collisionCheckAngle;
	double checkCollisionDistance;
	double checkCollisionRange;
	int hp = 100;
	int isBullet = 0; //0 = is not a bullet, 0 != is bullet, value equals damage upon impact
	Object* master;
};

#endif