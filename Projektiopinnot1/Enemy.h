#ifndef ENEMY_H
#define ENEMY_H

class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*, std::vector<Enemy*>&);
	~Enemy();

	bool spawnUpdate();

private:
	std::vector<Enemy*>& refVector;
};

#endif