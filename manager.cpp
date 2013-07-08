#include <cmath>
#include <algorithm>
#include <limits>
#include "manager.h"
#include "frameFactory.h"
#include "scaledDrawableFO.h"
#include "sprite.h"

float getRandFloat(int min, int max){
		return(min + (rand() / (std::numeric_limits<int>::max()+1.0f))*(max-min));
}

Manager::~Manager() { 
	// These deletions eliminate "definitely lost" and
	// "still reachable"s in Valgrind.
	delete Gamedata::getInstance();
	for(unsigned int i = 0; i < clouds.size(); i++) {
		delete clouds[i]->getDrawable();
		delete clouds[i];
	}

	for(unsigned int i = 0; i < pman.size(); i++) {
		delete pman[i]->getDrawable();
		delete pman[i];
	}

	std::list<Enemy *>::iterator itr = enemies.begin();
	while(itr != enemies.end()){
		delete (*itr)->getMFS();
		delete *itr;
		itr++;
	}

	std::list<ExplodingSprite *>::iterator eitr = explodingEnemies.begin();
	while(eitr != explodingEnemies.end()){
		delete *eitr;
		eitr++;
	}

	delete player.getMFS();
}

Manager::Manager() :
	env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
	gdata( Gamedata::getInstance() ),
	io( IOManager::getInstance() ),
	clock( Clock::getInstance() ),
	screen( io.getScreen() ),
	world1(FrameFactory::getInstance().getFrame("back3"), 8),
	world2(FrameFactory::getInstance().getFrame("back2"), 4),
	world3(FrameFactory::getInstance().getFrame("back"), 1),
	viewport( Viewport::getInstance() ),
	hud(),
	sound(),
	player(new TwoWaySprite("plane", 1)),
	delay(Gamedata::getInstance()->getXmlInt("delay")),
	numOfEnemies(Gamedata::getInstance()->getXmlInt("numOfEnemies"))
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }

	viewport.setObjectToTrack(player.getMFS());
	srand(time(0));
	clouds.reserve(Gamedata::getInstance()->getXmlInt("cloud2Num"));
	pman.reserve(Gamedata::getInstance()->getXmlInt("pmanNum"));
	int ww = Gamedata::getInstance()->getXmlInt("worldWidth");
	int wh = Gamedata::getInstance()->getXmlInt("worldHeight");
	int pw = player.getMFS()->getFrame()->getHeight();
	int ph = player.getMFS()->getFrame()->getWidth();

	for(int i = 0; i < Gamedata::getInstance()->getXmlInt("cloud2Num"); i++){
		double scale = static_cast<double>(getRandFloat(5 , 30) / 10.0);
		clouds.push_back(
			new ScaledDrawable(
				new Sprite("cloud2", FrameFactory::getInstance().getFrame("cloud2", scale)), scale)
      );
	}
	sort(clouds.begin(), clouds.end(), ScaledDrawableFO());

	for(int i = 0; i < Gamedata::getInstance()->getXmlInt("pmanNum"); i++){
		double scale = static_cast<double>(getRandFloat(5 , 200) / 100.0);
		pman.push_back(
			new ScaledDrawable(
				new OneWaySprite("pman",
				getRandFloat(0, ww),
				getRandFloat(0, wh),
				(rand()%2?1:-1)*getRandFloat(
					-Gamedata::getInstance()->getXmlInt("pmanSpeedX"),
					Gamedata::getInstance()->getXmlInt("pmanSpeedX")
				),
				(rand()%2?1:-1)*getRandFloat(
					-Gamedata::getInstance()->getXmlInt("pmanSpeedY"), 
					Gamedata::getInstance()->getXmlInt("pmanSpeedY")
				), 
				scale), scale)
      );
	}
	sort(pman.begin(), pman.end(), ScaledDrawableFO());
	
	for(int i = 0; i < numOfEnemies; i++){
		enemies.push_back(new Enemy(
				new TwoWaySprite("heli2", 
				getRandFloat(pw + Gamedata::getInstance()->getXmlInt("startRoom"),
					ww
				),
				getRandFloat(ph + Gamedata::getInstance()->getXmlInt("startRoom"), 
					wh 
					- 
					(Gamedata::getInstance()->getXmlInt("heli2Height") / Gamedata::getInstance()->getXmlInt("heli2YFrames"))
				),
				(rand()%2?1:-1)*getRandFloat(
					-Gamedata::getInstance()->getXmlInt("heli2SpeedX"),
					Gamedata::getInstance()->getXmlInt("heli2SpeedX")
				),
				(rand()%2?1:-1)*getRandFloat(
					-Gamedata::getInstance()->getXmlInt("heli2SpeedY"), 
					Gamedata::getInstance()->getXmlInt("heli2SpeedY")
				), 
				1
			),
			&player, 
			Gamedata::getInstance()->getXmlFloat("enemyAttackRange"), 
			Gamedata::getInstance()->getXmlFloat("enemyAttackSpeed"))
		);
	}

  atexit(SDL_Quit);
	clock.pause();
}

