#include "EnemyBee.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

EnemyBee::EnemyBee() : Entity(EntityType::ENEMYBEE) {
	
	name.Create("EnemyBee");
}

EnemyBee::~EnemyBee() {

}

bool EnemyBee::Awake() {

	//L03: DONE 2: Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	return true;
}

bool EnemyBee::Start() {

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	pbody = app->physics->CreateRectangle(position.x, position.y, texW, texH, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool EnemyBee::Update(float dt) {

	//Logica de movimiento Pathfinding con modulos de pathfinding
	//Implementacion maquina de estados

	app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool EnemyBee::CleanUp()
{
	return true;
}

void EnemyBee::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		// TODO : IMPLEMENTAR COLISION CON player
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void EnemyBee::StateMachine() {
	
}

void EnemyBee::Attack() {
	Shoot();
}

void EnemyBee::Shoot() {

}