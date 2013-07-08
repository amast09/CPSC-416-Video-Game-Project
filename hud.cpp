#include <sstream>
#include <string>
#include <vector>
#include "hud.h"
#include "aaline.h"
#include "ioManager.h"

void Hud::draw(int fps, int enemiesAlive) const{
	if(display){
		std::ostringstream convertFps;
		convertFps << fps;
		std::string fpsString = convertFps.str();

		std::ostringstream eas;
		eas << enemiesAlive;
		std::string enemiesAliveString = eas.str();

		IOManager::getInstance().printMessageCenteredAt(
			"Average FPS: " + fpsString + 
			" Enemies Alive: " + enemiesAliveString, 5);
		drawRect(height, width, offset, thick, r, g, b);

		IOManager::getInstance().printMessageCenteredAt(
			"Press the Up, Left, and Right Arrows to the control Jet Space = Missle", height - 15
		);
	}
}

void Hud::drawRect(float height, float width, float offset, float thick,
	unsigned r, unsigned g, unsigned b) const{

  SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center"));
  IOManager *io = &IOManager::getInstance();
  SDL_Surface * const screen = io->getScreen();
  const Uint32 color = SDL_MapRGB(screen->format, r, g, b);

	Draw_AALine(screen, offset,offset, width-offset,offset, thick, color);
	Draw_AALine(screen, width-offset,offset, width-offset,height-offset, thick, color);
	Draw_AALine(screen, width-offset,height-offset, offset,height-offset, thick, color);
	Draw_AALine(screen, offset,height-offset, offset,offset, thick, color);
}
