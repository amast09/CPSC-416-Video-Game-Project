#include <SDL/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "ioManager.h"
#include "clock.h"
#include "gamedata.h"
#include "multiFrameSprite.h"
#include "oneWaySprite.h"
#include "twoWaySprite.h"
#include "accSprite.h"
#include "explodingSprite.h"
#include "world.h"
#include "viewport.h"
#include "hud.h"
#include "player.h"
#include "enemy.h"
#include "scaledDrawable.h"
#include "sound.h"

class Manager {
public:
  Manager ();
  ~Manager ();
	void setNumberOfEnemies(int number);
	void setGravity(float number);
  void play();

private:
  const bool env;
  const Gamedata* gdata;
  const IOManager& io;
  Clock& clock;

  SDL_Surface *screen;
  World world1;
  World world2;
  World world3;
  Viewport& viewport;

	std::vector<ScaledDrawable*> clouds;
	std::vector<ScaledDrawable*> pman;
	std::list<Enemy*> enemies;
	std::list<ExplodingSprite*> explodingEnemies;
	Hud hud;
	SDLSound sound;
	Player player;
	int delay;
	int numOfEnemies;

  void draw() const;
  void update(Uint32 ticks);
	unsigned wait(unsigned next);
	void newGame();
  Manager(const Manager&);
  Manager& operator=(const Manager&);
};
