#ifndef TWOWAYSPRITE__H
#define TWOWAYSPRITE__H

#include <string>

#include "multiFrameSprite.h"

class TwoWaySprite : public MultiFrameSprite {
public:
	TwoWaySprite(const std::string& name, float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, scale) {  }

	TwoWaySprite(const std::string& name, 
							 float x, 
							 float y, 
							 float sX, 
							 float sY, 
							 float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, x, y, sX, sY, scale) {  }

	virtual ~TwoWaySprite() {}

	virtual void update(Uint32 ticks) {

		setTime(getTime() + 1);

// If the frame delay has expired move to the next frame
		if(!(getTime() % getDelay()))	{ setOffset(getOffset() + 1); }

// If you have moved past the last frame, start at the beginning of the
// sequence
		if(getOffset() == (getTotalOffset() - 1))	{ setOffset(0); }

// If the velocity changes direction, make sure you are on the correct sequence 
// of frames corresponding to the current direction it is moving in
		if(velocityX() > 0 && getLevel() == 1)	{ setLevel(0); }
		else if(velocityX() < 0 && getLevel() == 0) { setLevel(1); }

// Update parent MultiFrameSprite class
		MultiFrameSprite::update(ticks);
	}

};

#endif
