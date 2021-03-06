#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModuleFonts.h"
#include "ModulePlayer.h"
#include "ModulePlayer2.h"
#include "ModuleAudio.h"
#include "ModuleEnemies.h"
#include "Stageclear.h"
#include "ModulePowerUps.h"
#include "PowerUpRed.h"
#include "PowerUp.h"
#include "SDL/include/SDL_timer.h"
#include<stdio.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer2::ModulePlayer2()
{
	// idle animation (just the ship)
	idle.PushBack({ 154, 108, 27, 17 });

	stand_fire.PushBack({ 80,116,5,5 });
	stand_fire.PushBack({ 65,116,8,5 });
	stand_fire.PushBack({ 42,116,12,5 });
	stand_fire.loop = true;
	stand_fire.speed = 0.045f;

	// move upwards
	up.PushBack({ 154, 87, 27, 15 });
	up.PushBack({ 154, 66, 27, 15 });
	up.loop = false;
	up.speed = 0.1f;

	// Move down
	down.PushBack({ 154, 131, 27, 16 });
	down.PushBack({ 154, 153, 27, 17 });
	down.loop = false;
	down.speed = 0.1f;
}

ModulePlayer2::~ModulePlayer2()
{}

// Load assets
bool ModulePlayer2::Start()
{
	LOG("Loading player2");
	bool ret = true;
	bool insert2 = false;

	graphics = App->textures->Load("assets/ship/ships.png");

	position.x = App->player->position.x;
	position.y = App->player->position.y + 20;

	destroyed = false;
	// TODO 2: Add a collider to the player
	coll = App->collision->AddCollider({ position.x,position.y, 27, 16 }, COLLIDER_PLAYER2, this);

	fx_1 = App->audio->LoadEffect("assets/ship/Laser_Shot_Type-1_(Main_Ships).wav");
	fx_2 = App->audio->LoadEffect("assets/ship/Laser_Shot_Type-2_(Main_Ships).wav");
	fx_3 = App->audio->LoadEffect("assets/ship/Laser_Shot_Type-3_(Main_Ships).wav");
	fx_4 = App->audio->LoadEffect("assets/ship/Laser_Shot_Type-4_(Main_Ships).wav");

	return ret;
}

// Unload assets
bool ModulePlayer2::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(graphics);
	App->audio->UnloadSoundEffects(fx_1);
	App->audio->UnloadSoundEffects(fx_2);
	App->audio->UnloadSoundEffects(fx_3);
	App->audio->UnloadSoundEffects(fx_4);
	return true;
}

