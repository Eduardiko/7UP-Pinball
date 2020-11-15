#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModulePhysics.h"
#include "p2Point.h"
#include "math.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	world = NULL;
	mouse_joint = NULL;
	debug = false;
}

// Destructor
ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Start()
{
	LOG("Creating Physics 2D environment");

	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
	world->SetContactListener(this);

	b2BodyDef bd;
	ground = world->CreateBody(&bd);

	return true;
}

// 
update_status ModulePhysics::PreUpdate()
{
	world->Step(1.0f / 60.0f, 6, 2);

	for(b2Contact* c = world->GetContactList(); c; c = c->GetNext())
	{
		if(c->GetFixtureA()->IsSensor() && c->IsTouching())
		{
			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
			if(pb1 && pb2 && pb1->listener)
				pb1->listener->OnCollision(pb1, pb2);
		}
	}

	return UPDATE_CONTINUE;
}

PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width * 0.5f;
	pbody->height = height * 0.5f;

	return pbody;
}

PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height, SENSOR_TYPE sensorType)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);

	b2FixtureDef fixture;
	fixture.shape = &box;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = width;
	pbody->height = height;
	pbody->sensorType = sensorType;

	return pbody;
}

PhysBody* ModulePhysics::CreateCircleBumper(int x, int y, int radius, SENSOR_TYPE sensorType)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.restitution = 1.2f;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	pbody->sensorType = sensorType;

	return pbody;
}


PhysBody* ModulePhysics::CreateBumperChainSensor(int x, int y, int* points, int size, SENSOR_TYPE sensorType)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateChain(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.isSensor = true;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);


	pbody->sensorType = sensorType;


	pbody->width = pbody->height = 0;

	return pbody;
}


PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size, BODY_TYPE type)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for(uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateChain(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.filter.groupIndex = type;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}


PhysBody* ModulePhysics::CreateBumperChain(int x, int y, int* points, int size, BODY_TYPE type)
{
	b2BodyDef body;
	body.type = b2_staticBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* b = world->CreateBody(&body);

	b2ChainShape shape;
	b2Vec2* p = new b2Vec2[size / 2];

	for (uint i = 0; i < size / 2; ++i)
	{
		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
	}

	shape.CreateChain(p, size / 2);

	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.restitution = 1.0f;
	fixture.filter.groupIndex = type;

	b->CreateFixture(&fixture);

	delete p;

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = 0;

	return pbody;
}

PhysBody* ModulePhysics::CreateLeftTrigger()
{
	b2BodyDef triggerBodyDef;
	triggerBodyDef.type = b2_dynamicBody;
	triggerBodyDef.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));

	b2Body* triggerBody = world->CreateBody(&triggerBodyDef);

	b2PolygonShape triggerShape;

	int leftTriggerCoord[12] = {
	426, 477,
	419, 484,
	426, 494,
	473, 489,
	473, 481,
	426, 477,
	};

	b2Vec2 leftTriggerVec[6];
	for (int i = 0; i < 6; i++)
	{
		leftTriggerVec[i].x = PIXEL_TO_METERS(leftTriggerCoord[i * 2]);
		leftTriggerVec[i].y = PIXEL_TO_METERS(leftTriggerCoord[i * 2 + 1]);
	}

	triggerShape.Set(leftTriggerVec, 6);

	b2FixtureDef triggerFixtureDef;
	triggerFixtureDef.shape = &triggerShape;
	triggerFixtureDef.density = 1;
	triggerFixtureDef.filter.groupIndex = BODY_TYPE::TRIGGER;
	triggerBody->CreateFixture(&triggerFixtureDef);

	b2Vec2 centerRectangle = triggerBody->GetWorldCenter();
	centerRectangle += (b2Vec2(PIXEL_TO_METERS(-18), 0));

	b2BodyDef pivotBodyDef;
	pivotBodyDef.type = b2_staticBody;
	pivotBodyDef.position.Set(centerRectangle.x,centerRectangle.y);

	b2Body* pivotBody = world->CreateBody(&pivotBodyDef);

	b2CircleShape pivotCircle;
	pivotCircle.m_radius = PIXEL_TO_METERS(0.5f);
	b2FixtureDef pivotFixtureDef;
	pivotFixtureDef.shape = &pivotCircle;
	pivotFixtureDef.filter.groupIndex = BODY_TYPE::TRIGGER;
	pivotBody->CreateFixture(&pivotFixtureDef);

	b2RevoluteJointDef revJointDef;
	revJointDef.Initialize(triggerBody, pivotBody, centerRectangle);
	revJointDef.upperAngle = 0.5f;
	revJointDef.lowerAngle = -0.5f;
	revJointDef.enableLimit = true;
	revJointDef.maxMotorTorque = 15.0f;
	revJointDef.motorSpeed = 0.0f;
	revJointDef.enableMotor = true;
	b2Joint* joint = world->CreateJoint(&revJointDef);

	PhysBody* leftTrigger = new PhysBody();
	leftTrigger->body = triggerBody;
	leftTrigger->body2 = pivotBody;
	leftTrigger->joint = joint;
	triggerBody->SetUserData(leftTrigger);

	return leftTrigger;
}

