#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"


#include <string.h>
#include<stdio.h>


ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	score = 0;
}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading ModuleUI assets");
	bool ret = true;

	return ret;
}

// Update: draw background
update_status ModuleUI::Update()
{
	update_status status = UPDATE_CONTINUE;
	

	return status;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading ModuleUI");

	bool ret = true;

	

	return ret;
}




