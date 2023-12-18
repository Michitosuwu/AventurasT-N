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
#include "Map.h"

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
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	return true;
}

bool EnemyBee::Update(float dt) {

	b2Vec2 velocity = pbody->body->GetLinearVelocity();

	//Logica de movimiento Pathfinding con modulos de pathfinding
	//Implementacion maquina de estados
	//origin = app->map->WorldToMap(position.x, position.y);
	//destiny = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);

	//int distance = sqrt(pow((origin.x - destiny.x), 2) + pow((origin.y - destiny.y), 2)); //distancia entre el enemigo y el player

	//if (distance < 5)
	//{
	//	app->map->pathfinding->CreatePath(origin, destiny);
	//	lastPath = *app->map->pathfinding->GetLastPath();

	//	if (lastPath.Count() > 0)
	//	{
	//		iPoint* nextPath;
	//		nextPath = lastPath.At(lastPath.Count() - 1);

	//		if (nextPath->x < origin.x)
	//		{
	//			velocity.x = -speed;
	//		}
	//		else if (nextPath->x > origin.x)
	//		{
	//			velocity.x = +speed;
	//		}
	//		if (nextPath->x == origin.x) {
	//			lastPath.Pop(*nextPath);
	//		}
	//	}
	//}

	//if (!alive)
	//{
	//	pbody->body->SetActive(false);
	//	app->entityManager->DestroyEntity(this);
	//	app->physics->world->DestroyBody(pbody->body);
	//}

	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	pbody->body->SetLinearVelocity(velocity);

	// Obtener la posici�n del cuerpo
	b2Transform pbodyPos = pbody->body->GetTransform();

	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

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