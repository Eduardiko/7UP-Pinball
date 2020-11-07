#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

	void spawnBall();
	void setWalls();
	void setSensors();

public:

	p2List<PhysBody*> balls;

	SDL_Texture* backgroundTex;
	SDL_Rect background;
private:
	// win/fail
	unsigned int ballsLeft;

	//Fx
	uint hitWallFx;
	uint holeFx;
};
