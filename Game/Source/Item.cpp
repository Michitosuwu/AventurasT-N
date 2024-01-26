#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Module.h"
#include "Audio.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	position.x = config.attribute("x").as_int();
	position.y = config.attribute("y").as_int();
	texturePath = config.attribute("texturepath").as_string();
	id = config.attribute("id").as_int();
	type = config.attribute("type").as_int();

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 20, 100, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::CHECKPOINT;

	//initialize audio effect
	touched = app->audio->LoadFx(config.attribute("pickFxPath").as_string());

	return true;
}

bool Item::Update(float dt)
{ 
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Item::CleanUp()
{
	pbody->body->SetActive(false);
	app->entityManager->DestroyEntity(this);
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

void Item::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB->ctype == ColliderType::PLAYER)
	{
		app->audio->PlayFx(touched);
		if (this->type == 1)
		{
			app->scene->player->points += 100;
			LOG("Player points: %d", app->scene->player->points);
		}
		else if (this->type == 2) {
			app->scene->player->hp += 25;
			LOG("Player hp: %d", app->scene->player->hp);
		}
		this->CleanUp();
	}
}

int Item::GetPositionX() const
{
	return position.x;
}

int Item::GetPositionY() const
{
	return position.y;
}

int Item::GetId() const
{
	return id;
}

void Item::SetPositionX(int x)
{
	position.x = x;
}

void Item::SetPositionY(int y)
{
	position.y = y;
}

void Item::SetId(int id)
{
	this->id = id;
}

void Item::SetType(int type)
{
	this->type = type;
}

int Item::GetType() const
{
	return type;
}