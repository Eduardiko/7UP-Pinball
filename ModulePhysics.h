#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies

enum BODY_TYPE
{
	WALL_TOP = -1,
	PLUNGE = -1,
	BALL_FLOOR = -1,


	BALL_TOP = -2,
	MINI_PLUNGE = -2,
	WALL_FLOOR = -2,
	TRIGGER = -2,
	BUMPER = -2,


	NOT_DEFINED
};

enum SENSOR_TYPE
{
	_BALL,
	_WALL,
	_TRIGGER,
	_BUMPER,
	_PLUNGE,
	_DEAD_SENSOR,
	_REBOUNCER1,
	_REBOUNCER2,
	_REBOUNCER3,
	_MINI_PLUNGE_L,
	_MINI_PLUNGE_R,
	_CATAPULT,
	_FUNNEL,

	_LEVEL_CHANGE,
	_TOP_LEVEL,
	_FLOOR_LEVEL,

	_NOT_DEFINED
};

class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;

	b2Body* body;
	b2Body* body2;
	b2Joint* joint;
	Module* listener;



	SENSOR_TYPE sensorType = _NOT_DEFINED;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();


	PhysBody* CreateCircle(int x, int y, int radius);
	PhysBody* CreatePlunge(int x, int y);
	PhysBody* CreateMiniPlunge(int x, int y);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, BODY_TYPE type);
	PhysBody* CreateBall(int x, int y, int radius);
	PhysBody* CreateLeftTrigger();
	PhysBody* CreateRightTrigger();
	PhysBody* CreateBumperChain(int x, int y, int* points, int size, BODY_TYPE type);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, SENSOR_TYPE sensorType);
	PhysBody* CreateCircleBumper(int x, int y, int radius, SENSOR_TYPE sensorType);

	void BeginContact(b2Contact* contact);

	b2World* world;


private:
	bool debug = false;
	b2MouseJoint* mouse_joint;
	b2Body* ground;




	
};