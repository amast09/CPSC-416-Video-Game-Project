#ifndef ACCSPRITE__H
#define ACCSPRITE__H

#include <string>

#include "multiFrameSprite.h"

class AccSprite : public MultiFrameSprite {
public:

	AccSprite(const std::string& name, float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, scale),
		accDelay(Gamedata::getInstance()->getXmlInt(name+"AccDelay")),
		acc(Gamedata::getInstance()->getXmlFloat(name+"Acc")),
		row(Gamedata::getInstance()->getXmlInt(name+"YFrames")),
		initVelocity(velocityX()) {  }

	AccSprite(const std::string& name, float x, float y, float sX, float sY, float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, x, y, sX, sY, scale),
		accDelay(Gamedata::getInstance()->getXmlInt(name+"AccDelay")),
		acc(Gamedata::getInstance()->getXmlFloat(name+"Acc")),
		row(Gamedata::getInstance()->getXmlInt(name+"YFrames")),
		initVelocity(velocityX()) {  }

	virtual ~AccSprite() {}

	virtual void update(Uint32 ticks) {

		setTime(getTime() + 1);

// If an acceleration delay has expired, increase the X velocity  & move to the
// next acceleration sequence, if you are on the last acceleration sequence, 
// don't increment the frame vector
		if(!(getTime() %  accDelay)){
			if(getLevel() < (row - 2)){	
				setLevel(getLevel() + 2);
			}
			velocityX(velocityX() + acc);
		}

// If the frame delay has expired move to the next frame
		if(!(getTime() % getDelay()))	{ setOffset(getOffset() + 1); }

// If you have moved past the last frame, start at the beginning of the
// sequence
		if(getOffset() == (getTotalOffset() - 1))	{ setOffset(0); }

// If the velocity has changed, reset the velocity to the initial velocity
// and start from the initial sequence, from the initial frame corresponding
// to the new direction you are traveling in, also set the acceleration
// incriment to reflect the current direction of travel

		if(velocityX() > 0 && (getLevel() % 2)){
			setLevel(0);
			setOffset(0);
			setTime(0); 
			acc*=-1;
			velocityX(initVelocity);
		}
		else if(velocityX() < 0 && !(getLevel() % 2)){
			setLevel(1);
			setOffset(0);
			setTime(0); 
			acc*=-1;
			velocityX(initVelocity); 
		}

// Call parent update
		MultiFrameSprite::update(ticks);
	}

private:
// Delay between acceleration increases
	unsigned accDelay;
// Acceleration increase per accDelay
	float acc;
// Rows of sequences corresponding to acceleration increases
	unsigned row;
// Float to keep track of starting velocity
	float initVelocity;
};

#endif