void Manager::setNumberOfEnemies(int number){
	numOfEnemies = number;
	int pw = player.getMFS()->getFrame()->getHeight();
	int ph = player.getMFS()->getFrame()->getWidth();
  if(number > static_cast<int>(enemies.size())){ 
    number -= enemies.size();
    for(int i = 0; i < number; ++i) {
			enemies.push_back(new Enemy(
					new TwoWaySprite("heli2", 
					getRandFloat(pw + Gamedata::getInstance()->getXmlInt("startRoom"),
					Gamedata::getInstance()->getXmlInt("worldWidth")
					),
					getRandFloat(ph + Gamedata::getInstance()->getXmlInt("startRoom"), 
						Gamedata::getInstance()->getXmlInt("worldHeight")
						 - 
						(Gamedata::getInstance()->getXmlInt("heli2Height") 
						/ 
						Gamedata::getInstance()->getXmlInt("heli2YFrames"))
					),
					(rand()%2?1:-1)*getRandFloat(
						-Gamedata::getInstance()->getXmlInt("heli2SpeedX"),
						Gamedata::getInstance()->getXmlInt("heli2SpeedX")
					),
					(rand()%2?1:-1)*getRandFloat(
						-Gamedata::getInstance()->getXmlInt("heli2SpeedY"), 
						Gamedata::getInstance()->getXmlInt("heli2SpeedY")
					), 
					1
				),
				&player, 
				Gamedata::getInstance()->getXmlFloat("enemyAttackRange"), 
				Gamedata::getInstance()->getXmlFloat("enemyAttackSpeed"))
			);
 		}   
  }
  else{
    number = enemies.size() - number;
    for(int i = 0; i < number; ++i){
			delete enemies.back()->getMFS();
      delete enemies.back();
      enemies.pop_back();
    }   
  }
}

void Manager::setGravity(float number){
	player.setGravity(number);
}

void Manager::draw() const {
  world1.draw();
	for(unsigned int i = 0; i < clouds.size() / 3; ++i)
    clouds[i]->getDrawable()->draw();
	for(unsigned int i = 0; i < pman.size() / 3; ++i)
    pman[i]->getDrawable()->draw();

  world2.draw();
	for(unsigned int i = clouds.size() / 3; i < 2 * clouds.size() / 3; ++i)
    clouds[i]->getDrawable()->draw();
	for(unsigned int i = pman.size() / 3; i < 2 * pman.size() / 3; ++i)
    pman[i]->getDrawable()->draw();

  world3.draw();
	for(unsigned int i = clouds.size() / 3 * 2; i < clouds.size() / 3; ++i)
    clouds[i]->getDrawable()->draw();
	for(unsigned int i = pman.size() / 3 * 2; i < pman.size() / 3; ++i)
    pman[i]->getDrawable()->draw();

  std::list<Enemy*>::const_iterator itr = enemies.begin();
  while(itr != enemies.end()) { 
    (*itr)->draw();
    ++itr;
  }

  std::list<ExplodingSprite*>::const_iterator eitr = explodingEnemies.begin();
  while(eitr != explodingEnemies.end()) { 
    (*eitr)->draw();
    ++eitr;
  }

	player.draw();
 	hud.draw(clock.getFps(), 
					 enemies.size());
	clock++;

	if(player.deadYet()){
		IOManager::getInstance().printMessageCenteredAt(
		"Game Over... YOU LOSE!!!!!", 
		Gamedata::getInstance()->getXmlFloat("viewHeight") / 2);
	}
	else if(enemies.size() == 0){
		IOManager::getInstance().printMessageCenteredAt(
		"YOU ARE A GOD!!!! BECAUSE YOU JUST WON!!!!", 
		Gamedata::getInstance()->getXmlFloat("viewHeight") / 2);
	}
}

