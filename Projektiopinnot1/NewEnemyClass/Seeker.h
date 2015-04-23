#ifndef SEEKER_H
#define SEEKER_H

class Seeker : public Enemy
{
public:
	Seeker(sf::RenderWindow&, Game*, std::vector<Object*>&, int);
	~Seeker();

	bool update();
	void AIupdate();
	void dodgeMove();

private:
	bool dodging;
	int dodgeCounter;
	int explosionLimiterTimer;
};

#endif