// Update: draw background
update_status ModulePlayer2::Update()
{
	int speed = 2;
	


	bool UP = App->input->keyboard[SDL_SCANCODE_I] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller2, SDL_CONTROLLER_AXIS_LEFTY) < -CONTROLLER_DEAD_ZONE;
	bool LEFT = App->input->keyboard[SDL_SCANCODE_J] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller2, SDL_CONTROLLER_AXIS_LEFTX) < -CONTROLLER_DEAD_ZONE;
	bool DOWN = App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller2, SDL_CONTROLLER_AXIS_LEFTY) > CONTROLLER_DEAD_ZONE;
	bool RIGHT = App->input->keyboard[SDL_SCANCODE_L] == KEY_STATE::KEY_REPEAT || SDL_GameControllerGetAxis(App->input->controller2, SDL_CONTROLLER_AXIS_LEFTX) > CONTROLLER_DEAD_ZONE;

	if (SDL_GameControllerGetButton(App->input->controller2, SDL_CONTROLLER_BUTTON_X) == false)
	{
		x = false;
	}

	if (SDL_GameControllerGetButton(App->input->controller2, SDL_CONTROLLER_BUTTON_A) == false)
	{
		a = false;
	}

	if (LEFT)
	{
		position.x -= speed;
		if (position.x <= App->render->view.x)
		{
			position.x = App->render->view.x + 2;
		}
	}

	if (RIGHT)
	{
		position.x += speed;
		if (position.x + 27 >= App->render->view.x + App->render->view.w)
		{
			position.x = (App->render->view.x + App->render->view.w) - 28;
		}
	}

	if (DOWN)
	{
		position.y += speed;
		if (current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}
		if (position.y + 17 >= App->render->view.y + App->render->view.h)
		{
			position.y = (App->render->view.y + App->render->view.h) - 18;
		}
	}

	if (UP)
	{
		position.y -= speed;
		if (current_animation != &up)
		{
			up.Reset();
			current_animation = &up;
		}
		if (position.y <= App->render->view.y)
		{
			position.y = App->render->view.y + 1;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RCTRL] == KEY_STATE::KEY_DOWN || (SDL_GameControllerGetButton(App->input->controller2, SDL_CONTROLLER_BUTTON_A) && a == false))
	{
		a = true;
		currentTime = SDL_GetTicks();
		if (currentTime > lastTime + 50)
		{
			if (App->powerups->power2up1 && App->powerups->power2up2 == false) {
				if (weapon1) {
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 4, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 11, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser1powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_1);
				}
				else if (weapon2) {
					App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 10, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser2powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_2);
				}
				else if (weapon3) {
					App->particles->AddParticle(App->particles->laser3, position.x + 20, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser3powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_3);
				}
				else if (weapon4) {
					App->particles->AddParticle(App->particles->laser4up, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4down, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupredup, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupreddown, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_4);
				}
			}
			else if (App->powerups->power2up2 && App->powerups->power2up1 == false) {
				if (weapon1) {
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 4, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 18, COLLIDER_PLAYER_SHOT);
					if (App->powerups->blue == 0) {
						App->particles->AddParticle(App->particles->laser, position.x + 25, position.y + 11, COLLIDER_PLAYER_SHOT);
					}
					else {
						App->particles->AddParticle(App->particles->BlueLaserPlayer, position.x + 28, position.y + 10, COLLIDER_PLAYER_SHOT);
					}
					App->audio->PlaySoundEffect(fx_1);
				}
				else if (weapon2) {
					if (App->powerups->blue == 0) {
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 10, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 6, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 12, COLLIDER_PLAYER_SHOT);
					}
					else {
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 6, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 12, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->Laser2PowerupBlue2down, position.x - 7, position.y + 10, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->Laser2PowerupBlue2up, position.x - 7, position.y + 4, COLLIDER_PLAYER_SHOT);
					}
					App->audio->PlaySoundEffect(fx_2);
				}
				else if (weapon3) {
					App->particles->AddParticle(App->particles->laser3, position.x + 20, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_3);
				}
				else if (weapon4) {
					App->particles->AddParticle(App->particles->laser4up, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4down, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupblue, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_4);
				}
			}
			else if (App->powerups->power2up1 && App->powerups->power2up2) {
				if (weapon1) {
					App->particles->AddParticle(App->particles->laser1powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 4, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 18, COLLIDER_PLAYER_SHOT);
					if (App->powerups->blue == 0) {
						App->particles->AddParticle(App->particles->laser, position.x + 25, position.y + 11, COLLIDER_PLAYER_SHOT);
					}
					else {
						App->particles->AddParticle(App->particles->BlueLaserPlayer, position.x + 28, position.y + 10, COLLIDER_PLAYER_SHOT);
					}
					App->audio->PlaySoundEffect(fx_1);
				}
				else if (weapon2) {
					if (App->powerups->blue == 0) {
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 10, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 6, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 12, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					}
					else {
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 6, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 12, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->Laser2PowerupBlue2down, position.x - 7, position.y + 10, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->Laser2PowerupBlue2up, position.x - 7, position.y + 4, COLLIDER_PLAYER_SHOT);
						App->particles->AddParticle(App->particles->laser2powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					}
					App->audio->PlaySoundEffect(fx_2);
				}
				else if (weapon3) {
					App->particles->AddParticle(App->particles->laser3, position.x + 20, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser3powerupred, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_3);
				}
				else if (weapon4) {
					App->particles->AddParticle(App->particles->laser4up, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4down, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupblue, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupredup, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4powerupreddown, position.x + 10, position.y, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_4);
				}
			}
			else {
				if (weapon1) {
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 4, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 11, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_1);
				}
				else if (weapon2) {
					App->particles->AddParticle(App->particles->laser2right, position.x + 20, position.y + 10, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser2left, position.x - 7, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_2);
				}
				else if (weapon3) {
					App->particles->AddParticle(App->particles->laser3, position.x + 20, position.y + 7, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_3);
				}
				else if (weapon4) {
					App->particles->AddParticle(App->particles->laser4up, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->particles->AddParticle(App->particles->laser4down, position.x + 20, position.y + 10.5, COLLIDER_PLAYER_SHOT);
					App->audio->PlaySoundEffect(fx_4);
				}
			}
			score += 13;
			lastTime = currentTime;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_RSHIFT] == KEY_STATE::KEY_DOWN || (SDL_GameControllerGetButton(App->input->controller2, SDL_CONTROLLER_BUTTON_X) && x == false))
	{
		x = true;
		if (weapon1) {
			weapon1 = false;
			weapon2 = true;
		}
		else if (weapon2) {
			weapon2 = false;
			weapon3 = true;
		}
		else if (weapon3) {
			weapon3 = false;
			weapon4 = true;
		}
		else if (weapon4) {
			weapon4 = false;
			weapon1 = true;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_X] == KEY_STATE::KEY_DOWN)
	{
		App->player->Enable();
		App->player->position.x = App->player2->position.x;
		App->player->position.y = App->player2->position.y - 20;
	}

	if (App->input->keyboard[SDL_SCANCODE_K] == KEY_STATE::KEY_IDLE
		&& App->input->keyboard[SDL_SCANCODE_I] == KEY_STATE::KEY_IDLE)
	{
		current_animation = &idle;
	}
	if (App->input->keyboard[SDL_SCANCODE_E] == KEY_STATE::KEY_DOWN)
	{
		App->enemies->AddEnemy(ENEMY_TYPES::WHITETURRET, position.x + 250, position.y, false, 2);
		//App->powerups->AddPowerUp(POWERUPS_TYPES::SHIPPOWERUP, position.x + 250, position.y);

	}

	// TODO 3: Update collider position to player position
	coll->SetPos(position.x, position.y);

	// Draw everything --------------------------------------
	if (App->player2->IsEnabled() == true)
	{
		App->render->Blit(graphics, ((App->player2->position.x) - App->player2->stand_fire.GetCurrentFrame().w - 1), (App->player2->position.y + 7), &(stand_fire.GetCurrentFrame()));
		App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	}
	if (setcoll) {
		++settcoll;
		if (settcoll >= 250) {
			setcoll = false;
			settcoll = 0;
			App->player2->coll = App->collision->AddCollider({ App->player2->position.x,App->player2->position.y,27, 16 }, COLLIDER_PLAYER, App->player2);
		}
	}
	return UPDATE_CONTINUE;
}

