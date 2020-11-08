#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");

	leftTrigger = App->physics->CreateLeftTrigger();

	spriteSheet = App->textures->Load("pinball/pinballSpritesheet.png");

	lTriggerRect.x = 0;
	lTriggerRect.y = 203;
	lTriggerRect.w = 56;
	lTriggerRect.h = 19;

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	b2Vec2 anchorLVec = leftTrigger->joint->GetAnchorB();
	App->renderer->Blit(spriteSheet, 166, 760, &lTriggerRect, 1.0f, leftTrigger->GetRotation(), anchorLVec.x, anchorLVec.y - 4);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		leftTrigger->body->ApplyTorque(-65.0f, true);
	else
		leftTrigger->body->ApplyTorque(10.0f, true);

	return UPDATE_CONTINUE;
}



