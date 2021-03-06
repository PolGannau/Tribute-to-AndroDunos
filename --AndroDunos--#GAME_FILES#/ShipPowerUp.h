#ifndef __SHIPPOWERUP_H__
#define __SHIPPOWERUP_H__

#include "PowerUp.h"
#include "Path.h"

class ShipPowerUp : public PowerUp
{
private:
	float wave = -1.0f;
	bool going_up = true;
	int original_y = 0;
	Animation fly;
	iPoint original_pos;
	Path path;

public:

	ShipPowerUp(int x, int y);
	
	void OnCollision(Collider* collider);

	void Move();
};

#endif