PhysBody* ModulePhysics::CreateRightTrigger()
{
	b2BodyDef triggerBodyDef;
	triggerBodyDef.type = b2_dynamicBody;
	triggerBodyDef.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));

	b2Body* triggerBody = world->CreateBody(&triggerBodyDef);

	b2PolygonShape triggerShape;

	int rightTriggerCoord[12] = {
	543, 477,
	548, 484,
	543, 494,
	495, 490,
	495, 480,
	543, 477
	};

	b2Vec2 rightTriggerVec[6];
	for (int i = 0; i < 6; i++)
	{
		rightTriggerVec[i].x = PIXEL_TO_METERS(rightTriggerCoord[i * 2]);
		rightTriggerVec[i].y = PIXEL_TO_METERS(rightTriggerCoord[i * 2 + 1]);
	}

	triggerShape.Set(rightTriggerVec, 6);

	b2FixtureDef triggerFixtureDef;
	triggerFixtureDef.shape = &triggerShape;
	triggerFixtureDef.density = 1;
	triggerFixtureDef.filter.groupIndex = BODY_TYPE::TRIGGER;
	triggerBody->CreateFixture(&triggerFixtureDef);

	b2Vec2 centerRectangle = triggerBody->GetWorldCenter();
	centerRectangle += (b2Vec2(PIXEL_TO_METERS(18), 0));

	b2BodyDef pivotBodyDef;
	pivotBodyDef.type = b2_staticBody;
	pivotBodyDef.position.Set(centerRectangle.x, centerRectangle.y);

	b2Body* pivotBody = world->CreateBody(&pivotBodyDef);

	b2CircleShape pivotCircle;
	pivotCircle.m_radius = PIXEL_TO_METERS(0.5f);
	b2FixtureDef pivotFixtureDef;
	pivotFixtureDef.shape = &pivotCircle;
	pivotFixtureDef.filter.groupIndex = BODY_TYPE::TRIGGER;
	pivotBody->CreateFixture(&pivotFixtureDef);

	b2RevoluteJointDef revJointDef;
	revJointDef.Initialize(triggerBody, pivotBody, centerRectangle);
	revJointDef.upperAngle = 0.5f;
	revJointDef.lowerAngle = -0.5f;
	revJointDef.enableLimit = true;
	revJointDef.maxMotorTorque = 15.0f;
	revJointDef.motorSpeed = 0.0f;
	revJointDef.enableMotor = true;
	b2Joint* joint = world->CreateJoint(&revJointDef);

	PhysBody* rightTrigger = new PhysBody();
	rightTrigger->body = triggerBody;
	rightTrigger->body2 = pivotBody;
	rightTrigger->joint = joint;
	triggerBody->SetUserData(rightTrigger);

	return rightTrigger;
}
// 

PhysBody* ModulePhysics::CreateBall(int x, int y, int radius)
{
	b2BodyDef body;
	body.type = b2_dynamicBody;
	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
	body.bullet = true;

	b2Body* b = world->CreateBody(&body);

	b2CircleShape shape;
	shape.m_radius = PIXEL_TO_METERS(radius);
	b2FixtureDef fixture;
	fixture.shape = &shape;
	fixture.density = 1.0f;
	fixture.friction = 0.0f;
	fixture.restitution = 0.3f;
	fixture.filter.groupIndex = BODY_TYPE::BALL_TOP;

	b->CreateFixture(&fixture);

	PhysBody* pbody = new PhysBody();
	pbody->body = b;
	b->SetUserData(pbody);
	pbody->width = pbody->height = radius;
	pbody->sensorType = _BALL;

	return pbody;
}

