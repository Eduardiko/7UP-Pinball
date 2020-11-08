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
	

	ballAnim.PushBack({ 220, 126, 22, 22 });
	ballAnim.loop = true;
	ballAnim.speed = 0.0f;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	ballsLeft = 3;
	App->UI->score = 0;

	if (!App->audio->IsEnabled() ) {
		App->audio->Enable();
		App->audio->Init();
	}

	if (!App->textures->IsEnabled()) {
		App->textures->Enable();
		App->textures->Init();
	}

	if (!App->UI->IsEnabled()) {
		App->UI->Enable();
		App->UI->Start();
	}

	LOG("Loading Intro assets");
	bool ret = true;

	spriteSheet = App->textures->Load("pinball/pinballSpritesheet.png");
	if (spriteSheet == nullptr) LOG("Couldnt load sprite sheet!")
	else
		LOG("sprite sheet loaded succesfully!")

	backgroundTex = App->textures->Load("pinball/background.png");
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;	

	hitWallFx = App->audio->LoadFx(".wav");

	//spawned ball coordinates are in createBall()
	SpawnBall();

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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		SpawnBall();
	}

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

void ModuleSceneIntro::SpawnBall()
{
	//create in module physics the next functions

	balls.add(App->physics->CreateBall(30,30));
	//balls.getLast()->data->listener = this;
}