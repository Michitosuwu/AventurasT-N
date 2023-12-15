#include "EnemyWolf.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

EnemyWolf::EnemyWolf() : Entity(EntityType::ENEMYWOLF)
{
	name.Create("EnemyWolf");
}

EnemyWolf::~EnemyWolf() {

}

bool EnemyWolf::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool EnemyWolf::Start() {

	pbody = app->physics->CreateRectangle(position.x, position.y, 50, 50, bodyType::DYNAMIC);

	//texture = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	//app->tex->GetSize(texture, texW, texH);
	//pbody = app->physics->CreateRectangle(position.x, position.y, texW, texH, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool EnemyWolf::Update(float dt)
{
	return true;
}

bool EnemyWolf::CleanUp()
{
	return true;
}

void EnemyWolf::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		// TODO : IMPLEMENTAR COLISION CON player
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}