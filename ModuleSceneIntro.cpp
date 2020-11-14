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

	arrowLightsAnim.PushBack({ 349,266,290,290 });
	arrowLightsAnim.PushBack({ 0,0,290,290 });

	//arrowLightsAnim.PushBack({ 0,0,0,0 });
	arrowLightsAnim.loop = true;
	arrowLightsAnim.speed = 0.07f;


	reboundLightAnim.PushBack({ 244,120,30,30 });
	reboundLightAnim.PushBack({ 0,0,0,0 });
	reboundLightAnim.loop = true;
	reboundLightAnim.speed = 0.05f;

	ballLostAnim.PushBack({ 0,163,67,20 });
	ballLostAnim.loop = false;
	ballLostAnim.speed = 0.01f;

	ballLostAnim.PushBack({ 0,163,67,20 });
	ballLostAnim.PushBack({ 0,0,0,0 });
	ballLostAnim.loop = false;
	ballLostAnim.speed = 0.01f;

	//plunge pushback
	plungeRect.x = 275;
	plungeRect.y = 116;
	plungeRect.w = 9;
	plungeRect.h = 32;

	topTexRect.x = 1;
	topTexRect.y = 73;
	topTexRect.w = 110;
	topTexRect.h = 90;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	ballsLeft = 3;
	App->UI->halfScoreRight = 0;


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

	debug = App->textures->Load("pinball/debug.png");

	backgroundTex = App->textures->Load("pinball/background.png");
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;	

	backgroundAssets=App->textures->Load("pinball/background assets.png");
	if (backgroundAssets == nullptr) LOG("Couldnt load background assets!")
	else
		LOG("bg assets loaded succesfully!")

	//load Fx

	hole_in_fx = App->audio->LoadFx("audio/sound_fx/hole_in.wav");
	hole_out_fx = App->audio->LoadFx("audio/sound_fx/hole_out.wav");
	triangle_fx = App->audio->LoadFx("audio/sound_fx/bouncing_triangle.wav");
	start_canon_fx = App->audio->LoadFx("audio/sound_fx/canon_shot.wav");
	lose_fx = App->audio->LoadFx("audio/sound_fx/lose.wav");
	five_colors_fx = App->audio->LoadFx("audio/sound_fx/five_colors.wav");
	bonus_fx = App->audio->LoadFx("audio/sound_fx/yellow_dot.wav");
	win_fx = App->audio->LoadFx("audio/sound_fx/win.wav");
	monster_roar_fx = App->audio->LoadFx("audio/sound_fx/monster_roar.wav");
	hit_wall_fx = App->audio->LoadFx("audio/sound_fx/hit_wall.wav");
	four_dots_fx = App->audio->LoadFx("audio/sound_fx/four_dots.wav");
	rebouncer_fx = App->audio->LoadFx("audio/sound_fx/yellow_dot.wav");

	App->audio->PlayMusic("audio/music/Nightmaren.ogg");

	//SDL_Rect atributtes
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;

	hole.x = 484;
	hole.y = 550;
	hole.w = 60;
	hole.h = 27;

	plungeRect.x = 284;
	plungeRect.y = 72;
	plungeRect.w = 20;
	plungeRect.h = 76;

	plungeCompRect.x = 233;
	plungeCompRect.y = 193;
	plungeCompRect.w = 67;
	plungeCompRect.h = 21;

	//spawned ball coordinates are in createBall()
	setSensors();
	SpawnBall();
	setWalls();
	

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
		CreateBallInMousePos();
	}

	//render objects

	//ball
	
	p2List_item<PhysBody*>* ball_item = balls.getFirst();
	while (ball_item != NULL)
	{
		int x, y;
		ball_item->data->GetPosition(x, y);

		float vel = ball_item->data->body->GetLinearVelocity().Length();
		ballAnim.speed = vel / 15;

		App->renderer->Blit(spriteSheet, x-2, y-2, &ballAnim.GetCurrentFrame(), 1.0f);
		
		ball_item = ball_item->next;
	}

	App->renderer->Blit(spriteSheet, 245, 70, &topTexRect, 1.0f);
	App->renderer->Blit(backgroundAssets, 349, 266, &arrowLightsAnim.GetCurrentFrame(), 1.0f);
	App->renderer->Blit(spriteSheet, 275, 116, &plungeRect, 1.0f);

	App->renderer->Blit(spriteSheet, 663, 457, &plungeRect, 1.0f);
	App->renderer->Blit(spriteSheet, 665, 525, &plungeCompRect, 1.0f);
	

	if (reb1) {
		App->audio->PlayFx(App->scene_intro->rebouncer_fx);
		App->renderer->Blit(spriteSheet, 429, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}

	if (reb2) {

		App->renderer->Blit(spriteSheet, 470, 173, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}

	if (reb3) {
		App->renderer->Blit(spriteSheet, 513, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}
	if (reboundLightAnim.Finished())
		reb1 = reb2 = reb3 = false;

	if (ballLostBlit)
	{
		App->renderer->Blit(spriteSheet, 452, 540, &ballLostAnim.GetCurrentFrame(), 1.0f);
		App->audio->PlayFx(holeFx);
	}

	if (ballLost)
	{
		App->audio->PlayFx(App->scene_intro->hole_in_fx);

		for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
		{
			App->physics->world->DestroyBody(bc->data->body);
		}
		balls.clear();

		ballsLeft--;

		if (ballsLeft > 0) {
			SpawnBall();
		}

		ballLost = false;
	}

	if (enterFunnel)
	{
		App->audio->PlayFx(hole_in_fx);
		SDL_Delay(1000);

		for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
		{
			App->physics->world->DestroyBody(bc->data->body);
		}
		balls.clear();

		enterFunnel = false;

	}

	if (holdInCatapult)
	{
		SDL_Delay(500);
		holdInCatapult = false;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//iterate all balls to see if they collide
	for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
	{
		
		

		if (bodyB->bodyType == _DEAD_SENSOR)
		{
			ballLostBlit = true;

			ballLost = true;

		}

		if (bodyB->bodyType == _REBOUNCER1)
		{
			reb1 = true;
			reboundLightAnim.Reset();
		}
		if (bodyB->bodyType == _REBOUNCER2)
		{
			reb2 = true;
			reboundLightAnim.Reset();
		}
		if (bodyB->bodyType == _REBOUNCER3)
		{
			reb3 = true;
			reboundLightAnim.Reset();
		}


		if (bodyB->bodyType == _LEVEL_CHANGE)
		{
			topLevelActive = !topLevelActive;

			if (topLevelActive == true)
			{
				LOG("Entering top level");
			}
			else
			{
				LOG("Exiting top level");
			}
		}

		if (bodyB->bodyType == _FUNNEL)
		{
			enterFunnel = true;
		}

		if (bodyB->bodyType == _CATAPULT)
		{
			holdInCatapult = true;
		}
	}

}

void ModuleSceneIntro::setWalls() {

	// Here we create all chains of the scene	

	// Bouncing triangles

	// Static walls

	int backgroundFloor[188] = {
	514, 532,
	526, 530,
	537, 525,
	549, 518,
	562, 511,
	571, 504,
	581, 493,
	590, 483,
	598, 472,
	606, 459,
	610, 447,
	614, 434,
	616, 420,
	616, 406,
	615, 392,
	613, 378,
	610, 364,
	609, 349,
	609, 333,
	611, 319,
	615, 304,
	619, 291,
	626, 280,
	633, 268,
	642, 258,
	643, 247,
	638, 240,
	630, 237,
	623, 239,
	616, 247,
	606, 257,
	600, 253,
	606, 242,
	616, 221,
	620, 209,
	623, 191,
	623, 172,
	621, 154,
	623, 147,
	632, 146,
	638, 150,
	648, 128,
	633, 114,
	614, 102,
	588, 95,
	570, 90,
	543, 84,
	527, 82,
	519, 67,
	505, 54,
	484, 49,
	466, 53,
	448, 65,
	443, 76,
	440, 86,
	420, 88,
	403, 93,
	385, 102,
	370, 113,
	355, 126,
	344, 142,
	341, 162,
	343, 184,
	347, 204,
	355, 224,
	367, 248,
	369, 255,
	363, 260,
	348, 246,
	339, 232,
	320, 242,
	331, 258,
	342, 276,
	349, 293,
	354, 308,
	357, 325,
	357, 343,
	357, 356,
	356, 368,
	352, 385,
	352, 404,
	352, 422,
	355, 439,
	361, 454,
	369, 470,
	383, 488,
	393, 499,
	403, 508,
	416, 516,
	429, 523,
	444, 530,
	454, 536,
	454, 552,
	514, 552
	};
	//backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundFloor, 190, BODY_INDEX::WALL, 0.01f, _NOT_DEFINED));
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundFloor, 188, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int backgroundLeftFunnel[60] = {
	337, 267,
	353, 251,
	345, 240,
	336, 227,
	330, 217,
	341, 217,
	357, 213,
	372, 203,
	381, 192,
	387, 178,
	389, 164,
	388, 149,
	380, 131,
	371, 120,
	357, 111,
	342, 106,
	326, 105,
	314, 109,
	303, 114,
	291, 124,
	283, 135,
	278, 148,
	276, 162,
	278, 173,
	282, 184,
	287, 194,
	296, 207,
	304, 220,
	313, 233,
	337, 267
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundLeftFunnel, 60, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_TOP_LEVEL));
	int ballStartTunnel[120] = {
	688, 328,
	701, 328,
	708, 332,
	711, 338,
	711, 441,
	709, 450,
	702, 454,
	659, 453,
	659, 260,
	660, 239,
	663, 219,
	666, 203,
	670, 186,
	672, 166,
	666, 144,
	652, 128,
	634, 120,
	618, 126,
	605, 137,
	599, 151,
	599, 171,
	607, 184,
	620, 190,
	635, 188,
	642, 178,
	643, 162,
	637, 149,
	622, 144,
	638, 124,
	648, 129,
	655, 138,
	662, 148,
	666, 158,
	666, 175,
	665, 187,
	658, 204,
	647, 214,
	628, 219,
	604, 213,
	586, 201,
	575, 180,
	573, 154,
	579, 130,
	594, 110,
	614, 100,
	633, 97,
	652, 100,
	666, 107,
	679, 119,
	689, 136,
	695, 154,
	697, 174,
	695, 189,
	690, 205,
	687, 223,
	684, 233,
	683, 428,
	689, 428,
	689, 335,
	688, 328
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, ballStartTunnel, 120, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_TOP_LEVEL));

	int bottomRightBumper[60] = {
	556, 403,
	552, 405,
	550, 408,
	549, 412,
	547, 416,
	545, 419,
	543, 423,
	540, 428,
	539, 430,
	537, 434,
	534, 438,
	532, 443,
	532, 448,
	532, 452,
	534, 454,
	538, 455,
	542, 452,
	545, 449,
	549, 445,
	552, 441,
	555, 437,
	557, 432,
	559, 427,
	561, 423,
	562, 418,
	562, 414,
	562, 410,
	561, 406,
	559, 404,
	556, 403
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightBumper, 60, BODY_INDEX::BUMPER, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int bottomLeftBumper[48] = {
	410, 404,
	413, 403,
	417, 405,
	420, 412,
	423, 419,
	427, 427,
	430, 431,
	433, 436,
	437, 442,
	438, 445,
	438, 450,
	436, 453,
	433, 454,
	430, 453,
	426, 450,
	422, 447,
	419, 442,
	416, 437,
	413, 431,
	411, 426,
	409, 419,
	408, 412,
	408, 408,
	410, 404
};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftBumper, 48, BODY_INDEX::BUMPER, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int bottomRightRamp[70] = {
	592, 398,
	586, 401,
	585, 405,
	584, 411,
	583, 417,
	581, 424,
	579, 431,
	576, 437,
	574, 442,
	571, 447,
	567, 452,
	564, 456,
	560, 460,
	556, 464,
	551, 468,
	548, 471,
	544, 474,
	557, 487,
	562, 484,
	568, 479,
	573, 474,
	577, 469,
	581, 464,
	584, 458,
	587, 453,
	589, 447,
	591, 441,
	592, 435,
	593, 429,
	594, 423,
	594, 417,
	594, 411,
	595, 405,
	594, 401,
	592, 398
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightRamp, 70, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int bottomLeftRamp[74] = {
	374, 401,
	377, 398,
	381, 399,
	382, 403,
	382, 409,
	382, 414,
	382, 419,
	384, 425,
	387, 431,
	390, 436,
	392, 440,
	395, 444,
	399, 449,
	402, 453,
	406, 457,
	410, 461,
	414, 465,
	420, 470,
	424, 473,
	414, 487,
	409, 487,
	405, 485,
	402, 482,
	399, 479,
	395, 475,
	392, 471,
	389, 467,
	386, 462,
	383, 457,
	380, 451,
	378, 444,
	376, 437,
	375, 431,
	374, 424,
	374, 416,
	374, 410,
	374, 401
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftRamp, 74, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));

	int middleCannon[66] = {
	445, 283,
	447, 289,
	450, 296,
	454, 301,
	458, 305,
	463, 309,
	468, 312,
	472, 303,
	477, 291,
	482, 289,
	490, 294,
	496, 304,
	501, 313,
	507, 309,
	513, 305,
	518, 300,
	522, 294,
	524, 286,
	524, 280,
	521, 276,
	516, 274,
	510, 272,
	502, 271,
	495, 270,
	489, 269,
	481, 269,
	473, 270,
	467, 271,
	461, 272,
	455, 274,
	450, 276,
	446, 278,
	445, 282
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, middleCannon, 66, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));

	int topLeftBoomerang[44] = {
	388, 230,
	394, 226,
	383, 198,
	377, 184,
	376, 172,
	386, 162,
	412, 153,
	427, 147,
	431, 138,
	428, 127,
	421, 120,
	409, 121,
	390, 131,
	375, 143,
	367, 155,
	365, 168,
	365, 182,
	368, 197,
	372, 210,
	378, 221,
	382, 227,
	388, 230,
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftBoomerang, 44, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int topRightBoomerang[44] = {
	544, 148,
	580, 161,
	590, 167,
	591, 172,
	591, 180,
	588, 192,
	577, 223,
	579, 229,
	586, 228,
	593, 217,
	600, 202,
	604, 184,
	603, 169,
	598, 149,
	587, 136,
	574, 129,
	561, 124,
	549, 123,
	542, 128,
	537, 136,
	538, 143,
	544, 148
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightBoomerang, 44, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int topLeftStick[18] = {
	466, 112,
	461, 116,
	461, 130,
	461, 140,
	467, 144,
	472, 140,
	472, 126,
	471, 116,
	466, 112,
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftStick, 18, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
	int topRightStick[20] = {
	500, 110,
	497, 116,
	496, 127,
	496, 139,
	501, 145,
	506, 145,
	510, 139,
	510, 124,
	509, 115,
	500, 110
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightStick, 20, BODY_INDEX::WALL, PHYSIC_BODY_TYPE::_FLOOR_LEVEL));
}

void ModuleSceneIntro::setSensors()
{
	// Set triggers and sensors
	// Here we create all sensors in the scene

	sensors.add(App->physics->CreateRectangleSensor(hole.x, hole.y, hole.w, hole.h, _DEAD_SENSOR));

	sensors.add(App->physics->CreateCircleSensor(442, 223, 10, _REBOUNCER1));
	sensors.add(App->physics->CreateCircleSensor(485, 188, 10, _REBOUNCER2));
	sensors.add(App->physics->CreateCircleSensor(526, 223, 10, _REBOUNCER3));

	sensors.add(App->physics->CreateRectangleSensor(340, 255, 20, 20, _LEVEL_CHANGE));

	sensors.add(App->physics->CreateRectangleSensor(333, 162, 5, 5, _FUNNEL));

	//CATAPULTA
	sensors.add(App->physics->CreateRectangleSensor(632, 250, 20, 20, _CATAPULT));
	sensors.add(App->physics->CreateRectangleSensor(484, 300, 28, 28, _FUNNEL));

	sensors.add(App->physics->CreateRectangleSensor(363, 416, 28, 28, _MINI_PLUNGE));
	sensors.add(App->physics->CreateRectangleSensor(604, 416, 28, 28, _REBOUNCER3));

	
}

void ModuleSceneIntro::SpawnBall()
{
	//create in module physics the next functions

	balls.add(App->physics->CreateBall(350,30,9));
	balls.getLast()->data->listener = this;
}

void ModuleSceneIntro::SpawnBall(int x,int y)
{
	//create in module physics the next functions

	balls.add(App->physics->CreateBall(x,y, 9));
	balls.getLast()->data->listener = this;
}

void ModuleSceneIntro::CreateBallInMousePos()
{
	int x, y;
	int radius = 9;

	x = App->input->GetMouseX();
	y = App->input->GetMouseY();

	balls.add(App->physics->CreateBall(x, y, radius));
	balls.getLast()->data->listener = this;
}