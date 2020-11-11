//#include "Globals.h"
//#include "Application.h"
//#include "ModuleInput.h"
//#include "ModuleRender.h"
//#include "ModulePhysics.h"
//#include "p2Point.h"
//#include "math.h"
//
//#ifdef _DEBUG
//#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
//#else
//#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
//#endif
//
//ModulePhysics::ModulePhysics(Application* app, bool start_enabled) : Module(app, start_enabled)
//{
//	world = NULL;
//	mouse_joint = NULL;
//	debug = true;
//}
//
//// Destructor
//ModulePhysics::~ModulePhysics()
//{
//}
//
//bool ModulePhysics::Start()
//{
//	LOG("Creating Physics 2D environment");
//
//	world = new b2World(b2Vec2(GRAVITY_X, -GRAVITY_Y));
//	world->SetContactListener(this);
//
//	b2BodyDef bd;
//	ground = world->CreateBody(&bd);
//
//	return true;
//}
//
//// 
//update_status ModulePhysics::PreUpdate()
//{
//	world->Step(1.0f / 60.0f, 6, 2);
//
//	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext())
//	{
//		if (c->GetFixtureA()->IsSensor() && c->IsTouching())
//		{
//			PhysBody* pb1 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
//			PhysBody* pb2 = (PhysBody*)c->GetFixtureA()->GetBody()->GetUserData();
//			if (pb1 && pb2 && pb1->listener)
//				pb1->listener->OnCollision(pb1, pb2);
//		}
//	}
//
//	return UPDATE_CONTINUE;
//}
//
//PhysBody* ModulePhysics::CreateCircle(int x, int y, int radius)
//{
//	b2BodyDef body;
//	body.type = b2_dynamicBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	b2Body* b = world->CreateBody(&body);
//
//	b2CircleShape shape;
//	shape.m_radius = PIXEL_TO_METERS(radius);
//	b2FixtureDef fixture;
//	fixture.shape = &shape;
//	fixture.density = 1.0f;
//
//	b->CreateFixture(&fixture);
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = pbody->height = radius;
//
//	return pbody;
//}
//
//PhysBody* ModulePhysics::CreateRectangle(int x, int y, int width, int height)
//{
//	b2BodyDef body;
//	body.type = b2_dynamicBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	b2Body* b = world->CreateBody(&body);
//	b2PolygonShape box;
//	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
//
//	b2FixtureDef fixture;
//	fixture.shape = &box;
//	fixture.density = 1.0f;
//
//	b->CreateFixture(&fixture);
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = width * 0.5f;
//	pbody->height = height * 0.5f;
//
//	return pbody;
//}
//
//PhysBody* ModulePhysics::CreateRectangleSensor(int x, int y, int width, int height)
//{
//	b2BodyDef body;
//	body.type = b2_staticBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	b2Body* b = world->CreateBody(&body);
//
//	b2PolygonShape box;
//	box.SetAsBox(PIXEL_TO_METERS(width) * 0.5f, PIXEL_TO_METERS(height) * 0.5f);
//
//	b2FixtureDef fixture;
//	fixture.shape = &box;
//	fixture.density = 1.0f;
//	fixture.isSensor = true;
//
//	b->CreateFixture(&fixture);
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = width;
//	pbody->height = height;
//
//	return pbody;
//}
//
//PhysBody* ModulePhysics::CreateChain(int x, int y, int* points, int size)
//{
//	b2BodyDef body;
//	body.type = b2_dynamicBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	b2Body* b = world->CreateBody(&body);
//
//	b2ChainShape shape;
//	b2Vec2* p = new b2Vec2[size / 2];
//
//	for (uint i = 0; i < size / 2; ++i)
//	{
//		p[i].x = PIXEL_TO_METERS(points[i * 2 + 0]);
//		p[i].y = PIXEL_TO_METERS(points[i * 2 + 1]);
//	}
//
//	shape.CreateLoop(p, size / 2);
//
//	b2FixtureDef fixture;
//	fixture.shape = &shape;
//
//	b->CreateFixture(&fixture);
//
//	delete p;
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = pbody->height = 0;
//
//	return pbody;
//}
//
//PhysBody* ModulePhysics::CreateLeftTrigger()
//{
//	b2BodyDef triggerBodyDef;
//	triggerBodyDef.type = b2_dynamicBody;
//	triggerBodyDef.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
//
//	b2Body* triggerBody = world->CreateBody(&triggerBodyDef);
//
//	b2PolygonShape triggerShape;
//
//	int rightTriggerCoord[16] = {
//	538, 480,
//	501, 501,
//	501, 508,
//	508, 510,
//	548, 494,
//	552, 487,
//	547, 479,
//	538, 480
//	};
//
//	b2Vec2 rightTriggerVec[8];
//	for (int i = 0; i < 8; i++)
//	{
//		rightTriggerVec[i].x = PIXEL_TO_METERS(rightTriggerCoord[i * 2]);
//		rightTriggerVec[i].y = PIXEL_TO_METERS(rightTriggerCoord[i * 2 + 1]);
//	}
//
//	triggerShape.Set(rightTriggerVec, 8);
//
//	b2FixtureDef triggerFixtureDef;
//	triggerFixtureDef.shape = &triggerShape;
//	triggerFixtureDef.density = 1;
//	//triggerFixtureDef.filter.groupIndex = groupIndex::RIGID_PINBALL;
//	triggerBody->CreateFixture(&triggerFixtureDef);
//
//	b2Vec2 centerRectangle = triggerBody->GetWorldCenter();
//	centerRectangle += (b2Vec2(PIXEL_TO_METERS(-18), 0));
//
//	b2BodyDef pivotBodyDef;
//	pivotBodyDef.type = b2_staticBody;
//	pivotBodyDef.position.Set(centerRectangle.x, centerRectangle.y);
//
//	b2Body* pivotBody = world->CreateBody(&pivotBodyDef);
//
//	b2CircleShape pivotCircle;
//	pivotCircle.m_radius = PIXEL_TO_METERS(0.5f);
//	b2FixtureDef pivotFixtureDef;
//	pivotFixtureDef.shape = &pivotCircle;
//	//pivotFixtureDef.filter.groupIndex = groupIndex::RIGID_PINBALL;
//	pivotBody->CreateFixture(&pivotFixtureDef);
//
//	b2RevoluteJointDef revJointDef;
//	revJointDef.Initialize(triggerBody, pivotBody, centerRectangle);
//	revJointDef.upperAngle = 0.6f;
//	revJointDef.lowerAngle = -0.6f;
//	revJointDef.enableLimit = true;
//	revJointDef.maxMotorTorque = 10.0f;
//	revJointDef.motorSpeed = 0.0f;
//	revJointDef.enableMotor = true;
//	b2Joint* joint = world->CreateJoint(&revJointDef);
//
//	PhysBody* leftTrigger = new PhysBody();
//	leftTrigger->body = triggerBody;
//	leftTrigger->body2 = pivotBody;
//	leftTrigger->joint = joint;
//	triggerBody->SetUserData(leftTrigger);
//
//	return leftTrigger;
//}
//
//PhysBody* ModulePhysics::CreateRightTrigger()
//{
//	b2BodyDef triggerBodyDef;
//	triggerBodyDef.type = b2_dynamicBody;
//	triggerBodyDef.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
//
//	b2Body* triggerBody = world->CreateBody(&triggerBodyDef);
//
//	b2PolygonShape triggerShape;
//
//	int rightTriggerCoord[16] = {
//	538, 480,
//	501, 501,
//	501, 508,
//	508, 510,
//	548, 494,
//	552, 487,
//	547, 479,
//	538, 480
//	};
//
//	b2Vec2 rightTriggerVec[8];
//	for (int i = 0; i < 8; i++)
//	{
//		rightTriggerVec[i].x = PIXEL_TO_METERS(rightTriggerCoord[i * 2]);
//		rightTriggerVec[i].y = PIXEL_TO_METERS(rightTriggerCoord[i * 2 + 1]);
//	}
//
//	triggerShape.Set(rightTriggerVec, 8);
//
//	b2FixtureDef triggerFixtureDef;
//	triggerFixtureDef.shape = &triggerShape;
//	triggerFixtureDef.density = 1;
//	//triggerFixtureDef.filter.groupIndex = groupIndex::RIGID_PINBALL;
//	triggerBody->CreateFixture(&triggerFixtureDef);
//
//	b2Vec2 centerRectangle = triggerBody->GetWorldCenter();
//	centerRectangle += (b2Vec2(PIXEL_TO_METERS(18), 0));
//
//	b2BodyDef pivotBodyDef;
//	pivotBodyDef.type = b2_staticBody;
//	pivotBodyDef.position.Set(centerRectangle.x, centerRectangle.y);
//
//	b2Body* pivotBody = world->CreateBody(&pivotBodyDef);
//
//	b2CircleShape pivotCircle;
//	pivotCircle.m_radius = PIXEL_TO_METERS(0.5f);
//	b2FixtureDef pivotFixtureDef;
//	pivotFixtureDef.shape = &pivotCircle;
//	//pivotFixtureDef.filter.groupIndex = groupIndex::RIGID_PINBALL;
//	pivotBody->CreateFixture(&pivotFixtureDef);
//
//	b2RevoluteJointDef revJointDef;
//	revJointDef.Initialize(triggerBody, pivotBody, centerRectangle);
//	revJointDef.upperAngle = 0.6f;
//	revJointDef.lowerAngle = -0.6f;
//	revJointDef.enableLimit = true;
//	revJointDef.maxMotorTorque = 10.0f;
//	revJointDef.motorSpeed = 0.0f;
//	revJointDef.enableMotor = true;
//	b2Joint* joint = world->CreateJoint(&revJointDef);
//
//	PhysBody* rightTrigger = new PhysBody();
//	rightTrigger->body = triggerBody;
//	rightTrigger->body2 = pivotBody;
//	rightTrigger->joint = joint;
//	triggerBody->SetUserData(rightTrigger);
//
//	return rightTrigger;
//}
//// 
//update_status ModulePhysics::PostUpdate()
//{
//	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
//		debug = !debug;
//
//	if (!debug)
//		return UPDATE_CONTINUE;
//
//	// Bonus code: this will iterate all objects in the world and draw the circles
//	// You need to provide your own macro to translate meters to pixels
//	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
//	{
//		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
//		{
//			switch (f->GetType())
//			{
//				// Draw circles ------------------------------------------------
//			case b2Shape::e_circle:
//			{
//				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
//				b2Vec2 pos = f->GetBody()->GetPosition();
//				App->renderer->DrawCircle(METERS_TO_PIXELS(pos.x), METERS_TO_PIXELS(pos.y), METERS_TO_PIXELS(shape->m_radius), 255, 255, 255);
//			}
//			break;
//
//			// Draw polygons ------------------------------------------------
//			case b2Shape::e_polygon:
//			{
//				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
//				int32 count = polygonShape->GetVertexCount();
//				b2Vec2 prev, v;
//
//				for (int32 i = 0; i < count; ++i)
//				{
//					v = b->GetWorldPoint(polygonShape->GetVertex(i));
//					if (i > 0)
//						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
//
//					prev = v;
//				}
//
//				v = b->GetWorldPoint(polygonShape->GetVertex(0));
//				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
//			}
//			break;
//
//			// Draw chains contour -------------------------------------------
//			case b2Shape::e_chain:
//			{
//				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
//				b2Vec2 prev, v;
//
//				for (int32 i = 0; i < shape->m_count; ++i)
//				{
//					v = b->GetWorldPoint(shape->m_vertices[i]);
//					if (i > 0)
//						App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
//					prev = v;
//				}
//
//				v = b->GetWorldPoint(shape->m_vertices[0]);
//				App->renderer->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
//			}
//			break;
//
//			// Draw a single segment(edge) ----------------------------------
//			case b2Shape::e_edge:
//			{
//				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
//				b2Vec2 v1, v2;
//
//				v1 = b->GetWorldPoint(shape->m_vertex0);
//				v1 = b->GetWorldPoint(shape->m_vertex1);
//				App->renderer->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
//			}
//			break;
//			}
//
//			// TODO 1: If mouse button 1 is pressed ...
//			// App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
//			// test if the current body contains mouse position
//		}
//	}
//
//	// If a body was selected we will attach a mouse joint to it
//	// so we can pull it around
//	// TODO 2: If a body was selected, create a mouse joint
//	// using mouse_joint class property
//
//
//	// TODO 3: If the player keeps pressing the mouse button, update
//	// target position and draw a red line between both anchor points
//
//	// TODO 4: If the player releases the mouse button, destroy the joint
//
//	return UPDATE_CONTINUE;
//}
//
//
//// Called before quitting
//bool ModulePhysics::CleanUp()
//{
//	LOG("Destroying physics world");
//
//	// Delete the whole physics world!
//	delete world;
//
//	return true;
//}
//
//void PhysBody::GetPosition(int& x, int& y) const
//{
//	b2Vec2 pos = body->GetPosition();
//	x = METERS_TO_PIXELS(pos.x) - (width);
//	y = METERS_TO_PIXELS(pos.y) - (height);
//}
//
//float PhysBody::GetRotation() const
//{
//	return RADTODEG * body->GetAngle();
//}
//
//bool PhysBody::Contains(int x, int y) const
//{
//	b2Vec2 p(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//
//	const b2Fixture* fixture = body->GetFixtureList();
//
//	while (fixture != NULL)
//	{
//		if (fixture->GetShape()->TestPoint(body->GetTransform(), p) == true)
//			return true;
//		fixture = fixture->GetNext();
//	}
//
//	return false;
//}
//
//int PhysBody::RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const
//{
//	int ret = -1;
//
//	b2RayCastInput input;
//	b2RayCastOutput output;
//
//	input.p1.Set(PIXEL_TO_METERS(x1), PIXEL_TO_METERS(y1));
//	input.p2.Set(PIXEL_TO_METERS(x2), PIXEL_TO_METERS(y2));
//	input.maxFraction = 1.0f;
//
//	const b2Fixture* fixture = body->GetFixtureList();
//
//	while (fixture != NULL)
//	{
//		if (fixture->GetShape()->RayCast(&output, input, body->GetTransform(), 0) == true)
//		{
//			// do we want the normal ?
//
//			float fx = x2 - x1;
//			float fy = y2 - y1;
//			float dist = sqrtf((fx * fx) + (fy * fy));
//
//			normal_x = output.normal.x;
//			normal_y = output.normal.y;
//
//			return output.fraction * dist;
//		}
//		fixture = fixture->GetNext();
//	}
//
//	return ret;
//}
//
//void ModulePhysics::BeginContact(b2Contact* contact)
//{
//	PhysBody* physA = (PhysBody*)contact->GetFixtureA()->GetBody()->GetUserData();
//	PhysBody* physB = (PhysBody*)contact->GetFixtureB()->GetBody()->GetUserData();
//
//	if (physA && physA->listener != NULL)
//		physA->listener->OnCollision(physA, physB);
//
//	if (physB && physB->listener != NULL)
//		physB->listener->OnCollision(physB, physA);
//}
//
//PhysBody* ModulePhysics::CreateBall(int x, int y)
//{
//	int ballRadius = 13;
//	b2BodyDef body;
//	body.type = b2_dynamicBody;
//	body.position.Set(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y));
//	body.bullet = true;
//
//	b2Body* b = world->CreateBody(&body);
//
//	b2CircleShape shape;
//	shape.m_radius = PIXEL_TO_METERS(ballRadius);
//	b2FixtureDef fixture;
//	fixture.shape = &shape;
//	fixture.density = 5.0f;
//	fixture.friction = 0.0f;
//	fixture.restitution = 0.3f;
//	fixture.filter.groupIndex = BODY_INDEX::BALL;
//
//	fixture.density = 1.0f;
//
//	b->CreateFixture(&fixture);
//
//	PhysBody* pbody = new PhysBody();
//	pbody->body = b;
//	b->SetUserData(pbody);
//	pbody->width = pbody->height = ballRadius;
//	pbody->bodyType = _BALL;
//
//	return pbody;
//}