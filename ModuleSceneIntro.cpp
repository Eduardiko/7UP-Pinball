#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModuleUI.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	backgroundTex = NULL;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	ballsLeft = 3;
	App->UI->score = 0;

	LOG("Loading Intro assets");
	bool ret = true;

	backgroundTex = App->textures->Load("pinball/background.png");
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;	

	hitWallFx = App->audio->LoadFx(".wav");

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	App->renderer->Blit(backgroundTex, 0, 0, &background, 1.0f);

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{


}

void ModuleSceneIntro::setWalls() {

	// Here we create all chains of the scene

	// Bouncing triangles

	

	// Static walls

	int points_top_wall[160] =
	{
		
	};

	
	int points_bottom_wall[108] =
	{
		
	};

	

}

void ModuleSceneIntro::setSensors()
{
	// Set triggers and sensors

	

	
}

void ModuleSceneIntro::spawnBall()
{
	//create in module physics the next functions

	/*balls.add(App->physics->CreateBall
	balls.getLast()->data->listener = this;*/
}