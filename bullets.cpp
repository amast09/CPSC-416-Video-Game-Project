#include <iostream>
#include <cmath>
#include "ioManager.h"
#include "bullets.h"

Bullets::~Bullets() {
  SDL_FreeSurface( bulletSurface );
	for(unsigned i = 0; i < bulletFrames.size(); i++){
		delete bulletFrames[i];
	}
  delete strategy;
}

Bullets::Bullets(const std::string& n) :
  name(n),
  strategy( new PerPixelCollisionStrategy ),
  bulletSurface(IOManager::getInstance().loadAndSet(
    Gamedata::getInstance()->getXmlStr(name+"File"), true)
  ),
  frameInterval(Gamedata::getInstance()->getXmlInt(name+"Interval")),
  timeSinceLastFrame( 0 ),
  bulletList(),
  freeList()
{
		int xFrames = Gamedata::getInstance()->getXmlInt(name+"XFrames");
		int yFrames = Gamedata::getInstance()->getXmlInt(name+"YFrames");
		bulletFrames.reserve(xFrames * yFrames);
		int srcX = Gamedata::getInstance()->getXmlInt(name+"SrcX");
		int srcY = Gamedata::getInstance()->getXmlInt(name+"SrcY");
		int width = Gamedata::getInstance()->getXmlInt(name+"Width") / xFrames;
		int height = Gamedata::getInstance()->getXmlInt(name+"Height") / yFrames;

	for(int y = 0; y < yFrames; y++){
		for(int x = 0; x < xFrames; x++){
	 	  Frame * const frame =
				new Frame(bulletSurface, width, height, srcX+(x*width), srcY+(y*height));
			bulletFrames.push_back(frame);
		}
	}

}

Bullets::Bullets(const Bullets& b) :
  name(b.name),
  strategy(b.strategy),
  bulletSurface(b.bulletSurface),
  bulletFrames(b.bulletFrames), 
  frameInterval(b.frameInterval),
  timeSinceLastFrame( b.timeSinceLastFrame ),
  bulletList(b.bulletList), 
  freeList(b.freeList)
{ }

bool Bullets::collidedWith(const MultiFrameSprite* obj) const {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    if ( strategy->execute(*ptr, *obj) ) {
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
      return true;
    }
    ++ptr;
  }
  return false;
}

bool Bullets::shoot(const Vector2f& position, const Vector2f& velocity) {
	if (timeSinceLastFrame > frameInterval) {
    // If no bullets in pool, make one:
    if ( freeList.empty() ) {
      Bullet b(name, position, velocity, bulletFrames);
      bulletList.push_back( b );
    }
    else {
      Bullet b = freeList.front();
      freeList.pop_front();
      b.reset();
      b.setVelocity(velocity);
      b.setPosition(position);
      bulletList.push_back( b );
    }
		timeSinceLastFrame = 0;
		return(true);
	}
	else return(false);
}

void Bullets::draw() const { 
  std::list<Bullet>::const_iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->draw();
    ++ptr;
  }
}

void Bullets::update(Uint32 ticks) { 
	timeSinceLastFrame += ticks;
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a Bullet
      freeList.push_back(*ptr);
      ptr = bulletList.erase(ptr);
    }   
    else ++ptr;
  }
}
