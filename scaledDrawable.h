#ifndef SCALEDDRAWABLE__H
#define SCALEDDRAWABLE__H

#include "drawable.h"

class ScaledDrawable {
public:
  ScaledDrawable(Drawable * spr, float scl): sprite(spr), scale(scl) {}

	virtual ~ScaledDrawable()	{}

  bool operator<(const ScaledDrawable& rhs) const {
    return scale < rhs.scale;
  }
	Drawable * getDrawable() const {	return(sprite);	}
  virtual float getScale() const {	return(scale);	}

private:
	Drawable *sprite;
  float scale;
  ScaledDrawable& operator=(const ScaledDrawable&);
};
#endif
