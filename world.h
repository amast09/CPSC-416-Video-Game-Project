#include "frame.h"
#include "viewport.h"

class World {
public:
  World(Frame* const frm, int fact) : 
      frame(*frm),
      frameWidth( frame.getWidth() ),
      worldWidth( Gamedata::getInstance()->getXmlInt("worldWidth") ),
      factor(fact),
      viewX(0.0), viewY(0.0), 
      view(Viewport::getInstance()) 
  {}
  void update();
  void draw() const;
private:
  Frame const frame;
  unsigned frameWidth;
  unsigned worldWidth;
  int factor;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
