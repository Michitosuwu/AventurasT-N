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

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	id = parameters.attribute("id").as_int();
	type = parameters.attribute("type").as_int();

	return true;
}

bool Item::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 20, 100, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::CHECKPOINT;

	//initialize audio effect
	touched = app->audio->LoadFx(config.attribute("activationFxPath").as_string());

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