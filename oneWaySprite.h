#ifndef ONEWAYSPRITE__H
#define ONEWAYSPRITE__H

#include <string>

#include "multiFrameSprite.h"

class OneWaySprite : public MultiFrameSprite {
public:
	OneWaySprite(const std::string& name, float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, scale) {  }
	
	OneWaySprite(const std::string& name, float x, float y, float sX, float sY, float scale) : 
		MultiFrameSprite::MultiFrameSprite(name, x, y, sX, sY, scale) {  }

	virtual ~OneWaySprite() {}

	virtual void update(Uint32 ticks) {

		setTime(getTime() + 1);

// If you have moved past the last frame, start at the beginning of the
// sequence
		if(getOffset() == (getTotalOffset() - 1))	{ setOffset(0); }


// If the frame delay has expired move to the next frame
		if(!(getTime() % getDelay()))	{ setOffset(getOffset() + 1); }

		MultiFrameSprite::update(ticks);
	}

};

#endif