// TODO 4: Detect collision with a wall. If so, go back to intro screen.

void ModulePlayer2::OnCollision(Collider* c1, Collider* c2)
{
	if (c1 == coll && destroyed == false && App->fade->IsFading() == false)
	{
		App->particles->AddParticle(App->particles->explosionship2, position.x, position.y, COLLIDER_NONE);
		weapon1 = true;
		App->powerups->power2up1 = false;
		App->powerups->power2up2 = false;
		App->powerups->blue2 = -1;
		App->powerups->red2 = -1;
		App->powerups->green2 = -1;
		App->powerups->yellow2 = -1;
		life--;
		if (life == 0) {
			if (App->player->IsEnabled() == false)
			{
				App->fade->FadeToBlack((Module*)App->stage2, (Module*)App->gameover, 2.0f);
			}
			App->player2->coll->to_delete = true;
			App->player2->Disable();
			App->player2->CleanUp();
			destroyed = true;
			life = 3;
		}
		else {
			App->player2->Spawn();
		}

	}
}
void ModulePlayer2::Spawn() {
	App->player2->coll->to_delete = true;
	setcoll = true;
	App->player2->position.x = (App->render->camera.x / SCREEN_SIZE) + 30;
	App->player2->position.y = App->render->camera.y + 100;
	current_animation = &idle;

}