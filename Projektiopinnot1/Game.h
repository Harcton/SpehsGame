#ifndef GAME_H
#define GAME_H

class Object;
class Background;
class Enemy;


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
	std::vector<Enemy*> enemies;
};

#endif