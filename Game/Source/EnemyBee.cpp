﻿#include "EnemyBee.h"
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
#include "Animationn.h"

EnemyBee::EnemyBee() : Entity(EntityType::ENEMYBEE), currentAnim(&beeFlyAnim) {
	
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
	//app->tex->GetSize(texture, texW, texH);

	texW = 37;
	texH = 29;

	pbody = app->physics->CreateCircle(position.x, position.y, texW / 2, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L07 DONE 7: Assign collider type
	pbody->ctype = ColliderType::FLYINGGENEMY;

	//Load all animations
	beeFlyAnim.LoadAnimations("beeFly");
	beeAttackAnim.LoadAnimations("beeAttack");
	beeDeathAnim.LoadAnimations("beeDeath");

	return true;
}

bool EnemyBee::Update(float dt) {

	b2Vec2 velocity = pbody->body->GetLinearVelocity();

	origin = app->map->WorldToMap(this->position.x, this->position.y);
	destiny = app->map->WorldToMap(app->scene->player->GetPositionX(), app->scene->player->GetPositionY()-150);

	int distance = sqrt(pow((origin.x - destiny.x), 2) + pow((origin.y - destiny.y), 2)); //distancia entre el enemigo y el player

	if (alive) {
		if (distance < 10)
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
					currentAnim = &beeFlyAnim;
				}
				else if (nextPath->x > origin.x)
				{
					velocity.x = +speed;
					currentAnim = &beeFlyAnim;
				}
				if (nextPath->y < origin.y)
				{
					velocity.y = -speed;
					currentAnim = &beeFlyAnim;
				}
				else if (nextPath->y > origin.y)
				{
					velocity.y = +speed;
					currentAnim = &beeFlyAnim;
				}
				if (nextPath->x == origin.x) {
					lastPath.Pop(*nextPath);
				}
			}
		}
		else
		{
			velocity.x = 0;
			currentAnim = &beeAttackAnim;
		}
	}
	

	if (!alive)
	{
		currentAnim = &beeDeathAnim;
	}

	pbody->body->SetLinearVelocity(velocity);

	// Obtener la posici�n del cuerpo
	b2Transform pbodyPos = pbody->body->GetTransform();

	position.x = METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2;
	position.y = METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2;

	//Actualizamos animacion
	currentAnim->Update(dt);
	SDL_Rect currentAnimFrame = currentAnim->GetCurrentFrame(dt);

	app->render->DrawTexture(texture, position.x, position.y, &currentAnimFrame);

	if (currentAnim->Finished()) {
		currentAnim->Reset();
		currentAnim = &beeFlyAnim;
	}

	return true;
}

bool EnemyBee::CleanUp()
{
	pbody->body->SetActive(false);
	app->entityManager->DestroyEntity(this);
	app->physics->world->DestroyBody(pbody->body);
	return true;
}

void EnemyBee::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		LOG("Flying Enemy Collision PLATFORM");
		break;
	case ColliderType::PLAYER:
		LOG("Flying Enemy Collision PLAYER");
		if (app->scene->player->GetPositionY() < this->position.y)
		{
			this->alive = false;
		}
		break;
	case ColliderType::UNKNOWN:
		LOG("Flying Enemy Collision UNKNOWN");
		break;
	default:
		break;
	}
}

void EnemyBee::StateMachine() {
	
}

void EnemyBee::Attack() {
	//Shoot();
}

void EnemyBee::Shoot() {

	////Get the player position
	//iPoint playerPos = app->map->WorldToMap(app->scene->player->GetPositionX(), app->scene->player->GetPositionY() - 150);

	////calculate the direction vector from enemy position to player position
	//iPoint enemyPos = app->map->WorldToMap(this->position.x, this->position.y);
	//iPoint direction = playerPos - enemyPos;

	//// Normalize the direction vector
	//float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
	//if (length != 0) {
	//	direction.x /= length;
	//	direction.y /= length;
	//}


}

void EnemyBee::Teleport(int x, int y)
{
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(x), PIXEL_TO_METERS(y)), 0);
	pbody->body->SetLinearVelocity(b2Vec2(0, 0));
	// Actualizar la variable position con las coordenadas en p�xeles
	position.x = x;
	position.y = y;

}
int EnemyBee::GetPositionX() const
{
	return position.x;
}
int EnemyBee::GetPositionY() const
{
	return position.y;
}
bool EnemyBee::GetAlive() const
{
	return alive;
}
void EnemyBee::SetPositionX(int x)
{
	position.x = x;
}
void EnemyBee::SetPositionY(int y)
{
	position.y = y;
}
void EnemyBee::SetAlive(bool alive)
{
	this->alive = alive;
}