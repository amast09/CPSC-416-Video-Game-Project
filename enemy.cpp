#include <cmath>
#include "enemy.h"

float distance(float x1, float y1, float x2, float y2) {
  float x = x1 - x2;
  float y = y1 - y2;
  return hypot(x, y); 
}

Enemy::Enemy(MultiFrameSprite *en, Player *pl, float r, float a):
	enemy(en),
	player(pl),
  range(r),
	attackSpeed(a),
	normalX(en->velocityX()),
	normalY(en->velocityY()),
	state(NORMAL)
	{} 

void Enemy::update(Uint32 ticks){
	enemy->update(ticks);

  float ex = enemy->X() + enemy->getFrame()->getWidth() / 2;
  float ey = enemy->Y() + enemy->getFrame()->getHeight() / 2;
	float evx = enemy->velocityX();
	float evy = enemy->velocityY();
  float px = 
		player->getMFS()->X() + player->getMFS()->getFrame()->getWidth() / 2;
  float py = 
		player->getMFS()->Y() + player->getMFS()->getFrame()->getHeight() / 2;
  float distanceToPlayer = ::distance(ex,ey, px, py);

	//	If the player is dead, stop tracking and return to normal velocity
	if(player->deadYet()){
		state = VICTORY;
		state = NORMAL;
		enemy->velocityX(normalX);
		enemy->velocityY(normalY);
	}
	//	If the enemy is normal state only check if the enemy is in range
	else if(state == NORMAL){
		if(distanceToPlayer < range){
			state = ATTACK;
			//	If attackMode is triggered, save the velocities before the attack
			normalX = evx;
			normalY = evy;
		}
	}
	//	If the enemy is in attack range, start tracking him
	else if(state == ATTACK){
		//	If the enemy is out of range return to normal velocity
		if(distanceToPlayer > range){
			state = NORMAL;
			enemy->velocityX(normalX);
			enemy->velocityY(normalY);
		}
		//	If the enemy is facing the player and moving in the players direction
		else if((evx > 0 && ex < px) || (evx < 0 && ex > px)){
			//  If the enemy is within 20 of the player dont adjust the Y velocity
			//  padding eliminates jutter up and down
			if(ey < (py + 10) && ey > (py - 10)){
				enemy->velocityY(0);
			}
			//	If the player is above the enemy make sure the enemy is moving up
			else if(ey < py){
				enemy->velocityY(attackSpeed);
			}
			//	If the player is below the enemy make sure the enemy is moving down
			else if(ey > py){
				enemy->velocityY(-attackSpeed);
			}
			//	If the enemy is going right increase their X velocity because they are
			//	already facing them
			if(evx > 0) enemy->velocityX(attackSpeed);
			//	If the enemy is going left increase (negatively) their X velocity 
			//	because they are already facing them
			else enemy->velocityX(-attackSpeed);
		}
		//	The enemy is no longer facing the player, so forget it and normalize
		//	velocities
		else{
			enemy->velocityX(normalX);
			enemy->velocityY(normalY);
		}
	}

}



