#include <cmath>
#include <sstream>
#include "menuManager.h"
#include "manager.h"

MenuManager::MenuManager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  screen( IOManager::getInstance().getScreen() ),
  clock( Clock::getInstance() ),
  backColor(),
  menu(),
  numberOfEnemies(0),
	gravity(0)
{ 
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  backColor.r = Gamedata::getInstance()->getXmlInt("backRed");
  backColor.g = Gamedata::getInstance()->getXmlInt("backGreen");
  backColor.b = Gamedata::getInstance()->getXmlInt("backBlue");
  atexit(SDL_Quit); 
}

void MenuManager::drawBackground() const {
  SDL_FillRect( screen, NULL, 
    SDL_MapRGB(screen->format, backColor.r, backColor.g, backColor.b) );
  SDL_Rect dest = {0, 0, 0, 0};
  SDL_BlitSurface( screen, NULL, screen, &dest );
}

void MenuManager::getNumberOfEnemies() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool nameDone = false;
  const string msg("How Many Enemies: ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 120);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;
      strm << number;
      strm >> numberOfEnemies;
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      strm << numberOfEnemies << " Have Been Created, Good Luck!";
      io.printMessageAt(strm.str(), 20, 160);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::getGravity() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  bool nameDone = false;
  const string msg("How Much Gravity?: ");
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          nameDone = true;
        }
        io.buildString(event);
      }
    }
    drawBackground();
    io.printStringAfterMessage(msg, 20, 120);
    if ( nameDone ) {
      std::string number = io.getString();
      std::stringstream strm;
      strm << number;
      strm >> gravity;
      strm.clear(); // clear error flags
      strm.str(std::string()); // clear contents
      strm << "Gravity Set to " << gravity << " Good Luck!";
      io.printMessageAt(strm.str(), 20, 160);
      SDL_Flip(screen);
      SDL_Delay(1000);
      done = true;
    }
    if ( !done ) {
      SDL_Flip(screen);
    }
  }
}

void MenuManager::giveHelp() {
  IOManager& io = IOManager::getInstance().getInstance();
  SDL_Event event;
  bool done = false;
  io.clearString();
  while ( not done ) {
    Uint8 *keystate = SDL_GetKeyState(NULL);
    if ( SDL_PollEvent(&event) )
    switch (event.type) {
      case SDL_KEYDOWN: {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
        }
        if (keystate[SDLK_RETURN]) {
          done = true;
        }
      }
    }
    drawBackground();
    io.printStringAfterMessage("Goal:", 20, 60);
    io.printStringAfterMessage("Destroy All Helicopters", 40, 80);
    io.printStringAfterMessage("Gravity Input: ", 20, 100);
    io.printStringAfterMessage("Rookie  0 - 10", 40, 120);
    io.printStringAfterMessage("Amateur  11 - 15", 40, 140);
    io.printStringAfterMessage("Vetern  15 - 20", 40, 160);
    io.printStringAfterMessage("God  21+", 40, 180);
    io.printStringAfterMessage("Enemies Input: ", 20, 200);
    io.printStringAfterMessage("Rookie  0 - 5", 40, 220);
    io.printStringAfterMessage("Amateur  6 - 20", 40, 240);
    io.printStringAfterMessage("Vetern  21 - 30", 40, 260);
    io.printStringAfterMessage("God  31+", 40, 280);
    io.printStringAfterMessage("Controls: ", 20, 300);
    io.printStringAfterMessage("[Up Arrow] - Up", 40, 320);
    io.printStringAfterMessage("[Left Arrow] - Slow Down/Speed Up", 40, 340);
    io.printStringAfterMessage("[Right Arrow] - Slow Down/Speed Up", 40, 360);
    io.printStringAfterMessage("[Space] - Fire Missle", 40, 380);
    io.printStringAfterMessage("[F1] - Help HUD", 40, 400);
    if ( !done ) {
      SDL_Flip(screen);
    }
	}
}

void MenuManager::play() {
  bool keyCatch = false; // To get only 1 key per keydown
  SDL_Event event;
  bool done = false;
  // Here, we need to create an instance of the Manager,
  // the one that manages the game:
	Manager manager;

  while ( not done ) {

    drawBackground();
    menu.draw();
    SDL_Flip(screen);

    SDL_PollEvent(&event);
    if (event.type ==  SDL_QUIT) { break; }
    if(event.type == SDL_KEYDOWN) {
      switch ( event.key.keysym.sym ) {
        case SDLK_ESCAPE :
        case SDLK_q : {
          done = true;
          break;
        }
        case SDLK_RETURN : {
          if ( !keyCatch ) {
            menu.lightOn();
            if ( menu.getIconClicked() == "Start Game" ) {
              // Here is where we call the play() function in Manager:
              manager.play();
            }
            if ( menu.getIconClicked() == "Enemies" ) {
              getNumberOfEnemies();
              // After we get the parameter, we must pass it to Manager:
              manager.setNumberOfEnemies( numberOfEnemies );
            }
            if ( menu.getIconClicked() == "Gravity" ) {
              getGravity();
              // After we get the parameter, we must pass it to Manager:
              manager.setGravity( gravity );
            }
            if ( menu.getIconClicked() == "Help" ) {
              // Here is where we explain how to play the game"
							giveHelp();
            }
            if ( menu.getIconClicked() == "Exit" ) {
              drawBackground();
              menu.draw();
              SDL_Flip(screen);
              SDL_Delay(250);
              done = true;
            }
          }
          break;
        }
        case SDLK_DOWN   : {
          if ( !keyCatch ) {
            menu.increment();
          }
          break;
        }
        case SDLK_UP   : {
          if ( !keyCatch ) {
            menu.decrement();
          }
          break;
        }
        default          : break;
      }
      keyCatch = true;
    }
    if(event.type == SDL_KEYUP) { 
      keyCatch = false; 
      menu.lightOff();
    }
  }
}
