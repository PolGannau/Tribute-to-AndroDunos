#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleParticles.h"
#include "ModuleRender.h"
#include "ModuleCollision.h"
#include "ModuleFadeToBlack.h"
#include "ModulePlayer.h"

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModulePlayer::ModulePlayer()
{
	// idle animation (just the ship)
	idle.PushBack({ 94, 109, 27, 17 });

	// move upwards
	up.PushBack({ 94, 88, 27, 15 });
	up.PushBack({ 94, 67, 27, 15 });
	up.loop = false;
	up.speed = 0.1f;

	// Move down
	down.PushBack({ 94, 132, 27, 16 });
	down.PushBack({ 94, 154, 27, 17 });
	down.loop = false;
	down.speed = 0.1f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	graphics = App->textures->Load("assets/ship/ships.png");

	position.x = 150;
	position.y = 120;

	// TODO 2: Add a collider to the player
	coll=App->collision->AddCollider({ position.x,position.y, 27, 16 },COLLIDER_PLAYER);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
	App->textures->Unload(graphics);
	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	int speed = 2;

	if (App->input->keyboard[SDL_SCANCODE_A] == KEY_STATE::KEY_REPEAT)
	{
		position.x -= speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_D] == KEY_STATE::KEY_REPEAT)
	{
		position.x += speed;
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_REPEAT)
	{
		position.y += speed;
		if (current_animation != &down)
		{
			down.Reset();
			current_animation = &down;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_REPEAT)
	{
		position.y -= speed;
		if (current_animation != &up)
		{
			up.Reset();
			current_animation = &up;
		}
	}

	if (App->input->keyboard[SDL_SCANCODE_SPACE] == KEY_STATE::KEY_DOWN)
	{
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y + 4, COLLIDER_PLAYER_SHOT);
		App->particles->AddParticle(App->particles->laser, position.x + 20, position.y +11, COLLIDER_PLAYER_SHOT);
	}

	if (App->input->keyboard[SDL_SCANCODE_S] == KEY_STATE::KEY_IDLE
		&& App->input->keyboard[SDL_SCANCODE_W] == KEY_STATE::KEY_IDLE)
	{
		current_animation = &idle;
	}

	// TODO 3: Update collider position to player position
	coll->SetPos(position.x,position.y);

	// Draw everything --------------------------------------
	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return UPDATE_CONTINUE;
}

// TODO 4: Detect collision with a wall. If so, go back to intro screen.