PhysBody* ModulePhysics::CreatePlunge(int x, int y)
{
	
	b2BodyDef plungeBodyDef;
	plungeBodyDef.type = b2_dynamicBody;
	plungeBodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* plungeBody = world->CreateBody(&plungeBodyDef);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(20* 0.5f), PIXEL_TO_METERS(11 * 0.5f));

	b2FixtureDef plungeFixture;
	plungeFixture.shape = &box;
	plungeFixture.density = 20.0f;
	plungeFixture.restitution = 0.1f;
	plungeFixture.filter.groupIndex = BODY_TYPE::PLUNGE;

	plungeBody->CreateFixture(&plungeFixture);

	b2BodyDef plungeBodyBDef;
	plungeBodyBDef.type = b2_staticBody;
	int z = y + 30;
	plungeBodyBDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(z));

	b2Body* plungeBodyB = world->CreateBody(&plungeBodyBDef);
	b2PolygonShape box1;
	box1.SetAsBox(PIXEL_TO_METERS(20 * 0.5f), PIXEL_TO_METERS(11 * 0.5f));

	b2FixtureDef plungeFixture2;
	plungeFixture2.shape = &box1;
	plungeFixture2.density = 1.0f;
	plungeFixture2.filter.groupIndex = BODY_TYPE::PLUNGE;

	plungeBodyB->CreateFixture(&plungeFixture2);

	b2PrismaticJointDef jointDef;
	jointDef.bodyA = plungeBodyB;
	jointDef.bodyB = plungeBody;
	jointDef.collideConnected = true;

	jointDef.localAxisA.Set(0, 1);
	jointDef.localAxisA.Normalize();
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);

	jointDef.lowerTranslation = -1.0f;
	jointDef.upperTranslation = 1.0f;
	jointDef.enableLimit = true;
	jointDef.maxMotorForce = 200.0f;
	jointDef.motorSpeed = -200.0f;
	jointDef.enableMotor = true;
	world->CreateJoint(&jointDef);

	PhysBody* pbody = new PhysBody();
	pbody->body = plungeBody;
	pbody->body2 = plungeBodyB;
	plungeBody->SetUserData(pbody);

	return pbody;
}

PhysBody* ModulePhysics::CreateMiniPlunge(int x, int y)
{

	b2BodyDef plungeBodyDef;
	plungeBodyDef.type = b2_dynamicBody;
	plungeBodyDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	b2Body* plungeBody = world->CreateBody(&plungeBodyDef);
	b2PolygonShape box;
	box.SetAsBox(PIXEL_TO_METERS(20 * 0.5f), PIXEL_TO_METERS(11 * 0.5f));

	b2FixtureDef plungeFixture;
	plungeFixture.shape = &box;
	plungeFixture.density = 20.0f;
	plungeFixture.restitution = 4.0f;
	plungeFixture.filter.groupIndex = BODY_TYPE::MINI_PLUNGE;

	plungeBody->CreateFixture(&plungeFixture);

	b2BodyDef plungeBodyBDef;
	plungeBodyBDef.type = b2_staticBody;
	int z = y + 50;
	plungeBodyBDef.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(z));

	b2Body* plungeBodyB = world->CreateBody(&plungeBodyBDef);
	b2PolygonShape box1;
	box1.SetAsBox(PIXEL_TO_METERS(20 * 0.5f), PIXEL_TO_METERS(11 * 0.5f));

	b2FixtureDef plungeFixture2;
	plungeFixture2.shape = &box1;
	plungeFixture2.density = 1.0f;
	plungeFixture2.filter.groupIndex = BODY_TYPE::MINI_PLUNGE;

	plungeBodyB->CreateFixture(&plungeFixture2);

	b2PrismaticJointDef jointDef;
	jointDef.bodyA = plungeBodyB;
	jointDef.bodyB = plungeBody;
	jointDef.collideConnected = true;

	jointDef.localAxisA.Set(0, 1);
	jointDef.localAxisA.Normalize();
	jointDef.localAnchorA.Set(0, 0);
	jointDef.localAnchorB.Set(0, 0);

	jointDef.lowerTranslation = -1.0f;
	jointDef.upperTranslation = 1.0f;
	jointDef.enableLimit = true;
	jointDef.maxMotorForce = 200.0f;
	jointDef.motorSpeed = -200.0f;
	jointDef.enableMotor = true;
	world->CreateJoint(&jointDef);

	PhysBody* pbody = new PhysBody();
	pbody->body = plungeBody;
	pbody->body2 = plungeBodyB;
	plungeBody->SetUserData(pbody);

	return pbody;
}

