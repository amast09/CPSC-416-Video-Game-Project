#ifndef PLAYER__H
#define PLAYER__H

#include "multiFrameSprite.h"
#include "bullets.h"
#include "gamedata.h"

class Player {
public:
	Player(MultiFrameSprite *rhs);

	virtual ~Player() {}

	void update(Uint32 ticks);
	void draw() const; 
  bool shoot();
  bool playerCollision(const MultiFrameSprite*);
  bool missleCollision(const MultiFrameSprite*) const;

	void stop();
	void up();
	void right();
	void left();

	MultiFrameSprite* getMFS() const {	return(player);	}
	bool deadYet() const {	return(dead);	}
	void toggleMortality()	{	dead = !dead;	}
	void setGravity(float rhs)	{	gravity = rhs; }


private:
	MultiFrameSprite* player;
	bool dead;
	bool keyPress;

	float gravity;
	float lift;
	int minVelocity;
	int maxVelocity;
	float peakAcc;

  std::string bulletName;
  Bullets bullets;
  float minSpeed;
	Vector2f rightPosition;
	Vector2f leftPosition;
  Player& operator=(const Player&);
};

#endif
