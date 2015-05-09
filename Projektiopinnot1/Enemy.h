#ifndef ENEMY_H
#define ENEMY_H
//Forward declared dependencies
class Game;
//Included dependencies
#include "Object.h"
#include "Component.h"


enum AnimationID
{
	anim_spawn,
	anim_active,
	anim_passive,
	anim_dodge,
	anim_contact,
	anim_activation,
	anim_victory
};

enum enemyState
{
	state_spawned,
	state_activated,

	state_passive,
	state_detected,
	state_active,
	state_closeRange,
	state_contact,

	state_dodging,
	state_fleeing,
	state_repositioning,

	state_victory
};


class Enemy : public Object
{
public:
	Enemy(sf::RenderWindow&, Game*);
	~Enemy();

	virtual bool update();
	virtual void notifyComponentDestruction(Component*);
	virtual void complexUpdate();

	virtual void explosion(int, float);
	virtual void shootLaser(int);

protected:
	int metal = 0;//irandom max amount dropped upon death
	//Object
	bool rotationDirection;
	double closeAngle;
	double maxSpeedLimit;
	double maxTurnSpeedLimit;
	double accelerationConstant;
	double turnAccelerationConstant;
	std::vector<Component*>::iterator componentIt;
	
	//Ranges
	int aggroRange;
	int maxActionRange;
	int closeRange;

	//AI
	bool follow;
	int HPMemory;
	int enemyBehaviourLevel; //1: defensive 2: passive 3: aggressive
	double distance;
	double playerDirection;
	Component* nearestComponent;
	enemyState state;
	enemyState memoryState;
};

#endif