update_status ModulePhysics::PostUpdate()
{
	if(App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		debug = !debug;

	if(!debug)
		return UPDATE_CONTINUE;

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels

	for(b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for(b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
				switch (f->GetType())
				{
					// Draw circles ------------------------------------------------
				case b2Shape::e_circle:
				{
					b2CircleShape* shape = (b2CircleShape*)f->GetShape();
					b2Vec2 pos = f->GetBody()->GetPosition();
					App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
				}
				break;

				// Draw polygons ------------------------------------------------
				case b2Shape::e_polygon:
				{
					b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
					int32 count = polygonShape->GetVertexCount();
					b2Vec2 prev, v;

					for (int32 i = 0; i < count; ++i)
					{
						v = b->GetWorldPoint(polygonShape->GetVertex(i));
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);

						prev = v;
					}

					v = b->GetWorldPoint(polygonShape->GetVertex(0));
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
				}
				break;

				// Draw chains contour -------------------------------------------
				case b2Shape::e_chain:
				{
					b2ChainShape* shape = (b2ChainShape*)f->GetShape();
					b2Vec2 prev, v;

					for (int32 i = 0; i < shape->m_count; ++i)
					{
						v = b->GetWorldPoint(shape->m_vertices[i]);
						if (i > 0)
							App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
						prev = v;
					}

					v = b->GetWorldPoint(shape->m_vertices[0]);
					App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
				}
				break;

				// Draw a single segment(edge) ----------------------------------
				case b2Shape::e_edge:
				{
					b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
					b2Vec2 v1, v2;

					v1 = b->GetWorldPoint(shape->m_vertex0);
					v1 = b->GetWorldPoint(shape->m_vertex1);
					App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
				}
				break;
				}
			// TODO 1: If mouse button 1 is pressed ...
			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
	}

	// If a body was selected we will attach a mouse joint to it
	// so we can pull it around
	// TODO 2: If a body was selected, create a mouse joint
	// using mouse_joint class property


	// TODO 3: If the player keeps pressing the mouse button, update
	// target position and draw a red line between both anchor points

	// TODO 4: If the player releases the mouse button, destroy the joint

	return UPDATE_CONTINUE;
}


// Called before quitting
bool ModulePhysics::CleanUp()
{
	LOG("Destroying physics world");

	// Delete the whole physics world!
	delete world;

	return true;
}

void PhysBody::GetPosition(int& x, int &y) const
{
	b2Vec2 pos = body->GetPosition();
	x = METERS_TO_PIXELS(pos.x) - (width);
	y = METERS_TO_PIXELS(pos.y) - (height);
}

float PhysBody::GetRotation() const
{
	return RADTODEG * body->GetAngle();
}

bool PhysBody::Contains(int x, int y) const
{
	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
			return true;
		fixture = fixture->GetNext();
	}

	return false;
}

int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
{
	int ret = -1;

	b2RayCastInput input;
	b2RayCastOutput output;

	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
	input.maxFraction = 1.0f;

	const b2Fixture* fixture = body->GetFixtureList();

	while(fixture != NULL)
	{
		if(fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
		{
			// do we want the normal ?

			float fx = x2 - x1;
			float fy = y2 - y1;
			float dist = sqrtf((fx*fx) + (fy*fy));

			normal_x = output.normal.x;
			normal_y = output.normal.y;

			return output.fraction * dist;
		}
		fixture = fixture->GetNext();
	}

	return ret;
}

void ModulePhysics::BeginContact(b2Contact* contact)
{
	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();

	/*if ((physB->top && physA->sensorType == _TOP_LEVEL) || (!physB->top && physA->sensorType == _FLOOR_LEVEL))
	{*/
		if (physA && physA->listener != NULL)
			physA->listener->OnCollision(physA, physB);

		if (physB && physB->listener != NULL)
			physB->listener->OnCollision(physB, physA);
	//}

}
