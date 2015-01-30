#ifndef GAME_H
#define GAME_H

class Object;
class Background;
class Enemy;

class Debris;


class Game
{
public:
	Game();
	~Game();

	void run();
	Object* playerObj;
	std::vector<Object*> objects;

protected:
	std::vector<Background*> backgrounds;
<<<<<<< HEAD
	std::vector<Enemy*> enemies;
=======
	std::vector<Debris*> debris;
>>>>>>> origin/master
};

#endif