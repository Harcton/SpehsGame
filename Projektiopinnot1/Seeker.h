#ifndef SEEKER_H
#define SEEKER_H

class Component;

class Seeker : public Enemy
{
public:
	Seeker(sf::RenderWindow&, Game*, int);
	~Seeker();

	bool update();
	void AIupdate();
	void dodgeMove();

private:
	bool dodging;
	bool explosionLimiter;
	int dodgeCounter;
};

#endif