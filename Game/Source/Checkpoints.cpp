#include "Checkpoints.h"
#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Scene.h"
#include "Point.h"

Checkpoints::Checkpoints() : Entity(EntityType::CHECKPOINT)
{
	name.Create("Checkpoint");
}

Checkpoints::~Checkpoints(){}

bool Checkpoints::Awake()
{
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());
	id = config.attribute("id").as_int();

	return true;
}

bool Checkpoints::Start()
{
	texture = app->tex->Load(config.attribute("texturePath").as_string());
	pbody = app->physics->CreateRectangle(position.x, position.y, size.x, size.y, bodyType::STATIC);
	pbody->ctype = ColliderType::CHECKPOINT;
	pbody->listener = this;

	return true;
}

bool Checkpoints::Update(float dt)
{
	position.x = METERS_TO_PIXELS(pbody->body->GetPosition().x)-16;
	position.y = METERS_TO_PIXELS(pbody->body->GetPosition().y)-16;

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Checkpoints::CleanUp()
{
	app->tex->UnLoad(texture);

	return true;
}

void Checkpoints::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physA->ctype == ColliderType::PLAYER)
	{
	}
}