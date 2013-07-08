#ifndef ENEMY__H
#define ENEMY__H

#include "multiFrameSprite.h"
#include "player.h"
#include "gamedata.h"

class Enemy {
public:
	Enemy(MultiFrameSprite *en, Player *pl, float r, float a);

	virtual ~Enemy() {}

	void update(Uint32 ticks);
	void draw() const	{	enemy->draw();	} 

	MultiFrameSprite* getMFS() const {	return(enemy);	}

	enum STATE {NORMAL, ATTACK, VICTORY};

private:
	MultiFrameSprite* enemy;
	Player* player;
	float range;
	float attackSpeed;
	float normalX;
	float normalY;
	STATE state;
};

#endif
