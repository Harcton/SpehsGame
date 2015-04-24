#ifndef SENTRY_H
#define SENTRY_H

class Sentry : public Enemy
{
public:
	Sentry(sf::RenderWindow&, Game*, int);
	~Sentry();

	bool update();
	void AIupdate();
	void dodgeMove();
	void reposition();
	void flee();

private:
	bool dodging;
	bool repositioning;
	bool fleeing;
	int dodgeCounter;
	int repositionCounter;
	int shootingCounter;
	int rotationCounter;
};

#endif