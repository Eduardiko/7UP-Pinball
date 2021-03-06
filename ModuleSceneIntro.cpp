#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
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

	playGameAnim.PushBack({ 0,0,153,71 });
	playGameAnim.PushBack({ 153,0,153,71 });
	playGameAnim.PushBack({ 0,0,153,71 });
	playGameAnim.loop = true;
	playGameAnim.speed = 0.01f;

	starAnim.PushBack({ 0,184,19,19 });
	starAnim.loop = true;
	starAnim.speed = 0.01f;

	//arrows 
	arr1.PushBack({ 0,0,3000,3000 });
	arr2.PushBack({ 0,0,3000,3000 });
	arr3.PushBack({ 0,0,3000,3000 });
	arr4.PushBack({ 0,0,3000,3000 });
	arr5.PushBack({ 0,0,3000,3000 });
	arr6.PushBack({ 0,0,3000,3000 });
	arr1.loop = arr2.loop = arr3.loop = arr4.loop = arr5.loop = arr6.loop = true;
	arr1.speed = arr2.speed = arr3.speed = arr4.speed = arr5.speed = arr6.speed = 0.01f;

	plungeAnim.PushBack({ 284,72,20,76 });
	plungeAnim.loop = true;

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


	bool ret = true;

	spriteSheet = App->textures->Load("pinball/pinballSpritesheet.png");

	tri1 = App->textures->Load("pinball/tri 1.png");
	tri2 = App->textures->Load("pinball/tri 2.png");
	tri3 = App->textures->Load("pinball/tri 3.png");
	tri4 = App->textures->Load("pinball/tri 4.png");
	tri5 = App->textures->Load("pinball/tri 5.png");
	tri6 = App->textures->Load("pinball/tri 6.png");

	debug = App->textures->Load("pinball/debug.png");

	backgroundTex = App->textures->Load("pinball/background.png");
	background.x = 0;
	background.y = 0;
	background.w = SCREEN_WIDTH;
	background.h = SCREEN_HEIGHT;	

	backgroundAssets=App->textures->Load("pinball/background assets.png");
	backgroundTextTex = App->textures->Load("pinball/StartMenuBackground.png");
	//load Fx

	trigger_hit_fx = App->audio->LoadFx("audio/sound_fx/flipper_hit.wav");
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
	combo_fx = App->audio->LoadFx("audio/sound_fx/combo.wav");

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

	littlePlungeRect.x = 275;
	littlePlungeRect.y = 116;
	littlePlungeRect.w = 9;
	littlePlungeRect.h = 32;

	littlePlungeTriggerL = App->physics->CreateMiniPlunge(364, 440);
	littlePlungeTriggerR = App->physics->CreateMiniPlunge(602, 440);

	//spawned ball coordinates are in createBall()
	setSensors();
	SpawnBall();
	setWalls();
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (ballsLeft == 0)
	{
		gameStarted = false;
	}
	if (ballLost) {
		ballLost = false;
		ResetBallPos(ballPendingToDelete, 663, 400);
	}

	if (ballInHole) {
		ballInHole = false;
		int offset = rand() % 6;

		ResetBallPos(ballPendingToDelete, 483 + offset, 82);
	}

	App->renderer->Blit(backgroundTex, 0, 0, &background, 1.0f);


	//render objects
	
	App->renderer->Blit(spriteSheet, 245, 70, &topTexRect, 1.0f);

	if (gameStarted == true)
	{
		if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			if (goingDown < 20)
				goingDown = goingDown + 2;

			App->renderer->Blit(spriteSheet, 663, 430 + goingDown, &plungeRect, 1.0f);

		}
		else {
			if (goingDown > 0)
				goingDown = goingDown - 5;

			App->renderer->Blit(spriteSheet, 663, 430 + goingDown, &plungeRect, 1.0f);
		}

		App->renderer->Blit(spriteSheet, 660, 505, &plungeCompRect, 1.0f);
		App->renderer->Blit(spriteSheet, 365, 438, &littlePlungeRect, 1.0f);
		App->renderer->Blit(spriteSheet, 596, 438, &littlePlungeRect, 1.0f);
	}
	else {
		App->renderer->Blit(spriteSheet, 660, 505, &plungeCompRect, 1.0f);
		App->renderer->Blit(spriteSheet, 365, 438, &littlePlungeRect, 1.0f);
		App->renderer->Blit(spriteSheet, 596, 438, &littlePlungeRect, 1.0f);
		App->renderer->Blit(spriteSheet, 663, 430, &plungeRect, 1.0f);
	}
	//plunge ani	

	if (reb1) {
		App->audio->PlayFx(App->scene_intro->rebouncer_fx);
		App->renderer->Blit(spriteSheet, 429, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}

	if (reb2) {
		App->audio->PlayFx(App->scene_intro->rebouncer_fx);
		App->renderer->Blit(spriteSheet, 470, 173, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}

	if (reb3) {
		App->audio->PlayFx(App->scene_intro->rebouncer_fx);
		App->renderer->Blit(spriteSheet, 513, 209, &reboundLightAnim.GetCurrentFrame(), 1.0f);
	}
	if (reboundLightAnim.Finished())
		reb1 = reb2 = reb3 = false;

	if (ballLostBlit)
	{
		App->renderer->Blit(spriteSheet, 452, 540, &ballLostAnim.GetCurrentFrame(), 1.0f);
	}

	if (holdInCatapult)
	{
		//SDL_Delay(500);
		//holdInCatapult = false;
	}

	if (star1)
	{
		App->renderer->Blit(spriteSheet, 439, 103, &starAnim.GetCurrentFrame(), 1.0f);
	}
	if (star2)
	{
		App->audio->PlayFx(holeFx);
		App->renderer->Blit(spriteSheet, 476, 99, &starAnim.GetCurrentFrame(), 1.0f);
	}
	if (star3)
	{
		App->audio->PlayFx(holeFx);
		App->renderer->Blit(spriteSheet, 514, 103, &starAnim.GetCurrentFrame(), 1.0f);
	}

	if (star1 && star2 & star3 == true)
	{
		ballsLeft++;
		counter++;
		if (counter == 75)
			counter = 0;
			star1 = star2 = star3 = false;
	}

	App->renderer->Blit(tri1, 110, 110, &arr1.GetCurrentFrame(), 1.0f);

	switch (arrowCounter)
	{
	case(1):
		App->renderer->Blit(tri1, 0, 0, &arr1.GetCurrentFrame(), 1.0f);
		break;
	case(2):
		App->renderer->Blit(tri2,0, 0, &arr2.GetCurrentFrame(), 1.0f);
		break;
	case(3):
		App->renderer->Blit(tri3, 0, 0, &arr3.GetCurrentFrame(), 1.0f);
		break;
	case(4):
		App->renderer->Blit(tri4, 0, 0, &arr4.GetCurrentFrame(), 1.0f);
		break;
	case(5):
		App->renderer->Blit(tri5, 0, 0, &arr5.GetCurrentFrame(), 1.0f);
		break;
	case(6):
		App->renderer->Blit(tri6, 0, 0, &arr6.GetCurrentFrame(), 1.0f);
		break;
	}

	arrowCounter++;
	if (arrowCounter == 7) arrowCounter = 1;


	if (gameStarted == false)
	{
		App->renderer->Blit(backgroundTextTex, 0, 0, &background, 1.0f);
		App->renderer->Blit(spriteSheet, 410, 250, &playGameAnim.GetCurrentFrame(), 1.0f);
	}
	else
	{
		App->renderer->Blit(backgroundAssets, 349, 266, &arrowLightsAnim.GetCurrentFrame(), 1.0f);
	}
	//ball
	

	p2List_item<PhysBody*>* ball_item = balls.getFirst();
	while (ball_item != NULL)
	{
		int x, y;
		ball_item->data->GetPosition(x, y);

		float vel = ball_item->data->body->GetLinearVelocity().Length();
		ballAnim.speed = vel / 15;

		App->renderer->Blit(spriteSheet, x - 2, y - 2, &ballAnim.GetCurrentFrame(), 1.0f);

		ball_item = ball_item->next;
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{

	if (bodyB->sensorType == _DEAD_SENSOR)
	{
		star1 = star2 = star3 = false;
		ballLostBlit = true;
		App->audio->PlayFx(App->scene_intro->hole_in_fx);
		ballLost = true;
		ballPendingToDelete = bodyA;
		ballsLeft--;

		b2Fixture* fixture = bodyA->body->GetFixtureList();

		while (fixture != NULL)
		{
			b2Filter newFilter;
			newFilter.groupIndex = BODY_TYPE::BALL_TOP;
			ballInTop = false;
			fixture->SetFilterData(newFilter);
			fixture = fixture->GetNext();
		}


		ballInTop = true;


	}

		if (bodyB->sensorType == _REBOUNCER1)
		{
			reb1 = true;
			reboundLightAnim.Reset();
			App->UI->score += 1000;
		}
		if (bodyB->sensorType == _REBOUNCER2)
		{
			reb2 = true;
			reboundLightAnim.Reset();
			App->UI->score += 1000;
		}
		if (bodyB->sensorType == _REBOUNCER3)
		{
			reb3 = true;
			reboundLightAnim.Reset();
			App->UI->score += 1000;
		}


		if (bodyB->sensorType == _LEVEL_CHANGE)
		{
		
			b2Fixture* fixture = bodyA->body->GetFixtureList();

			while (fixture != NULL)
			{
				b2Filter newFilter;
				if (ballInTop == true)
				{
					newFilter.groupIndex = BODY_TYPE::BALL_FLOOR;
					ballInTop = false;
				}
				else
				{
					newFilter.groupIndex = BODY_TYPE::BALL_TOP;
					ballInTop = true;

				}
				
				fixture->SetFilterData(newFilter);
				fixture = fixture->GetNext();
			}

		}

		if (bodyB->sensorType == _FUNNEL)
		{
			enterFunnel = true;
			App->UI->score += 4500;

			b2Fixture* fixture = bodyA->body->GetFixtureList();

			while (fixture != NULL)
			{
				b2Filter newFilter;
				newFilter.groupIndex = BODY_TYPE::BALL_FLOOR;
				ballInTop = false;
				fixture->SetFilterData(newFilter);
				fixture = fixture->GetNext();
			}

			ballInHole = true;
			App->audio->PlayFx(App->scene_intro->hole_in_fx);
			ballPendingToDelete = bodyA;
			ballInTop = false;
		}

		if (bodyB->sensorType == _CATAPULT)
		{
			App->UI->score += 4500;
			//holdInCatapult = true;
		}

		if (bodyB->sensorType == _MINI_PLUNGE_L || _MINI_PLUNGE_R)
		{
			App->UI->score += 1500;
		}

		if (bodyB->sensorType == _BUMPER_L)
		{
			bodyA->body->ApplyLinearImpulse({ 0.5f * 2.5, -0.5f * 2.5 }, { 0.0f,0.0f }, true);
		}
		if (bodyB->sensorType == _BUMPER_R)
		{
			bodyA->body->ApplyLinearImpulse({ -0.5f * 2.5, -0.5f * 2.5 }, { 0.0f,0.0f }, true);
		}


		if (bodyB->sensorType == _STAR1)
		{
			App->audio->PlayFx(holeFx);
			star1 = true;
			starAnim.Reset();
			App->UI->score += 2000;
		}
		if (bodyB->sensorType == _STAR2)
		{
			star2 = true;
			starAnim.Reset();
			App->UI->score += 2000;
		}
		if (bodyB->sensorType == _STAR3)
		{
			star3 = true;
			starAnim.Reset();
			App->UI->score += 2000;
		}
		
}


void ModuleSceneIntro::setWalls() {

	// Here we create all chains of the scene	

	// Bouncing triangles

	// Static walls

	int backgroundFloor[196] = {
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
	644, 160,
	645, 170,
	641, 182,
	664, 190,
	668, 170,
	662, 145,
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundFloor, 196, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, backgroundLeftFunnel, 60, BODY_TYPE::WALL_TOP));
	int ballStartTunnel[120] = {
	688, 328,
	701, 328,
	708, 332,
	711, 338,
	711, 441,
	709, 450,
	702, 452,
	659, 456,
	659, 260,
	660, 239,
	663, 219,
	666, 203,
	670, 186,
	672, 166,
	666, 144,
	652, 128,
	627, 122,
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
	610, 137,
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, ballStartTunnel, 120, BODY_TYPE::WALL_TOP));

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
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightBumper, 60, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftBumper, 48, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomRightRamp, 70, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, bottomLeftRamp, 74, BODY_TYPE::WALL_FLOOR));

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
	backgroundWalls.add(App->physics->CreateChain(0, 0, middleCannon, 66, BODY_TYPE::WALL_FLOOR));

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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftBoomerang, 44, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightBoomerang, 44, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topLeftStick, 18, BODY_TYPE::WALL_FLOOR));
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
	backgroundWalls.add(App->physics->CreateChain(0, 0, topRightStick, 20, BODY_TYPE::WALL_FLOOR));
	int bumperSensorL[4] =
	{
		420, 403,
		436, 440,
	};
	backgroundWalls.add(App->physics->CreateBumperChainSensor(0, 0, bumperSensorL, 4, SENSOR_TYPE::_BUMPER_L));
	int bumperSensorR[4] =
	{
		549, 408,
		533, 440
	};
	backgroundWalls.add(App->physics->CreateBumperChainSensor(0, 0, bumperSensorR, 4, SENSOR_TYPE::_BUMPER_R));
	
	int topLeftBullseye[10]
	{
		426, 146,
		429, 150,
		380, 170,
		377, 165,
		426, 146
	};
	backgroundWalls.add(App->physics->CreateBumperChain(0, 0, topLeftBullseye, 10, BODY_TYPE::WALL_FLOOR));
	int topRightBullseye[10]
	{
		541, 144,
		538, 148,
		592, 170,
		594, 165,
		541, 144
	};
	backgroundWalls.add(App->physics->CreateBumperChain(0, 0, topRightBullseye, 10, BODY_TYPE::WALL_FLOOR));
	int middleBullseye[10]
	{
		509, 268,
		460, 268,
		460, 275,
		508, 275,
		509, 268
	};
	backgroundWalls.add(App->physics->CreateBumperChain(0, 0, middleBullseye, 10, BODY_TYPE::WALL_FLOOR));
}


