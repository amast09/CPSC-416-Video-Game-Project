#ifndef HUD__H
#define HUD__H

#include<string>
#include"gamedata.h"

class Hud {
public:
	Hud(const bool vis = false) : 
		display(vis),
		height(Gamedata::getInstance()->getXmlFloat("viewHeight")),
		width(Gamedata::getInstance()->getXmlFloat("viewWidth")),
		offset(Gamedata::getInstance()->getXmlFloat("hudOffset")),
		thick(Gamedata::getInstance()->getXmlFloat("hudThick")), 
		r(Gamedata::getInstance()->getXmlFloat("hudR")),
		g(Gamedata::getInstance()->getXmlFloat("hudG")),
		b(Gamedata::getInstance()->getXmlFloat("hudB")) {} 

	virtual ~Hud() {}

	bool getBool() const {	return display;  }
	void setBool(const bool rhs) {	display = rhs;  } 
	void draw(int fps, int enemiesAlive) const;
	void drawRect(float height, float width, float offset, float thick,
		unsigned r, unsigned g, unsigned b) const;

private:
	bool display;
	float height;
	float width;
	float offset;
	float thick;
	int r;
	int g;
	int b;
	Hud& operator=(const Hud rhs);
};

#endif
