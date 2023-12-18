﻿#include "EnemyWolf.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "DynArray.h"
#include "Map.h"
#include "EntityManager.h"

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

	texture = app->tex->Load(config.attribute("texturePath").as_string());

	// L07 DONE 5: Add physics to the player - initialize physics body
	app->tex->GetSize(texture, texW, texH);
	//pbody = app->physics->CreateRectangle(position.x, position.y, texW, texH, bodyType::DYNAMIC);
	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::WALKINGENEMY;

	return true;
}

bool EnemyWolf::Update(float dt)
{
	b2Vec2 velocity = pbody->body->GetLinearVelocity();

	origin = app->map->WorldToMap(this->position.x, this->position.y);
	destiny = app->map->WorldToMap(app->scene->player->GetPositionX(), app->scene->player->GetPositionY());

	int distance = sqrt(pow((origin.x - destiny.x), 2) + pow((origin.y - destiny.y), 2)); //distancia entre el enemigo y el player

	if (distance < 8)
	{
		app->map->pathfinding->CreatePath(origin, destiny);
		lastPath = *app->map->pathfinding->GetLastPath();

		if (lastPath.Count() > 0)
		{
			iPoint* nextPath;
			nextPath = lastPath.At(lastPath.Count() - 1);

			if (nextPath->x < origin.x)
			{
				velocity.x = -speed;
			}
			else if (nextPath->x > origin.x)
			{
				velocity.x = +speed;
			}
			if (nextPath->x == origin.x) {
				lastPath.Pop(*nextPath);
			}
		}
	}
	else
	{
		velocity.x = 0;
	}

	if (!alive)
	{
		pbody->body->SetActive(false);
		app->entityManager->DestroyEntity(this);
		app->physics->world->DestroyBody(pbody->body);
	}

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

bool EnemyWolf::CleanUp()
{
	return true;
}

void EnemyWolf::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Enemy Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Enemy Collision PLAYER");
		if (app->scene->player->GetPositionY() < this->position.y)
		{
			this->alive=false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Enemy Collision UNKNOWN");
		break;
	default:
		break;
	}
}