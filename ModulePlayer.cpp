#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleUI.h"

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
	rightTrigger = App->physics->CreateRightTrigger();
	plungeTrigger = App->physics->CreatePlunge();
	spriteSheet = App->textures->Load("pinball/pinballSpritesheet.png");

	lTriggerRect.x = 0;
	lTriggerRect.y = 203;
	lTriggerRect.w = 56;
	lTriggerRect.h = 19;

	rTriggerRect.x = 56;
	rTriggerRect.y = 202;
	rTriggerRect.w = 56;
	rTriggerRect.h = 19;

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
	b2Vec2 anchorRVec = rightTrigger->joint->GetAnchorB();

	App->renderer->Blit(spriteSheet, 418, 476, &lTriggerRect, 1.0f, leftTrigger->GetRotation(), anchorLVec.x, anchorLVec.y - 4);
	App->renderer->Blit(spriteSheet, 494, 476, &rTriggerRect, 1.0f, rightTrigger->GetRotation(), anchorRVec.x + 36, anchorRVec.y - 4);

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		leftTrigger->body->ApplyTorque(-65.0f, true);
		App->UI->halfScoreRight++;
	}
	else
	{
		leftTrigger->body->ApplyTorque(15.0f, true);
	}

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		rightTrigger->body->ApplyTorque(65.0f, true);
	else
	{
		rightTrigger->body->ApplyTorque(-15.0f, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		plungeTrigger->body->ApplyForceToCenter(b2Vec2(0, 250), true);
	}
	return UPDATE_CONTINUE;
}



