#ifndef __ENEMY_ELEVATOR2_H__
#define __ENEMY_ELEVATOR2_H__

#include "Enemy.h"

class Enemy_Elevator2 : public Enemy
{
private:
	int original_y = 0;
	Animation elevator;


public:

	Enemy_Elevator2(int x, int y);
	void OnCollision(Collider* collider);
	void Move();
};

#endif