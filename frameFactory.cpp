#include <sstream>
#include "SDL/SDL_rotozoom.h"
#include "frameFactory.h"
#include "ioManager.h"
#include "vector2f.h"

FrameFactory::~FrameFactory() {
	std::list<SDL_Surface*>::iterator itScaled = scaled.begin();
	while(itScaled != scaled.end()){
		SDL_FreeSurface(*itScaled);
		++itScaled;
	}
	scaled.clear();

  std::map<std::string, SDL_Surface*>::iterator itSurf = surfaces.begin();
  while ( itSurf != surfaces.end() ) {
    SDL_FreeSurface(itSurf->second);
    ++itSurf;
  }

  std::map<std::string, Frame*>::iterator itFrame = frames.begin();
  while ( itFrame != frames.end() ) {
    delete itFrame->second;
    ++itFrame;
  }

// Delete each vector out of the map by deleting each element in each vector
// in the map of vectors
  std::map<std::string, std::vector<Frame*> >::iterator itFrameVec = 
		frameVectors.begin();
  while ( itFrameVec != frameVectors.end() ) {
		for(unsigned i = 0; i < itFrameVec->second.size(); i++){
    	delete itFrameVec->second[i];
		}
    ++itFrameVec;
	}
}

FrameFactory& FrameFactory::getInstance() {
  static FrameFactory factory;
  return factory;
}

SDL_Surface* FrameFactory::getSurface(const std::string& name, 
																			const double scale){
  std::map<std::string, SDL_Surface*>::const_iterator surfItr = 
		surfaces.find(name); 

	SDL_Surface * retSurf = NULL;

  if(surfItr == surfaces.end()){
    retSurf =	IOManager::getInstance().loadAndSet(
          			gdata->getXmlStr(name+"File"),
          			gdata->getXmlBool(name+"Transparency"));
    surfaces[name] = retSurf;
 	}
	else	retSurf = surfItr->second;

	if(scale != 1){
		int numberOfFrames = Gamedata::getInstance()->getXmlInt(name+"XFrames");
		return(getScaledSurface(retSurf, numberOfFrames, scale));
	}

	return(retSurf);

}

SDL_Surface* FrameFactory::getScaledSurface(SDL_Surface* base, 
																						const int numberOfFrames,
																						const double scale){	
    double oldWidth = base->w;
    // need to subtract 0.5 because rotozoom will ceil it!
    double newWidth = 
      numberOfFrames*ceil((oldWidth*scale-0.5)/numberOfFrames);
    //Since scale*width0 = width1, scale = width1/width0
    double newScale = newWidth/oldWidth;

		SDL_Surface * scaledSurf = 
			rotozoomSurface(base, 0, newScale, SMOOTHING_ON);
    scaled.push_back(scaledSurf);
		return(scaledSurf);
}

Frame* FrameFactory::getFrame(const std::string& name, const double scale) {

	std::ostringstream convertName;
	convertName << scale;
	std::string cName = name + convertName.str();

  std::map<std::string, Frame*>::const_iterator pos = frames.find(cName); 
  if ( pos == frames.end() ) {
    SDL_Surface * const surface = getSurface(name, scale);
    Frame * frame = new Frame(surface,
                (scale == 1) ? gdata->getXmlInt(name+"Width") : surface->w, 
                (scale == 1) ? gdata->getXmlInt(name+"Height") : surface->h,
                gdata->getXmlInt(name+"SrcX"), 
                gdata->getXmlInt(name+"SrcY")); 
    frames[cName] = frame;
    return frame;
  }
  else {
    return pos->second;
  }
}

// Read in the size of each frame and how many rows and columns are in the
// surface to be able to read in the frames associated with a surface into a
// single vector that is able to function like a multidimensional array this
// gives the vector the ability to act as an N x N matrix and can be used in
// both one way two way all the way up to N way sprites
std::vector<Frame*> FrameFactory::getFrameVector(const std::string& name,
																								 const double scale) {
	std::ostringstream convertName;
	convertName << scale;
	std::string cName = name + convertName.str();

  std::map<std::string, std::vector<Frame *> >::const_iterator pos =
		frameVectors.find(cName); 
  if ( pos == frameVectors.end() ) {
		int xFrames = gdata->getXmlInt(name+"XFrames");
		int yFrames = gdata->getXmlInt(name+"YFrames");
		std::vector<Frame *> frameVec;
		frameVec.reserve(xFrames * yFrames);
		int srcX = gdata->getXmlInt(name+"SrcX");
		int srcY = gdata->getXmlInt(name+"SrcY");
 	  SDL_Surface * const surface = getSurface(name, scale);
		int width = 
			((scale == 1) ? gdata->getXmlInt(name+"Width") : surface->w) / xFrames;
		int height = 
			((scale == 1) ? gdata->getXmlInt(name+"Height") : surface->h) / yFrames;

		for(int y = 0; y < yFrames; y++){
			for(int x = 0; x < xFrames; x++){
	 	  	Frame * const frame =
					new Frame(surface, width, height, srcX+(x*width), srcY+(y*height));
				frameVec.push_back(frame);
			}
		}

		frameVectors[cName] = frameVec;
		return frameVec;
	}
  else {
    return pos->second;
  }
}

