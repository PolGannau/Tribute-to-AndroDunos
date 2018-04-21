#include "Application.h"
#include "PowerUpRed.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModulePowerUps.h"

PowerUpRed::PowerUpRed(int x, int y) : PowerUp(x, y)
{
	fly.PushBack({ 294,0,16,16 });

	fly.speed = 0.05f;

	animation = &fly;

	collider = App->collision->AddCollider({ 0, 0, 16, 16 }, COLLIDER_TYPE::COLLIDER_POWERUP, (Module*)App->powerups);

	original_y = y;
}

void PowerUpRed::Move()
{
	if (going_up)
	{
		if (wave > 1.0f)
			going_up = false;
		else
			wave += 0.05f;
	}
	else
	{
		if (wave < -1.0f)
			going_up = true;
		else
			wave -= 0.05f;
	}

	position.y = int(float(original_y) + (25.0f * sinf(wave)));
	position.x -= 1;
}

void PowerUpRed::OnCollision(Collider* collider)
{
	App->powerups->powerup1 = true;
}
