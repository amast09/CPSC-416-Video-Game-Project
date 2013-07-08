#include <iostream>
#include "multiFrameSprite.h"
#include "gamedata.h"

class Bullet : public MultiFrameSprite {
public:
  explicit Bullet(const string& name, const Vector2f& pos, const Vector2f& vel, 
									std::vector<Frame *> frames) :
    MultiFrameSprite(name, pos, vel, frames), 
    distance(0), 
    maxDistance(Gamedata::getInstance()->getXmlInt(name+"Distance")), 
    tooFar(false) 
  { }
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset() {
    tooFar = false;
    distance = 0;
  }
  bool collidedWith(const MultiFrameSprite* obj) const;

private:
  float distance;
  float maxDistance;
  bool tooFar;
};
