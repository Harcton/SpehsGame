#ifndef SEEKER_H
#define SEEKER_H
//Included dependencies
#include "Enemy.h"


class Seeker : public Enemy
{
public:
	Seeker(sf::RenderWindow&, Game*, int);
	~Seeker();

	bool update();
	void AIupdate();
	void dodgeMove(const double, const double);
	void animationHandler(AnimationID);

private:
	bool dodging;
	bool explosionLimiter;
	int dodgeCounter;
	int activationCounter;
};

#endif