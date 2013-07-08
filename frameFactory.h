#include <SDL.h>
#include <string>
#include <vector>
#include <map>
#include <list>
#include "frame.h"
#include "gamedata.h"

class FrameFactory {
public:
  static FrameFactory& getInstance();
  ~FrameFactory();
	SDL_Surface* getSurface(const std::string& name, const double scale);
	SDL_Surface* getScaledSurface(SDL_Surface* base, const int numberOfFrames, const double scale = 1);
  Frame* getFrame(const std::string&, const double scale = 1);
  std::vector<Frame *> getFrameVector(const std::string&, 
																			const double scale);

private:
  Gamedata* gdata;
  std::map<std::string, SDL_Surface*> surfaces;
  std::map<std::string, Frame*> frames;
	std::map<std::string, std::vector<Frame *> > frameVectors;
	std::list<SDL_Surface*> scaled;

  FrameFactory() : 
    gdata(Gamedata::getInstance()), 
		surfaces(), 
		frames(), 
		frameVectors(), 
		scaled()
  {}
  FrameFactory(const FrameFactory&);
  FrameFactory& operator=(const FrameFactory&);
};
