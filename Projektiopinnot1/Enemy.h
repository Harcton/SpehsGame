#ifndef ENEMY_H
#define ENEMY_H

class Object;

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Object*>&);
	~Enemy();

	bool update();

private:
	std::vector<Object*>& refVector;
};

#endif