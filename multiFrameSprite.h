#ifndef MULTIFRAMESPRITE__H
#define MULTIFRAMESPRITE__H

#include <string>
#include <iostream>
#include <vector>

#include "drawable.h"

class CollisionStrategy;
class MultiFrameSprite : public Drawable {
public:
	MultiFrameSprite(const std::string& name, float scale);
	MultiFrameSprite(const std::string& name, const Vector2f& pos, const Vector2f& vel, const std::vector<Frame *> frms);
	MultiFrameSprite(
		const std::string& name, float x, float y, float sX, float sY, float scale);
	MultiFrameSprite(const MultiFrameSprite& rhs);

  virtual ~MultiFrameSprite(); 

  MultiFrameSprite& operator=(const MultiFrameSprite& rhs);

	const std::vector<Frame *> getFrames() const { return frames; }
  virtual const Frame* getFrame() const { return frames[getIndex()]; }
  virtual void draw() const;
  virtual void update(Uint32 ticks);
	virtual bool collidedWith(const MultiFrameSprite*) const;
  unsigned getPixel(Uint32, Uint32) const;
	unsigned getTotalOffset() const { return tOffset; }
	unsigned getDelay() const	{ return delay; }
	unsigned getIndex() const { return((level * tOffset) + offset); }
	unsigned getOffset() const { return offset; }
	unsigned getLevel() const	{ return level; }
	unsigned getTime() const	{ return time; }
	unsigned getFrameWidth() const { return frameWidth; }
	unsigned getFrameHeight() const { return frameHeight; }
	unsigned getWorldWidth() const { return worldWidth; }
	unsigned getWorldHeight() const { return worldHeight; }
	void setOffset(const int rhs) { offset = rhs; } 
	void setLevel(const int rhs) { level = rhs; } 
	void setTime(const int rhs) { time = rhs; } 

private:
// Vector of frames is treated like a multi dimensional array
	std::vector<Frame *> frames;
	CollisionStrategy* strategy;
// Specifies the number of columns
  int tOffset;
// Specifies the current column you are at
	int offset;
// Specifies the current row you are at
	int level;
// Used for delay between switching frames
	int delay;
// Used as a measure of time other than ticks, giving
// a more linear progress of time / numbers
	int time;
  int frameWidth;
  int frameHeight;
  int worldWidth;
  int worldHeight;
};
#endif
