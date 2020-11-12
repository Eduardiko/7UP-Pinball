#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "Animation.h"

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

	void SpawnBall();
	void CreateBallInMousePos();
	void setWalls();
	void setSensors();

public:

	p2List<PhysBody*> balls;
	p2List<PhysBody*> backgroundWalls;
	p2List<PhysBody*> sensors;

	SDL_Texture* backgroundTex;
	SDL_Rect background;
private:
	// win/fail
	unsigned int ballsLeft;


	//Fx
	uint hitWallFx;
	uint holeFx;

	SDL_Texture *spriteSheet;
	SDL_Texture* backgroundAssets;

	Animation ballAnim, arrowLightsAnim,reboundLightAnim,ballLostAnim;


	//sensors and collision detectors
	SDL_Rect hole, rebouncer1, rebouncer2, rebouncer3;
};
