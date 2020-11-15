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
	void SpawnBall(int x, int y);
	void CreateBallInMousePos();
	void setWalls();
	void setSensors();
	void ResetBallPos(PhysBody* ball, int x, int y);


public:
	bool gameStarted = false;

	bool reb1, reb2, reb3, ballLostBlit, enterFunnel, holdInCatapult, star1, star2, star3;

	p2List<PhysBody*> balls;
	PhysBody* ballPendingToDelete;
	PhysBody* littlePlungeTriggerR;
	PhysBody* littlePlungeTriggerL;

	p2List<PhysBody*> backgroundWalls;
	p2List<PhysBody*> sensors;

	SDL_Texture* backgroundTex;
	SDL_Rect background;

	SDL_Texture* topTextures;
	SDL_Rect topTexRect;

	SDL_Rect plungeRect;
	SDL_Rect plungeCompRect;
	SDL_Rect littlePlungeRect;

	uint trigger_hit_fx;
private:
	// win/fail
	unsigned int ballsLeft;
	bool ballLost = false;
	bool pendingToDelete = false;

	bool ballInHole = false;
	bool ballInTop = true;

	//Fx
	uint hitWallFx;
	uint holeFx;

	uint hole_in_fx;
	uint hole_out_fx;
	uint triangle_fx;
	uint start_canon_fx;
	uint bonus_fx;
	uint lose_fx;
	uint five_colors_fx;
	uint four_dots_fx;
	uint win_fx;
	uint monster_roar_fx;
	uint hit_wall_fx;
	uint rebouncer_fx;
	uint combo_fx;

	SDL_Texture* spriteSheet;
	SDL_Texture* backgroundAssets;
	SDL_Texture* debug;
	Animation ballAnim, arrowLightsAnim, reboundLightAnim, ballLostAnim, thinkClearAnim, playGameAnim, starAnim;


	//sensors and collision detectors
	SDL_Rect hole, rebouncer1, rebouncer2, rebouncer3;
};
