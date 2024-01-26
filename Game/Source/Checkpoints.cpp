#include "Checkpoints.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"


Checkpoints::Checkpoints() : Entity(EntityType::CHECKPOINT)
{
	name.Create("checkpoint");
}

Checkpoints::~Checkpoints() {}

bool Checkpoints::Awake() {

	position.x = config.attribute("x").as_int();
	position.y = config.attribute("y").as_int();
	id = config.attribute("id").as_int();
	texturePath = config.attribute("texturepath").as_string();

	return true;
}

bool Checkpoints::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 20, 100, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::CHECKPOINT;

	//initialize audio effect
	touched = app->audio->LoadFx(config.attribute("activationFxPath").as_string());

	return true;
}

bool Checkpoints::Update(float dt)
{
	texW = 25;
	texH = 25;

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 5;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 5;

	app->render->DrawTexture(texture, position.x, position.y);

	if (last == true)
	{
		app->scene->player->SetPositionX(3688);
		app->scene->player->SetPositionY(700);
		app->scene->player->Teleport(3688, 700);
		last = false;
		app->SaveRequest();
	}

	return true;
}

bool Checkpoints::CleanUp()
{
	pbody->body->SetActive(false);
	app->entityManager->DestroyEntity(this);
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

void Checkpoints::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Checkpoint Collision PLAYER");
		if (isPicked == false)
		{
			app->audio->PlayFx(touched);
			app->SaveRequest();
			if (id == 3)
			{
				last = true;
			}
			isPicked = true;
		}
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

int Checkpoints::GetId() const
{
	return id;
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

void Checkpoints::SetId(int id)
{
	this->id = id;
}