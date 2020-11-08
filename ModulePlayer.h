#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"
#include "ModuleTextures.h"
#include "ModulePhysics.h"

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update();
	bool CleanUp();

public:

	PhysBody* rightTrigger;
	SDL_Rect rTriggerRect;

	PhysBody* leftTrigger;
	SDL_Rect lTriggerRect;

	SDL_Texture* spriteSheet;

	PhysBody* elDisparador;
};