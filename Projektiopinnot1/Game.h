#ifndef GAME_H
#define GAME_H

class Object;
class Background;

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
	std::vector<Debris*> debris;
};

#endif