#include <iostream>
#include <cmath>
#include <limits>
#include "multiFrameSprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "collisionStrategy.h"

using std::cout; using std::endl;

MultiFrameSprite:: ~MultiFrameSprite(){
	delete strategy;
}
 
float getRandMFS(int min, int max) {
  return min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min);
}

// Call the frameFactory's getFrameVector to return a vector of frames 
// corresponding to the sprite
MultiFrameSprite::MultiFrameSprite(const std::string& name, float scale):
  Drawable(name,
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"X"), 
                    Gamedata::getInstance()->getXmlInt(name+"Y")), 
           Vector2f(Gamedata::getInstance()->getXmlInt(name+"SpeedX"),
                    Gamedata::getInstance()->getXmlInt(name+"SpeedY"))
  ),  
  frames(FrameFactory::getInstance().getFrameVector(name, scale)),
	strategy(new PerPixelCollisionStrategy),
  tOffset(Gamedata::getInstance()->getXmlInt(name+"XFrames")),
  offset(Gamedata::getInstance()->getXmlInt(name+"Offset")),
  level(Gamedata::getInstance()->getXmlInt(name+"Level")),
  delay(Gamedata::getInstance()->getXmlInt(name+"Delay")),
  time(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{ }

 MultiFrameSprite::MultiFrameSprite(const std::string& name,
																		const Vector2f& pos, const Vector2f& vel,
																		const std::vector<Frame *> frms):
  Drawable(name, pos, vel),  
  frames(frms),
	strategy(new PerPixelCollisionStrategy),
  tOffset(Gamedata::getInstance()->getXmlInt(name+"XFrames")),
  offset(Gamedata::getInstance()->getXmlInt(name+"Offset")),
  level(Gamedata::getInstance()->getXmlInt(name+"Level")),
  delay(Gamedata::getInstance()->getXmlInt(name+"Delay")),
  time(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{}


MultiFrameSprite::MultiFrameSprite(const std::string& name, float x, float y, float sX, float sY, float scale):
  Drawable(name, Vector2f(x,y), Vector2f(sX,sY)),  
  frames(FrameFactory::getInstance().getFrameVector(name, scale)),
	strategy(new PerPixelCollisionStrategy),
  tOffset(Gamedata::getInstance()->getXmlInt(name+"XFrames")),
  offset(Gamedata::getInstance()->getXmlInt(name+"Offset")),
  level(Gamedata::getInstance()->getXmlInt(name+"Level")),
  delay(Gamedata::getInstance()->getXmlInt(name+"Delay")),
  time(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{}

MultiFrameSprite::MultiFrameSprite(const MultiFrameSprite& rhs) :
  Drawable(rhs.getName(), rhs.getPosition(), rhs.getVelocity()), 
	frames(rhs.frames),
	strategy(new PerPixelCollisionStrategy),
  tOffset(rhs.tOffset),
	offset(rhs.offset),
	level(rhs.level),
	delay(rhs.delay),
	time(rhs.time), 
	frameWidth(rhs.getFrame()->getWidth()),
  frameHeight(rhs.getFrame()->getHeight()),
  worldWidth(Gamedata::getInstance()->getXmlInt("worldWidth")),
  worldHeight(Gamedata::getInstance()->getXmlInt("worldHeight"))
{ }

MultiFrameSprite& MultiFrameSprite::operator=(const MultiFrameSprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
	frames = rhs.frames;
	strategy = new PerPixelCollisionStrategy;
  tOffset = rhs.tOffset;
	offset = rhs.offset;
	level = rhs.level;
	delay = rhs.delay;
 	time = rhs.time;
	frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = Gamedata::getInstance()->getXmlInt("worldWidth");
  worldHeight = Gamedata::getInstance()->getXmlInt("worldHeight");
  return *this;
}

bool MultiFrameSprite::collidedWith(const MultiFrameSprite * obj) const{
	return strategy->execute(*this, *obj);
}

// Based off Sprite draw()
void MultiFrameSprite::draw() const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[getIndex()]->draw(x, y); 
}

// Based off Sprite getPixel()
unsigned MultiFrameSprite::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = 
		static_cast<Uint32 *>(frames[getIndex()]->getSurface()->pixels);
  return pixels[ ( y * frames[getIndex()]->getWidth() ) + x ];
}

// MultiFrameSprite takes care of updating the viewport for child classes
// MultiFrameSprite does no other actions updating the sprite,
// child classes have their own logic that takes care of how to update
// the current frame of the sprite
void MultiFrameSprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}
