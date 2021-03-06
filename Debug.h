#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"

#include "SDL/include/SDL_scancode.h"

class ModuleDebug : public Module
{
public:
	ModuleDebug(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleDebug();

	//Called at the beginning of the application execution
	bool Awake();

	//Called when the module is activated
	//By now we will consider all modules to be permanently active
	bool Start();

	//Called at the beginning of each application loop
	update_status PreUpdate();

	//Called at the middle of each application loop
	update_status Update();

	//Called at the end of each application loop
	update_status PostUpdate();

	//Called at the end of the application
	bool CleanUp();

	void ToggleColliders();
	

private:
	bool showColliders = false;

};

#endif //__DEBUG_H__
