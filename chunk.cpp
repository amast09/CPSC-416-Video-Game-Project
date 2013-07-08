#include <iostream>
#include <cmath>

#include "chunk.h"

void Chunk::update(Uint32 ticks) { 
  float yincr = velocityY() * static_cast<float>(ticks) * 0.001;
  Y(Y()- yincr);
  float xincr = velocityX() * static_cast<float>(ticks) * 0.001;
  X(X()- xincr);
	if(previousTicks == 0) previousTicks += ticks;
  else if((ticks + previousTicks) > maxDuration) tooFar = true;
	else previousTicks += ticks;
}

