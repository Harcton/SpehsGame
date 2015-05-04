#ifndef SEEKER_H
#define SEEKER_H
//Included dependencies
#include "Enemy.h"




enum AnimationID
{
	anim_active,
	anim_passive
};

enum enemyState
{
	state_spawned,
	state_contact,
	state_closeRange,
	state_active,
	state_detected,
	state_passive,
	state_dodging,
	state_activated
};

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
	enemyState state;
	enemyState memoryState;
};

#endif