#ifndef GAME_H
#define GAME_H

class Object;
class Background;


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
};

#endif