void ModuleSceneIntro::setSensors()
{
	// Set triggers and sensors
	// Here we create all sensors in the scene

	sensors.add(App->physics->CreateRectangleSensor(hole.x, hole.y, hole.w, hole.h, _DEAD_SENSOR));

	sensors.add(App->physics->CreateCircleBumper(442, 223, 10, _REBOUNCER1));
	sensors.add(App->physics->CreateCircleBumper(485, 188, 10, _REBOUNCER2));
	sensors.add(App->physics->CreateCircleBumper(526, 223, 10, _REBOUNCER3));

	sensors.add(App->physics->CreateRectangleSensor(337, 247, 2, 2, _LEVEL_CHANGE));
	sensors.add(App->physics->CreateRectangleSensor(652, 156, 2, 2, _LEVEL_CHANGE));

	sensors.add(App->physics->CreateRectangleSensor(333, 162, 5, 5, _FUNNEL));

	//CATAPULTA
	sensors.add(App->physics->CreateRectangleSensor(632, 250, 20, 20, _FUNNEL));
	sensors.add(App->physics->CreateRectangleSensor(484, 300, 28, 28, _FUNNEL));

	sensors.add(App->physics->CreateRectangleSensor(363, 416, 28, 28, _MINI_PLUNGE_L));
	sensors.add(App->physics->CreateRectangleSensor(604, 416, 28, 28, _MINI_PLUNGE_R));

	sensors.add(App->physics->CreateRectangleSensor(446, 134, 14,14, _STAR1));
	sensors.add(App->physics->CreateRectangleSensor(485, 133, 14, 14, _STAR2));
	sensors.add(App->physics->CreateRectangleSensor(523, 133, 14, 14, _STAR3));

	/*sensors.add(App->physics->CreateBumperChainSensor(420, 403, &28, 4, _BUMPER_L));
	sensors.add(App->physics->CreateBumperChainSensor(549, 533, &28, 4, _BUMPER_R));*/
}

void ModuleSceneIntro::SpawnBall()
{
	//create in module physics the next functions

	balls.add(App->physics->CreateBall(663,400,9));
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

void ModuleSceneIntro::ResetBallPos(PhysBody* ball, int x, int y)
{
	b2Vec2 ballpos;
	ballpos.x = PIXEL_TO_METERS(x);
	ballpos.y = PIXEL_TO_METERS(y);
	ball->body->SetTransform(ballpos, 0.0f);
	ball->body->SetLinearVelocity(b2Vec2(0, 0));
}