#include <iostream>
#include "sprite.h"
#include "gamedata.h"

class Chunk : public Sprite {
public:
  explicit Chunk(
    const Vector2f& pos, const Vector2f vel, 
    const string& name, const Frame* fm) :
    Sprite(name, pos, vel, fm), 
		previousTicks(0),
    maxDuration(Gamedata::getInstance()->getXmlInt(name+"Duration")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    previousTicks = 0;
  }
private:
	Uint32 previousTicks;
  Uint32 maxDuration;
  bool tooFar;
};
