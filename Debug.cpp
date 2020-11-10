#define _CRT_SECURE_NO_WARNINGS

#include "Debug.h"
#include "ModuleInput.h"
#include "Application.h"

//#include "App.h"
//#include "Input.h"
//#include "Collisions.h"
//#include "Log.h";

ModuleDebug::ModuleDebug(Application* app, bool start_enabled) :  Module(app, start_enabled)
{
	//name.Create("debug");
}


ModuleDebug::~ModuleDebug()
{

}

bool ModuleDebug::Awake()
{
	return true;
}

bool ModuleDebug::Start()
{
	return true;
}

update_status ModuleDebug::PreUpdate()
{
	update_status status = UPDATE_CONTINUE;


	return status;
}

update_status ModuleDebug::Update()
{
	update_status status = UPDATE_CONTINUE;

	
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		ToggleColliders();
	}

	if (App->input->GetMouseButton(1))
	{
		int x, y;
		x = App->input->GetMouseX(); y = App->input->GetMouseY();
		LOG("Mouse Position X: %d , Y: %d", x, y);
	}

	return status;
}

update_status ModuleDebug::PostUpdate()
{
	update_status status = UPDATE_CONTINUE;

	return status;
}

bool ModuleDebug::CleanUp()
{
	return true;
}

void ModuleDebug::ToggleColliders()
{
	if (App->debug->showColliders == false)
	{
		LOG("Draw Colliders");
		App->debug->showColliders = true;
	}
	else if (App->debug->showColliders == true) {
		LOG("Undraw Colliders")
		App->debug->showColliders = false;
	}
}