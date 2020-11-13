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
	ballLostAnim.loop = true;
	ballLostAnim.speed = 0.01f;

	//plunge pushback
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

	backgroundTex = App->textures->Load("pinball/background.png");
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;	

	backgroundAssets=App->textures->Load("pinball/background assets.png");
	if (backgroundAssets == nullptr) LOG("Couldnt load background assets!")
	else
		LOG("bg assets loaded succesfully!")

	hitWallFx = App->audio->LoadFx(".wav");

	//SDL_Rect atributtes
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;

	hole.x = 484;
	hole.y = 550;
	hole.w = 60;
	hole.h = 27;

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

	/*if (start_canon.GetCurrentFrame().x == 801 && !ball_created && inside_start_canon)
	{
		for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
		{
			App->physics->world->DestroyBody(bc->data->body);
		}
		balls.clear();

		balls.add(App->physics->CreateBall(485, 608, 14));
		balls.getLast()->data->listener = this;
		ball_created = true;

		for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
		{
			int x, y;
			bc->data->GetPosition(x, y);
			bc->data->body->ApplyLinearImpulse(b2Vec2(-2.3f, -2.9f), b2Vec2(x, y), true);
		}
		//App->audio->PlayFx();
		//start_canon_fx
		if (inside_start_canon)
			inside_start_canon = false;
	}*/
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
	//App->renderer->Blit(spriteSheet, x, y, &ballAnim.GetCurrentFrame(), 1.0f);

	App->renderer->Blit(backgroundAssets, 349, 266, &arrowLightsAnim.GetCurrentFrame(), 1.0f);

	/*App->renderer->Blit(spriteSheet, 429, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	App->renderer->Blit(spriteSheet, 513, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	App->renderer->Blit(spriteSheet, 470, 173, &reboundLightAnim.GetCurrentFrame(), 1.0f);*/

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	//iterate all balls to see if they collide
	for (p2List_item<PhysBody*>* bc = balls.getFirst(); bc != NULL; bc = bc->next)
	{
		
		if (bodyB->bodyType == _DEAD_SENSOR)
		{
			LOG("Ball lost");
			App->renderer->Blit(spriteSheet, 452, 540, &ballLostAnim.GetCurrentFrame(), 1.0f);
			App->audio->PlayFx(holeFx);
			ballLost = true;
			ballsLeft--;
		}

		if (bodyB->bodyType == _REBOUNCER1)
		{
			LOG("rebouncer 1");
			App->renderer->Blit(spriteSheet, 429, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
		}
		if (bodyB->bodyType == _REBOUNCER2)
		{
			App->renderer->Blit(spriteSheet, 513, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
		}
		if (bodyB->bodyType == _REBOUNCER3)
		{
			App->renderer->Blit(spriteSheet, 470, 173, &reboundLightAnim.GetCurrentFrame(), 1.0f);
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundFloor, 188));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundLeftFunnel, 60));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, ballStartTunnel, 120));

	int bottomRightBumper[22] = {
	553, 404,
	560, 405,
	560, 419,
	556, 430,
	549, 443,
	540, 453,
	531, 452,
	533, 443,
	539, 433,
	546, 420,
	553, 404
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightBumper, 22));
	int bottomLeftBumper[22] = {
	415, 404,
	408, 407,
	409, 419,
	414, 433,
	419, 439,
	425, 448,
	432, 452,
	437, 446,
	430, 434,
	424, 421,
	415, 404
};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftBumper, 22));
	int bottomRightRamp[28] = {
	586, 400,
	593, 401,
	592, 426,
	587, 447,
	576, 469,
	564, 481,
	555, 486,
	546, 477,
	552, 469,
	561, 462,
	570, 451,
	577, 439,
	584, 419,
	586, 400
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightRamp, 28));
	int bottomLeftRamp[30] = {
	381, 402,
	383, 418,
	388, 438,
	400, 455,
	412, 466,
	420, 474,
	419, 481,
	412, 486,
	400, 477,
	391, 466,
	382, 453,
	377, 435,
	374, 419,
	374, 405,
	381, 402
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftRamp, 30));

	int middleCannon[44] = {
	452, 277,
	447, 280,
	446, 289,
	451, 299,
	460, 308,
	468, 312,
	474, 299,
	485, 288,
	495, 301,
	501, 316,
	515, 307,
	522, 291,
	523, 281,
	517, 274,
	508, 272,
	503, 275,
	493, 271,
	485, 274,
	477, 271,
	470, 275,
	460, 274,
	452, 277
	};
	backgroundWalls.add(App->physics->CreateChain(0, 0, middleCannon, 44));

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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftBoomerang, 44));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightBoomerang, 44));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftStick, 18));

	printf("jamon");

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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightStick, 20));
}

void ModuleSceneIntro::setSensors()
{
	// Set triggers and sensors
	// Here we create all sensors in the scene

	sensors.add(App->physics->CreateRectangleSensor(hole.x, hole.y, hole.w, hole.h, _DEAD_SENSOR));
	
	sensors.add(App->physics->CreateRectangleSensor(443, 223, 28,28, _REBOUNCER1));
	sensors.add(App->physics->CreateRectangleSensor(485, 188, 28,28, _REBOUNCER2));
	sensors.add(App->physics->CreateRectangleSensor(528, 223, 28,28, _REBOUNCER3));
	
	sensors.add(App->physics->CreateRectangleSensor(340, 255, 20, 20, _LEVEL_CHANGE));
	
}

void ModuleSceneIntro::SpawnBall()
{
	//create in module physics the next functions

	balls.add(App->physics->CreateBall(350,30));
	balls.getLast()->data->listener = this;
}

void ModuleSceneIntro::CreateBallInMousePos()
{
	int x, y;
	x = App->input->GetMouseX();
	y = App->input->GetMouseY();

	balls.add(App->physics->CreateBall(x, y));
	balls.getLast()->data->listener = this;
}