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

}

ModuleUI::~ModuleUI()
{}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading ModuleUI assets");
	bool ret = true;

	char lookupTable[] = { "0123456789" };
	font = Load("pinball/font.png", lookupTable, 1);

	return ret;
}

// Update: draw background
update_status ModuleUI::Update()
{
	update_status status = UPDATE_CONTINUE;
	
	halfScoreRight = 123456;
	char scoreTextRight[DYNAMIC_TEXT_LEN + 1];
	IntToString(scoreTextRight, halfScoreRight);
	RenderDynamicText(scoreTextRight, 491, 29, font, true);

	halfScoreLeft = 123456;
	char scoreTextLeft[DYNAMIC_TEXT_LEN + 1];
	IntToString(scoreTextLeft, halfScoreLeft);
	RenderDynamicText(scoreTextLeft, 347, 29, font, true);

	return status;
}

bool ModuleUI::CleanUp()
{
	LOG("Unloading ModuleUI");

	bool ret = true;

	UnLoad(font);

	return ret;
}


// Load new texture from file path
int ModuleUI::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w = 130;
	uint tex_h = 27;
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;

	LOG("Successfully loaded BMP font from %s", texture_path);

	k++;

	return id;
}

void ModuleUI::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		App->textures->Unload(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
	k--;
}

void ModuleUI::BlitText(int x, int y, int font_id, const char* text) const
{
	LOG("printing");

	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		App->renderer->Blit(font->texture, x + (i * 11), y, &spriteRect, 0.0f, false);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}

void ModuleUI::IntToString(char* buffer, int k) {

	for (int i = 0; i < DYNAMIC_TEXT_LEN; i++) {
		buffer[i] = '0';
	}

	buffer[DYNAMIC_TEXT_LEN] = 0;

	int i = DYNAMIC_TEXT_LEN - 1;
	while (k != 0) {
		if (i < 0) break;
		buffer[i--] += k % 10;
		k /= 10;
	}
}

void ModuleUI::RenderDynamicText(char* text, int x, int y, int fontIndex, bool inverse) {
	int i = 0;

	for (; i < DYNAMIC_TEXT_LEN; i++) {
		if (text[i] != '0') break;
	}
	if (i == DYNAMIC_TEXT_LEN) {
		BlitText(x - (i * 12), y, fontIndex, "0");
	}
	else {
		BlitText(x - (i * 12), y, fontIndex, text + i);
	}
}