void Manager::update(Uint32 ticks) {
  world1.update();
  world2.update();
  world3.update();
  for(unsigned int i = 0; i < clouds.size(); ++i) {
    clouds[i]->getDrawable()->update(ticks);
	}
  for(unsigned int i = 0; i < pman.size(); ++i) {
    pman[i]->getDrawable()->update(ticks);
	}

  std::list<ExplodingSprite*>::iterator eitr = explodingEnemies.begin();
  while ( eitr != explodingEnemies.end() ) {
    (*eitr)->update(ticks);
    ++eitr;
  }

  std::list<Enemy*>::iterator itr = enemies.begin();
  while(itr != enemies.end()){
    (*itr)->update(ticks);

		if(player.missleCollision((*itr)->getMFS())){
			sound[1];
			explodingEnemies.push_back(
				new ExplodingSprite((*itr)->getMFS()->getName(), 
														(*itr)->getMFS()->getPosition(), 
														(*itr)->getMFS()->getVelocity(), 
														(*itr)->getMFS()->getFrame())
			);
			delete (*itr)->getMFS();
			delete *itr;
			itr = enemies.erase(itr);
		}
		else if(!player.deadYet() && player.playerCollision((*itr)->getMFS())){
			sound[1];
			explodingEnemies.push_back(
				new ExplodingSprite(player.getMFS()->getName(), 
														player.getMFS()->getPosition(), 
														player.getMFS()->getVelocity(), 
														player.getMFS()->getFrame())
				);
			++itr;
		}
		else ++itr;
	}

	player.update(ticks);
  viewport.update();
}

unsigned Manager::wait(unsigned next){
	unsigned now = clock.getTicks();
	return((next <= now) ? 0 : next - now);
}

void Manager::newGame(){
	string playerName = player.getMFS()->getName();
	int vx = Gamedata::getInstance()->getXmlInt(playerName + "SpeedX");
	int vy = Gamedata::getInstance()->getXmlInt(playerName + "SpeedY");
	player.getMFS()->X(0);
	player.getMFS()->Y(0);
	player.getMFS()->velocityX(vx);
	player.getMFS()->velocityY(vy);
	player.toggleMortality();
	setNumberOfEnemies(numOfEnemies);
}


void Manager::play() {
	clock.unpause();
	
	if(player.deadYet()) newGame();

  SDL_Event event;

	bool done = false;
	unsigned next = clock.getTicks() + delay;
  bool keyCatch = false;

  while ( not done ) {
    draw();
    SDL_Flip(screen);

    Uint32 ticks = clock.getElapsedTicks();
		if(!clock.isPaused()) update(ticks);
		SDL_Delay(wait(next));
		next += delay;


    SDL_PollEvent(&event);
		Uint8 *keystate = SDL_GetKeyState(NULL);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYUP) { keyCatch = false; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE : clock.pause(); done = true; break;
        case SDLK_q      : done = true; break;
				case SDLK_F1 :
					if(!keyCatch){
						keyCatch = true;
						hud.setBool(!hud.getBool());
					}
					break;
        case SDLK_p      : {
          if (!keyCatch) {
            keyCatch = true;
            if ( clock.isPaused() ) clock.unpause();
            else clock.pause();
          }
          break;
        }
        default          : break;
      }
    }
		if(keystate[SDLK_SPACE]  && !player.deadYet()){
			if(player.shoot()){
				sound[0];
			}
		}
		if(keystate[SDLK_UP]  && !player.deadYet()) player.up();
		if(keystate[SDLK_LEFT] && !player.deadYet()) player.left();
		if(keystate[SDLK_RIGHT] && !player.deadYet()) player.right();
  }

}

