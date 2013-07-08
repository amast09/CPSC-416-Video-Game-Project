#include <iostream>
#include <cmath>
#include "bullet.h"
#include "collisionStrategy.h"

bool Bullet::collidedWith(const MultiFrameSprite* obj) const {
  return MultiFrameSprite::collidedWith(obj);
}

void Bullet::update(Uint32 ticks) { 
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

  Vector2f pos = getPosition();
// Update parent MultiFrameSprite class
	MultiFrameSprite::update(ticks);

  distance += ( hypot(X()-pos[0], Y()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
	if(X() < 0 || X() > getWorldWidth() - getFrameWidth()) tooFar = true;
}

