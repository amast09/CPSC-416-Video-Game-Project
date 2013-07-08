#include "player.h"

Player::Player(MultiFrameSprite *rhs):
	player(rhs),
	dead(false),
	keyPress(false),
	gravity(Gamedata::getInstance()->getXmlFloat("playerGravity")),
	lift(Gamedata::getInstance()->getXmlFloat("playerLift")),
	minVelocity(Gamedata::getInstance()->getXmlInt("playerMinVelocity")),
	maxVelocity(Gamedata::getInstance()->getXmlInt("playerMaxVelocity")),
	peakAcc(0),
  bulletName(Gamedata::getInstance()->getXmlStr("playerBulletName")),
  bullets(bulletName),
  minSpeed(Gamedata::getInstance()->getXmlInt(bulletName+"SpeedX")),
  rightPosition(Gamedata::getInstance()->getXmlFloat(bulletName+"RightPositionX"),
							 Gamedata::getInstance()->getXmlFloat(bulletName+"RightPositionY")
	),
  leftPosition(Gamedata::getInstance()->getXmlFloat(bulletName+"LeftPositionX"),
							 Gamedata::getInstance()->getXmlFloat(bulletName+"LeftPositionY")
	)
	{} 


void Player::draw() const{
	if(!dead){
		player->draw();
		bullets.draw();
	}
}

void Player::update(Uint32 ticks){
	if(!dead){
	  bullets.update(ticks);
		player->update(ticks);
  	
		if(player->Y() <= 0) {
			player->velocityY(0);
			player->Y(0);
 		}
	 	if(player->Y() >= player->getWorldHeight() - player->getFrameHeight()) {
 	  	player->velocityY(0);
			player->Y(player->getWorldHeight() - player->getFrameHeight());
 		}

		if(!keyPress) stop();
		keyPress = false;
	}
}

void Player::stop(){
	if(player->Y() < player->getWorldHeight() - player->getFrameHeight())
		player->velocityY(player->velocityY() + gravity);
	else player->velocityY(0);
	if(player->Y() <= 0) player->velocityY(peakAcc + gravity);
}

void Player::up(){
	keyPress = true;
	if(player->Y() > 0){
		player->velocityY(player->velocityY() - lift);
	}
	else{
		peakAcc = player->velocityY();
		player->velocityY(0);
	}
}

void Player::right(){
	keyPress = true;
	if(player->velocityX() > 0 && abs(player->velocityX()) < maxVelocity){
		player->velocityX(player->velocityX() + 10);
	}
	else if(player->velocityX() < 0 && abs(player->velocityX()) > minVelocity){
		player->velocityX(player->velocityX() + 10);
	}
}

void Player::left(){
	keyPress = true;
	if(player->velocityX() < 0 && abs(player->velocityX()) < maxVelocity){
		player->velocityX(player->velocityX() - 10);
	}
	else if(player->velocityX() > 0 && abs(player->velocityX()) > minVelocity){
		player->velocityX(player->velocityX() - 10);
	}
}

bool Player::playerCollision(const MultiFrameSprite* obj){
	if(player->collidedWith(obj)){
		dead = true;
	} 
	return(dead); 
}

bool Player::missleCollision(const MultiFrameSprite* obj) const {
  return bullets.collidedWith( obj );
}

bool Player::shoot() { 
	Vector2f pos(0, 0);	
	float xVelocity = 0;
	if(player->velocityX() > 0){
		pos[0] = player->X() + rightPosition[0];
		pos[1] = player->Y() + rightPosition[1];
		xVelocity = player->velocityX() + minSpeed;
	}
	else{
		pos[0] = player->X() + leftPosition[0];
		pos[1] = player->Y() + leftPosition[1];
		xVelocity = player->velocityX() - minSpeed;
	}
  return(bullets.shoot(pos, Vector2f(xVelocity, 0)));
}


