#include "Checkpoints.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Module.h"

Checkpoints::Checkpoints() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoints::~Checkpoints() {}

bool Checkpoints::Awake() {

	position.x = config.attribute("x").as_int();
	position.y = config.attribute("y").as_int();
	texturePath = config.attribute("texturepath").as_string();

	return true;
}

bool Checkpoints::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 20, 100, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::CHECKPOINT;

	return true;
}

bool Checkpoints::Update(float dt)
{
	texW = 37;
	texH = 29;

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Checkpoints::CleanUp()
{
	return true;
}

void Checkpoints::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Checkpoint Collision PLAYER");
		isPicked = true;
		break;
	}
}

int Checkpoints::GetPositionX() const
{
	return position.x;
}

int Checkpoints::GetPositionY() const
{
	return position.y;
}

bool Checkpoints::GetPicked() const
{
	return isPicked;
}

void Checkpoints::SetPositionX(int x)
{
	position.x = x;
}

void Checkpoints::SetPositionY(int y)
{
	position.y = y;
}

void Checkpoints::SetPicked(bool picked)
{
	isPicked = picked;
}